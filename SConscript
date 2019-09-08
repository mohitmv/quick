import tools.scons
Import('env')
new_build_targets = tools.scons.DeclareToSCons(env, BUILD_TARGETS, ARGUMENTS);

del BUILD_TARGETS[:]
BUILD_TARGETS.extend(new_build_targets);