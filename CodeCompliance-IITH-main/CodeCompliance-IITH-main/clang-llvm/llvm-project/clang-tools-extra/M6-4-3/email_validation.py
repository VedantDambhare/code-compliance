import re 

def check_email(email):
    
    regex = r'([A-Za-z0-9]+[.-_])*[A-Za-z0-9]+@[A-Za-z0-9-]+(\.[A-Z|a-z]{2,7})+'
    # reg_2 = r'[A-Z0-9_!#$%&'*+/=?`{|}~^.-]+@[A-Z0-9.-]+$'
    reg_cgpt = r'[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'
    if (re.match(reg_cgpt, email)) == True:
        return True
    else:
        return False

def is_valid_email(email):
    # Define the regular expression pattern
    pattern = r"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$"
    
    # Use re.match to check if the email matches the pattern
    if re.match(pattern, email):
        return True
    else:
        return False


print(check_email('test@test.com'),is_valid_email('test@test.com'))