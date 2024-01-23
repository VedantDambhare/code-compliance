int Fn1() noexcept {
    return 0;
}

void Fn2() noexcept {
    
    // Both are compliant as they are being used in if and while.
    if (Fn1()) {

    }
    
    while(Fn1()) {

    }

    
}