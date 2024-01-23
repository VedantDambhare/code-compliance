import os
import subprocess
import stat

result = subprocess.run(['pwd'], stdout=subprocess.PIPE).stdout.decode('utf-8')
SH_DIR = result.rstrip() + '/compliance_app/templates/'
cmd = 'echo $HOME'
result_home = subprocess.check_output(cmd, shell=True)
cchecker = result_home.decode("utf-8").rstrip()  + "/codechecker/venv/bin/activate"
input_path = result.rstrip() + '/compliance_app/templates/'

def code():
    script_path = os.path.join(SH_DIR,"activate_codechecker.sh")
    os.chmod(script_path,0o0777)
    try:
        rc = subprocess.check_output([str(script_path),str(cchecker),str(input_path)])
        print(rc)
    except subprocess.CalledProcessError as grepexc:                                                                                 
        pass
    if os.path.isdir(input_path):
        # Delete all .cpp files in the target directory
        subprocess.run(['find', input_path, '-type', 'f', '-name', '*.cpp', '-delete'])

        # Remove the "reports_html" folder from one level above the current input path
        parent_path = os.path.dirname(input_path)
        reports_html_path = os.path.join(parent_path, 'reports_html')
        if os.path.exists(reports_html_path):
            subprocess.run(['rm', '-r', reports_html_path])

code()
