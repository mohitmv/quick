import tools.infra_lib, os
Import('env')

try:
  from tools.local_configs import configs
except ImportError as e:
  from tools.global_configs import configs

new_build_targets = tools.infra_lib.SconsUtility(env, configs).DeclareToSCons(BUILD_TARGETS);
del BUILD_TARGETS[:]
BUILD_TARGETS.extend(new_build_targets);

