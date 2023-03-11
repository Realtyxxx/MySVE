#include <float.h>

// void shl_ref_maxpool2d_nhwc_f32(struct csinn_tensor *input, struct csinn_tensor *output,
//                                       struct csinn_pool_params *params)
void nchw_maxpooling(float *input, float *output, const int b, const int c, const int h, const int w, const int oh, const int ow, const int ph, const int pw, const int sh, const int sw, const int fh, const int fw) {
  float    *input_data    = input;
  float    *output_data   = output;
  const int batches       = b;
  const int depth         = c;
  const int input_height  = h;
  const int input_width   = w;
  const int output_height = oh;
  const int output_width  = ow;

  for (int batch = 0; batch < batches; ++batch) {
    for (int out_y = 0; out_y < output_height; ++out_y) {
      for (int out_x = 0; out_x < output_width; ++out_x) {
        for (int channel = 0; channel < depth; ++channel) {
          const int in_x_origin = (out_x * sw) - pw;
          const int in_y_origin = (out_y * sh) - ph;
          // Compute the boundaries of the filter region clamped so as to
          // ensure that the filter window fits in the input array.
          const int filter_x_start = (0 >= -in_x_origin) ? 0 : -in_x_origin;
          const int filter_x_end   = (fw <= input_width - in_x_origin) ? fw : input_width - in_x_origin;
          const int filter_y_start = (0 >= -in_y_origin) ? 0 : -in_y_origin;
          const int filter_y_end =
              (fh <= input_height - in_y_origin) ? fh : input_height - in_y_origin;
          float max        = -FLT_MAX;
          int   filter_cnt = 0;
          for (int filter_y = filter_y_start; filter_y < filter_y_end; ++filter_y) {
            for (int filter_x = filter_x_start; filter_x < filter_x_end; ++filter_x) {
              const int in_x = in_x_origin + filter_x;
              const int in_y = in_y_origin + filter_y;

              max = fmax(max, input_data[((batch * h + in_y) * w + in_x) * depth + channel]);
              filter_cnt++;
            }
          }
          // consider padding with constant 0
          if (filter_cnt != fh * fw) {
            max = fmax(max, 0);
          }
          // output_data[shl_ref_get_index(output->dim, batch, out_y, out_x, channel)] = max;
          output_data[(((batch * oh + out_y)* ow + out_x) * depth + channel] = max;
        }
      }
    }
  }
}
