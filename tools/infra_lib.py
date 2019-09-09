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

def CppSourceFilesList(configs):
  internal_module_types = set(["CppProgram", "CppLibrary", "CppTest"]);
  output = set()
  for i in configs.dependency_configs:
    if i["type"] in internal_module_types:
      output |= set(i["srcs"]);
      output |= set(i["hdrs"]);
  for i in output:
    assert os.path.exists(i), "File '" + i  + "', declared in dependency_configs doesn't exists";
  return output;


###################### Build Utilities ########################

module_default_fields = dict(srcs=[],
                             hdrs=[],
                             deps=[],
                             global_include_dir=[],
                             local_include_dir=[]);

def BuildRule():
  br = Object();
  def BuildRuleBuilder(module_type):
    def ActionBuilder(name, **params):
      return SoftUpdate(dict(type = module_type, name = name, **params),
                        module_default_fields);
    return ActionBuilder;
  for i in ["CppLibrary", "CppTest", "CppProgram", "ToolchainLibrary"]:
    br[i] = BuildRuleBuilder(i);
  return br;

class SconsUtility:
  toolchain_module_types = set(["ToolchainLibrary"]);
  def __init__(self, scons_env, configs):
    self.env = scons_env;
    self.configs = configs;

  def ToolchainPath(self, path):
    return os.path.join(self.configs.toolchain_path, path);

  def IsToolchainModule(self, module):
    return (module["type"] in toolchain_module_types);

  def PreProcessConfigs(self, env):
    configs = self.configs;
    configs.global_include_dir.append("include");

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
      return env.Object(module["name"], module["srcs"]);

  def DeclareCppProgram(self, env,
                        module,
                        complete_dependency,
                        declared_targets):
    required_objects = list(declared_targets[i]
                              for i in complete_dependency
                              if declared_targets[i] != None);
    required_objects.append(env.Object(module["srcs"]));
    return env.Program(module["name"], required_objects);


  def DeclareToolchainLibrary(self, env, module, configs):
    local_include_dir = module["local_include_dir"];
    srcs = list(self.ToolchainPath(i) for i in module["srcs"])
    cpppath_flags = list(self.ToolchainPath(i) for i in local_include_dir);
    return env.Object(module["name"], srcs,
                      CPPPATH = cpppath_flags + env["CPPPATH"]);

  def DeclareToSCons(self, build_targets):
    env = self.env;
    configs = self.configs;
    self.PreProcessConfigs(configs);
    all_modules = dict((i["name"], i) for i in configs["dependency_configs"]);
    if len(build_targets) == 0:
      build_targets = list(all_modules.keys());
    build_sequence = self.TopologicalSorting(build_targets, all_modules);
    complete_dependency_dict = self.CalculateCompleteDependency(build_targets,
                                                           all_modules);
    declared_targets = {};
    for module_name in build_sequence:
      module = all_modules[module_name];
      for i in module["global_include_dir"]:
        include_dir = (self.ToolchainPath(i) if module["type"] == "ToolchainLibrary" else  i);
        configs["global_include_dir"].append(include_dir);
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
      elif module["type"] == "ToolchainLibrary":
        declaration = self.DeclareToolchainLibrary(env, module, configs);
      else:
        print(module);
        assert False;
      declared_targets[module_name] = declaration;
    return list(("build-dbg/" + i) for i in build_targets);

