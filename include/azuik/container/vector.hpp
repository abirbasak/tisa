#ifndef AZUIK_CONTAINER_VECTOR_HPP
#define AZUIK_CONTAINER_VECTOR_HPP
#include <azuik/core/allocator.hpp>
#include <azuik/container/iterable.hpp>
namespace azuik
{
    namespace core
    {
        template <class T, class A = allocator>
        class vector;

        template <class T, class A>
        struct iterable_traits<vector<T, A>> : iterable_traits_from_allocator<T, A> {
            using iterator = contiguous_iterator<vector<T, A>>;
            using const_iterator = contiguous_iterator<vector<T, A> const>;
        };

        template <class T, class A>
        struct sequence_traits<vector<T, A>> {
            using allocator_type = A;
        };

        template <class T, class A>
        class vector : private A {
        public:
            using self_type = vector;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using pointer = core::pointer<self_type>;
            using reference = core::reference<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using const_reference = core::const_reference<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;

        public:
            auto constexpr get_allocator() const noexcept -> allocator_type
            {
                return static_cast<allocator_type>(*this);
            }
            auto constexpr capacity() const noexcept -> size_type
            {
                return m_capacity;
            }
            auto constexpr size() const noexcept -> size_type
            {
                return m_size;
            }
            auto constexpr empty() const noexcept -> bool
            {
                return m_size == 0;
            }
            auto constexpr begin() const noexcept -> const_iterator
            {
                return const_iterator{*this, m_ptr};
            }
            auto constexpr end() const noexcept -> const_iterator
            {
                return const_iterator{*this, m_ptr + m_size};
            }
            auto constexpr begin() noexcept -> iterator
            {
                return iterator{*this, m_ptr};
            }
            auto constexpr end() noexcept -> iterator
            {
                return iterator{*this, m_ptr + m_size};
            }

        private:
            auto constexpr alloc_ref() const noexcept -> allocator_type const&
            {
                return static_cast<allocator_type const&>(*this);
            }
            auto constexpr alloc_ref() noexcept -> allocator_type&
            {
                return static_cast<allocator_type&>(*this);
            }

        private:
            pointer m_ptr;
            size_type m_size;
            size_type m_capacity;
        };

    } // namespace core
} // namespace azuik
#endif
