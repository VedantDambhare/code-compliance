
void f1() {
    int j = 0;
    goto L1;

    // Non compliant
    for (j=0; j<10; ++j) {
        L1:j;
    }
}

void f2() {

    for (int j=0;j < 10;++j){
        for(int i=0; i< 10; ++i) {
            goto L1;
        }
    }
    L1: f1();

}
int main() {

    f1();
    f2();

}