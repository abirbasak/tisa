#ifndef AZUIK_CONTAINER_VECTOR_HPP
#define AZUIK_CONTAINER_VECTOR_HPP
#include <azuik/core/allocator.hpp>
#include <azuik/container/iterator.hpp>
#include <azuik/algorithm/functional.hpp>
#include <azuik/algorithm/sequential.hpp>
#include <algorithm>
#include <vector>
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
            struct contiguous_storage : private A {
                using allocator_type = A;
                using size_type = core::allocator_size<T, A>;
                using value_type = core::allocator_value<T, A>;
                using pointer = core::allocator_pointer<T, A>;
                constexpr explicit contiguous_storage(allocator_type const& a = {}) noexcept
                    : allocator_type{a}
                    , bos{nullptr}
                    , eos{nullptr}
                {}
                constexpr explicit contiguous_storage(size_type n, allocator_type const& a = {})
                    : allocator_type{a}
                {
                    bos = allocate(n);
                    eos = bos + n;
                }
                ~contiguous_storage()
                {
                    deallocate(bos, capacity());
                }
                auto constexpr capacity() const noexcept -> size_type
                {
                    return eos - bos;
                }
                auto constexpr get_allocator() const noexcept -> allocator_type
                {
                    return static_cast<allocator_type const&>(*this);
                }
                auto constexpr alloc_ref() noexcept -> allocator_type&
                {
                    return static_cast<allocator_type&>(*this);
                }
                auto allocate(size_type n) -> pointer
                {
                    return alloc_ref().template allocate<value_type>(n);
                }
                auto constexpr deallocate(pointer p, size_type n) -> void
                {
                    return alloc_ref().template deallocate<value_type>(p, n);
                }
                auto constexpr swap(contiguous_storage& that) noexcept
                {
                    std::swap(alloc_ref(), that.alloc_ref());
                    std::swap(bos, that.bos);
                    std::swap(eos, that.eos);
                }
                pointer bos;
                pointer eos;
            };

            template <class S>
            struct contiguous_operations {
                auto constexpr operator==(S const& that) const noexcept -> bool
                {
                    return ((self().size() == that.size())
                            && std::equal(self().begin(), self().end(), that.begin()));
                }
                auto constexpr operator!=(S const& that) const noexcept -> bool
                {
                    return ((self().size() != that.size())
                            || !std::equal(self().begin(), self().end(), that.begin()));
                }
                auto constexpr operator<(S const& that) const noexcept -> bool
                {
                    return std::lexicographical_compare(self().begin(), self().end(), that.begin(),
                                                        that.end());
                }
                auto constexpr operator>(S const& that) const noexcept -> bool
                {
                    return that < (*this);
                }
                auto constexpr operator<=(S const& that) const noexcept -> bool
                {
                    return !(that < (*this));
                }
                auto constexpr operator>=(S const& that) const noexcept -> bool
                {
                    return !((*this) < that);
                }

            private:
                auto constexpr self() const noexcept -> S const&
                {
                    return static_cast<S const&>(*this);
                }
            };

        } // namespace detail_

        struct with_capacity {
            size_t value;
        };
        template <class T, class A>
        class vector : private detail_::contiguous_storage<T, A>,
                       public detail_::contiguous_operations<vector<T, A>> {
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
            friend struct detail_::contiguous_policy;
            using storage_type = detail_::contiguous_storage<T, A>;
            using base_type = detail_::contiguous_operations<vector<T, A>>;
            using node_ptr = pointer;
            using node_cptr = const_pointer;

        public:
            explicit constexpr vector(allocator_type const& a = {}) noexcept
                : storage_type{a}
                , eod{storage_type::bos}
            {}
            explicit constexpr vector(with_capacity c, allocator_type const& a = {}) noexcept
                : storage_type{c.value, a}
                , eod{storage_type::bos}
            {}
            explicit constexpr vector(size_type n, allocator_type const& a = {})
                : storage_type{n, a}
                , eod{storage_type::bos}
            {
                eod = core::uninitialized_value_construct_n(eod, n);
            }
            explicit constexpr vector(size_type n, value_type const& x,
                                      allocator_type const& a = {})
                : storage_type{n, a}
                , eod{storage_type::bos}
            {
                eod = core::uninitialized_fill_n(eod, n, x);
            }

            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            constexpr vector(InIter first, InIter last, allocator_type const& a = {})
                : storage_type{a}
                , eod{storage_type::bos}
            {
                append(first, last);
            }
            vector(self_type const& that)
                : storage_type{that.size(), that.get_allocator()}
                , eod{storage_type::bos}
            {
                eod = core::uninitialized_copy(that.bos, that.eod, eod);
            }
            vector(self_type const& that, allocator_type const& a)
                : storage_type{that.size(), a}
                , eod{storage_type::bos}
            {
                eod = core::uninitialized_copy(that.bos, that.eod, eod);
            }
            vector(self_type&& that) noexcept
                : storage_type(static_cast<storage_type&>(that))
                , eod{that.eod}
            {
                that.bos = that.eos = that.eod = nullptr;
            }
            ~vector()
            {
                core::destroy(storage_type::bos, eod);
            }
            auto constexpr operator=(self_type& that) -> vector&
            {
                if (this != &that)
                {
                    assign(that.begin(), that.end());
                }
                return *this;
            }
            auto constexpr operator=(self_type&& that) noexcept -> vector&
            {
                if (this != &that)
                {
                    (*this).swap(that);
                }
                return *this;
            }
            using storage_type::get_allocator;
            using storage_type::capacity;
            using base_type::operator==;
            using base_type::operator!=;
            using base_type::operator<;
            using base_type::operator<=;
            using base_type::operator>;
            using base_type::operator>=;

            auto constexpr size() const noexcept -> size_type
            {
                return eod - storage_type::bos;
            }
            auto constexpr remaining() const noexcept -> size_type
            {
                return storage_type::eos - eod;
            }
            auto constexpr empty() const noexcept -> bool
            {
                return eod == storage_type::bos;
            }
            auto constexpr full() const noexcept -> bool
            {
                return eod == storage_type::eos;
            }
            auto constexpr begin() const noexcept -> const_iterator
            {
                return const_iterator{*this, storage_type::bos};
            }
            auto constexpr end() const noexcept -> const_iterator
            {
                return const_iterator{*this, eod};
            }
            auto constexpr begin() noexcept -> iterator
            {
                return iterator{*this, storage_type::bos};
            }
            auto constexpr end() noexcept -> iterator
            {
                return iterator{*this, eod};
            }
            auto constexpr operator[](size_type i) noexcept -> reference
            {
                assert(i < size() && "out_of_range");
                return storage_type::bos[i];
            }
            auto constexpr operator[](size_type i) const noexcept -> const_reference
            {
                assert(i < size() && "out_of_range");
                return storage_type::bos[i];
            }
            template <class... Args>
            auto constexpr push_back(Args&&... args) -> void
            {
                //NOTE: we are not handling self references
                //it should be user's responsibility
                //value_type x{static_cast<Args&&>(args)...};
                if (full())
                {
                    reserve(2 * size() + 1);
                }
                core::construct(eod, static_cast<Args&&>(args)...);
                ++eod;
            }
            auto constexpr pop_back() noexcept -> void
            {
                assert(!empty() && "empty vector");
                eod = core::destroy(eod - 1);
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            auto constexpr append(InIter first, InIter last) -> void
            {
                append(first, last, core::iterator_category<InIter>{});
            }
            auto constexpr append(size_type n, value_type const& x) -> void
            {
                if (remaining() < n)
                {
                    reserve(std::max(size() + n, 2 * size()));
                }
                eod = core::uninitialized_fill_n(eod, n, x);
            }

            auto constexpr insert(const_iterator p, value_type const& x) -> iterator
            {
                return insert(p, &x, &x + 1);
            }
            auto constexpr insert(const_iterator p, size_type n, value_type const& x) -> iterator
            {
                node_ptr pos = p.get_node();
                size_type offset = pos - storage_type::bos;

                if (remaining() < n)
                {
                    self_type temp;
                    temp.reserve(std::max(size() + n, 2 * size()));
                    temp.eod = core::uninitialized_safe_move(storage_type::bos, pos, temp.eod);
                    temp.eod = core::uninitialized_fill_n(temp.eod, n, x);
                    temp.eod = core::uninitialized_safe_move(pos, eod, temp.eod);
                    swap(temp);
                }
                else
                {
                    size_type old_size = size();
                    append(n, x);
                    std::rotate(storage_type::bos + offset, storage_type::bos + old_size, eod);
                }
                return iterator{*this, storage_type::bos + offset + n};
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            auto constexpr insert(const_iterator p, InIter first, InIter last) -> iterator
            {
                return insert(p, first, last, core::iterator_category<InIter>{});
            }
            auto constexpr erase(const_iterator first, const_iterator last) -> iterator
            {
                auto p = std::move(last.get_node(), eod, first.get_node());
                eod = core::destroy(p, eod);
                return iterator{*this, first.get_node()};
            }
            auto constexpr erase(const_iterator p) -> iterator
            {
                return erase(p, core::next(p));
            }
            template <class InIter, core::disable_if<core::is_integral<InIter>, int> = 0>
            void assign(InIter first, InIter last)
            {
                assign(first, last, core::iterator_category<InIter>{});
            }
            void assign(size_type n, value_type const& x)
            {
                if (capacity() < n)
                {
                    self_type temp{n, x, storage_type::alloc_ref()};
                    (*this).swap(temp);
                }
                else
                {
                    if (n > size())
                    {
                        std::fill(storage_type::bos, eod, x);
                        eod = core::uninitialized_fill_n(eod, n - size(), x);
                    }
                    else
                    {
                        auto p = std::fill_n(storage_type::bos, n, x);
                        eod = core::destroy(p, eod);
                    }
                }
            }
            void reserve(size_type n)
            {
                if (n > capacity())
                {
                    self_type temp{with_capacity{n}, storage_type::alloc_ref()};
                    temp.eod = core::uninitialized_safe_move(storage_type::bos, eod, temp.bos);
                    (*this).swap(temp);
                }
            }
            void resize(size_type n)
            {
                if (n < size())
                {
                    erase(begin() + n, end());
                }
                else
                {
                    append(n - size(), value_type{});
                }
            }
            void resize(size_type n, value_type const& x)
            {
                if (n < size())
                {
                    erase(begin() + n, end());
                }
                else
                {
                    append(n - size(), x);
                }
            }
            auto constexpr clear() noexcept -> void
            {
                eod = core::destroy(storage_type::bos, eod);
            }

            void swap(self_type& that) noexcept
            {
                storage_type::swap(static_cast<storage_type&>(that));
                std::swap(eod, that.eod);
            }

        private:
            template <class InIter>
            auto assign(InIter first, InIter last, core::input_iterator_tag) -> void
            {
                auto [c, p] = core::copy(first, last, storage_type::bos, eod);
                if (c == last)
                {
                    core::destroy(p, eod);
                    eod = p;
                }
                else
                {
                    append(first, last);
                }
            }
            template <class FwdIter>
            auto assign(FwdIter first, FwdIter last, core::forward_iterator_tag) -> void
            {
                auto n = core::distance(first, last);
                if (capacity() < n)
                {
                    self_type temp{first, last, storage_type::alloc_ref()};
                    (*this).swap(temp);
                }
                else
                {
                    if (n > size())
                    {
                        auto p = first + size();
                        core::copy(first, p, storage_type::bos);
                        eod = core::uninitialized_copy(p, last, eod);
                    }
                    else
                    {
                        auto p = core::copy(first, last, storage_type::bos);
                        eod = core::destroy(p, eod);
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
                auto n = core::distance(first, last);
                if (remaining() < n)
                {
                    reserve(std::max(size() + n, 2 * size()));
                }
                eod = core::uninitialized_copy_n(first, n, eod);
            }

            template <class InIter>
            auto constexpr insert(const_iterator p, InIter first, InIter last,
                                  core::input_iterator_tag tag) -> iterator
            {
                size_type offset = p.get_node() - storage_type::bos;
                size_type old_size = size();
                append(first, last, tag);
                std::rotate(storage_type::bos + offset, storage_type::bos + old_size,
                            storage_type::bos + size());
                return end() - old_size + offset;
            }
            template <class FwdIter>
            auto constexpr insert(const_iterator p, FwdIter first, FwdIter last,
                                  core::forward_iterator_tag) -> iterator
            {
                auto n = core::distance(first, last);
                node_ptr pos = p.get_node();
                size_type offset = pos - storage_type::bos;

                if (remaining() < n)
                {
                    self_type temp;
                    temp.reserve(std::max(size() + n, 2 * size()));
                    temp.append(storage_type::bos, pos);
                    temp.append(first, last);
                    temp.append(pos, eod);
                    swap(temp);
                }
                else
                {
                    size_type old_size = size();
                    append(first, last);
                    std::rotate(storage_type::bos + offset, storage_type::bos + old_size, eod);
                }
                return iterator{*this, storage_type::bos + offset};
            }

        private:
            pointer eod;
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
        class dvector : detail_::contiguous_storage<T, A>,
                        detail_::contiguous_operations<dvector<T, A>> {
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

        private:
            using storage_type = detail_::contiguous_storage<T, A>;
            using base_type = detail_::contiguous_operations<dvector<T, A>>;
            using node_ptr = pointer;
            template <class>
            friend struct detail_::contiguous_policy;

        public:
            explicit constexpr dvector(allocator_type const& a = {}) noexcept
                : storage_type{a}
                , bod{storage_type::bos}
                , eod{storage_type::bos}
            {}
            explicit constexpr dvector(with_capacity c, allocator_type const& a = {})
                : storage_type{c.value, a}
                , bod{storage_type::bos}
                , eod{storage_type::bos}
            {}

            explicit constexpr dvector(size_type n, allocator_type const& a = {})
                : storage_type{n, a}
                , bod{storage_type::bos}
                , eod{storage_type::bos}
            {
                eod = core::uninitialized_value_construct_n(storage_type::bod, n);
            }
            explicit constexpr dvector(size_type n, value_type const& x,
                                       allocator_type const& a = {})
                : storage_type{n, a}
                , bod{storage_type::bos}
                , eod{storage_type::bos}
            {
                eod = core::uninitialized_fill_n(storage_type::bod, n, x);
            }

            template <class InIter>
            explicit constexpr dvector(InIter first, InIter last, allocator_type const& a = {})
                : storage_type{a}
                , bod{storage_type::bos}
                , eod{storage_type::bos}
            {
                append(first, last);
            }

            constexpr dvector(self_type const& that)
                : storage_type{that.size(), that.alloc_ref()}
                , bod{storage_type::bos}
                , eod{storage_type::bos}
            {
                eod = core::uninitialized_copy(that.bod, that.eod, eod);
            }
            constexpr dvector(self_type const& that, allocator_type const& a)
                : storage_type{that.size(), a}
                , bod{storage_type::bos}
                , eod{storage_type::bos}
            {
                eod = core::uninitialized_copy(that.bod, that.eod, eod);
            }

            constexpr dvector(self_type&& that) noexcept
                : storage_type{static_cast<storage_type&&>(that)}
                , bod(that.bod)
                , eod{that.eod}
            {
                that.bos = that.eos = that.bod = that.eod = nullptr;
            }
            ~dvector()
            {
                core::destroy(bod, eod);
            }

            constexpr auto operator=(self_type const& that) -> dvector&
            {
                if (this != &that)
                {
                    assign(that.bod, that.eod);
                }
                return *this;
            }
            constexpr auto operator=(self_type&& that) noexcept -> dvector&
            {
                if (this != &that)
                {
                    (*this).swap(that);
                }
                return *this;
            }

            auto constexpr assign(size_type n, value_type const& x) -> void {}
            template <class InIter>
            auto constexpr assign(InIter first, InIter last) -> void
            {
                assign(first, last, core::iterator_category<InIter>{});
            }

            auto constexpr swap(self_type& that) noexcept
            {
                storage_type::swap(static_cast<storage_type&>(that));
                std::swap(bod, that.bod);
                std::swap(eod, that.eod);
            }

            auto constexpr reserve_front(size_type f) -> void
            {
                if (remaining_front() < f)
                {
                    size_type b = std::max(remaining_back(), f);
                    size_type c = f + size() + b;
                    self_type temp{with_capacity{c}, storage_type::alloc_ref()};
                    temp.eod = core::uninitialized_safe_move(bod, eod, temp.bos + f);
                    temp.bod = temp.bos + f;
                    (*this).swap(temp);
                }
            }
            auto constexpr reserve_back(size_type b) -> void
            {
                if (remaining_back() < b)
                {
                    size_type f = std::max(remaining_front(), b);
                    size_type c = f + size() + b;
                    self_type temp{with_capacity{c}, storage_type::alloc_ref()};
                    temp.eod = core::uninitialized_safe_move(bod, eod, temp.bos + f);
                    temp.bod = temp.bos + f;
                    (*this).swap(temp);
                }
            }

            auto constexpr resize(size_type n)
            {
                if (n < size())
                {
                    erase(begin() + n, end());
                }
                else
                {
                    append(n - size(), value_type{});
                }
            }
            auto constexpr resize(size_type n, value_type const& x)
            {
                if (n < size())
                {
                    erase(begin() + n, end());
                }
                else
                {
                    append(n - size(), x);
                }
            }

            template <class... Args>
            auto constexpr push_front(Args&&... args) -> void
            {
                if (full_front())
                {
                    reserve_front(2 * size() + 1);
                }
                core::construct(bod - 1, static_cast<Args&&>(args)...);
                --bod;
            }

            template <class... Args>
            auto constexpr push_back(Args&&... args) -> void
            {
                if (full_back())
                {
                    reserve_back(2 * size() + 1);
                }
                core::construct(eod, static_cast<Args&&>(args)...);
                ++eod;
            }

            auto constexpr pop_front() noexcept -> void
            {
                assert(!empty() && "empty vector");
                bod = ++core::destroy(bod);
            }
            auto constexpr pop_back() noexcept -> void
            {
                assert(!empty() && "empty vector");
                eod = core::destroy(eod - 1);
            }
            template <class InIter, disable_if<core::is_integral<InIter>, int> = 0>
            auto constexpr append(InIter first, InIter last) -> void
            {
                append(first, last, core::iterator_category<InIter>{});
            }

            auto constexpr append(size_type n, value_type const& x) -> void
            {
                if (remaining_back() < n)
                {
                    reserve_back(std::max(n, size()));
                }
                eod = core::uninitialized_fill_n(eod, n, x);
            }
            auto constexpr prepend(size_type n, value_type const& x) -> void
            {
                if (remaining_front() < n)
                {
                    reserve_front(std::max(n, size()));
                }
                core::uninitialized_fill_n(bod - n, n, x);
                bod -= n;
            }

            template <class InIter>
            auto constexpr prepend(InIter first, InIter last) -> void
            {
                prepend(first, last, core::iterator_category<InIter>{});
            }

            template <class... Args>
            auto constexpr insert(const_iterator p, Args&&... args) -> iterator;
            template <class InIter>
            auto constexpr insert(const_iterator p, InIter first, InIter last) -> iterator;
            auto constexpr insert(const_iterator p, size_type n, value_type const& x) -> iterator;

            auto constexpr erase(const_iterator p) -> iterator
            {
                return erase(p, core::next(p));
            }
            auto constexpr erase(const_iterator first, const_iterator last) -> iterator
            {
                pointer f = first.get_node();
                pointer l = last.get_node();

                if ((storage_type::eos - l) > (f - storage_type::bos))
                {
                    auto p = core::shift_left(l, eod, l - f);
                    eod = core::destroy(p, eod);
                }
                else
                {
                    auto p = core::shift_right(bod, f, l - f);
                    core::destroy(bod, p);
                    bod = ++p;
                }

                return iterator{*this, first.get_node()};
            }
            auto constexpr clear() -> void
            {
                core::destroy(bod, eod);
                bod = eod = (storage_type::eos - storage_type::bos) / 2;
            }

            using storage_type::get_allocator;
            using storage_type::capacity;

            using base_type::operator==;
            using base_type::operator!=;
            using base_type::operator<;
            using base_type::operator<=;
            using base_type::operator>;
            using base_type::operator>=;

            auto constexpr size() const noexcept -> size_type
            {
                return eod - bod;
            }

            auto constexpr remaining_front() const noexcept -> size_type
            {
                return bod - storage_type::bos;
            }
            auto constexpr remaining_back() const noexcept -> size_type
            {
                return storage_type::eos - eod;
            }
            auto constexpr empty() const noexcept -> bool
            {
                return eod == bod;
            }
            auto constexpr full_front() const noexcept -> bool
            {
                return bod == storage_type::bos;
            }
            auto constexpr full_back() const noexcept -> bool
            {
                return storage_type::eos == eod;
            }
            auto constexpr begin() const noexcept -> const_iterator
            {
                return const_iterator{*this, bod};
            }
            auto constexpr end() const noexcept -> const_iterator
            {
                return const_iterator{*this, eod};
            }
            auto constexpr begin() noexcept -> iterator
            {
                return iterator{*this, bod};
            }
            auto constexpr end() noexcept -> iterator
            {
                return iterator{*this, eod};
            }
            auto constexpr operator[](size_type i) noexcept -> reference
            {
                assert(i < size() && "out_of_range");
                return bod[i];
            }
            auto constexpr operator[](size_type i) const noexcept -> const_reference
            {
                assert(i < size() && "out_of_range");
                return bod[i];
            }

        private:
            template <class InIter>
            auto constexpr append(InIter first, InIter last, core::input_iterator_tag) -> void
            {
                for (; first != last; ++first)
                {
                    push_back(*first);
                }
            }
            template <class FwdIter>
            auto constexpr append(FwdIter first, FwdIter last, core::forward_iterator_tag) -> void
            {}

            template <class InIter>
            auto constexpr prepend(InIter first, InIter last, core::input_iterator_tag) -> void
            {
                //NOTE: this is wrong, correct it using insert
                for (; first != last; ++first)
                {
                    push_front(*first);
                }
            }
            template <class InIter>
            auto constexpr assign(InIter first, InIter last, core::input_iterator_tag) -> void
            {
                auto [c, p] = core::copy(first, last, storage_type::bos, eod);
                if (c == last)
                {
                    core::destroy(p, eod);
                    eod = p;
                }
                else
                {
                    append(first, last);
                }
            }
            template <class FwdIter>
            auto constexpr assign(FwdIter first, FwdIter last, core::forward_iterator_tag) -> void
            {
                auto n = core::distance(first, last);
                if (capacity() < n)
                {
                    self_type temp{first, last, storage_type::alloc_ref()};
                    (*this).swap(temp);
                }
                else
                {
                    if (n > size())
                    {
                        auto p = first + size();
                        core::copy(first, p, storage_type::bod);
                        eod = core::uninitialized_copy(p, last, eod);
                    }
                    else
                    {
                        if (remaining_front() > remaining_back())
                        {
                            auto p = core::copy(first, last, storage_type::bod);
                            eod = core::destroy(p, eod);
                        }
                        else
                        {
                            auto p = core::copy_backward(first, last, storage_type::eod);
                            core::destroy(bod, p);
                            bod = p;
                        }
                    }
                }
            }

        private:
            pointer bod;
            pointer eod;
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
