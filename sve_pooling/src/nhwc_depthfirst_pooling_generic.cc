#include "nhwc_depthfirst_kernel/nhwc_depthfirst_pooling_kernel.h"
#include "pooling_generic.h"

/* Compute a portion of the output tensor with no padding.
   *
   * The default implementation of this repeatedly calls into the padded
   * variant.
   */
template <class TInput, class TOutput>
void PoolingCommon<TInput, TOutput>::compute_tiles_unpadded(
    unsigned int start_output_i, unsigned int start_output_j,
    unsigned int n_tile_rows, unsigned int n_tile_cols,
    unsigned int output_channel_start, unsigned int output_channel_end,
    const TensorSpec<const TInput *> &input,
    const TensorSpec<TOutput *> &     output,
    void *                            working_space) const {
  for (unsigned int tile_i = 0; tile_i < n_tile_rows; tile_i++) {
    this->compute_row_padded_tile_row(
        start_output_i, start_output_j, n_tile_cols,
        output_channel_start, output_channel_end,
        input, output, working_space);
    start_output_i += m_strat->get_output_rows();
  }
}

template <class TInput, class TOutput>
void PoolingCommon<TInput, TOutput>::compute_tile_padded(
    unsigned int output_i, unsigned int output_j,
    unsigned int channel_start, unsigned int channel_end,
    const TensorSpec<const TInput *> &input,
    const TensorSpec<TOutput *> &     output,
    void *) const {
  // Determine start position and padding
  const int  start_i    = static_cast<int>(output_i * this->m_args.pool_stride.rows) - this->m_args.padding.top;
  const auto input_i    = static_cast<unsigned int>(start_i < 0 ? 0 : start_i);
  const auto pad_top    = static_cast<unsigned int>(start_i < 0 ? -start_i : 0);
  const int  end_i      = start_i + this->m_args.pool_window.rows;
  const auto pad_bottom = static_cast<unsigned int>((unsigned int)end_i < this->m_args.input_rows ? 0 : end_i - this->m_args.input_rows);
  const auto valid_rows = this->m_args.pool_window.rows - (pad_top + pad_bottom);

  const int  start_j    = static_cast<int>(output_j * this->m_args.pool_stride.cols) - this->m_args.padding.left;
  const auto input_j    = static_cast<unsigned int>(start_j < 0 ? 0 : start_j);
  const auto pad_left   = static_cast<unsigned int>(start_j < 0 ? -start_j : 0);
  const int  end_j      = start_j + this->m_args.pool_window.cols;
  const auto pad_right  = static_cast<unsigned int>((unsigned int)end_j < this->m_args.input_cols ? 0 : end_j - this->m_args.input_cols);
  const auto valid_cols = this->m_args.pool_window.cols - (pad_left + pad_right);

  // Determine the number of valid cells and prepare the pointers
  const auto n_valid_cells = valid_rows * valid_cols;
  auto       inptrs        = reinterpret_cast<const TInput **>(alloca(n_valid_cells * sizeof(TInput *)));  //TYX_NOTES: where kernel input and output made;
  {
    auto my_ptr  = inptrs;
    auto row_ptr = input.base + input_i * input.ld_row + input_j * input.ld_col + channel_start;
    for (auto i = valid_rows; i; i--) {
      auto ptr = row_ptr;
      row_ptr += input.ld_row;

      for (auto j = valid_cols; j; j--) {
        *(my_ptr++) = ptr;
        ptr += input.ld_col;
      }
    }
  }

  auto outptr = output.base + output_i * output.ld_row + output_j * output.ld_col + channel_start;

  // Some padding variants include (or exclude) the padding values; we handle
  // this by computing the extent of the padded input tensor and hence
  // computing the total number of cells captured in the pooling window.
  const auto bottom_padded_height = this->m_args.input_rows + this->m_args.padding.bottom;
  const auto captured_rows        = std::min<int>(end_i, bottom_padded_height) - start_i;
  const auto right_padded_width   = this->m_args.input_cols + this->m_args.padding.right;
  const auto captured_cols        = std::min<int>(end_j, right_padded_width) - start_j;
  const auto captured_cells       = captured_rows * captured_cols;
  const auto window_cells         = this->m_args.exclude_padding ? n_valid_cells : captured_cells;

  // Execute the kernel
  sve_fp32_nhwc_max_generic_depthfirst_impl(window_cells, n_valid_cells, channel_end - channel_start, inptrs, outptr, nullptr);  //TODO: only max generic only
}

// Compute a portion of the work with only top/bottom padding.
template <class TInput, class TOutput>
void PoolingCommon<TInput, TOutput>::compute_row_padded_tile_row(
    const unsigned int output_i, unsigned int output_j, unsigned int n_tile_cols,
    const unsigned int channel_start, const unsigned int channel_end,
    const TensorSpec<const TInput *> &input,
    const TensorSpec<TOutput *> &     output,
    void *                            working_space) const {
  // Determine start position and padding
  const int  start_i    = static_cast<int>(output_i * this->m_args.pool_stride.rows) - this->m_args.padding.top;
  const auto input_i    = static_cast<unsigned int>(start_i < 0 ? 0 : start_i);
  const auto pad_top    = static_cast<unsigned int>(start_i < 0 ? -start_i : 0);
  const int  end_i      = start_i + this->m_args.pool_window.rows;
  const auto pad_bottom = static_cast<unsigned int>((unsigned int)end_i < this->m_args.input_rows ? 0 : end_i - this->m_args.input_rows);
  const auto valid_rows = this->m_args.pool_window.rows - (pad_top + pad_bottom);  //TYX_COMMENTS:有效执行的数目

  const int  start_j    = static_cast<int>(output_j * this->m_args.pool_stride.cols) - this->m_args.padding.left;
  const auto input_j    = static_cast<unsigned int>(start_j < 0 ? 0 : start_j);
  const auto valid_cols = this->m_args.pool_window.cols;  //TYX_COMMENT: 本身就是排除了row

  // Determine the number of valid cells and prepare the pointers
  const auto n_valid_cells = valid_rows * valid_cols;                                                      //TYX_COMMENT: valid_cells is the padding square
  auto       inptrs        = reinterpret_cast<const TInput **>(alloca(n_valid_cells * sizeof(TInput *)));  //TYX_COMMENT: alloca a region to have the look at the position, 栈内存
  {                                                                                                        // 用二维的指针将数据连续存储，先列方向连续再行方向连续
    auto my_ptr  = inptrs;
    auto row_ptr = input.base + input_i * input.ld_row + input_j * input.ld_col + channel_start;  //TYX_COMMENT: 找到开始坐标
    for (auto i = valid_rows; i; i--) {
      auto ptr = row_ptr;
      row_ptr += input.ld_row;

      for (auto j = valid_cols; j; j--) {
        *(my_ptr++) = ptr;  //TYX_COMMENT: inptrs is (float)[point in one valid_cells][channel]
        ptr += input.ld_col;
      }
    }
  }

  auto outptr = output.base + output_i * output.ld_row + output_j * output.ld_col + channel_start;

  // Some padding variants include (or exclude) the padding values; we handle
  // this by computing the extent of the padded input tensor and hence
  // computing the total number of cells captured in the pooling window.
  const auto bottom_padded_height = this->m_args.input_rows + this->m_args.padding.bottom;
  const auto captured_rows        = std::min<int>(end_i, bottom_padded_height) - start_i;
  const auto captured_cells       = captured_rows * valid_cols;
  const auto window_cells         = this->m_args.exclude_padding ? n_valid_cells : captured_cells;  //TYX_COMMENT: in max kernel is useless

  for (; n_tile_cols; n_tile_cols--)  // TYX_COMMENT: n_tile_cols' pooling area
  {
    // Execute the kernel
    sve_fp32_nhwc_max_generic_depthfirst_impl(window_cells, n_valid_cells, channel_end - channel_start, inptrs, outptr, nullptr);

    // Update the pointers; the output strides by a column and the inputs
    // stride by a number of columns.
    outptr += output.ld_col;
    for (auto n = 0u; n < n_valid_cells; n++)  //TYX_COMMENT:移动到下一个计算点
    {
      inptrs[n] += this->m_args.pool_stride.cols * input.ld_col;
    }
  }
}

template <class TInput, class TOutput>
void PoolingCommon<TInput, TOutput>::execute_internal(
    unsigned int         n_batches,
    unsigned int         input_height,
    unsigned int         input_width,
    unsigned int         n_channels,
    const PaddingValues &padding,
    const void *         input,
    size_t               ld_input_col,
    size_t               ld_input_row,
    size_t               ld_input_batch,
    unsigned int         output_height,
    unsigned int         output_width,
    void *               output,
    size_t               ld_output_col,
    size_t               ld_output_row,
    size_t               ld_output_batch,
    void *               working_space,
    unsigned int         thread_id,
    unsigned int         n_threads) const {

  // void *thread_working_space =
  //     static_cast<uint8_t *>(working_space) + thread_id * this->get_working_size_per_thread(n_channels);
  // this->initialise_working_space(thread_working_space, n_channels);

  void *thread_working_space =
      static_cast<uint8_t *>(working_space);

  // Construct convenient representations of the input/output tensors.
  TensorSpec<const TInput *> input_tensor(reinterpret_cast<const TInput *>(input), ld_input_row, ld_input_col);
  TensorSpec<TOutput *>      output_tensor(reinterpret_cast<TOutput *>(output), ld_output_row, ld_output_col);

  // If the output is a 1x1 tensor, which commonly occurs at the end of a
  // network, then we change the threading strategy to parallelise over
  // channels rather than rows of the tensor.
  if (n_threads > 1 && output_height == 1 && output_width == 1) {
    // Determine how many channels should be assigned to each thread, we
    // round up first to ensure we get a reasonable spread across the
    // threads.
    const auto channels_per_thread = pooling::roundup(pooling::roundup(n_channels, 16u), n_threads) / n_threads;
    const auto start_channel       = thread_id * channels_per_thread;
    const auto end_channel         = std::min(start_channel + channels_per_thread, n_channels);

    if (start_channel >= end_channel) {
      // This thread should move on if we have insufficient work to do.
      return;
    }

    for (; n_batches; n_batches--) {
      // We know we don't need to iterate over rows or columns here; so just
      // execute the tile.
      this->compute_tile_padded(
          0, 0,  // Compute the only output point
          start_channel, end_channel,
          input_tensor, output_tensor, thread_working_space);

      // Progress the pointers for the next batch.
      input_tensor.base += ld_input_batch;
      output_tensor.base += ld_output_batch;
    }

    // Exit here, since we've done all the work using the different strategy.
    return;
  }

  for (unsigned int batch = 0; batch < n_batches; batch++) {  //TYX_COMMENT: batch(batch) , i(height), j(width), from output to whole
    // Iterate over rows of the output tensor; we stripe over the tiles.
    for (unsigned int start_output_i = thread_id * m_strat->get_output_rows();  //TYX_LEARNING:pooling:for depthwise generic, get_output_row is 1
         start_output_i < output_height;
         start_output_i += n_threads * m_strat->get_output_rows()) {  //TYX_COMMENT: deal the rows, judge if pad
      // Determine what (if any padding) is required on the top/bottom of
      // this row of the convolution.
      const auto end_output_i      = start_output_i + m_strat->get_output_rows();
      const bool pad_output_bottom = output_height < end_output_i;

      const int  start_input_i    = start_output_i * this->m_args.pool_stride.rows - padding.top;
      const bool pad_input_top    = start_input_i < 0;
      const int  end_input_i      = start_input_i + m_strat->get_input_rows();
      const bool pad_input_bottom = static_cast<int>(input_height) < end_input_i;
      const bool pad_row          = pad_input_top || pad_input_bottom || pad_output_bottom;

      // Iterate over the columns of the output tensor; we attempt to grab as
      // much as possible of the unpadded regions, so the loop structure is a
      // bit odd.
      unsigned int start_output_j = 0;
      while (start_output_j < output_width) {                                                       //TYX_COMMENT: deal the cols
        const int  start_in_j     = start_output_j * this->m_args.pool_stride.cols - padding.left;  //TYX_COMMENT: in_start is start oj * stride - padding
        const bool pad_input_left = start_in_j < 0;

        // Determine if we can process a number of unpadded tiles in one go.
        int n_unpadded_tiles = 0;
        if (!pad_input_left) {
          // Determine the maximum number of tiles we could handle.
          n_unpadded_tiles = (output_width - start_output_j) / m_strat->get_output_cols();  //TYX_COMMENT: output_width: 这里的kernel需要计算的 输出宽度， 然后 减去 开始得到 需要计算多少个输出， 再除以每个pooling可以得到的输出数目，得到需要计算多少个窗口，

          // Handle padding on the right hand edge
          const int tile_stride  = m_strat->get_output_cols() * this->m_args.pool_stride.cols;                     //TYX_COMMENT: output col * pool cols = compute_length total
          int       end_output_j = start_output_j + n_unpadded_tiles * m_strat->get_output_cols();                 //TYX_COMMENT: 然后通过width方向开始 +　计算窗口数量 *　窗口输出大小得到最终的output尾端
          int       end_input_j  = start_in_j + m_strat->get_input_cols() + (n_unpadded_tiles - 1) * tile_stride;  //TYX_COMMENT: 每一个window中的输入的末尾元素是开始 + 每个window的长，再加个每个元素由于stride产生的额外长

          while (n_unpadded_tiles > 0 &&
                 (static_cast<int>(output_width) < end_output_j ||  //TYX_COMMENT: output right padding
                  static_cast<int>(input_width) < end_input_j))     //TYX_COMMENT: input right padding
          {                                                         //TYX_COMMENT: 除去右边padding部分，也验证了，tile是输出的个数。
            n_unpadded_tiles--;                                     //TYX_COMMENT: 如果为0, 也有可能会是右边部分
            end_output_j -= m_strat->get_output_cols();
            end_input_j -= tile_stride;
          }
        }

        // Process unpadded tiles, if possible, otherwise process a padded tile.
        if (n_unpadded_tiles)  //TYX_COMMENT:还存在没padding的部分
        {
          if (!pad_row)  //TYX_COMMENT, calling the same way
          {
            // Completely unpadded execution
            compute_tiles_unpadded(              //TYX_COMMENT:in the middle, pooling could have focus here first
                start_output_i, start_output_j,  //TYX_COMMENT: 开始的 h, w
                1, n_unpadded_tiles,             // Compute a row of unpadded tiles
                0, n_channels,                   // Compute all channels
                input_tensor, output_tensor, thread_working_space);
          } else {
            // Top/bottom padding only
            this->compute_row_padded_tile_row(
                start_output_i, start_output_j, n_unpadded_tiles,
                0, n_channels,  // Compute all channels
                input_tensor, output_tensor, thread_working_space);
          }
          start_output_j += n_unpadded_tiles * m_strat->get_output_cols();
        } else  //TYX_COMMENT: also padded left
        {
          this->compute_tile_padded(
              start_output_i, start_output_j,
              0, n_channels,  // Compute all channels
              input_tensor, output_tensor, thread_working_space);
          start_output_j += m_strat->get_output_cols();
        }
      }
    }

    // Progress the pointers for the next batch.
    input_tensor.base += ld_input_batch;
    output_tensor.base += ld_output_batch;
  }
}

template class PoolingCommon<float, float>;