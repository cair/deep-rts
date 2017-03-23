import subprocess

def hasCUDASupport():
    process = subprocess.Popen("nvcc --version", shell=True, stdout=subprocess.PIPE)
    process.wait()
    returncode = process.returncode
    if(returncode is 127):
        return False
    return True
