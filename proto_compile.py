import subprocess, os
my_env = os.environ.copy()

bashCommand = "protoc.exe --proto_path=proto --cpp_out=game/proto/ --python_out=algorithms/proto proto/*"
import subprocess
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE, env=my_env)
output, error = process.communicate()