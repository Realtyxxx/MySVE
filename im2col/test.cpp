#include <iostream>
#include <memory>

using namespace std;

class Deleter {
 public:
  void operator()(float *ptr) {
    cout << "call Deleter\n";
    delete[] ptr;
  }
};

class A {
 public:
  A(int size) {
    cout << "init\n";
    ptr = make_shared<float *>((new float[size]));
  }
  ~A() { cout << "delete\n"; }

 private:
  shared_ptr<float *> ptr;
};
int main() {
  A a(10);
}
