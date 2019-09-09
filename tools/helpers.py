import os

################# Useful Configs #################

ROOT = os.getcwd();
TOOLCHAIN_PATH = os.environ["HOME"] + "/toolchain";

def ReadFile(fn):
  fd = open(fn);
  data = fd.read();
  fd.close();
  return data;

def RunLinuxCommand(c):
  print("Running: " + c);
  output = os.system(c);
  if ((output >> 8) != 0):
    print("Error Exit !");
    exit(1);


# Update dict a by values of b, but without overriding 
def SoftUpdate(a, b):
  for i in b:
    if i not in a:
      a[i] = b[i];

