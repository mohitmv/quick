import os
env = Environment(ROOT_PATH = os.getcwd());
env['CXXCOMSTR'] = 'Compiling [$SOURCES] > $TARGET'
env['LINKCOMSTR'] = 'Linking [...] > $TARGET'
Export('env');
SConscript('SConscript', variant_dir='build-dbg', duplicate=0)
