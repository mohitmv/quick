import os

scons_vars = Variables(args=ARGUMENTS);
scons_vars.Add(EnumVariable('mode', 'Compiler Optimization Mode', 'dbg',
                                 allowed_values=('dbg', 'opt')))

env = Environment(ROOT_PATH = os.getcwd(), variables = scons_vars);
env['CXXCOMSTR'] = 'Compiling [$SOURCES] > $TARGET'
env['LINKCOMSTR'] = 'Linking [...] > $TARGET'

unknown_vars = scons_vars.UnknownVariables()
if unknown_vars:
  raise Exception('UnknownVariables: ' + str(unknown_vars))

Export('env');
SConscript('SConscript', variant_dir='build-dbg', duplicate=0)
