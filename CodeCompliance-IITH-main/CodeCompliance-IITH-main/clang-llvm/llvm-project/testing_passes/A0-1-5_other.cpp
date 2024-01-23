class Base {
    // private member variable
    int x;
 
public:
    // pure virtual function
    virtual void fun(int data) = 0;
 
    // getter function to access x
    int getX() { return x; }

};
 
class abc : public Base {
    int y;
    public:
    
    // implementation of the pure virtual function with parameter used COMPLIANT
    void fun()  {
        y = 1;
    }

};


// Function not overriden 
class xyz : public Base {
    
    int y;
    public:

    void print(){
        int x = 1;
    }
};