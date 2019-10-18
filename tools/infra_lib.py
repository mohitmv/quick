import os

class Object(dict):
  def __init__(self, initial_value={}, **kwargs):
    self.__dict__ = self;
    dict.__init__(self, initial_value, **kwargs);

ROOT = os.getcwd();
HOME = os.environ["HOME"];

def ReadFile(fn):
  fd = open(fn);
  data = fd.read();
  fd.close();
  return data;

def RunLinuxCommand(c):
  print("Running: " + c);
  error_code = (os.system(c) >> 8);
  if (error_code != 0):
    print("["+str(error_code)+"]Error Exit !");
    exit(error_code);

# Update dict a by values of b, but without overriding 
def SoftUpdate(a, b):
  for i in b:
    if i not in a:
      a[i] = b[i];
  return a;


def WriteFilePipe(file_pipe, data):
  file_pipe.write(data);
  file_pipe.close();

def WriteFile(fn, data, mode='w'):
  return WriteFilePipe(open(fn, mode, encoding='utf-8'), data);


def Exit(message):
  print(message);
  exit(1);

def CppSourceFilesList(configs, filter):
  internal_module_types = set(["CppProgram", "CppLibrary", "CppTest"]);
  files = set()
  for i in configs.dependency_configs:
    if i["type"] in internal_module_types and filter(i):
      files |= set(i["srcs"]);
      files |= set(i["hdrs"]);
  for i in files:
    assert os.path.exists(i), "File '" + i  + "', declared in dependency_configs doesn't exists";
  output = list();
  cwd =  os.path.join(os.getcwd(), "");
  # print(cwd, list(files)[0][:len(cwd)], files)
  for i in files:
    if os.path.abspath(i)[:len(cwd)] == cwd:
      output.append(i);
  # print(output)
  return output;


###################### Build Utilities ########################

module_default_fields = dict(srcs=[],
                             hdrs=[],
                             deps=[],
                             global_include_dir=[],
                             local_include_dir=[],
                             global_link_flags = "",
                             local_link_flags = "",
                             global_cc_flags = "",
                             local_cc_flags = "");

def BuildRule():
  br = Object();
  def BuildRuleBuilder(module_type):
    def ActionBuilder(name, **params):
      return SoftUpdate(dict(type = module_type, name = name, **params),
                        module_default_fields);
    return ActionBuilder;
  for i in ["CppLibrary", "CppTest", "CppProgram"]:
    br[i] = BuildRuleBuilder(i);
  return br;

class SconsUtility:
  def __init__(self, scons_env, configs):
    self.env = scons_env;
    self.configs = configs;

  def SetGccConfigs(self):
    env = self.env;
    configs = self.configs;
    env["CPPPATH"] = configs.global_include_dir;
    for i in ["CXX", "LINKFLAGS", "CCFLAGS"]:
      if i in configs:
        env[i] = configs[i];

  def TopologicalSorting(self, build_targets, all_modules):
    output = [];
    visited_set = set();
    ancester_set = set();
    def AddDependency(module_name):
      if module_name in visited_set:
        return;
      ancester_set.add(module_name);
      module = all_modules[module_name];
      for i in module["deps"]:
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

  def CalculateCompleteDependency(self, build_targets, all_modules):
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

  def DeclareCppLibrary(self, env, module):
    if len(module['srcs']) > 0:
      return env.Object(
                module["name"],
                module["srcs"],
                CPPPATH = module["local_include_dir"] + env["CPPPATH"],
                CCFLAGS = env["CCFLAGS"] + " " + module["local_cc_flags"]);

  def DeclareCppProgram(self, env,
                        module,
                        complete_dependency,
                        declared_targets):
    required_objects = list(declared_targets[i]
                              for i in complete_dependency
                              if declared_targets[i] != None);
    required_objects.append(env.Object(module["srcs"]));
    return env.Program(
              module["name"],
              required_objects,
              CPPPATH = module["local_include_dir"] + env["CPPPATH"],
              CCFLAGS = env["CCFLAGS"] + " " + module["local_cc_flags"],
              LINKFLAGS = env["LINKFLAGS"] + " " + module["local_link_flags"]);

  def DeclareToSCons(self, build_targets):
    env = self.env;
    configs = self.configs;
    all_modules = dict((i["name"], i) for i in configs["dependency_configs"]);
    if len(build_targets) == 0:
      build_targets = list(all_modules.keys());
    build_sequence = self.TopologicalSorting(build_targets, all_modules);
    complete_dependency_dict = self.CalculateCompleteDependency(build_targets,
                                                           all_modules);
    declared_targets = {};
    for module_name in build_sequence:
      module = all_modules[module_name];
      configs["global_include_dir"].extend(module["global_include_dir"]);
      configs["CCFLAGS"] += " " + module["global_cc_flags"];
      configs["LINKFLAGS"] += " " + module["global_link_flags"];
    self.SetGccConfigs();
    for module_name in build_sequence:
      module = all_modules[module_name];
      complete_dependency = complete_dependency_dict[module["name"]];
      if module["type"] == "CppLibrary":
        declaration = self.DeclareCppLibrary(env, module);
      elif module["type"] in ["CppProgram", "CppTest"]:
        declaration = self.DeclareCppProgram(env,
                                        module,
                                        complete_dependency,
                                        declared_targets);
      else:
        print(module);
        assert False;
      declared_targets[module_name] = declaration;
    return list((env["build_dir"] + "/" + i) for i in build_targets);

