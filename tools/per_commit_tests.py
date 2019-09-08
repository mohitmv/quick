#!/usr/bin/env python3

import os, helpers, json

cpplint_script = "~/toolchain/cpplint.py"

# def CppLintChecks():
#   os.system("build/header_guard");


# CppLintChecks();

configs = json.loads(helpers.ReadFile(helpers.ROOT + "/tools/scons/configs.py"));
dependency_configs = configs["dependency_configs"];


test_modules = set();
# cpp_program_files = set();

for i in dependency_configs:
  if i["type"] == "CppProgram" and i["name"][-5:] == "_test":
    test_modules.add(i["name"]);

os.system(cpplint_script + " --filter=-build/header_guard include/* include/*/* include/*/*/* include/*/*/* src/*  src/*/* src*/*/* tests/* tests/*/*");

for i in test_modules:
  os.system("scons " + i + " && ./build-dbg/"+i)


