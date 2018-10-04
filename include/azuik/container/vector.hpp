#ifndef AZUIK_CONTAINER_VECTOR_HPP
#define AZUIK_CONTAINER_VECTOR_HPP
#include <azuik/core/allocator.hpp>
#include <azuik/container/iterator.hpp>
#include <azuik/algorithm/functional.hpp>
#include <azuik/algorithm/sequential.hpp>
#include <algorithm>
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

        namespace detail_
        {
            template <class T, class A>
            struct vector_base : private A {
                using allocator_type = A;
                using size_type = core::allocator_size<T, A>;
                using value_type = core::allocator_value<T, A>;
                using pointer = core::allocator_pointer<T, A>;

                constexpr vector_base(allocator_type const& a = {}) noexcept
                    : allocator_type{a}
                    , m_ptr{nullptr}
                    , m_capacity{0}
                    , m_size{0}
                {}
                constexpr vector_base(size_type n, allocator_type const& a = {})
                    : allocator_type{a}
                {
                    m_ptr = allocate(n);
                    m_capacity = n;
                    m_size = 0;
                }
                ~vector_base()
                {
                    deallocate(m_ptr, m_capacity);
                }
                auto constexpr allocate(size_type n) -> pointer
                {
                    return alloc_ref().template allocate<value_type>(n);
                }
                auto constexpr deallocate(pointer p, size_type n) -> void
                {
                    return alloc_ref().template deallocate<value_type>(m_ptr, n);
                }
                auto constexpr alloc_ref() noexcept
                {
                    return static_cast<allocator_type&>(*this);
                }
                auto constexpr alloc_ref() const noexcept
                {
                    return static_cast<allocator_type const&>(*this);
                }
                auto constexpr capacity() const noexcept
                {
                    return m_capacity;
                }
                auto constexpr size() const noexcept
                {
                    return m_size;
                }
                auto swap(vector_base& that) noexcept
                {
                    std::swap(m_ptr, that.m_ptr);
                    std::swap(m_capacity, that.m_capacity);
                    std::swap(m_size, that.m_size);
                }
                pointer m_ptr;
                size_type m_capacity;
                size_type m_size;
            };

        } // namespace detail_
        template <class T, class A>
        class vector : private detail_::vector_base<T, A> {
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

        private:
            template <class>
            friend class detail_::contiguous_policy;
            using base_type = detail_::vector_base<T, A>;
            using node_ptr = pointer;
            using node_cptr = const_pointer;

        public:
            explicit constexpr vector(allocator_type const& a = {}) noexcept
                : base_type{a}
            {}
            explicit constexpr vector(size_type n, allocator_type const& a = {})
                : base_type{n, a}
            {
                core::uninitialized_value_construct_n(this->m_ptr, n);
                this->m_size = n;
            }
            explicit constexpr vector(size_type n, value_type const& x,
                                      allocator_type const& a = {})
                : base_type{n, a}
            {
                core::uninitialized_fill_n(this->m_ptr, n, x);
                this->m_size = n;
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            constexpr vector(InIter first, InIter last, allocator_type const& a = {})
                : base_type{a}
            {
                append(first, last);
            }
            vector(self_type const& that)
                : base_type{that.size(), that.get_allocator()}
            {
                core::uninitialized_copy_n(that.m_ptr, that.m_size, this->m_ptr);
                this->m_size = that.m_size;
            }
            vector(self_type const& that, allocator_type const& a)
                : base_type{that.size(), a}
            {
                core::uninitialized_copy_n(that.m_ptr, that.m_size, this->m_ptr);
                this->m_size = that.m_size;
            }
            ~vector()
            {
                core::destroy_n(this->m_ptr, this->m_size);
            }
            auto constexpr get_allocator() const noexcept -> allocator_type
            {
                return base_type::alloc_ref();
            }
            auto constexpr capacity() const noexcept -> size_type
            {
                return base_type::capacity();
            }
            auto constexpr size() const noexcept -> size_type
            {
                return base_type::size();
            }
            auto constexpr empty() const noexcept -> bool
            {
                return base_type::size() == 0;
            }
            auto constexpr begin() const noexcept -> const_iterator
            {
                return const_iterator{*this, static_cast<node_ptr>(this->m_ptr)};
            }
            auto constexpr end() const noexcept -> const_iterator
            {
                return const_iterator{*this, static_cast<node_ptr>(this->m_ptr) + this->m_size};
            }
            auto constexpr begin() noexcept -> iterator
            {
                return iterator{*this, this->m_ptr};
            }
            auto constexpr end() noexcept -> iterator
            {
                return iterator{*this, this->m_ptr + this->m_size};
            }
            auto constexpr operator[](size_type i) noexcept -> reference
            {
                assert(i < this->m_size && "out_of_range");
                return this->m_ptr[i];
            }
            auto constexpr operator[](size_type i) const noexcept -> const_reference
            {
                assert(i < this->m_size && "out_of_range");
                return this->m_ptr[i];
            }
            template <class... Args>
            auto constexpr push_back(Args&&... args) -> void
            {
                if (this->m_capacity == this->m_size)
                {
                    reserve(2 * this->m_size + 1);
                }
                core::construct(this->m_ptr + this->m_size, static_cast<Args&&>(args)...);
                ++this->m_size;
            }
            auto constexpr pop_back() noexcept -> void
            {
                core::destroy(this->m_ptr + this->m_size - 1);
                --this->m_size;
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            auto constexpr append(InIter first, InIter last) -> void
            {
                append(first, last, core::iterator_category<InIter>{});
            }
            template <class FwdIter>
            auto constexpr append(FwdIter first, size_type n) -> void
            {
                append_n(first, n);
            }
            template <class... Args>
            auto constexpr insert(const_iterator p, Args&&... args) -> iterator
            {
                throw;
            }
            template <class InIter>
            auto constexpr insert(const_iterator p, InIter first, InIter last) -> void
            {}
            auto constexpr erase(const_iterator first, const_iterator last) -> iterator
            {
                auto p = std::move(last.get_node(), this->m_ptr + this->m_size, first.get_node());
                core::destroy(p, this->m_ptr + this->m_size);
                this->m_size = p - this->m_ptr;
                return iterator{*this, first.get_node()};
            }
            auto constexpr erase(const_iterator p) -> iterator
            {
                return erase(p, std::next(p));
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            void assign(InIter first, InIter last)
            {}
            void assign(size_type n, value_type const& x) {}
            void reserve(size_type n)
            {
                if (n > this->m_capacity)
                {
                    resize_memory(n);
                }
            }
            void resize(size_type n)
            {
                if (n < this->m_size)
                {
                    erase(begin() + n, end());
                }
                else
                {
                    append_n(n - this->m_size, value_type{});
                }
            }
            void resize(size_type n, value_type const& x)
            {
                if (n < this->m_size)
                {
                    erase(begin() + n, end());
                }
                else
                {
                    append_n(n - this->m_size, x);
                }
            }
            auto constexpr clear() noexcept -> void
            {
                core::destroy_n(this->m_ptr, this->m_size);
                this->m_size = 0;
            }

            void swap(self_type& that) noexcept
            {
                base_type::swap(static_cast<base_type&>(that));
            }
            auto constexpr operator==(self_type const& that) const noexcept -> bool
            {
                return ((size() == that.size()) && std::equal(begin(), end(), that.begin()));
            }
            auto constexpr operator!=(self_type const& that) const noexcept -> bool
            {
                return ((size() != that.size()) || !std::equal(begin(), end(), that.begin()));
            }
            auto constexpr operator<(self_type const& that) const noexcept -> bool
            {
                return std::lexicographical_compare(begin(), end(), that.begin(), that.end());
            }
            auto constexpr operator>(self_type const& that) const noexcept -> bool
            {
                return that < (*this);
            }
            auto constexpr operator<=(self_type const& that) const noexcept -> bool
            {
                return !(that < (*this));
            }
            auto constexpr operator>=(self_type const& that) const noexcept -> bool
            {
                return !((*this) < that);
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
            void resize_memory(size_type n)
            {
                auto ptr = base_type::allocate(n);
                core::uninitialized_safe_move(this->m_ptr, this->m_ptr + this->m_size, ptr);
                core::destroy_n(this->m_ptr, this->m_size);
                base_type::deallocate(this->m_ptr, this->m_size);
                this->m_ptr = ptr;
                this->m_capacity = n;
            }
            void assign_n(size_type n, value_type const& x)
            {
                if (n > this->m_size)
                {
                    std::fill_n(this->m_ptr, this->m_size, x);
                    core::uninitialized_fill_n(this->m_ptr + this->m_size, n - this->m_size, x);
                    this->m_size = n;
                }
                else
                {
                    std::fill_n(this->m_ptr, n, x);
                    core::destroy_n(this->m_ptr + this->m_size, this->m_size - n);
                    this->m_size = n;
                }
            }
            template <class InIter>
            void append(InIter first, InIter last, std::input_iterator_tag)
            {
                for (; first != last; ++first)
                {
                    push_back(*first);
                }
            }
            template <class FwdIter>
            void append(FwdIter first, FwdIter last, std::forward_iterator_tag)
            {
                auto n = std::distance(first, last);
                append_n(first, n);
            }
            template <class FwdIter>
            void append_n(FwdIter first, size_type n)
            {
                if (this->m_capacity - this->m_size < n)
                {
                    reserve(std::max(this->m_size + n, 2 * this->m_size));
                }
                core::uninitialized_copy_n(first, n, this->m_ptr + this->m_size);
                this->m_size += n;
            }
        };

        template <class T, class A = allocator>
        class dvector;

        template <class T, class A>
        struct iterable_traits<dvector<T, A>> : iterable_traits_from_allocator<T, A> {
            using iterator = contiguous_iterator<dvector<T, A>>;
            using const_iterator = contiguous_iterator<dvector<T, A> const>;
        };

        template <class T, class A>
        struct sequence_traits<dvector<T, A>> {
            using allocator_type = A;
        };

        template <class T, class A>
        class dvector {
        public:
            using self_type = dvector;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using reference = core::reference<self_type>;
            using pointer = core::pointer<self_type>;
            using const_reference = core::const_reference<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;

        public:
            auto constexpr size() const noexcept -> size_type
            {
                return m_size;
            }
            auto constexpr empty() const noexcept -> bool
            {
                return m_size == 0;
            }
            auto constexpr capacity() const noexcept -> size_type
            {
                return m_capacity;
            }
            auto constexpr begin() const noexcept -> const_iterator
            {
                return const_iterator{m_ptr + m_offset};
            }
            auto constexpr end() const noexcept -> const_iterator
            {
                return const_iterator{m_ptr + m_offset + m_size};
            }
            auto constexpr begin() noexcept -> iterator
            {
                return iterator{m_ptr + m_offset};
            }
            auto constexpr end() noexcept -> iterator
            {
                return iterator{m_ptr + m_offset + m_size};
            }
            auto constexpr operator[](size_type i) noexcept -> reference
            {
                assert(i < m_size && "out_of_range");
                return m_ptr[m_offset + i];
            }
            auto constexpr operator[](size_type i) const noexcept -> const_reference
            {
                assert(i < m_size && "out_of_range");
                return m_ptr[m_offset + i];
            }

        private:
            pointer m_ptr;
            size_type m_size;
            size_type m_capacity;
            size_type m_offset;
        };

        template <class V, class Comp = less_fn>
        class assoc_vector;

        template <class V, class Comp>
        struct iterable_traits<assoc_vector<V, Comp>> : public iterable_traits_from_sequence<V> {
            using iterator = core::iterator<V>;
            using const_iterator = core::const_iterator<V>;
        };

        template <class V, class Comp>
        struct sequence_traits<assoc_vector<V, Comp>> {
            using allocator_type = core::allocator_type<V>;
        };
        template <class V, class Comp>
        class assoc_vector : private V, Comp {
        private:
            using base_type = V;

        public:
            using self_type = assoc_vector;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using reference = core::reference<self_type>;
            using pointer = core::pointer<self_type>;
            using const_reference = core::const_reference<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;
            using key_compare = Comp;

        public:
            constexpr explicit assoc_vector(allocator_type const& a = {})
                : base_type{a}
                , key_compare{}
            {}
            constexpr explicit assoc_vector(key_compare const& comp,
                                            allocator_type const& a = {}) noexcept
                : base_type{a}
                , key_compare{comp}
            {}
            template <class Key>
            auto constexpr lower_bound(Key const& key) noexcept -> iterator
            {
                return std::lower_bound(begin(), end(), key, comp_ref());
            }
            template <class Key>
            auto constexpr lower_bound(Key const& key) const noexcept -> const_iterator
            {
                return std::lower_bound(begin(), end(), key, comp_ref());
            }

            template <class Key>
            auto constexpr upper_bound(Key const& key) noexcept -> iterator
            {
                return std::upper_bound(begin(), end(), key, comp_ref());
            }

            template <class Key>
            auto constexpr upper_bound(Key const& key) const noexcept -> const_iterator
            {
                return std::upper_bound(begin(), end(), key, comp_ref());
            }

            template <class Key>
            auto constexpr equal_range(Key const& key) noexcept -> iterator
            {
                return std::equal_range(begin(), end(), key, comp_ref());
            }

            template <class Key>
            auto constexpr equal_range(Key const& key) const noexcept -> const_iterator
            {
                return std::equal_range(begin(), end(), key, comp_ref());
            }
            template <class Key>
            auto constexpr find(Key const& key) const noexcept -> const_iterator
            {
                return core::find_sorted(begin(), end(), key, comp_ref());
            }
            template <class Key>
            auto constexpr find(Key const& key) noexcept -> iterator
            {
                return core::find_sorted(begin(), end(), key, comp_ref());
            }
            template <class Key>
            auto constexpr erase(Key const& key) -> size_type
            {
                auto pos = find(key);
                if (pos == end())
                {
                    return 0;
                }
                erase(pos);
                return 1;
            }
            auto constexpr key_comp() const noexcept -> key_compare
            {
                return static_cast<key_compare const&>(*this);
            }

        public:
            using base_type::get_allocator;

            using base_type::begin;
            using base_type::end;

            using base_type::empty;
            using base_type::size;
            using base_type::capacity;

            using base_type::erase;
            using base_type::reserve;
            using base_type::clear;

        private:
            auto constexpr comp_ref() const noexcept -> key_compare const&
            {
                return static_cast<key_compare const&>(*this);
            }
        };

        template <class T, class A>
        class stable_vector;

        template <class T, class A>
        class stable_vector : private A {
        public:
            using self_type = stable_vector<T, A>;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using reference = core::reference<self_type>;
            using pointer = core::pointer<self_type>;
            using const_reference = core::const_reference<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;

        private:
            struct sentinel_node {
                pointer up;
            };
            using sentinel_ptr = core::allocator_pointer<allocator_type, sentinel_node>;
            struct node;
            using node_ptr = core::allocator_pointer<allocator_type, node>;
            using node_cptr = core::allocator_const_pointer<allocator_type, node>;

            struct node : sentinel_node {
                template <class... Args>
                node(std::in_place_t, pointer up, Args&&... args)
                    : sentinel_node{up}
                    , value{static_cast<Args&&>(args)...}
                {}

                template <class... Args>
                static auto make(allocator_type& a, Args&&... args, node_ptr up) -> node_ptr
                {
                    node_ptr np =
                        core::allocate_and_construct<node>(a, static_cast<Args&&>(args)...);
                    np->up = up;
                    return np;
                }
                static void drop(allocator_type& a, node_cptr p)
                {
                    core::destroy(p);
                    a.deallocate(p);
                }
                value_type value;
            };

            using index_type = std::vector<node_ptr>;
            using index_iterator = core::iterator<index_type>;

        public:
            explicit stable_vector(allocator_type const& a = {});

            ~stable_vector()
            {
                clear();
                drop_sentinel();
            }

            auto constexpr begin() noexcept -> iterator
            {
                return iterator{*m_indices.begin()};
            }
            auto constexpr end() noexcept -> iterator
            {
                return iterator{*std::prev(m_indices.end())};
            }
            auto constexpr begin() const noexcept -> const_iterator
            {
                return const_iterator{*m_indices.begin()};
            }
            auto constexpr end() const noexcept -> const_iterator
            {
                return const_iterator{*std::prev(m_indices.back())};
            }
            auto constexpr get_allocator() const noexcept -> allocator_type
            {
                return static_cast<allocator_type const&>(*this);
            }
            auto constexpr size() const noexcept
            {
                return m_indices.size() - 1;
            }
            auto constexpr capacity() const noexcept
            {
                return m_indices.capacity() - 1;
            }
            auto constexpr empty() const noexcept -> bool
            {
                return m_indices.size() == 1;
            }
            auto constexpr operator[](size_type n) noexcept -> reference
            {
                return m_indices[n]->value;
            }
            auto constexpr operator[](size_type n) const noexcept -> const_reference
            {
                return m_indices[n]->value;
            }
            template <class... Args>
            void push_back(Args&&... args)
            {
                insert(end(), static_cast<Args&&>(args)...);
            }
            void pop_back()
            {
                erase(std::prev(end()));
            }
            auto constexpr erase(const_iterator p) -> iterator
            {
                difference_type offset = p - begin();
                auto it = m_indices.begin() + offset;
                node::drop(*it);
                m_indices.erase(it);
                reindex(m_indices.begin() + offset, m_indices.end());
                return begin() + offset;
            }

            auto constexpr erase(const_iterator first, const_iterator last) -> iterator
            {
                difference_type offset1 = first - begin();
                difference_type offset2 = last - begin();
                auto it1 = m_indices.begin() + offset1;
                auto it2 = m_indices.begin() + offset2;
                for (auto it = it1; it != it2; ++it)
                {
                    node::drop(*it);
                }
                m_indices.erase(it1, it2);
                reindex(m_indices.begin() + offset1, m_indices.end());
                return begin() + offset1;
            }
            auto constexpr clear() -> void
            {
                erase(begin(), end());
            }

        private:
            auto constexpr alloc_ref() noexcept -> allocator_type&
            {
                return static_cast<allocator_type&>(*this);
            }
            void make_sentinel()
            {
                node_ptr p = alloc_ref().allocate(1);
                m_indices.back() = p;
                p->up = &m_indices.back();
            }

            void drop_sentinel()
            {
                alloc_ref().deallocate(static_cast<sentinel_ptr>(m_indices.back()));
            }
            static auto constexpr reindex(index_iterator first, index_iterator last) noexcept
            {
                for (; first != last; ++first)
                {
                    (*first)->up = &*first;
                }
            }

        private:
            index_type m_indices;
        };
    } // namespace core
} // namespace azuik
#endif
