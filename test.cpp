#include <bits/stdc++.h>
using namespace std;
class A{
public:
    virtual void foo() {
        cout<<"from A"<<endl;
    }
};
class B: public A{
public:
    virtual void foo() {
        cout<<"from B"<<endl;
    }
};

int main() {
    A* a = new B();
    a->foo();
}
