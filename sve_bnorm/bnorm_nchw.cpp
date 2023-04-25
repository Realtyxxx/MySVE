
#if 0
template <typename T, bool fused_activation, typename F>
void NEBatchNormalizationLayerKernel::batch_normalization_nchw(const Window &window)
{
    /** SIMD vector tag type. */
    using ExactTagType = typename wrapper::traits::neon_bitvector_tag_t<T, wrapper::traits::BitWidth::W128>;

    const int  window_step_x  = 16 / sizeof(T);
    const auto window_start_x = static_cast<int>(window.x().start());
    const auto window_end_x   = static_cast<int>(window.x().end());

    Window win_to_use = window;
    win_to_use.set(Window::DimX, Window::Dimension(0, 1, 1));

    Iterator input(_input, win_to_use);
    Iterator output(_output, win_to_use);

    F activation_functor(_act_info);

    // Hold information about the current feature map we are iterating.
    // Only compute denominator and constants once per feature map.
    int slice = -1;

    const auto input_mean  = reinterpret_cast<const T *>(_mean->ptr_to_element(Coordinates(0, 0)));
    const auto input_var   = reinterpret_cast<const T *>(_var->ptr_to_element(Coordinates(0, 0)));
    const auto input_gamma = (_gamma != nullptr) ? reinterpret_cast<const T *>(_gamma->ptr_to_element(Coordinates(0, 0))) : nullptr;
    const auto input_beta  = (_beta != nullptr) ? reinterpret_cast<const T *>(_beta->ptr_to_element(Coordinates(0, 0))) : nullptr;

    T mean        = static_cast<T>(0);
    T var         = static_cast<T>(0);
    T gamma       = static_cast<T>(1);
    T beta        = static_cast<T>(0);
    T denominator = static_cast<T>(0);

    auto       mean_vec        = wrapper::vdup_n(mean, ExactTagType{});
    auto       var_vec         = wrapper::vdup_n(var, ExactTagType{});
    auto       gamma_vec       = wrapper::vdup_n(gamma, ExactTagType{});
    auto       beta_vec        = wrapper::vdup_n(beta, ExactTagType{});
    auto       denominator_vec = wrapper::vdup_n(denominator, ExactTagType{});
    const auto epsilon_vec     = wrapper::vdup_n(static_cast<T>(_epsilon), ExactTagType{});
    execute_window_loop(win_to_use, [&](const Coordinates & id)
    {
        const auto input_ptr  = reinterpret_cast<const T *>(input.ptr());
        const auto output_ptr = reinterpret_cast<T *>(output.ptr());

        if(slice != id.z())
        {
            mean     = input_mean[id.z()];
            var      = input_var[id.z()];
            mean_vec = wrapper::vdup_n(mean, ExactTagType{});
            var_vec  = wrapper::vdup_n(var, ExactTagType{});
            if(input_gamma != nullptr)
            {
                gamma     = input_gamma[id.z()];
                gamma_vec = wrapper::vdup_n(gamma, ExactTagType{});
            }
            if(input_beta != nullptr)
            {
                beta     = input_beta[id.z()];
                beta_vec = wrapper::vdup_n(beta, ExactTagType{});
            }

            // Calculate denominator
            denominator_vec = wrapper::vinvsqrt(wrapper::vadd(var_vec, epsilon_vec));
            denominator     = wrapper::vgetlane(denominator_vec, 0);
            slice           = id.z();
        }

        // Perform core calculations using vector operations
        int x = window_start_x;
        for(; x <= (window_end_x - window_step_x); x += window_step_x)
        {
            // Calculate x bar
            const auto numerator = wrapper::vsub(wrapper::vloadq(input_ptr + x), mean_vec);
            const auto x_bar     = wrapper::vmul(numerator, denominator_vec);
            auto       res       = wrapper::vmla(beta_vec, x_bar, gamma_vec);

            // Perform fused activation
            if(fused_activation)
            {
                activation_functor(res);
            }

            // Store results
            wrapper::vstore(output_ptr + x, res);
        }

        // Compute left-over elements
        for(; x < window_end_x; ++x)
        {
            const T numerator = input_ptr[x] - mean;
            const T x_bar     = numerator * denominator;
            T       res       = beta + x_bar * gamma;

            // Perform fused activation
            if(fused_activation)
            {
                activation_functor(res);
            }

            // Store results
            *(output_ptr + x) = res;
        }
    },
    input, output);
}
#endif