 #include <iostream>

// Test Case 1: Missing 'else' clause
void ifElseIfChainWithoutElse(int value) {
    if (value == 1) {
        std::cout << "TRUE";
    }
    else if (value == 2) {
        std::cout<<"FALSE";
    }
    // Missing 'else' clause
}


 int main() {
    // Testing the functions
    int value = 3;

    ifElseIfChainWithoutElse(value); // This should raise a violation

    return 0;
    }