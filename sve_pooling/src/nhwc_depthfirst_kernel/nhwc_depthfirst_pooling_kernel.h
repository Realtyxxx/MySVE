#include <cstdint>
void naive_fp32_nhwc_max_generic_depthfirst_impl(
    const uint64_t,
    const uint64_t            hw_num,
    uint64_t                  n_channels,
    const float *const *const inptrs,
    float *                   outptr,
    void *);

void sve_fp32_nhwc_max_generic_depthfirst_impl(
    const uint64_t,
    const uint64_t            n_valid_cells,
    uint64_t                  n_channels,
    const float *const *const inptrs,
    float *                   outptr,
    void *);
