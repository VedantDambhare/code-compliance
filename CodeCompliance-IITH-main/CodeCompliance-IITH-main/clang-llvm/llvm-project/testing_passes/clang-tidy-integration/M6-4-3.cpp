

void test() {
    int x = 1;
    switch(x) {
        case 0:  
            x+=1; // Non compliant - No break statement in a non empty case
        case 1: 
        case 2:
            break;
        // Non compliant - Do default Statement
            
    }
}
int main() {
    test();
}
