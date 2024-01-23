

int foo() {
    return 0;
}

void test() {

    int test1 = 0;
    int x = 0;
    int y = 0;
    if (x = 0) { // Non compliant 
        x = 1;
    }
    if (test1) {
        x = 1;
    }

    if (int i = foo()){ // Compliant 
        x = 1;
    }

    x = y = test1; // Non compliant 
    bool b1 = x != y;
    
    bool b2 = x != y != test1; 

}
