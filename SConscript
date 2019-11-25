import os
import tools.infra_lib, tools.helpers
Import('env')

try:
  from tools.local_configs import configs
except ImportError as e:
  from tools.global_configs import configs

configs.compiler_options.mode = env["mode"];
configs.build_dir = env["build_dir"];
configs = tools.helpers.PreProcessDependencyConfigs(configs);

new_build_targets = tools.infra_lib.SconsUtility(env, configs).DeclareToSCons(BUILD_TARGETS);
del BUILD_TARGETS[:]
BUILD_TARGETS.extend(new_build_targets);

