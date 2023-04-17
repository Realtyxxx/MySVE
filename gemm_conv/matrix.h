
#include "args.h"
template <typename dtype>
class Matrix {
 public:
  Matrix()                        = delete;
  Matrix(Matrix &other)           = delete;
  Matrix(Matrix &&other)          = delete;
  Matrix &operator=(Matrix other) = delete;
  ~Matrix() {
    if (ptr)
      delete[] ptr;
  }

  Matrix(int C, int H, int W) : c(C), h(H), w(W) { ptr = new dtype[C * H * W]; }

  inline int size() const { return h * w * c; }
  dtype     *get() const { return ptr; }

  inline int H() const { return h; }
  inline int W() const { return w; }
  inline int C() const { return w; }

  void init(int flag, dtype val) {
    switch (flag) {
      case 0:
        for (int i = 0; i < size(); ++i) { ptr[i] = val; }
        break;
      case 1:
        for (int i = 0; i < size(); ++i) { ptr[i] = val++; }
        break;
      default: {
        static auto getVal = []() { return rand() / 1000.f; };
        for (int i = 0; i < size(); ++i) { ptr[i] = getVal(); }
      }
    }
  }
  void print(int align = 0) const {
    for (int k = 0; k < c; ++k) {
      dtype *i_ptr = ptr + k * h * w;
      for (int i = 0; i < h; ++i) {
        dtype *d = i_ptr + i * w;
        for (int j = 0; j < w; ++j) {
          std::cout << d[j] << " ";
          if (align && (j + 1) % align == 0) {
            std::cout << "||";
          }
        }
        std::cout << "\n";
      }
    }
  }
  void print(const char *msg, int align = 0) const {
    printf("%s : \n", msg);
    for (int k = 0; k < c; ++k) {
      dtype *i_ptr = ptr + k * h * w;
      for (int i = 0; i < h; ++i) {
        dtype *d = i_ptr + i * w;
        for (int j = 0; j < w; ++j) {
          std::cout << d[j] << " ";
          if (align && (j + 1) % align == 0) {
            std::cout << "||";
          }
        }
        std::cout << "\n";
      }
    }
  }

 private:
  int    c;
  int    h;
  int    w;
  dtype *ptr;
};
