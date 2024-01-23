// Test Case 2: 'else' clause is present
// #include<iostream>
void ifElseIfChainWithElse(int value) {
    if (value == 1) {
        // std::cout<<"No";
    }
    else if (value == 2) {
        // std::cout<<"Yes";
    }
    else {
        // std::cout<<"None";
}
}
 int main() {
    // Testing the functions
    int value = 3;


    ifElseIfChainWithElse(value);    // This should not raise any violation
    return 0;
 }