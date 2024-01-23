# CodeCompliance-IITH

**Prerequisites required**

Ninja
https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages
                                            
Make sure that make is installed on the system using the following command: 

sudo apt update
make  - - version

If make is not installed run the following command on Ubuntu:

sudo apt install make
sudo apt install python3.10-venv


**Setting up clang-tidy **

Clone the following github directory:
https://github.com/nileshshah21/CodeCompliance-IITH/tree/main
cd llvm-project
mkdir build 
cd build
cmake -G "Unix Makefiles" -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DCMAKE_BUILD_TYPE=Release -DLLVM-BUILD-TESTS=ON ../llvm
make clang clang-tidy OR ninja  clang clang-tidy


**Setting up code checker **

Please follow the steps mentioned under the heading Linux in the following link:

https://codechecker.readthedocs.io/en/latest/#install-guide

Additional comments:

In case the “make package” doesn't work run: 
git config --global http.sslBackend schannel


**Linking clang-tidy and code checker**

We have to integrate clang-tidy with code checker. Here the idea is that once you build clang-tidy with the changes we need to link it.
Capture the path to the clang-tidy build folder (Where the clang-tidy binary is available).
Navigate to the code checker folder (~/codechecker/) and then navigate to config (~/codechecker/build/Co	deChecker/config).
Inside the config, there's a file named package_layout.json.
Set the path of your clang-tidy as mentioned in the link below (https://codechecker.readthedocs.io/en/latest/package_layout/#runtime-section)

Please find the screenshot below for reference


**Setting up CPPCheck**

Download the zip folder of release version 2.6 of CPPCheck using the following link:
https://github.com/danmar/cppcheck/archive/refs/tags/2.6.zip

Extract the downloaded zip file and move it inside the main directory.
cd cppcheck-2.6
mkdir build
cd build 
cmake ..
make MATCHCOMPILER=yes FILESDIR=/usr/share/cppcheck HAVE_RULES=yes CXXFLAGS="-O2 -DNDEBUG -Wall -Wno-sign-compare -Wno-unused-function"
Replace the default file misra.py on the following path with the one provided on the github directory 
Example: mv ./main_directory/misra/py /PATH_TO_CPPCHECK/cppcheck-2.6/build/bin/addons/misra.py

Linking CPP checker and code checker

The same steps need to be followed as clang-tidy integration. Just navigate to the cppcheck folder and find the binary that has been built and use it as a path for package_layout.json.


**Setting up Django **

First, create a virtual environment - python-venv env
Then, active environment - source env/bin/activate
Now create requirements.in the file and then write in it only 'django' (without quotes).
Run the following command:
pip-compile requirements.in
After running the above command, the requirement.txt file gets generated. 
Run the following command for the final installation:
pip3 install -r requirements.txt

**Linking code checker to Django web app**

Inside the web application, two Python scripts invoke CodeChecker in the backend.

Edit /PATH_MAIN_DIRECTORY/CODE_COMPLIANCE_WEB_APP_1/CODE_COMPLIANCE_WEB_APP/code_compliance/compliance_app/templates/call_codechecker.py
Replace three paths as shown below in the images
SH_DIR
/PATH_MAIN_DIRECTORY/CODE_COMPLIANCE_WEB_APP_1/CODE_COMPLIANCE_WEB_APP/code_compliance/compliance_app/templates
Cchecker
/PATH_TO_CODECHECKER/codechecker/venv/bin/activate
Input_path
/PATH_MAIN_DIRECTORY/CODE_COMPLIANCE_WEB_APP_1/CODE_COMPLIANCE_WEB_APP/code_compliance/compliance_app/templates

Running the CodeCompliance Web application

Go to /PATH_MAIN_DIRECTORY/CODE_COMPLIANCE_WEB_APP_1/CODE_COMPLIANCE_WEB_APP/code_compliance
Run below command
python manage.py runserver 192.168.XX.XXX:8000
Open in browser 
192.168.XX.XXX:8000/uploads

Important points:

Please ensure that there is only ONE main function across all input files uploaded



**Running checks using CodeChecker without the web application**

To run the checks using the CodeChecker alone do the following steps: 

Ensure that you have enabled the code checker venv environment 
Run the following command using the input file path: 
CodeChecker check -b "g++ /PATH_TO_INPUT_FILE --output ./results --analyzers clang-tidy -e compliance
The below command will export the result in report files : 
CodeChecker parse -e html ./results -o ./reports_html


Before running the above commands please ensure that the codechecker and clang-tidy are properly integrated.
