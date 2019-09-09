import tools.infra_lib, os
Import('env')

try:
  from tools.local_configs import configs
except ImportError as e:
  tools.infra_lib.Exit("dev_setup is not ready. run 'chmod +x tools/infra.py && ./tools/infra.py dev_setup.py' before anything else");

new_build_targets = tools.infra_lib.SconsUtility(env, configs).DeclareToSCons(BUILD_TARGETS);
del BUILD_TARGETS[:]
BUILD_TARGETS.extend(new_build_targets);

