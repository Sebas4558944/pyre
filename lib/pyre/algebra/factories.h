// code guard
#if !defined(pyre_algebra_factories_h)
#define pyre_algebra_factories_h


namespace pyre::algebra {
    
    template <class tensorT>
    constexpr auto make_zeros() -> tensorT
    {
        constexpr auto _make_zeros = []<size_t... J>(std::index_sequence<J...>) -> tensorT
        {
            constexpr auto fill_zeros = []<size_t>() consteval-> tensorT::type { return 0; };
            // return a tensor filled with zeros
            return tensorT(fill_zeros.template operator()<J>()...);
        };

        // fill tensor with zeros
        return _make_zeros(std::make_index_sequence<tensorT::size>{});
    }

    template <class tensorT>
    constexpr auto make_ones() -> tensorT 
    {
        constexpr auto _make_ones = []<size_t... J>(std::index_sequence<J...>) -> tensorT
        {
            constexpr auto fill_ones = []<size_t>() consteval-> tensorT::type { return 1; };
            // return a tensor filled with ones
            return tensorT(fill_ones.template operator()<J>()...);
        };

        // fill tensor with ones
        return _make_ones(std::make_index_sequence<tensorT::size>{});
    }

    // make the element of the tensor basis that has a one at the index given by {args...}
    template <class tensorT>
    constexpr auto make_basis_element(typename tensorT::index_t index)
        -> tensorT
    {
        // typedef for index type
        using index_t = tensorT::index_t;

        constexpr auto _make_basis_element = []<size_t... J>(index_t K, std::index_sequence<J...>) 
            -> tensorT
        {
            constexpr auto delta = [](size_t II, size_t JJ) -> tensorT::type 
            { 
                if (II == JJ) return 1; 
                return 0;
            };

            // fill tensor with delta_ij
            return tensorT(delta(tensorT::layout()[K] /* I */, J)...);
        };

        return _make_basis_element(index, std::make_index_sequence<tensorT::size>{});
    }

    // factory for identity tensor (for now only for second order tensors)
    template <class tensorT>
    static constexpr auto make_identity() -> tensorT::diagonal_tensor_t 
    requires (tensorT::order == 2)
    {
        typename tensorT::diagonal_tensor_t identity;

        auto _loop = [&identity]<size_t... I>(std::index_sequence<I...>)
        {
            ((identity[{I, I}] = 1), ... );
            return;
        };

        _loop(std::make_index_sequence<tensorT::diagonal_tensor_t::size> {});

        return identity;
    }
}


#endif

// end of file
