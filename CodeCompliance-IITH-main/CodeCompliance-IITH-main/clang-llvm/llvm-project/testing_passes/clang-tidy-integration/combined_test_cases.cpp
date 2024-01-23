#include <cstdint>
#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>

namespace NS1 {
class G {};
void G() {} // non-compliant, hides class G
} // namespace NS1

namespace NS2 {
enum class H {
  VALUE = 0,
};
std::uint8_t H = 17; // non-compliant, hides scoped enum H
} // namespace NS2

namespace NS3 {
class J {};
enum H {
  J = 0, // non-compliant, hides class J
};
} // namespace NS3


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


enum class Enum1 : std::uint32_t
{
    One,
    Two = 2, // Non-compliant
    Three
};

enum class Enum2 : std::uint32_t // Compliant (none)
{
   One,
   Two,
   Three
};

enum class Enum3 : std::uint32_t // Compliant (the first)
{
   One = 1,
   Two,
   Three
};

enum class Enum4 : std::uint32_t // Compliant (all)
{
    One = 1,
    Two = 2,
    Three = 3
};


std::int32_t Fn1(std::int32_t b) noexcept
{
    std::int32_t ret = 0;
    // ...
    asm("pushq %%rax \n"
    "movl %0, %%eax \n"
    "addl %1, %%eax \n"
    "movl %%eax, %0 \n"
    "popq %%rax"
    :"=r"(ret)
    : "r"(b)); // Non-compliant
    return ret;
}
std::int32_t Fn2(std::int32_t b) noexcept
{
    std::int32_t ret = 0;
    // ...
    ret += b; // Compliant - equivalent to asm(...) above
    return ret;
}


void ifcheck() {
    int x = 2;

    if (x > 2) {

    }
    else if (x > 3) {

    }
    else {
            // Compliant   
    }

    int y = 5;
    if (y > 3) {

    } 
    else if (y > 2) {

    }
}

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

namespace {
    // Non-Compliant
    int main() {
        return 0;
    }
}

namespace NS {
    // Non - Compliant
    int main() {    
        return 0;
    }
}

int main(void) {
  class J {};
  int J = 0; // non-compliant
  
  
  long double a = 3.14159265358979323846L; // Non-compliant
  long double expResult = std::exp(a);  // Non-compliant
  long double b = 2.71828182845904523536L; // Non-compliant
  long double sum = a + b; // Non-compliant
  long double f3{0.1L}; // Non-compliant
  long double *ptr = &a; // Non-compliant
  

  // Local volatile variable
  volatile float localVar = 3.14f; // Non-compliant
  return 0;

}