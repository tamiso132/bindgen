import os
import subprocess
import sys

subprocess.run(['python', 'build.py'], check=True)
subprocess.run('./build/libDynamicLibVulkan')