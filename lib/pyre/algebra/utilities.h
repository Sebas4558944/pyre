// -*- c++ -*-
//
// bianca giovanardi
// (c) 2021 all rights reserved


// code guard
#if !defined(pyre_algebra_utilities_h)
#define pyre_algebra_utilities_h


namespace pyre::algebra {
    // factory for identity matrices
    template <int D, typename T>
    static constexpr auto _make_identity_matrix() -> diagonal_matrix_t<D, T> 
    {
        diagonal_matrix_t<D, T> identity;

        auto _loop = [&identity]<size_t... I>(std::index_sequence<I...>)
        {
            ((identity[{I, I}] = 1), ... );
            return;
        };

        _loop(std::make_index_sequence<D> {});

        return identity;
    }

    template <int D, typename T = real>
    static constexpr diagonal_matrix_t<D, T> identity_matrix = _make_identity_matrix<D, T>();

    template <int D, typename T = real>
    static constexpr diagonal_matrix_t<D, T> zero_matrix = diagonal_matrix_t<D, T>::zero;

    template <int D, typename T = real>
    static constexpr matrix_t<D, D, T> one_matrix = matrix_t<D, D, T>::one;

    // helper functions for print
    template <typename Arg, typename... Args>
    inline std::ostream & _print(std::ostream & os, Arg && arg, Args &&... args)
    {
        os << std::forward<Arg>(arg);
        ((os << ", " << std::forward<Args>(args)), ...);
        return os;
    }

    template <int D, typename T, std::size_t... J>
    inline std::ostream & _print_vector(
        std::ostream & os, const pyre::algebra::vector_t<D, T> & vector, std::index_sequence<J...>)
    {
        os << "[ ";
        if (sizeof...(J) > 0)
            _print(os, vector[J]...);
        os << " ]";
        return os;
    }

    template <int D1, int D2, typename T, class packingT, size_t... J>
    std::ostream & _print_row(
        std::ostream & os, const matrix_t<D1, D2, T, packingT> & tensor, size_t row, std::index_sequence<J...>)
    {
        os << "[ ";
        if (sizeof...(J) > 0)
            _print(os, tensor[{ row, J }]...);
        os << " ]";
        return os;
    }

    template <int D1, int D2, typename T, class packingT, size_t... J>
    std::ostream & _print_comma_row(
        std::ostream & os, const matrix_t<D1, D2, T, packingT> & tensor, size_t row, 
        std::index_sequence<J...>)
    {
        os << ",";
        return _print_row(os, tensor, row, std::make_index_sequence<D2> {});
    }

    template <int D1, int D2, typename T, class packingT, size_t... J>
    std::ostream & _print_matrix(
        std::ostream & os, const matrix_t<D1, D2, T, packingT> & tensor, std::index_sequence<J...>)
    {
        os << "[ ";
        _print_row(os, tensor, 0, std::make_index_sequence<D2> {});
        ((_print_comma_row(os, tensor, J + 1, std::make_index_sequence<D2> {})), ...);
        os << " ]";
        return os;
    }

    // overload operator<< for vectors
    template <int D, typename T>
    std::ostream & operator<<(std::ostream & os, const pyre::algebra::vector_t<D, T> & vector)
    {
        return _print_vector(os, vector, std::make_index_sequence<D> {});
    }

    // overload operator<< for second order tensors
    template <int D1, int D2, typename T, class packingT>
    std::ostream & operator<<(std::ostream & os, const pyre::algebra::matrix_t<D1, D2, T, packingT> & tensor)
    {
        return _print_matrix(os, tensor, std::make_index_sequence<D1-1> {});
    }

    template <typename T>
    constexpr bool is_equal(T lhs, T rhs)
    {
        if ((lhs < rhs + epsilon_right(rhs)) && (lhs > rhs - epsilon_left(rhs)))
        {
            return true;
        }

        return false;
    }

    template <typename T, class packingT, int... I>
    constexpr bool is_equal(const Tensor<T, packingT, I...> & lhs, 
        const Tensor<T, packingT, I...> & rhs)
    {
        // helper function (component-wise)
        constexpr auto _is_equal = []<size_t... J>(const Tensor<T, packingT, I...> & lhs, 
            const Tensor<T, packingT, I...> & rhs, std::index_sequence<J...>) {

            // if all components are equal
            if ((is_equal(lhs[J], rhs[J]) && ...)) {
                // then the tensors are equal
                return true;
            }
            // then the tensors differ
            return false;
        };

        // the size of the tensor
        constexpr int D = Tensor<T, packingT, I...>::size;
        // all done
        return _is_equal(lhs, rhs, std::make_index_sequence<D> {});
    }

    template <typename T, class packingT, int... I>
    constexpr bool is_zero(const Tensor<T, packingT, I...> & A, T tolerance)
    {
        // helper function (component-wise)
        constexpr auto _is_zero = []<size_t... J>(const Tensor<T, packingT, I...> & A, 
            T tolerance, std::index_sequence<J...>) {

            // if all components are zero
            if (((A[J] <= tolerance) && ...)) {
                // then the tensor is zero
                return true;
            }
            // then the tensors is not zero
            return false;
        };

        // the size of the tensor
        constexpr int D = Tensor<T, packingT, I...>::size;
        // all done
        return _is_zero(A, tolerance, std::make_index_sequence<D> {});
    }
}


#endif

// end of file
