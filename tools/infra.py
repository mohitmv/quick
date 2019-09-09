#!/usr/bin/env python3
import getopt, sys, infra_lib, os, helpers

is_local_configs_setup_done = True;
try:
  from local_configs import configs
except ImportError as e:
  from global_configs import configs
  is_local_configs_setup_done = False;


help_message = """
usage: <python script> command [options]
       This utility displays .......

""";


default_local_configs = """
import os, infra_lib, helpers
from global_configs import configs

# configs.toolchain_path = infra_lib.HOME + "/../my_toolchain"

br = infra_lib.BuildRule();
configs.dependency_configs.extend([
  # All paths are relative w.r.t `configs.toolchain_path`
  br.ToolchainLibrary(
      "toolchain/gtest",
      srcs = [ "googletest-release-1.8.1/googletest/src/gtest-all.cc" ],
      global_include_dir = [ "googletest-release-1.8.1/googletest/include" ],
      local_include_dir = [ "googletest-release-1.8.1/googletest" ],
      download_zip_from = None,
      download_to = None),

  # All paths are relative w.r.t `configs.toolchain_path`
  br.ToolchainLibrary("toolchain/json11",
                      srcs = ["json11-master/json/json11.cpp"],
                      global_include_dir = [ "json11-master"],
                      download_zip_from = None,
                      download_to = None),

]);


configs = helpers.PreProcessDependencyConfigs(configs);

""";

def main():
  if len(sys.argv) == 1:
    return help_message;
  command = sys.argv[1]
  opts, args = getopt.getopt(sys.argv[2:], "", ["arg1=", "force"])
  opts = dict(opts);
  if ((not is_local_configs_setup_done) and (command not in ["dev_setup",  "-h", "--help"])):
    infra_lib.Exit("dev_setup is not ready. run 'chmod +x tools/infra.py && ./tools/infra.py dev_setup.py' before anything else");
  elif command in ["-h", "--help"]:
    return help_message;
  elif command == "push":
    infra_lib.RunLinuxCommand("git add '*'; git commit -m 'Some Change' ; git push origin HEAD:" + configs.active_remote_branch);
  elif command == "remote_branch":
    return configs.active_remote_branch;
  elif command == "dev_setup":
    local_configs_file = os.path.join(infra_lib.ROOT,
                                      "tools",
                                      "local_configs.py");
    if os.path.exists(local_configs_file):
      infra_lib.Exit("'tools/local_configs.py' already exists.");
    else:
      infra_lib.WriteFile(local_configs_file, default_local_configs);
      return "Created 'tools/local_configs.py' for local_configs.";
  elif command == "download_toolchain":
    print("Toolchain packages will be downloaded in '" + configs.toolchain_path + "' directory.");
    print("ToDo(Mohit): Implement the toolchain downloading script");
  elif command == "nop":
    pass
  elif command == "lint":
    helpers.RunLintChecks(configs, (args if len(args) > 0 else None));
  elif command == "run_test":
    helpers.RunAllTests(configs, pp = 20, tests = (args if len(args) > 0 else None));
  elif command == "per_commit_check":
    helpers.RunLintChecks(configs);
    helpers.RunAllTests(configs, pp = 20);
  else:
    return infra_lib.Exit("Invalid command '" + command + "'");

  return "";

print(main());
