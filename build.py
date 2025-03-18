import os
import subprocess
import sys

BUILD_DIR = "build"
if not os.path.exists(BUILD_DIR):
    os.makedirs(BUILD_DIR)

# Create a new build directory

os.chdir(BUILD_DIR)


    # Run the cmake command and redirect stdout and stderr to the file
subprocess.run(['cmake', '-G',  'Ninja', '-DCMAKE_C_STANDARD=23' '-DBUILD_MAIN_EXECUTABLE=ON', '..'], check=True)

try:
   
    # Build the project
    print("Building the project...")
    subprocess.run(["cmake", "--build", "."], check=True)   
except subprocess.CalledProcessError as e:
    print("Error during building project:", e.stderr or e.output)
    sys.exit(1)
    

# Optional: Change back to the original directory
os.chdir("..")

