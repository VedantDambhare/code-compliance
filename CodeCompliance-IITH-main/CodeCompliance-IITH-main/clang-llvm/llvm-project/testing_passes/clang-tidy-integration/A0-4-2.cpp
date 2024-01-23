#include <bits/stdc++.h>    

void assign(long double a, long double b){ // Non-compliant
      a = b;
}
void modifyValue(long double *value){ // Non-compliant
    *value *= 2;
}

long double calculateCircleArea(long double radius); // Non-compliant

long double calculateCircleArea(long double radius) { // Non-compliant
    return 3.141592653589793238462643383279502884 * radius * radius;
}

int main()  
{  
    long double a = 3.14159265358979323846L; // Non-compliant
    long double expResult = std::exp(a);  // Non-compliant
    long double b = 2.71828182845904523536L; // Non-compliant
    long double sum = a + b; // Non-compliant
    long double f3{0.1L}; // Non-compliant
    long double *ptr = &a; // Non-compliant
}  