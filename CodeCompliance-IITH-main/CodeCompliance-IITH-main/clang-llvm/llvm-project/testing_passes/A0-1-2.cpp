
int Fn1() noexcept {
    return 0;
}

void Fn2() noexcept {
    
    int x = Fn1(); // Compliant
    Fn1(); // Non-compliant as return value is not captured and used.

}

int main() {
	Fn2();
}
