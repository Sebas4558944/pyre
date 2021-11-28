// -*- coding: utf-8 -*-
//
// bianca giovanardi
// (c) 2021 all rights reserved
//


#if !defined(pyre_algebra_tensor_algebra_h)
#define pyre_algebra_tensor_algebra_h


namespace pyre::algebra {
    template <typename T, class packingT, int... I>
    constexpr T norm(const Tensor<T, packingT, I...> & tensor)
    {
        T result(0);
        // iterate on the packing
        for (auto idx : Tensor<T, packingT, I...>::layout()) {
            result += tensor[idx] * tensor[idx];
        }
        return std::sqrt(result);
    }

    template <typename T, class packingT1, class packingT2, int... I>
    constexpr bool operator==(const Tensor<T, packingT1, I...> & lhs, 
        const Tensor<T, packingT2, I...> & rhs)
    {
        // typedef for the repacked tensor based on {packingT1} and {packingT2}
        using repacked_tensor_t = Tensor<T, 
            typename repacking<packingT1, packingT2>::packing_type, I...>;
        // iterate on the packing
        for (auto idx : repacked_tensor_t::layout()) {
            if(lhs[idx] != rhs[idx]){
                return false;
            }
        }
        return true;
    }

    // Algebraic operations on vectors, tensors, ...
    // vector_t times scalar
    template <typename T, class packingT, int... I, size_t... J>
    constexpr void _vector_times_scalar(
        T a, const Tensor<T, packingT, I...> & y, Tensor<T, packingT, I...> & result,
        std::index_sequence<J...>)
    {
        ((result[J] = y[J] * a), ...);
        return;
    }

    template <typename T, class packingT, int... I>
    constexpr Tensor<T, packingT, I...> && operator*(T a, 
        Tensor<T, packingT, I...> && y) 
        requires(Tensor<T, packingT, I...>::size != 1)
    {
        constexpr int D = Tensor<T, packingT, I...>::size;
        _vector_times_scalar(a, y, y, std::make_index_sequence<D> {});
        return std::move(y);
    }

    template <typename T, class packingT, int... I>
    constexpr Tensor<T, packingT, I...> operator*(T a, 
        const Tensor<T, packingT, I...> & y) 
        requires(Tensor<T, packingT, I...>::size != 1)
    {
        Tensor<T, packingT, I...> result;
        constexpr int D = Tensor<T, packingT, I...>::size;
        _vector_times_scalar(a, y, result, std::make_index_sequence<D> {});
        return result;
    }

    template <typename T, class packingT, int... I>
    constexpr Tensor<T, packingT, I...> && operator*(Tensor<T, packingT, I...> && y, T a) 
        requires(Tensor<T, packingT, I...>::size != 1)
    {
        return a * std::move(y);
    }

    template <typename T, class packingT, int... I>
    constexpr Tensor<T, packingT, I...> operator*(const Tensor<T, packingT, I...> & y, T a) 
        requires(Tensor<T, packingT, I...>::size != 1)
    {
        return a * y;
    }

    // operator+ for tensors
    template <typename T, class packingT1, class packingT2, int... I>
    constexpr Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> operator+(
        const Tensor<T, packingT1, I...> & y1, const Tensor<T, packingT2, I...> & y2)
    {
        // std::cout << "operator+ & &" << std::endl;
        // instantiate the result
        Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> result;
        // typedef for the repacked tensor based on {packingT1} and {packingT2}
        using repacked_tensor_t = Tensor<T, 
            typename repacking<packingT1, packingT2>::packing_type, I...>;
        // iterate on the packing
        for (auto idx : repacked_tensor_t::layout()) {
            result[idx] = y1[idx] + y2[idx];
        }
        // all done
        return result;
    }
    template <typename T, class packingT1, class packingT2, int... I>
    constexpr Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> operator+(
        const Tensor<T, packingT1, I...> & y1, Tensor<T, packingT2, I...> && y2)
        requires (std::is_same_v<typename repacking<packingT1, packingT2>::packing_type, packingT2>)
    {
        // std::cout << "operator+ & &&" << std::endl;
        // write the result on y1
        y2 = y1 + std::as_const(y2);
        // all done
        return y2;
    }
    template <typename T, class packingT1, class packingT2, int... I>
    constexpr Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> operator+(
        Tensor<T, packingT1, I...> && y1, const Tensor<T, packingT2, I...> & y2)
        requires (std::is_same_v<typename repacking<packingT1, packingT2>::packing_type, packingT1>)
    {
        // std::cout << "operator+ && &" << std::endl;
        // easy enough
        return y2 + std::move(y1);
    }
    template <typename T, class packingT1, class packingT2, int... I>
    constexpr Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> 
        operator+(Tensor<T, packingT1, I...> && y1, Tensor<T, packingT2, I...> && y2)
    {
        // std::cout << "operator+ && &&" << std::endl;
        // typedef for the repacked tensor based on {packingT1} and {packingT2}
        using repacking_t = typename repacking<packingT1, packingT2>::packing_type;
        // if the repacking type is the packing type of y1
        if constexpr(std::is_same_v<repacking_t, packingT1>) {
            // pass down y1 as temporary and y2 as const reference 
            return std::move(y1) + std::as_const(y2);
        }
        else {
            // pass down y2 as temporary and y1 as const reference 
            return std::move(y2) + std::as_const(y1);
        }        
    }

    // vector_t operator-
    template <typename T, class packingT, int... I, std::size_t... J>
    constexpr void _vector_minus(
        const Tensor<T, packingT, I...> & y, 
        Tensor<T, packingT, I...> & result, std::index_sequence<J...>)
    {
        ((result[J] = -y[J]), ...);
        return;
    }

    template <typename T, class packingT, int... I>
    constexpr Tensor<T, packingT, I...> operator-(const Tensor<T, packingT, I...> & y)
    {
        // std::cout << "unary operator- new temp" << std::endl;
        Tensor<T, packingT, I...> result;
        constexpr int D = Tensor<T, packingT, I...>::size;
        _vector_minus(y, result, std::make_index_sequence<D> {});
        return result;
    }

    template <typename T, class packingT, int... I>
    constexpr Tensor<T, packingT, I...> && operator-(Tensor<T, packingT, I...> && y)
    {
        // std::cout << "unary operator- no temp &&" << std::endl;
        constexpr int D = Tensor<T, packingT, I...>::size;
        _vector_minus(y, y, std::make_index_sequence<D> {});
        return std::move(y);
    }

    template <typename T, class packingT1, class packingT2, int... I>
    constexpr void _vector_minus(
        const Tensor<T, packingT1, I...> & y1, const Tensor<T, packingT2, I...> & y2,
        Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> & result)
    {
        // typedef for the repacked tensor based on {packingT1} and {packingT2}
        using repacked_tensor_t = Tensor<T, 
            typename repacking<packingT1, packingT2>::packing_type, I...>;
        // iterate on the packing
        for (auto idx : repacked_tensor_t::layout()) {
            result[idx] = y1[idx] - y2[idx];
        }
        // all done
        return;
    }

    template <typename T, class packingT1, class packingT2, int... I>
    constexpr Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> operator-(
        const Tensor<T, packingT1, I...> & y1, const Tensor<T, packingT2, I...> & y2)
    {
        // std::cout << "binary operator- new temp" << std::endl;
        Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> result;
        _vector_minus(y1, y2, result);
        return result;
        // return y1 + (-y2);
    }

    template <typename T, class packingT1, class packingT2, int... I>
    constexpr Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> && operator-(
        Tensor<T, packingT1, I...> && y1, const Tensor<T, packingT2, I...> & y2)
    {
        // std::cout << "binary operator- no temp && &" << std::endl;
        _vector_minus(y1, y2, y1);
        return std::move(y1); // TOFIX
        // return std::move(y1) + (-y2);
    }

    template <typename T, class packingT1, class packingT2, int... I>
    constexpr Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> && operator-(
        const Tensor<T, packingT1, I...> & y1, Tensor<T, packingT2, I...> && y2)
    {
        // std::cout << "binary operator- no temp & &&" << std::endl;
        _vector_minus(y1, y2, y2);
        return std::move(y2); // TOFIX
        // return y1 + (-std::move(y2));
    }

    template <typename T, class packingT1, class packingT2, int... I>
    constexpr Tensor<T, typename repacking<packingT1, packingT2>::packing_type, I...> && operator-(
        Tensor<T, packingT1, I...> && y1, Tensor<T, packingT2, I...> && y2)
    {
        // std::cout << "binary operator- no temp && &&" << std::endl;
        _vector_minus(y1, y2, y1);
        return std::move(y1); // TOFIX
        // return std::move(y1) + (-std::move(y2));
    }

    template <typename T, class packingT, int... I>
    constexpr Tensor<T, packingT, I...> operator/(const Tensor<T, packingT, I...> & y, T a) requires(
        Tensor<T, packingT, I...>::size != 1)
    {
        return (1.0 / a) * y;
    }

    template <typename T, class packingT, int... I>
    constexpr Tensor<T, packingT, I...> && operator/(Tensor<T, packingT, I...> && y, T a) requires(
        Tensor<T, packingT, I...>::size != 1)
    {
        return (1.0 / a) * std::move(y);
    }
    // builds a square matrix with all zeros except the K-th row is equal to v
    template <int K, int D, typename T, class packingT>
    constexpr matrix_t<D, D, T, packingT> matrix_row(const vector_t<D, T> & v)
    {
        constexpr auto _fill_matrix_row = []<size_t... I>(
            matrix_t<D, D, T, packingT> A, const vector_t<D, T> & v, std::index_sequence<I...>) 
            -> matrix_t<D, D, T, packingT>
        {
            ((A[{K, I}] = v[{ I }]), ...);
            return A;
        };
        // fill row K of a zero matrix with vector v
        return _fill_matrix_row(matrix_t<D, D, T, packingT>::zero, v, 
            std::make_index_sequence<D> {});
    }
    // builds a square matrix with all zeros except the K-th column is equal to v
    template <int K, int D, typename T>
    constexpr matrix_t<D, D, T> matrix_column(const vector_t<D, T> & v)
    {
        constexpr auto _fill_matrix_column = []<size_t... I>(
            matrix_t<D, D, T> A, const vector_t<D, T> & v, std::index_sequence<I...>) 
            -> matrix_t<D, D, T>
        {
            ((A[{I, K}] = v[{ I }]), ...);
            return A;
        };
        // fill column K of a zero matrix with vector v
        return _fill_matrix_column(matrix_t<D, D, T>::zero, v, std::make_index_sequence<D> {});
    }
    // builds a square matrix with all zeros except the diagonal is equal to v
    template <int D, typename T>
    constexpr diagonal_matrix_t<D, T> matrix_diagonal(const vector_t<D, T> & v)
    {
        constexpr auto _fill_matrix_diagonal = []<size_t... I>(
            diagonal_matrix_t<D, T> A, const vector_t<D, T> & v, std::index_sequence<I...>) 
            -> diagonal_matrix_t<D, T>
        {
            ((A[{I, I}] = v[{ I }]), ...);
            return A;
        };
        // instantiate a diagonal matrix
        diagonal_matrix_t<D, T> A;
        // fill diagonal of a zero matrix with vector v
        return _fill_matrix_diagonal(A, v, std::make_index_sequence<D> {});
    }
    // builds the vector with the diagonal entries of a matrix
    template <int D, typename T, class packingT>
    constexpr vector_t<D, T> matrix_diagonal(const matrix_t<D, D, T, packingT> & A)
    {
        auto _fill_vector_with_matrix_diagonal = [&A]<size_t... J>(std::index_sequence<J...>) 
            -> vector_t<D, T>
        {
            auto wrap = [&A]<size_t K>()->T { return  A[{K, K}]; };
            return vector_t<D, T>(wrap.template operator()<J>()...);
        };
        // fill a vector with the diagonal of A and return it
        return _fill_vector_with_matrix_diagonal(std::make_index_sequence<D> {});
    }
    // row-column vector product
    template <int D, typename T>
    constexpr T operator*(
        const vector_t<D, T> & v1, const vector_t<D, T> & v2) 
    {
        // helper function (scalar product)
        constexpr auto _vector_times_vector = []<size_t... K>(
            const vector_t<D, T> & v1, const vector_t<D, T> & v2, 
            std::index_sequence<K...>) ->T 
        { 
            return ((v1[{ K }] * v2[{ K }]) + ...);
        };
        return _vector_times_vector(v1, v2, std::make_index_sequence<D> {});
    }
    // matrix-vector multiplication
    template <int D1, int D2, typename T, class packingT>
    constexpr vector_t<D1, T> operator*(
        const matrix_t<D1, D2, T, packingT> & A, const vector_t<D2, T> & v) 
    {
        // helper function
        constexpr auto _matrix_times_vector = []<size_t... K>(
            const matrix_t<D1, D2, T, packingT> & A, const vector_t<D2, T> & v, 
            std::index_sequence<K...>) -> vector_t<D1, T> 
        { 
            return vector_t<D1, T>((row<K>(A) * v)...);
        };
        return _matrix_times_vector(A, v, std::make_index_sequence<D2> {});
    }
    // vector-matrix multiplication
    template <int D1, int D2, typename T, class packingT>
    constexpr vector_t<D1, T> operator*(const vector_t<D2, T> & v, 
        const matrix_t<D1, D2, T, packingT> & A) 
    {
        return transpose(A) * v;
    }
    // matrix-matrix multiplication
    template <int D1, int D2, int D3, typename T, class packingT1, class packingT2>
    constexpr matrix_t<D1, D3, T> operator*(
        const matrix_t<D1, D2, T, packingT1> & A1, const matrix_t<D2, D3, T, packingT2> & A2) 
        requires(D1 != 1 && D2 != 1 && D3 != 1)
    {
        // helper function
        constexpr auto _matrix_times_matrix = []<size_t... K>(
            const matrix_t<D1, D2, T, packingT1> & A1, 
            const matrix_t<D2, D3, T, packingT2> & A2, 
            std::index_sequence<K...>) -> matrix_t<D1, D3, T> 
        {
            // for each K build the matrix whose column K is equal to A1 * col<K>(A2)
            // then add them all up
            return (matrix_column<K>(A1 * col<K>(A2)) + ...);
        };
        return _matrix_times_matrix(A1, A2, std::make_index_sequence<D3> {});
    }
    // the skew symmetric matrix representing vector a  
    template <typename T>
    constexpr matrix_t<3, 3, T> skew(const vector_t<3, T> & a)
    {
        matrix_t<3, 3, T> A = matrix_t<3, 3, T>::zero;
        A[{0, 1}] = -a[2];
        A[{0, 2}] = a[1];
        A[{1, 0}] = a[2];
        A[{1, 2}] = -a[0];
        A[{2, 0}] = -a[1];
        A[{2, 1}] = a[0];
        return A;
    }

    template <typename T>
    constexpr auto cross(const vector_t<3, T> & a, const vector_t<3, T> & b)
    {
        return skew(a) * b;
    }

    template <typename T>
    constexpr T cross(const vector_t<2, T> & a, const vector_t<2, T> & b)
    {
        vector_t<3, T> a3 {a[0], a[1], 0.0};
        vector_t<3, T> b3 {b[0], b[1], 0.0};
        return cross(a3, b3)[2];
    }
    // factorial
    template <int D>
    constexpr int factorial()
    {
        return D * factorial<int(D - 1)>();
    }

    template <>
    constexpr int factorial<1>()
    {
        return 1;
    }

    template <typename T, class packingT>
    constexpr T determinant(const matrix_t<4, 4, T, packingT> & A)
    {
        return A[{0, 1}] * A[{2, 3}] * A[{3, 2}] * A[{1, 0}] - A[{0, 1}] * A[{2, 2}] * A[{3, 3}]
                 * A[{1, 0}]
             - A[{2, 3}] * A[{3, 1}] * A[{0, 2}] * A[{1, 0}] + A[{2, 2}] * A[{3, 1}] * A[{0, 3}]
                 * A[{1, 0}]
             - A[{0, 0}] * A[{2, 3}] * A[{3, 2}] * A[{1, 1}] + A[{0, 0}] * A[{2, 2}] * A[{3, 3}]
                 * A[{1, 1}]
             + A[{2, 3}] * A[{3, 0}] * A[{0, 2}] * A[{1, 1}] - A[{2, 2}] * A[{3, 0}] * A[{0, 3}]
                 * A[{1, 1}]
             - A[{0, 1}] * A[{2, 3}] * A[{3, 0}] * A[{1, 2}] + A[{0, 0}] * A[{2, 3}] * A[{3, 1}]
                 * A[{1, 2}]
             + A[{0, 1}] * A[{2, 2}] * A[{3, 0}] * A[{1, 3}] - A[{0, 0}] * A[{2, 2}] * A[{3, 1}]
                 * A[{1, 3}]
             - A[{3, 3}] * A[{0, 2}] * A[{1, 1}] * A[{2, 0}] + A[{3, 2}] * A[{0, 3}] * A[{1, 1}]
                 * A[{2, 0}]
             + A[{0, 1}] * A[{3, 3}] * A[{1, 2}] * A[{2, 0}] - A[{3, 1}] * A[{0, 3}] * A[{1, 2}]
                 * A[{2, 0}]
             - A[{0, 1}] * A[{3, 2}] * A[{1, 3}] * A[{2, 0}] + A[{3, 1}] * A[{0, 2}] * A[{1, 3}]
                 * A[{2, 0}]
             + A[{3, 3}] * A[{0, 2}] * A[{1, 0}] * A[{2, 1}] - A[{3, 2}] * A[{0, 3}] * A[{1, 0}]
                 * A[{2, 1}]
             - A[{0, 0}] * A[{3, 3}] * A[{1, 2}] * A[{2, 1}] + A[{3, 0}] * A[{0, 3}] * A[{1, 2}]
                 * A[{2, 1}]
             + A[{0, 0}] * A[{3, 2}] * A[{1, 3}] * A[{2, 1}] - A[{3, 0}] * A[{0, 2}] * A[{1, 3}]
                 * A[{2, 1}];
    }

    template <typename T, class packingT>
    constexpr T determinant(const matrix_t<3, 3, T, packingT> & A)
    {
        return A[{0, 0}] * (A[{1, 1}] * A[{2, 2}] - A[{1, 2}] * A[{2, 1}]) 
             - A[{0, 1}] * (A[{1, 0}] * A[{2, 2}] - A[{1, 2}] * A[{2, 0}])
             + A[{0, 2}] * (A[{1, 0}] * A[{2, 1}] - A[{1, 1}] * A[{2, 0}]);
    }

    template <typename T, class packingT>
    constexpr T determinant(const matrix_t<2, 2, T, packingT> & A)
    {
        return A[{0, 0}] * A[{1, 1}] - A[{0, 1}] * A[{1, 0}];
    }

    template <typename T, class packingT>
    constexpr matrix_t<3, 3, T, packingT> inverse(const matrix_t<3, 3, T, packingT> & A)
    {
        matrix_t<3, 3, T, packingT> invA;
        T det = determinant(A);
        assert(det != 0.0);
        T detinv = 1.0 / det;
        invA[{0, 0}] = detinv * (A[{1, 1}] * A[{2, 2}] - A[{1, 2}] * A[{2, 1}]);
        invA[{0, 1}] = detinv * (-A[{0, 1}] * A[{2, 2}] + A[{0, 2}] * A[{2, 1}]);
        invA[{0, 2}] = detinv * (A[{0, 1}] * A[{1, 2}] - A[{0, 2}] * A[{1, 1}]);
        invA[{1, 0}] = detinv * (-A[{1, 0}] * A[{2, 2}] + A[{1, 2}] * A[{2, 0}]);
        invA[{1, 1}] = detinv * (A[{0, 0}] * A[{2, 2}] - A[{0, 2}] * A[{2, 0}]);
        invA[{1, 2}] = detinv * (-A[{0, 0}] * A[{1, 2}] + A[{0, 2}] * A[{1, 0}]);
        invA[{2, 0}] = detinv * (A[{1, 0}] * A[{2, 1}] - A[{1, 1}] * A[{2, 0}]);
        invA[{2, 1}] = detinv * (-A[{0, 0}] * A[{2, 1}] + A[{0, 1}] * A[{2, 0}]);
        invA[{2, 2}] = detinv * (A[{0, 0}] * A[{1, 1}] - A[{0, 1}] * A[{1, 0}]);
        return invA;
    }

    template <typename T, class packingT>
    constexpr matrix_t<2, 2, T, packingT> inverse(const matrix_t<2, 2, T, packingT> & A)
    {
        matrix_t<2, 2, T, packingT> invA;
        T det = determinant(A);
        assert(det != 0.0);
        T detinv = 1.0 / det;
        invA[{0, 0}] = detinv * (A[{1, 1}]);
        invA[{0, 1}] = detinv * (-A[{0, 1}]);
        invA[{1, 0}] = detinv * (-A[{1, 0}]);
        invA[{1, 1}] = detinv * (A[{0, 0}]);
        return invA;
    }

    template <int D, typename T, class packingT>
    constexpr T trace(const matrix_t<D, D, T, packingT> & A)
    {
        auto _trace = [&A]<size_t... J>(std::index_sequence<J...>) ->T
        {
            return (A[{J, J}]+ ... );
        };
        return _trace(std::make_index_sequence<D> {});
    }

    template <int D1, int D2, typename T, class packingT>
    constexpr auto transpose(const matrix_t<D1, D2, T, packingT> & A)
    {
        // A transposed
        matrix_t<D2, D1, T, packingT> AT;
        auto _transposeJ = [&A, &AT]<size_t... J>(std::index_sequence<J...>){
            auto _transposeI = [&A, &AT]<size_t K, size_t... I>(std::index_sequence<I...>)
            {
                ((AT[{K, I}] = A [{I, K}]), ... );
                return;
            };
            (_transposeI.template operator()<J>(std::make_index_sequence<D1> {}), ...);
        };
        _transposeJ(std::make_index_sequence<D2> {});
        return AT;
    }

    template <int D, typename T, class packingT>
    constexpr symmetric_matrix_t<D, T> symmetric(const matrix_t<D, D, T, packingT> & A)
    {
        symmetric_matrix_t<D, T> sym;
        auto _fill_column = [&A, &sym]<size_t... K>(std::index_sequence<K...>){
            auto _fill_row = [&A, &sym]<size_t J, size_t... I>(std::index_sequence<I...>)
            {
                ((sym[{I, J}] = 0.5 * (A[{I, J}] + A[{J, I}])), ... );
                return;
            };
            (_fill_row.template operator()<K>(std::make_index_sequence<D> {}), ...);
        };
        _fill_column(std::make_index_sequence<D> {});
        return sym;
    }

    template <int D, typename T, class packingT>
    constexpr auto skew(const matrix_t<D, D, T, packingT> & A) 
    requires (!std::is_same_v<packingT, pyre::grid::symmetric_t<2>>) 
    {
        return 0.5 * (A - transpose(A));
    }

    template <int D, typename T>
    constexpr auto skew(const symmetric_matrix_t<D, T> & A) 
    {
        return zero_matrix<D, T>;
    }

    template <typename T>
    constexpr vector_t<2, T> eigenvalues(const symmetric_matrix_t<2, T> & A)
    {
        T delta = sqrt(4.0 * A[{0, 1}] * A[{0, 1}] 
            + (A[{0, 0}] - A[{1, 1}]) * (A[{0, 0}] - A[{1, 1}]));
        return vector_t<2, T>{
            0.5 * (A[{0, 0}] + A[{1, 1}] + delta), 
            0.5 * (A[{0, 0}] + A[{1, 1}] - delta)};
    }

    template <typename T>
    constexpr matrix_t<2, 2, T> eigenvectors(const symmetric_matrix_t<2, T> & A)
    {
        T delta = sqrt(4.0 * A[{0, 1}] * A[{0, 1}] 
            + (A[{0, 0}] - A[{1, 1}]) * (A[{0, 0}] - A[{1, 1}]));
        matrix_t<2, 2, T> eigenvector_matrix;
        eigenvector_matrix[{0, 0}] = (A[{0, 0}] - A[{1, 1}] + delta) / (2.0 * A[{1, 0}]);
        eigenvector_matrix[{0, 1}] = (A[{0, 0}] - A[{1, 1}] - delta) / (2.0 * A[{1, 0}]);
        eigenvector_matrix[{1, 0}] = 1.0;
        eigenvector_matrix[{1, 1}] = 1.0;
        return eigenvector_matrix;
    }

    template <typename T>
    constexpr vector_t<3, T> eigenvalues(const symmetric_matrix_t<3, T> & A)
    {
        // https://hal.archives-ouvertes.fr/hal-01501221
        T x1 = A[{0, 0}] * A[{0, 0}] + A[{1, 1}] * A[{1, 1}] + A[{2, 2}] * A[{2, 2}] 
            - A[{0, 0}] * A[{1, 1}] - A[{0, 0}] * A[{2, 2}] - A[{1, 1}] * A[{2, 2}]
            + 3.0 * (A[{0, 1}] * A[{0, 1}] + A[{0, 2}] * A[{0, 2}] + A[{1, 2}] * A[{1, 2}]);
        T a = 2.0 * A[{0, 0}] - A[{1, 1}] - A[{2, 2}];
        T b = 2.0 * A[{1, 1}] - A[{0, 0}] - A[{2, 2}];
        T c = 2.0 * A[{2, 2}] - A[{0, 0}] - A[{1, 1}];
        T x2 =  - a * b * c
                + 9.0 * (c * A[{0, 1}] * A[{0, 1}] + b * A[{0, 2}] * A[{0, 2}] 
                    + a * A[{1, 2}] * A[{1, 2}])
                - 54.0 * A[{0, 1}] * A[{0, 2}] * A[{1, 2}];
        T d = (A[{0, 0}] + A[{1, 1}] + A[{2, 2}]) / 3.0;
        T e = 2.0 * sqrt(x1) / 3.0;
        double PI = 4.0 * atan(1.0);
        T phi = 0.0;
        T two_sqrt_x1 = 2.0 * sqrt(x1);
        T num = sqrt((two_sqrt_x1 * x1 - x2) * (two_sqrt_x1 * x1 + x2));
        T den = x2;
        // avoid dividing by zero (use the identity atan(a / b) + atan(b / a) = PI / 2
        if (fabs(den) < num) {
            // phi = 0.5 * PI - atan(den / num);
            phi = atan(num / den);
        }
        else {
            phi = atan(num / den);
        }
        if (x2 < 0) {
            phi += PI;
        }

        return vector_t<3, T>{ 
            d - e * cos(phi / 3.0), 
            d + e * cos((phi - PI) / 3.0), 
            d + e * cos((phi + PI) / 3.0)
        };
    }

    template <typename T>
    constexpr matrix_t<3, 3, T> eigenvectors(const symmetric_matrix_t<3, T> & A)
    {
        // TODO: Manage special cases f = 0, denominators = 0
        auto lambda = eigenvalues(A);
        T m0 = (A[{0, 1}] * (A[{2, 2}] - lambda[0]) - A[{1, 2}] * A[{0, 2}]) / 
            (A[{0, 2}] * (A[{1, 1}] - lambda[0]) - A[{0, 1}] * A[{1, 2}]);
        T m1 = (A[{0, 1}] * (A[{2, 2}] - lambda[1]) - A[{1, 2}] * A[{0, 2}]) / 
            (A[{0, 2}] * (A[{1, 1}] - lambda[1]) - A[{0, 1}] * A[{1, 2}]);
        T m2 = (A[{0, 1}] * (A[{2, 2}] - lambda[2]) - A[{1, 2}] * A[{0, 2}]) / 
            (A[{0, 2}] * (A[{1, 1}] - lambda[2]) - A[{0, 1}] * A[{1, 2}]);
        matrix_t<3, 3, T> eigenvector_matrix;
        eigenvector_matrix[{0, 0}] = (lambda[0] - A[{2, 2}] - A[{1, 2}] * m0) / A[{0, 2}];
        eigenvector_matrix[{0, 1}] = (lambda[1] - A[{2, 2}] - A[{1, 2}] * m1) / A[{0, 2}];
        eigenvector_matrix[{0, 2}] = (lambda[2] - A[{2, 2}] - A[{1, 2}] * m2) / A[{0, 2}];
        eigenvector_matrix[{1, 0}] = m0;
        eigenvector_matrix[{1, 1}] = m1;
        eigenvector_matrix[{1, 2}] = m2;
        eigenvector_matrix[{2, 0}] = 1.0;
        eigenvector_matrix[{2, 1}] = 1.0;
        eigenvector_matrix[{2, 2}] = 1.0;
        return eigenvector_matrix;
    }

    template <int D, typename T>
    constexpr auto eigenvalues(const diagonal_matrix_t<D, T> & A)
    {
        // the diagonal entries are the eigenvalues
        return matrix_diagonal(A);
    }

    template <int D, typename T>
    constexpr auto eigenvectors(const diagonal_matrix_t<D, T> & A)
    {
        // the canonical basis is the set of eigenvectors
        return identity_matrix<D>;
    }

    template <int I, int D1, int D2, typename T, class packingT>
    constexpr vector_t<D2, T> row(const matrix_t<D1, D2, T, packingT> & A)
    {
        auto _row = [&A]<size_t... J>(std::index_sequence<J...>) -> vector_t<D2, T>
        {
            auto wrap = [&A]<size_t K>()->T { return  A[{I, K}]; };
            return vector_t<D2, T>(wrap.template operator()<J>()...);
        };
        return _row(std::make_index_sequence<D1> {});
    }

    template <int I, int D1, int D2, typename T, class packingT>
    constexpr vector_t<D1, T> col(const matrix_t<D1, D2, T, packingT> & A)
    {
        auto _col = [&A]<size_t... J>(std::index_sequence<J...>) -> vector_t<D1, T>
        {
            auto wrap = [&A]<size_t K>()->T { return A[{K, I}]; };
            return vector_t<D1, T>(wrap.template operator()<J>()...);
        };
        return _col(std::make_index_sequence<D2> {});
    }

    template <int D, typename T, class packingT>
    constexpr auto function(const matrix_t<D, D, T, packingT> & A, auto f)
    {
        // compute eigenvalues
        auto lambda = matrix_diagonal(eigenvalues(A));
        // compute eigenvectors
        auto P = eigenvectors(A);
        // helper function (component-wise)
        constexpr auto _apply_f_to_diagonal = []<size_t... I>(auto & lambda, auto f, 
            std::index_sequence<I...>)
        {
            // apply f to diagonal
            ((lambda[{I, I}] = f(lambda[{I, I}])), ...);
            // all done
            return;
        };
        // change eigenvalues into f(eigenvalues)
        _apply_f_to_diagonal(lambda, f, std::make_index_sequence<D> {});
        // rebuild matrix
        return symmetric(P * lambda * inverse(P));
    }
}


#endif

// end of file
