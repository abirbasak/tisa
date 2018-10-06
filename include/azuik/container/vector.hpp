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
                    , m_bos{nullptr}
                    , m_eos{nullptr}
                    , eod{nullptr}
                {}
                constexpr vector_base(size_type n, allocator_type const& a = {})
                    : allocator_type{a}
                {
                    m_bos = eod = allocate(n);
                    m_eos = m_bos + n;
                }
                ~vector_base()
                {
                    deallocate(m_bos, capacity());
                }
                auto constexpr make_empty() noexcept -> void
                {
                    m_bos = m_eos = eod = nullptr;
                }
                auto constexpr allocate(size_type n) -> pointer
                {
                    return alloc_ref().template allocate<value_type>(n);
                }
                auto constexpr deallocate(pointer p, size_type n) -> void
                {
                    return alloc_ref().template deallocate<value_type>(m_bos, n);
                }
                auto constexpr alloc_ref() noexcept -> allocator_type&
                {
                    return static_cast<allocator_type&>(*this);
                }
                auto constexpr alloc_ref() const noexcept -> allocator_type const&
                {
                    return static_cast<allocator_type const&>(*this);
                }
                auto constexpr capacity() const noexcept
                {
                    return m_eos - m_bos;
                }
                auto constexpr size() const noexcept
                {
                    return eod - m_bos;
                }
                auto constexpr remaining1() const noexcept -> size_type
                {
                    return 0;
                }
                auto constexpr remaining2() const noexcept -> size_type
                {
                    return m_eos - eod;
                }

                auto constexpr full1() const noexcept -> bool
                {
                    return true;
                }
                auto constexpr full2() const noexcept -> bool
                {
                    return eod == m_eos;
                }
                auto constexpr bos() const noexcept -> pointer
                {
                    return static_cast<pointer>(m_bos);
                }
                auto constexpr eos() const noexcept -> pointer
                {
                    return static_cast<pointer>(m_eos);
                }

                auto swap(vector_base& that) noexcept
                {
                    std::swap(alloc_ref(), that.alloc_ref());
                    std::swap(m_bos, that.m_bos);
                    std::swap(m_eos, that.m_eos);
                    std::swap(eod, that.eod);
                }
                pointer eod;

            private:
                pointer m_bos;
                pointer m_eos;
            };

        } // namespace detail_

        struct with_capacity {
            size_t value;
        };
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
            explicit constexpr vector(with_capacity c, allocator_type const& a = {}) noexcept
                : base_type{c.value, a}
            {}
            explicit constexpr vector(size_type n, allocator_type const& a = {})
                : base_type{n, a}
            {
                this->eod = core::uninitialized_value_construct_n(this->eod, n);
            }
            explicit constexpr vector(size_type n, value_type const& x,
                                      allocator_type const& a = {})
                : base_type{n, a}
            {
                this->eod = core::uninitialized_fill_n(this->eod, n, x);
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
                this->eod = core::uninitialized_copy(that.bos(), that.eod, this->eod);
            }
            vector(self_type const& that, allocator_type const& a)
                : base_type{that.size(), a}
            {
                this->eod = core::uninitialized_copy(that.bos(), that.eod, this->eod);
            }
            vector(self_type&& that) noexcept
                : base_type(static_cast<base_type&>(that))
            {
                that.make_empty();
            }
            ~vector()
            {
                core::destroy(this->bos(), this->eod);
            }
            self_type& operator=(self_type& that)
            {
                if (this != &that)
                {
                    assign(that.begin(), that.end());
                }
                return *this;
            }
            self_type& operator=(self_type&& that)
            {
                if (this != &that)
                {
                    (*this).swap(that);
                }
                return *this;
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
                return const_iterator{*this, this->bos()};
            }
            auto constexpr end() const noexcept -> const_iterator
            {
                return const_iterator{*this, this->eod};
            }
            auto constexpr begin() noexcept -> iterator
            {
                return iterator{*this, this->bos()};
            }
            auto constexpr end() noexcept -> iterator
            {
                return iterator{*this, this->eod};
            }
            auto constexpr operator[](size_type i) noexcept -> reference
            {
                assert(i < size() && "out_of_range");
                return this->bos()[i];
            }
            auto constexpr operator[](size_type i) const noexcept -> const_reference
            {
                assert(i < size() && "out_of_range");
                return this->bos()[i];
            }
            template <class... Args>
            auto constexpr push_back(Args&&... args) -> void
            {
                if (this->full2())
                {
                    reserve(2 * size() + 1);
                }
                core::construct(this->eod, static_cast<Args&&>(args)...);
                ++this->eod;
            }
            auto constexpr pop_back() noexcept -> void
            {
                core::destroy(this->eod - 1);
                --this->eod;
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            auto constexpr append(InIter first, InIter last) -> void
            {
                append(first, last, core::iterator_category<InIter>{});
            }
            auto constexpr append(size_type n, value_type const& x) -> void
            {
                if (this->remaining2() < n)
                {
                    reserve(std::max(size() + n, 2 * size()));
                }
                this->eod = core::uninitialized_fill_n(this->eod, n, x);
            }

            auto constexpr insert(const_iterator p, value_type const& x) -> iterator
            {
                return insert(p, &x, &x + 1);
            }
            auto constexpr insert(const_iterator p, size_type n, value_type const& x) -> iterator
            {
                node_ptr pos = p.get_node();
                size_type offset = pos - this->bos();

                if (this->remaining2() < n)
                {
                    self_type temp;
                    temp.reserve(std::max(this->size() + n, 2 * this->size()));
                    temp.eod = core::uninitialized_safe_move(this->bos(), pos, temp.eod);
                    temp.eod = core::uninitialized_fill_n(temp.eod, n, x);
                    temp.eod = core::uninitialized_safe_move(pos, this->eod, temp.eod);
                    swap(temp);
                }
                else
                {
                    size_type old_size = size();
                    append(n, x);
                    std::rotate(this->bos() + offset, this->bos() + old_size, this->eod);
                }
                return iterator{*this, this->bos() + offset + n};
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            auto constexpr insert(const_iterator p, InIter first, InIter last) -> iterator
            {
                return insert(p, first, last, core::iterator_category<InIter>{});
            }
            auto constexpr erase(const_iterator first, const_iterator last) -> iterator
            {
                auto p = std::move(last.get_node(), this->eod, first.get_node());
                core::destroy(p, this->eod);
                this->eod = p;
                return iterator{*this, first.get_node()};
            }
            auto constexpr erase(const_iterator p) -> iterator
            {
                return erase(p, std::next(p));
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            void assign(InIter first, InIter last)
            {
                assign(first, last, core::iterator_category<InIter>{});
            }
            void assign(size_type n, value_type const& x)
            {
                if (this->remaining2() < n)
                {
                    self_type temp{n, x, base_type::alloc_ref()};
                    (*this).swap(temp);
                }
                else
                {
                    if (n > size())
                    {
                        std::fill(this->bos(), this->eod, x);
                        this->eod = core::uninitialized_fill_n(this->eod, n - size(), x);
                    }
                    else
                    {
                        auto p = std::fill_n(this->bos(), n, x);
                        core::destroy(p, this->eod);
                        this->eod = p;
                    }
                }
            }
            void reserve(size_type n)
            {
                if (n > this->capacity())
                {
                    self_type temp{with_capacity{n}, base_type::alloc_ref()};
                    temp.eod = core::uninitialized_safe_move(this->bos(), this->eod, temp.bos());
                    (*this).swap(temp);
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
                    append(n - this->m_size, value_type{});
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
                    append(n - this->m_size, x);
                }
            }
            auto constexpr clear() noexcept -> void
            {
                core::destroy(this->bos(), this->eod);
                this->eod = this->bos();
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
            template <class InIter>
            auto assign(InIter first, InIter last, core::input_iterator_tag) -> void
            {
                auto p = this->bos();
                for (; first != last && p != this->eod; ++first, void(), ++p)
                {
                    *p = *first;
                }
                if (first == last)
                {
                    core::destroy(p, this->eod);
                    this->eod = p;
                }
                else
                {
                    append(first, last);
                }
            }
            template <class FwdIter>
            auto assign(FwdIter first, FwdIter last, core::forward_iterator_tag) -> void
            {
                auto n = std::distance(first, last);
                if (this->remaining2() < n)
                {
                    self_type temp{first, last, base_type::alloc_ref()};
                    (*this).swap(temp);
                }
                else
                {
                    if (n > size())
                    {
                        auto p = first + size();
                        std::copy(first, p, this->bos());
                        this->eod = core::uninitialized_copy(p, last, this->eod);
                    }
                    else
                    {
                        auto p = std::copy(first, last, this->bos());
                        core::destroy(p, this->eod);
                        this->eod = p;
                    }
                }
            }

            template <class InIter>
            void append(InIter first, InIter last, core::input_iterator_tag)
            {
                for (; first != last; ++first)
                {
                    push_back(*first);
                }
            }
            template <class FwdIter>
            void append(FwdIter first, FwdIter last, core::forward_iterator_tag)
            {
                auto n = std::distance(first, last);
                if (this->remaining2() < n)
                {
                    reserve(std::max(size() + n, 2 * size()));
                }
                this->eod = core::uninitialized_copy_n(first, n, this->eod);
            }

            template <class InIter>
            auto constexpr insert(const_iterator p, InIter first, InIter last,
                                  core::input_iterator_tag tag) -> iterator
            {
                size_type offset = p.get_node() - this->m_bos;
                size_type old_size = this->m_size;
                append(first, last, tag);
                std::rotate(this->m_bos + offset, this->m_bos + old_size,
                            this->m_bos + this->m_size);
                return end() - old_size + offset;
            }
            template <class FwdIter>
            auto constexpr insert(const_iterator p, FwdIter first, FwdIter last,
                                  core::forward_iterator_tag tag) -> iterator
            {
                auto n = std::distance(first, last);
                node_ptr pos = p.get_node();
                size_type offset = pos - this->bos();

                if (this->remaining2() < n)
                {
                    self_type temp;
                    temp.reserve(std::max(size() + n, 2 * size()));
                    temp.append(this->bos(), pos);
                    temp.append(first, last);
                    temp.append(pos, this->eod);
                    swap(temp);
                }
                else
                {
                    size_type old_size = size();
                    append(first, last);
                    std::rotate(this->bos() + offset, this->bos() + old_size, this->eod);
                }
                return iterator{*this, this->bos() + offset};
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
