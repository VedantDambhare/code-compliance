// RUN: %check_clang_tidy %s misc-passes %t

// FIXME: Add something that triggers the check here.

int Fn1() noexcept {
    return 0;
}

void Fn2() noexcept {
    
    Fn1(); // Non-compliant as return value is not captured and used.

}

// CHECK-MESSAGES: :[[@LINE-1]]:5: warning: AUTOSAR C++ (Rule A0-1-2): Violated -The value returned by a function %Fn1 having a non-void return type that is not an overloaded operator shall be used. [misc-passes]

// FIXME: Verify the applied fix.
//   * Make the CHECK patterns specific enough and try to make verified lines
//     unique to avoid incorrect matches.
//   * Use {{}} for regular expressions.
// CHECK-FIXES: {{^}} int x = Fn1(); {{$}}

// FIXME: Add something that doesn't trigger the check here.


int Fn1() noexcept {
    return 0;
}

void Fn2() noexcept {
    
    int x = Fn1(); // Compliant
}

