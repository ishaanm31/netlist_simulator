#include <bits/stdc++.h>
using namespace std;

class A {
int x;
int y;
public:
    A() {
        cout<<"A ka constructor"<<endl;
    }
    ~A() {
        cout<<"A ka distructor"<<endl;
    }
};
class B: public A {
int a;
int b;
public:
    B() {
        cout<<"B ka constructor"<<endl;
    }
    ~B() {
        cout<<"B ka distructor"<<endl;
    }
};
int main(){
    A* a = new B();
    delete ((B*)a);

}