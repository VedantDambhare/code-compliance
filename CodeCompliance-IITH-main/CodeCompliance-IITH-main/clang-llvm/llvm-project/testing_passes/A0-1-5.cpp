class Base {
    // private member variable
    int x;
 
public:
    // pure virtual function
    virtual void fun(int data) = 0;
 
    // getter function to access x
    int getX() { return x; }

};
 
// This class inherits from Base and implements fun()
class Derived : public Base {
    // private member variable
    int y;
 
public:
    // implementation of the pure virtual function wiht unused parameter NON - COMPLIANT
    void fun(int) override {
        y = 1;
    }
};

class abc : public Base {
    int y;
    public:
    
    // implementation of the pure virtual function with parameter used COMPLIANT
    void fun(int data) override {
        y = data;
    }

};