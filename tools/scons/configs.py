{
  "dependency_configs": [
    {
      "name": "toolchain/gtest",
      "type": "ToolchainLibrary",
      "srcs":  ["googletest-release-1.8.1/googletest/src/gtest-all.cc"],
      "global_include_dir": ["googletest-release-1.8.1/googletest/include"],
      "local_include_dir": ["googletest-release-1.8.1/googletest"]
    },
    {
      "name": "toolchain/json11",
      "type": "ToolchainLibrary",
      "srcs":  ["json11-master/json/json11.cpp"],
      "global_include_dir": ["json11-master"]
    },
    {
      "name": "src/stl_utils_test",
      "type": "CppProgram",
      "hdrs": [],
      "srcs": ["src/stl_utils_test.cpp"],
      "deps": ["toolchain/gtest",
               "toolchain/json11"]
    }
  ]
}

