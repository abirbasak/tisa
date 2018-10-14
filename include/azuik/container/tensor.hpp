#ifndef AZUIK_CONTAINER_TENSOR_HPP
#define AZUIK_CONTAINER_TENSOR_HPP
#include <azuik/core/allocator.hpp>
namespace azuik
{
    namespace core
    {
        using ssize_t = std::ptrdiff_t;

        template <ssize_t... Extents>
        class shape {
        private:
            ssize_t value[sizeof...(Extents)];
        };
        template <class T, class Shape, class A>
        class tensor;

        template <class T, class Shape, class A>
        class tensor {
        public:
            using self_type = tensor<T, A>;
            using allocator_type = A;
            using value_type = core::allocator_value<T, A>;
            using size_type = core::allocator_size<T, A>;
            using difference_type = core::allocator_difference<T, A>;
            using reference = core::allocator_reference<T, A>;
            using pointer = core::allocator_pointer<T, A>;
            using const_reference = core::allocator_const_reference<T, A>;
            using const_pointer = core::allocator_const_pointer<T, A>;

        private:
            pointer m_ptr;
        };
    } // namespace core
} // namespace azuik
#endif
