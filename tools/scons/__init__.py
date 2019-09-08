import sys, os, json

import SCons.Variables

########## Helper Functions Starts Here ##########

def ReadFile(fn):
  fd = open(fn);
  data = fd.read();
  fd.close();
  return data;

toolchain_module_types = set(["ToolchainLibrary"]);

def IsToolchainModule(module):
  return (module["type"] in toolchain_module_types);

########## Helper Functions Ends Here ##########


def GetConfigs(env):
  config_file = env['ROOT_PATH'] + "/tools/scons/configs.py";
  configs = json.loads(ReadFile(config_file));
  configs["toolchain_path"] = os.getenv("HOME") + "/toolchain/";
  configs["prod_cc_flags"] = " -Wno-unused-function  -Wno-unused-parameter -Wno-unused-local-typedefs -Werror ";
  configs["global_include_dir"] = ["include"];
  return configs;

# core_libs_path = ["src/lexer", "src/parser", "src/regex", "src/error", "src/regex_builder", "src/aparse_machine_builder", "src/utils", "src/traverser", "src/parse_regex", "src/grammar"];

def AddArguments(ARGUMENTS):
  variables = SCons.Variables.Variables(args=ARGUMENTS);
  variables.Add(SCons.Variables.EnumVariable(
              'env',
              'Running Environment',
              'laptop',
              ['laptop', 'ts']));
  variables.Add(SCons.Variables.BoolVariable(
              'run',
              'run the program or not ?',
              False));
  args = SCons.Environment.Environment(variables = variables);
  return args;

def SetGccConfigs(args, env, configs):
  env["CPPPATH"] = configs["global_include_dir"];
  if (args["env"] == "ts"):
    env["CXX"] = "/usr/local/scaligent/toolchain/crosstool/v4/x86_64-unknown-linux-gnu/bin/x86_64-unknown-linux-gnu-g++";
    env["LINKFLAGS"] = "-Wl,--compress-debug-sections=zlib -Wl,--dynamic-linker=/usr/local/scaligent/toolchain/crosstool/v4/x86_64-unknown-linux-gnu/x86_64-unknown-linux-gnu/sysroot/lib/ld-linux-x86-64.so.2 -B/usr/local/scaligent/toolchain/crosstool/v4/x86_64-unknown-linux-gnu/x86_64-unknown-linux-gnu/bin.gold -Wl,-rpath=/usr/local/scaligent/toolchain/crosstool/v4/x86_64-unknown-linux-gnu/x86_64-unknown-linux-gnu/sysroot/lib -Wl,--no-whole-archive  -lpthread -ldl ";
    env["CCFLAGS"] = "-std=c++14 -O0 -Wall -Wextra -Wno-sign-compare -fno-omit-frame-pointer -Wnon-virtual-dtor -mpopcnt -msse4.2 -DBOOST_NETWORK_ENABLE_HTTPS -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -DGTEST_USE_OWN_TR1_TUPLE=0 -D_GLIBCXX_USE_CXX11_ABI=0 -DGLOG_STL_LOGGING_FOR_UNORDERED -Dlinux -g3 -DGLIBCXX_DEBUG -Woverloaded-virtual -Wno-char-subscripts -Werror=deprecated-declarations -Wa,--compress-debug-sections -fdiagnostics-color=always -D _APARSE_DEBUG_FLAG=true";
  elif args["env"] == "laptop":
    env["CXX"] = "g++";
    env["CCFLAGS"] = "-std=c++14 -O0 -Wall -Wextra -Wno-sign-compare -fno-omit-frame-pointer -Wnon-virtual-dtor -mpopcnt -msse4.2 -g3 -Woverloaded-virtual -Wno-char-subscripts -Werror=deprecated-declarations -Wa,--compress-debug-sections -fdiagnostics-color=always -D _APARSE_DEBUG_FLAG=true";


# core_libs_path = [];

def TopologicalSorting(build_targets, all_modules):
  output = [];
  visited_set = set();
  ancester_set = set();
  def AddDependency(module_name):
    if module_name in visited_set:
      return;
    ancester_set.add(module_name);
    module = all_modules[module_name];
    for i in module.get("deps", []):
      assert (i in all_modules), "module '" + i + "' is required by '" + module_name + "', but not declared.";
      assert (i not in ancester_set), "there is a cyclic dependency from module '" + i + "' to module '" + i + "'"
      AddDependency(i);
    output.append(module_name);
    ancester_set.remove(module_name);
    visited_set.add(module_name);
  for i in build_targets:
    assert (i in all_modules), "module '" + i + "' can't be built because it's not declared."
    AddDependency(i);
  return output;

def CalculateCompleteDependency(build_targets, all_modules):
  complete_dependency_dict = {};
  def CompleteDependency(module_name):
    if module_name in complete_dependency_dict:
      return complete_dependency_dict[module_name];
    output = complete_dependency_dict[module_name] = set();
    for i in all_modules[module_name].get("deps", []):
      output |= CompleteDependency(i);
      output.add(i);
    return output;
  for i in build_targets:
    CompleteDependency(i);
  return complete_dependency_dict;

def DeclareCppProgram(env,
                      module,
                      complete_dependency,
                      declared_targets):
  required_objects = list(declared_targets[i] for i in complete_dependency)
  required_objects.append(env.Object(module["srcs"]));
  return env.Program(module["name"], required_objects);


def DeclareToolchainLibrary(env, module, configs):
  toolchain_path = configs["toolchain_path"];
  local_include_dir = module.get("local_include_dir", []);
  srcs = list(toolchain_path+i for i in module["srcs"])
  cpppath_flags = list(toolchain_path+i for i in local_include_dir);
  return env.Object(module["name"], srcs,
                    CPPPATH = cpppath_flags + env["CPPPATH"]);

def DeclareToSCons(env, build_targets, scons_ARGUMENTS):
  env['CXXCOMSTR'] = 'Compiling [$SOURCES] > $TARGET'
  env['LINKCOMSTR'] = 'Linking [...] > $TARGET'
  configs = GetConfigs(env);
  args = AddArguments(scons_ARGUMENTS);
  all_modules = dict((i["name"], i) for i in configs["dependency_configs"]);
  if len(build_targets) == 0:
    build_targets = list(all_modules.keys());
  build_sequence = TopologicalSorting(build_targets, all_modules);
  complete_dependency_dict = CalculateCompleteDependency(build_targets,
                                                         all_modules);
  declared_targets = {};
  for module_name in build_sequence:
    module = all_modules[module_name];
    for i in module.get("global_include_dir", []):
      include_dir = (configs["toolchain_path"] if module["type"] == "ToolchainLibrary" else "") + i;
      configs["global_include_dir"].append(include_dir);
  print("build_sequence = ", build_sequence);
  SetGccConfigs(args, env, configs);
  for module_name in build_sequence:
    module = all_modules[module_name];
    complete_dependency = complete_dependency_dict[module["name"]];
    if module["type"] == "CppLibrary":
      declaration = env.Object(module["name"], module["srcs"]);
      print("Declation of ", module["name"], module["srcs"]);
    elif module["type"] == "CppProgram":
      declaration = DeclareCppProgram(env,
                                      module,
                                      complete_dependency,
                                      declared_targets);
    elif module["type"] == "ToolchainLibrary":
      declaration = DeclareToolchainLibrary(env, module, configs);
    else:
      assert False;
    declared_targets[module_name] = declaration;

  return list(("build-dbg/" + i) for i in build_targets);






# def BuildObject(env, path):
#   return env.Object(path+".cpp");

# def BuildProgram(env, path, objects=[]):
#   env.Program(path, objects);

# def BuildCoreLibs(env):
#   return list(BuildObject(env, i) for i in core_libs_path);

# def BuildToolchain(env):
#   return list(env.Object(i) for i in toolchain_src);

# def BuildModule(env, path, dependency=[]):
#   objects = BuildCoreLibs(env) + BuildToolchain(env);
#   for i in dependency+[path]:
#     objects.append(BuildObject(env, i));
#   BuildProgram(env, path, objects);

# def DeclareToSCons(env, build_targets, scons_ARGUMENTS):
#   args = add_arguments(scons_ARGUMENTS);
#   set_gcc_params(args, env);
#   env['CXXCOMSTR'] = 'Compiling [$SOURCES] > $TARGET'
#   env['LINKCOMSTR'] = 'Linking [...] > $TARGET'
#   if len(build_targets) == 0:
#     build_targets = core_libs_path;
#   for i in build_targets:
#     BuildModule(env, i)
#   return list(("build-dbg/" + i) for i in build_targets);


