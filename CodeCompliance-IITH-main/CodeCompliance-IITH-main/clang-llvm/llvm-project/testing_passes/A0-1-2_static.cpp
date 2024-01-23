int Fn1() noexcept {
    return 0;
}

void Fn2() noexcept {
    
    static_cast<void>(Fn1()); // Compliant by exception

}