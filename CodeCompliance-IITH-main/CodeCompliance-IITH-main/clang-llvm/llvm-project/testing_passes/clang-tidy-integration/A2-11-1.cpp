#include <iostream>

// Global volatile variable
volatile int globalVar = 0; // Non-compliant

// Structure with a volatile member
struct VolatileStruct {
    volatile double volatileMember; // Non-compliant
};

// Function with a volatile parameter
void volatileFunction(volatile bool flag) { // Non-compliant
    if (flag) {
        std::cout << "Flag is true" << std::endl;
    } else {
        std::cout << "Flag is false" << std::endl;
    }
}

int main() {
    // Local volatile variable
    volatile float localVar = 3.14f; // Non-compliant
    return 0;
}