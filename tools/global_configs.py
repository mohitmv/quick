import os, infra_lib

configs = infra_lib.Object();
configs.compiler_options = infra_lib.Object(mode = "opt");
configs.package = "quick-1.0.0"
configs.prod_cc_flags = " ".join([" -Wno-unused-function ",
                                  " -Wno-unused-parameter ",
                                  " -Wno-unused-local-typedefs "]);
configs.global_include_dir = ["include"];
configs.active_remote_branch = "dev-quick-1.1";
toolchain_path = configs.toolchain_path = os.path.join(os.environ["HOME"], "toolchain");

br = infra_lib.BuildRule();
configs.dependency_configs = [

  br.CppLibrary(
      "toolchain/gtest",
      srcs = [ toolchain_path + "/googletest-release-1.8.1/googletest/src/gtest-all.cc" ],
      global_include_dir = [ toolchain_path + "/googletest-release-1.8.1/googletest/include" ],
      local_include_dir = [ toolchain_path+ "/googletest-release-1.8.1/googletest" ],
      global_link_flags = "-lpthread"),

  br.CppLibrary("toolchain/json11",
                      srcs = [toolchain_path + "/json11-master/json/json11.cpp"],
                      global_include_dir = [ toolchain_path + "/json11-master"]),

  br.CppProgram("tools/experiments/try",
                ignore_cpplint = True,
                srcs = ["tools/experiments/try.cpp"],
                deps = [],
                global_link_flags = "-lpthread"),

  br.CppProgram("tools/experiments/try2",
                ignore_cpplint = True,
                srcs = ["tools/experiments/try2.cpp"],
                deps = ["src/debug"]),



  br.CppLibrary("src/type_traits",
                hdrs = ["include/quick/type_traits.hpp"]),

  br.CppLibrary("src/utility",
                hdrs = ["include/quick/utility.hpp"]),

  br.CppLibrary("src/stl_utils",
                hdrs = ["include/quick/stl_utils.hpp"]),

  br.CppLibrary("src/debug",
                hdrs = ["include/quick/debug.hpp"]),

  br.CppLibrary("src/alias",
                hdrs = ["include/quick/alias.hpp"]),

  br.CppLibrary("src/file_utils",
                hdrs = ["include/quick/file_utils.hpp"],
                srcs = ["src/file_utils.cpp"]),

  br.CppLibrary("src/hash",
                hdrs = ["include/quick/hash.hpp"]),

  br.CppLibrary("src/unordered_map",
                hdrs = ["include/quick/unordered_map.hpp"],
                deps = ["src/hash"]),

  br.CppLibrary("src/time",
                hdrs = ["include/quick/time.hpp"]),

  br.CppLibrary("src/byte_stream",
                hdrs = ["include/quick/byte_stream.hpp"],
                deps = []),

  br.CppLibrary("src/debug_stream",
                hdrs = ["include/quick/debug_stream.hpp"],
                deps = []),

  br.CppTest("tests/debug_stream_test",
                srcs = ["tests/debug_stream_test.cpp"],
                deps = ["src/debug_stream"]),

  br.CppTest("tests/byte_stream_test",
             srcs = ["tests/byte_stream_test.cpp"],
             deps = ["src/byte_stream"]),

  br.CppTest("tests/stl_utils_test",
             srcs = ["tests/stl_utils_test.cpp"],
             deps = ["src/stl_utils"]),


  br.CppTest("tests/debug_test",
             srcs = ["tests/debug_test.cpp"],
             deps = ["src/debug"]),

  br.CppTest("tests/debug_test_c1",
             srcs = ["tests/debug_test_c1.cpp"],
             deps = ["src/debug"]),

  br.CppTest("tests/debug_test_c2",
             srcs = ["tests/debug_test_c2.cpp"],
             deps = ["src/debug"]),

  br.CppTest("tests/alias_test",
             srcs = ["tests/alias_test.cpp"],
             deps = ["src/alias"]),


  br.CppTest("tests/file_utils_test",
                srcs = ["tests/file_utils_test.cpp"],
                deps = ["src/file_utils"]),

  br.CppTest("tests/hash_test",
                srcs = ["tests/hash_test.cpp"],
                deps = ["src/hash"]),


  br.CppTest("tests/unordered_map_test",
                srcs = ["tests/unordered_map_test.cpp"],
                deps = ["src/unordered_map"]),

  br.CppTest("tests/time_test",
                srcs = ["tests/time_test.cpp"],
                deps = ["src/time"]),

  br.CppTest("tests/type_traits_test",
                srcs = ["tests/type_traits_test.cpp"],
                deps = ["src/type_traits"]),

  br.CppTest("tests/utility_test",
                srcs = ["tests/utility_test.cpp"],
                deps = ["src/utility"]),


];


configs["CCFLAGS"] = "--std=c++14 -Wall -Wextra -Wno-sign-compare -fno-omit-frame-pointer -Wnon-virtual-dtor -mpopcnt -msse4.2 -g3 -Woverloaded-virtual -Wno-char-subscripts -Werror=deprecated-declarations -Wa,--compress-debug-sections -fdiagnostics-color=always  -Werror  -Wpedantic  -Wextra ";
if (configs.compiler_options["mode"] == "debug"):
  configs["CCFLAGS"] += " -O0 ";
elif (configs.compiler_options["mode"] == "opt"):
  configs["CCFLAGS"] += " -O3 " + configs.prod_cc_flags ;
elif (configs.compiler_options["mode"] == "release"):
  configs["CCFLAGS"] += " -O3 " + configs.prod_cc_flags ;

configs["LINKFLAGS"] = ""

if (os.environ.get("DEV_MACHINE") == "ts"):
  configs["CXX"] = "/usr/local/scaligent/toolchain/crosstool/v4/x86_64-unknown-linux-gnu/bin/x86_64-unknown-linux-gnu-g++";
  configs["LINKFLAGS"] = "-Wl,--compress-debug-sections=zlib -Wl,--dynamic-linker=/usr/local/scaligent/toolchain/crosstool/v4/x86_64-unknown-linux-gnu/x86_64-unknown-linux-gnu/sysroot/lib/ld-linux-x86-64.so.2 -B/usr/local/scaligent/toolchain/crosstool/v4/x86_64-unknown-linux-gnu/x86_64-unknown-linux-gnu/bin.gold -Wl,-rpath=/usr/local/scaligent/toolchain/crosstool/v4/x86_64-unknown-linux-gnu/x86_64-unknown-linux-gnu/sysroot/lib -Wl,--no-whole-archive ";
