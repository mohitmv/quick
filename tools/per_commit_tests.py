#!/usr/bin/env python3

import os, helpers, json

cpplint_script = "~/toolchain/cpplint.py"

configs = json.loads(helpers.ReadFile(helpers.ROOT + "/tools/scons/configs.py"));
dependency_configs = configs["dependency_configs"];


test_modules = set();
cpp_program_files = set();

for i in dependency_configs:
  if i["type"] == "CppProgram" and i["name"][-5:] == "_test":
    test_modules.add(i["name"]);
  if i["type"] in set(["CppProgram", "CppLibrary"]):
    cpp_program_files |= set(i.get("srcs", []))
    cpp_program_files |= set(i.get("hdrs", []))

for i in cpp_program_files:
  assert os.path.exists(i), "File " + i +" doesn't exists";

helpers.RunLinuxCommand(cpplint_script + " --filter=-build/header_guard " + " ".join(cpp_program_files))

for i in test_modules:
  helpers.RunLinuxCommand("scons " + i + " && ./build-dbg/"+i)


