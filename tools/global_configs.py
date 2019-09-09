import os, infra_lib

configs = infra_lib.Object();
configs.package = "quick-1.0"
configs.prod_cc_flags = " -Wno-unused-function  -Wno-unused-parameter -Wno-unused-local-typedefs -Werror ";
configs.global_include_dir = [];
configs.active_remote_branch = "master";
configs.toolchain_path = os.path.expanduser("~/toolchain");
configs["CCFLAGS"] = "--std=c++14 -O0 -Wall -Wextra -Wno-sign-compare -fno-omit-frame-pointer -Wnon-virtual-dtor -mpopcnt -msse4.2 -g3 -Woverloaded-virtual -Wno-char-subscripts -Werror=deprecated-declarations -Wa,--compress-debug-sections -fdiagnostics-color=always ";

br = infra_lib.BuildRule();
configs.dependency_configs = [

  br.CppTest("src/stl_utils_test",
             srcs = ["src/stl_utils_test.cpp"],
             deps = ["tests/test_main",
                     "src/stl_utils"]),

  br.CppLibrary("src/stl_utils",
                hdrs = ["include/quick/stl_utils.hpp"]),

  br.CppTest("src/debug_test",
             srcs = ["src/debug_test.cpp"],
             deps = ["tests/test_main",
                     "src/debug"]),

  br.CppLibrary("src/debug",
                hdrs = ["include/quick/debug.hpp"]),

  br.CppLibrary("src/file_utils",
                hdrs = ["include/quick/file_utils.hpp"],
                srcs = ["src/file_utils.cpp"]),

  br.CppTest("src/file_utils_test",
                srcs = ["src/file_utils_test.cpp"],
                deps = ["src/file_utils"]),

];

