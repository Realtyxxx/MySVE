#include <string>

namespace pooling {
enum class PoolingType {
  AVERAGE,
  MAX,
};

enum class PoolingMethod {
  DEFAULT,
  DEPTHFIRST,
  PLANAR,
};

struct PoolingWindow {
  unsigned int rows, cols;
};

struct PoolingStride {
  unsigned int rows, cols;
};

struct PaddingValues {
  unsigned int left, top, right, bottom;
};

struct PoolingConfig {
  PoolingMethod method = PoolingMethod::DEFAULT;
  std::string   filter = "";

  PoolingConfig(PoolingMethod method)
      : method(method){};
  PoolingConfig(){};
};

template <typename T>
inline T roundup(const T a, const T b) {
  T rem = a % b;

  if (rem) {
    return a + b - rem;
  } else {
    return a;
  }
}

}  // namespace pooling
