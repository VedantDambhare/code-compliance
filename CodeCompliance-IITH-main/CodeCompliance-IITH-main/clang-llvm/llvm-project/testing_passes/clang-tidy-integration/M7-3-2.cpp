int main(){
// Compliant
    return 0;
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