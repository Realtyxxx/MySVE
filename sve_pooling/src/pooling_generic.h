/**
 * @Author       : Realtyxxx
 * @version      : 1.0
 * @Date         : 2022-12-03 15:15:19
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-12-08 23:53:20
 * @FilePath     : /sve_pooling/src/pooling_generic.h
 * @ToDo         : 
 */

#include "pooling_common.h"

using namespace pooling;

struct PoolingArgs {

  PoolingType   pool_type;
  PoolingWindow pool_window;
  PoolingStride pool_stride;
  bool          exclude_padding;

  unsigned int n_batches, input_rows, input_cols, n_channels;
  unsigned int output_rows, output_cols;

  PaddingValues padding;

  const PoolingConfig *config;  // unused

  PoolingArgs(
      PoolingType          pool_type,
      const PoolingWindow &window,
      const PoolingStride &stride,
      bool                 exclude_padding,
      unsigned int         n_batches,
      unsigned int         input_rows,
      unsigned int         input_cols,
      unsigned int         n_channels,
      unsigned int         output_rows,
      unsigned int         output_cols,
      const PaddingValues &padding,
      const PoolingConfig *cfg)
      : pool_type(pool_type), pool_window(window), pool_stride(stride), exclude_padding(exclude_padding), n_batches(n_batches), input_rows(input_rows), input_cols(input_cols), n_channels(n_channels), output_rows(output_rows), output_cols(output_cols), padding(padding), config(cfg) {
    // 如果pooling window 的长宽位0 那么意味着将他们都pooling
    if (pool_window.rows == 0) {
      pool_window.rows = input_rows;
    }
    if (pool_window.cols == 0) {
      pool_window.cols = input_cols;
    }
  }
};

template <typename T>
struct TensorSpec {
  T      base;
  size_t ld_row, ld_col;

  TensorSpec(T ptr, size_t ld_row, size_t ld_col)
      : base(ptr), ld_row(ld_row), ld_col(ld_col) {}
};

class PoolingStrategy {
 public:
  PoolingStrategy() = delete;
  PoolingStrategy(const PoolingArgs &args, const bool generic) {
    input_cols = args.input_cols;
    input_rows = args.input_rows;
    if (generic) {
      output_cols = 1;
      output_rows = 1;
    } else {
      output_cols = args.output_cols;
      output_rows = args.output_rows;
    }
  }

  int get_input_cols() const { return input_cols; }
  int get_input_rows() const { return input_rows; }
  int get_output_cols() const { return output_cols; }
  int get_output_rows() const { return output_rows; }

 private:
  int input_cols{0};
  int input_rows{0};
  int output_cols{0};
  int output_rows{0};
};

template <typename TInput, typename TOutput>
class PoolingCommon {  //TYX_LEARNING:pooling op2kernel | kernel class
 protected:
  const PoolingArgs m_args;

 public:
  PoolingCommon(const PoolingArgs &args, bool generic = true)
      : m_args(args), m_strat(new PoolingStrategy(args, generic)) {
  }

  ~PoolingCommon() {
    delete m_strat;
  }

  PoolingCommon(PoolingCommon &) = delete;
  PoolingCommon &operator=(PoolingCommon &) = delete;

  // size_t get_working_size(unsigned int, unsigned int) const  = 0;
  // size_t get_working_size(unsigned int n_threads) const  {
  //   return this->get_working_size(n_threads, m_args.n_channels);
  // }

  PoolingStrategy *m_strat;

  // Execute pooling over the specified area of memory.
  void execute(  //最外部分只需要传入需要计算的数据位置和存回的数据位置, 线程数目和总数目
      const void *const input,
      void *const       output,
      void *            working_space,
      unsigned int      thread_id,
      unsigned int      num_threads) const {
    this->execute(
        input,
        m_args.n_channels,
        m_args.n_channels * m_args.input_cols,
        m_args.n_channels * m_args.input_cols * m_args.input_rows,
        output,
        m_args.n_channels,
        m_args.n_channels * m_args.output_cols,
        m_args.n_channels * m_args.output_cols * m_args.output_rows,
        working_space,
        thread_id, num_threads);
  }

  void execute(
      const void *const input,
      size_t            ld_input_col,
      size_t            ld_input_row,
      size_t            ld_input_batch,
      void *const       output,
      size_t            ld_output_col,
      size_t            ld_output_row,
      size_t            ld_output_batch,
      void *            working_space,
      unsigned int      thread_id,
      unsigned int      num_threads) const {
    this->execute(
        m_args.n_batches, m_args.input_rows, m_args.input_cols, m_args.n_channels,
        input, ld_input_col, ld_input_row, ld_input_batch,
        m_args.padding, m_args.output_rows, m_args.output_cols,
        output, ld_output_col, ld_output_row, ld_output_batch,
        working_space, thread_id, num_threads);
  }

  void execute(
      unsigned int         batches,
      unsigned int         height,
      unsigned int         width,
      unsigned int         channels,
      const void *const    input,
      size_t               ld_input_col,
      size_t               ld_input_row,
      size_t               ld_input_batch,
      const PaddingValues &padding,
      unsigned int         output_height,
      unsigned int         output_width,
      void *const          output,
      size_t               ld_output_col,
      size_t               ld_output_row,
      size_t               ld_output_batch,
      void *               working_space,
      unsigned int         thread_id,
      unsigned int         num_threads) const {
    this->execute_internal(
        batches, height, width, channels, padding,
        input, ld_input_col, ld_input_row, ld_input_batch,
        output_height, output_width,
        output, ld_output_col, ld_output_row, ld_output_batch,
        working_space, thread_id, num_threads);
  }

 protected:
  void compute_tiles_unpadded(
      unsigned int start_output_i, unsigned int start_output_j,
      unsigned int n_tile_rows, unsigned int n_tile_cols,
      unsigned int output_channel_start, unsigned int output_channel_end,
      const TensorSpec<const TInput *> &input,
      const TensorSpec<TOutput *> &     output,
      void *                            working_space) const;

  void compute_tile_padded(
      unsigned int output_i, unsigned int output_j,
      unsigned int channel_start, unsigned int channel_end,
      const TensorSpec<const TInput *> &input,
      const TensorSpec<TOutput *> &     output,
      void *) const;

  void compute_row_padded_tile_row(
      const unsigned int output_i, unsigned int output_j, unsigned int n_tile_cols,
      const unsigned int channel_start, const unsigned int channel_end,
      const TensorSpec<const TInput *> &input,
      const TensorSpec<TOutput *> &     output,
      void *                            working_space) const;

  void execute_internal(
      unsigned int batches,
      unsigned int height,
      unsigned int width,
      unsigned int channels,
      const PaddingValues &,
      const void *const input,
      size_t            ld_input_col,
      size_t            ld_input_row,
      size_t            ld_input_batch,
      unsigned int      output_height,
      unsigned int      output_width,
      void *const       output,
      size_t            ld_output_col,
      size_t            ld_output_row,
      size_t            ld_output_batch,
      void *            working_space,
      unsigned int      thread_id,
      unsigned int      num_threads) const;
};
