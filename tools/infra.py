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
  # Declare more dependency rules here.
  # if a module is redeclared here, it will override previous declaration.
]);

""";

configs = helpers.PreProcessDependencyConfigs(configs);

def main():
  if len(sys.argv) == 1:
    return help_message;
  command = sys.argv[1]
  opts, args = getopt.getopt(sys.argv[2:], "", ["arg1=", "force", "=package", "=from"])
  opts = dict(opts);
  if command in ["-h", "--help"]:
    return help_message;
  elif command == "push":
    if (len(args) > 0):
      branch = args[0];
    else:
      branch = configs.active_remote_branch;
    infra_lib.RunLinuxCommand("git add '*'; git commit -m 'Some Change' ; git push origin HEAD:" + branch);
  elif command == "remote_branch":
    return configs.active_remote_branch;
  elif command == "local_setup":
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
  elif command in ["per_commit_check", "pcc"]:
    helpers.RunLintChecks(configs);
    helpers.RunAllTests(configs, pp = 20);
  else:
    return infra_lib.Exit("Invalid command '" + command + "'");

  return "";

print(main());
