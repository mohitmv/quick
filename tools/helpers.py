import infra_lib, os

def PreProcessDependencyConfigs(configs):
  for i in configs.dependency_configs:
    if i["type"] == "CppTest":
      i["deps"].append("tests/test_main");
  br = infra_lib.BuildRule();
  configs.dependency_configs.append(
    br.CppLibrary("tests/test_main",
                  srcs = ["tests/test_main.cpp"],
                  deps = ["toolchain/gtest"]));
  return configs;


def RunLintChecks(configs, files = None):
  if (files == None):
    files = infra_lib.CppSourceFilesList(configs);
  infra_lib.RunLinuxCommand(os.path.join(configs.toolchain_path + "/cpplint.py") + " --filter=-build/header_guard " + " ".join(files));

def RunAllTests(configs, pp = 20, tests = None):
  if (tests == None):
    tests = list(i["name"] for i in configs.dependency_configs if i["type"] == "CppTest")
  infra_lib.RunLinuxCommand("scons -j"+str(pp) + " " + " ".join(tests));
  for i in tests:
    infra_lib.RunLinuxCommand("./build-dbg/" + i);
