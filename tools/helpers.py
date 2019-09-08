import os

################# Useful Configs #################

ROOT = os.getcwd();
# TOOLCHAIN = ReadFile(ROOT + "tools/dev_setup/toolchain_path.");

def ReadFile(fn):
  fd = open(fn);
  data = fd.read();
  fd.close();
  return data;
