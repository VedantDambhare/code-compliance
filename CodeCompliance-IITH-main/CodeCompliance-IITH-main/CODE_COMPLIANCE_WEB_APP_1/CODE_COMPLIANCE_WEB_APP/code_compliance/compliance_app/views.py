import datetime
from django.http import HttpResponse
from django.http import JsonResponse
from django.shortcuts import render
import os
import subprocess
import zipfile
import re
def index(request):
    # if request.method == 'POST':
    #     uploaded_files = request.FILES.getlist('files')
    #     selected_option = request.POST.get('option')

    #     # For demonstration purposes, let's process only .cpp files
    #     cpp_files = []
    #     for file in uploaded_files:
    #         if file.name.endswith('.cpp'):
    #             cpp_files.append(file)
    #         else:
    #             print("Skipped non-.cpp file:", file.name)
                
    #     # Handle the selected option and the list of .cpp files here
    #     for cpp_file in cpp_files:
    #         # Process or save the .cpp file here
    #         print("Uploaded .cpp File:", cpp_file.name)
    #     print("Selected Option:", selected_option)
    # code_compliance/compliance_app/templates

    return render(request, "home.html")

    html = "<html><body>LEtys goooo!</body></html>"
    return HttpResponse(html)
    return render(request, "code_compliance/compliance_app/templates/index.html")

    return render(request, 'compliance_app/templates/index.html')

def current_datetime(request):
    now = datetime.datetime.now()
    html = "<html><body>It is now %s.</body></html>" % now
    return HttpResponse(html)
    

def upload_files(request):
    result = subprocess.run(['pwd'], stdout=subprocess.PIPE).stdout.decode('utf-8')
    result = result.rstrip()+'compliance_app/templates/'
    if request.method == 'POST':
        option = request.POST.get('option')
        files = request.FILES.getlist('files')

        # Handle the uploaded files
        for file in files:
            file_path = os.path.join(result + 'test_files_for_webapp', file.name)
            with open(file_path, 'wb+') as destination:
                for chunk in file.chunks():
                    destination.write(chunk)
        if 'upload_button' in request.POST:
            # if option == 'option1':
            script_path = result + 'call_codechecker.py'
            # elif option == 'option2':
                # script_path = 'path_to_your_script/script2.py'
            # elif option == 'option3':
                # script_path = 'path_to_your_script/script3.py'
            
            # Call the Python script using subprocess
            subprocess.call(['python', script_path])

        # Invoke your Python script based on the selected option
        # if option == 'option1':
        #     # Call your script or function for Option 1
        #      script_path = '/home/cse/Desktop/CODE_COMPLIANCE_WEB_APP/code_compliance/compliance_app/templates/call_codechecker.py'
        # elif option == 'option2':
        #     # Call your script or function for Option 2
        #     pass
        # elif option == 'option3':
        #     # Call your script or function for Option 3
        #     pass
        # subprocess.call(['python', script_path])

    return render(request, 'index.html')


def run_script(request):
    result = subprocess.run(['pwd'], stdout=subprocess.PIPE).stdout.decode('utf-8')
    result = result.rstrip()+'/compliance_app/templates/'
    if request.method == 'POST':
        # print("yes")
        files = request.FILES.getlist('files')
        option = request.POST.get('option')
        for file in files:
            with open(result + file.name, 'wb+') as destination:
                for chunk in file.chunks():
                    destination.write(chunk)
        # if option == 'AUTOSAR':
        script_path = result + 'call_codechecker.py'
        # elif option == 'MISRA':
        #     script_path = 'path_to_your_script/script2.py'
        # elif option == 'ISO':
        #     script_path = 'path_to_your_script/script3.py'
        # Run Python script using subprocess
        result = subprocess.call(['python3', script_path])
        print(result)
            
        return JsonResponse({'result': result})
    
    return render(request, 'index.html')


def download_folder(request):
    result = subprocess.run(['pwd'], stdout=subprocess.PIPE).stdout.decode('utf-8')
    result = result.rstrip()+'/reports_html'
    folder_to_zip = result  # Folder to be zipped
    zip_filename = 'reports_html.zip'
    
    # Create a BytesIO stream to store the zip content
    response = HttpResponse(content_type='application/zip')
    response['Content-Disposition'] = f'attachment; filename="{zip_filename}"'

    with zipfile.ZipFile(response, 'w') as zipf:
        for root, _, files in os.walk(folder_to_zip):
            for file in files:
                file_path = os.path.join(root, file)
                zipf.write(file_path, os.path.relpath(file_path, folder_to_zip))

    return response

# def open_local_html(request):
#     html_path = '//home/cse/Desktop/CODE_COMPLIANCE_WEB_APP_1/CODE_COMPLIANCE_WEB_APP/code_compliance/reports_html/index.html'  # Replace with your local file path
#     with open(html_path, 'r') as file:
#         content = file.read()
#     return HttpResponse(content)

# def open_local_html_statistics(request):
#     html_path = '//home/cse/Desktop/CODE_COMPLIANCE_WEB_APP_1/CODE_COMPLIANCE_WEB_APP/code_compliance/reports_html/statistics.html'  # Replace with your local file path
#     with open(html_path, 'r') as file:
#         content = file.read()
#     return HttpResponse(content)

# def open_local_html_error(request):
#     directory_path = '//home/cse/Desktop/CODE_COMPLIANCE_WEB_APP_1/CODE_COMPLIANCE_WEB_APP/code_compliance/reports_html'  # Replace with the actual directory path
#     filename_regex = re.compile(r'^(?!.*clang-tidy).*$')

#     matched_file = None
#     for filename in os.listdir(directory_path):
#         if filename_regex.match(filename):
#             matched_file = filename
#             break

#     if matched_file:
#         file_path = os.path.join(directory_path, matched_file)
#         with open(file_path, 'rb') as file:
#             response = HttpResponse(file.read(), content_type='application/octet-stream')
#             response['Content-Disposition'] = f'attachment; filename="{matched_file}"'
#             return response
#     else:
#         return HttpResponse('File not found', status=404)
#    directory_path = '/path/to/your/files/'  # Replace with the actual directory path    filename_regex = re.compile(r'^(?!.*clang-tidy).*$')    matched_file = None    for filename in os.listdir(directory_path):        if filename_regex.match(filename):            matched_file = filename            break    if matched_file:        file_path = os.path.join(directory_path, matched_file)        with open(file_path, 'rb') as file:            response = HttpResponse(file.read(), content_type='application/octet-stream')            response['Content-Disposition'] = f'attachment; filename="{matched_file}"'            return response    else:        return HttpResponse('File not found', status=404)
    # if re.match(r'^(?!.*clang-tidy).*$', filename)
    # html_path = '//home/cse/Desktop/CODE_COMPLIANCE_WEB_APP_1/CODE_COMPLIANCE_WEB_APP/code_compliance/reports_html/index.html'  # Replace with your local file path
    # with open(html_path, 'r') as file:
    #     content = file.read()
    # return HttpResponse(content)
