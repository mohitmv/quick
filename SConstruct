import os

scons_vars = Variables(args=ARGUMENTS);
scons_vars.Add(EnumVariable('mode', 'Compiler Optimization Mode', 'opt',
                                 allowed_values=('debug', 'opt', 'release')))
scons_vars.Add(BoolVariable('verbose', 'Compiler Optimization Mode', False))


env = Environment(ROOT_PATH = os.getcwd(), variables = scons_vars);
if (not env["verbose"]):
  env['CXXCOMSTR'] = 'Compiling [$SOURCES] > $TARGET'
  env['LINKCOMSTR'] = 'Linking [...] > $TARGET'

unknown_vars = scons_vars.UnknownVariables()
if unknown_vars:
  raise Exception('UnknownVariables: ' + str(unknown_vars))
env["build_dir"] = "build-" + env["mode"];


Export('env');
SConscript('SConscript', variant_dir=env["build_dir"], duplicate=0)
