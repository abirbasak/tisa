#ifndef AZUIK_CONTAINER_TENSOR_HPP
#define AZUIK_CONTAINER_TENSOR_HPP
#include <azuik/core/allocator.hpp>
namespace azuik
{
    namespace core
    {
        template <size_t... Extents>
        class shape {
        public:
            template <class... Args>
            shape(std::in_place_t, Args... e)
                : value{e...}
            {}
            template <class... Args>
            auto constexpr linear_index(Args... args) const noexcept -> size_t
            {
                std::array<size_t, sizeof...(Args)> coord{args...};
                size_t offset = 0;
                for (size_t i = 0; i != sizeof...(Args); ++i)
                {
                    offset += value[i] * coord[i];
                }
                return offset;
            }

        private:
            size_t value[sizeof...(Extents)];
        }; // namespace core
        template <class T, class Shape, class A>
        class tensor;

        template <class T, class Shape, class A>
        class tensor : private A, Shape {
        public:
            using self_type = tensor<T, Shape, A>;
            using shape_type = Shape;
            using allocator_type = A;
            using value_type = core::allocator_value<T, A>;
            using size_type = core::allocator_size<T, A>;
            using difference_type = core::allocator_difference<T, A>;
            using reference = core::allocator_reference<T, A>;
            using pointer = core::allocator_pointer<T, A>;
            using const_reference = core::allocator_const_reference<T, A>;
            using const_pointer = core::allocator_const_pointer<T, A>;
            using iterator = pointer;
            using const_iterator = const_pointer;

        public:
            auto constexpr rank() const noexcept -> size_type
            {
                return shape_type::rank();
            }
            auto constexpr extent(size_type i) const noexcept -> difference_type
            {
                return shape_type::extent(i);
            }
            auto constexpr shape() const noexcept -> shape_type const&
            {
                return static_cast<shape_type const&>(*this);
            }
            auto constexpr size() const noexcept -> size_type
            {
                return shape_type::size();
            }
            auto constexpr empty() const noexcept -> bool
            {
                return 0 == shape_type::size();
            }
            auto constexpr get_allocator() const noexcept
            {
                return static_cast<allocator_type const&>(*this);
            }
            auto constexpr begin() const noexcept -> const_iterator
            {
                return m_ptr;
            }
            auto constexpr end() const noexcept -> const_iterator
            {
                return m_ptr + shape_type::size();
            }
            auto constexpr begin() noexcept -> iterator
            {
                return m_ptr;
            }
            auto constexpr end() noexcept -> iterator
            {
                return m_ptr + shape_type::size();
            }

        private:
            auto constexpr alloc_ref() noexcept -> allocator_type&
            {
                return static_cast<allocator_type&>(*this);
            }

        private:
            pointer m_ptr;
        };
    } // namespace core
} // namespace azuik
#endif
