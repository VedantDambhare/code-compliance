import os
import subprocess
import stat

SH_DIR = "/home/nachiket/clang-llvm"
cchecker = "/home/nachiket/codechecker/venv/bin/activate"
input_path = "/home/nachiket/clang-llvm/llvm-project/testing_passes/A0-1-2.cpp"

def code():
    script_path = os.path.join(SH_DIR,"activate_codechecker.sh")
    os.chmod(script_path,0o0777)
    try:
        rc = subprocess.check_output([str(script_path),str(cchecker),str(input_path)])
    except subprocess.CalledProcessError as grepexc:                                                                                                   
        pass

code()

