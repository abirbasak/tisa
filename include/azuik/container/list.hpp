#ifndef AZUIK_CONTAINER_LIST_HPP
#define AZUIK_CONTAINER_LIST_HPP
#include <azuik/core/allocator.hpp>
#include <azuik/container/iterable.hpp>
namespace azuik
{
    namespace core
    {
        namespace detail_
        {
            template <class A>
            struct allocator_base : private A {
                using allocator_type = A;
                explicit allocator_base(allocator_type const& a)
                    : A{a}
                {}
                auto constexpr alloc_ref() noexcept -> allocator_type&
                {
                    return static_cast<allocator_type&>(*this);
                }
                auto constexpr alloc_ref() const noexcept -> allocator_type const&
                {
                    return static_cast<allocator_type const&>(*this);
                }
            };
        } // namespace detail_
        template <class T, class A>
        class forward_list;

        template <class T, class A>
        struct iterable_traits<forward_list<T, A>> : iterable_traits_from_allocator<T, A> {
            using iterator = forward_iterator<forward_list<T, A>>;
            using const_iterator = forward_iterator<forward_list<T, A> const>;
        };

        template <class T, class A>
        struct sequence_traits<forward_list<T, A>> {
            using allocator_type = A;
        };

        template <class T, class A = allocator>
        class forward_list : private detail_::allocator_base<A> {
        public:
            using self_type = forward_list;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;

        private:
            using base_type = detail_::allocator_base<A>;
            template <class>
            friend class detail_::forward_policy;
            struct node;
            using node_ptr = core::allocator_pointer<node, allocator_type>;
            using node_cptr = core::allocator_const_pointer<node, allocator_type>;
            struct empty_node {
                node_ptr next;
            };

            struct node : empty_node {
                template <class... Args>
                explicit constexpr node(std::in_place_t, Args&&... args)
                    : value{static_cast<Args&&>(args)...}
                {}
                value_type value;
            };

        public:
            constexpr explicit forward_list(allocator_type const& a = {}) noexcept
                : base_type{a}
                , m_head{}
            {
                m_head.next = static_cast<node_ptr>(&m_head);
            }
            constexpr explicit forward_list(std::initializer_list<value_type> init,
                                            allocator_type const& a = {})
                : forward_list{a}
            {
                insert_after(before_begin(), init.begin(), init.end());
            }
            template <class InIter>
            constexpr explicit forward_list(InIter first, InIter last, allocator_type const& a = {})
                : forward_list{a}
            {
                insert_after(before_begin(), first, last);
            }

            constexpr auto empty() const noexcept
            {
                return m_head.next == static_cast<node_cptr>(&m_head);
            }
            constexpr auto begin() noexcept
            {
                return iterator{*this, m_head.next};
            }
            constexpr auto end() noexcept
            {
                return iterator{*this, static_cast<node_ptr>(&m_head)};
            }
            constexpr auto begin() const noexcept
            {
                return const_iterator{*this, m_head.next};
            }
            constexpr auto end() const noexcept
            {
                return const_iterator{*this, &m_head};
            }
            template <class... Args>
            auto constexpr insert_after(const_iterator p, Args&&... args) -> iterator
            {
                return iterator{*this, insert_after(p.get_node(), static_cast<Args&&>(args)...)};
            }
            template <class InIter>
            auto constexpr insert_after(const_iterator p, InIter first, InIter last) -> iterator
            {
                for (; first != last; ++first)
                {
                    insert_after(p, *first);
                }
                return iterator{*this, p.get_node()};
            }
            auto constexpr erase_after(const_iterator p) -> iterator
            {
                return iterator{*this, erase_after(get_node(p))};
            }
            template <class... Args>
            auto constexpr push_front(Args&&... args)
            {
                insert_after(static_cast<node_ptr>(&m_head), static_cast<Args&&>(args)...);
            }
            auto constexpr pop_front() -> void
            {
                erase_after(static_cast<node_ptr>(&m_head));
            }

        private:
            template <class... Args>
            auto constexpr insert_after(node_ptr p, Args&&... args) -> node_ptr
            {
                node_ptr n = new_node(static_cast<Args>(args)...);
                n->next = p->next;
                p->next = n;
                return n;
            }
            auto constexpr erase_after(node_ptr p) -> node_ptr
            {
                node_ptr n = p->next;
                p->next = n->next;
                delete_node(n);
                return p;
            }
            template <class... Args>
            auto constexpr new_node(Args&&... args) -> node_ptr
            {
                return allocate_and_construct<node>(base_type::alloc_ref(), std::in_place,
                                                    static_cast<Args&&>(args)...);
            }
            auto constexpr delete_node(node_ptr n)
            {
                core::destroy(n);
                base_type::alloc_ref().template deallocate<node>(n);
            }
            auto constexpr before_begin() noexcept -> iterator
            {
                return iterator{*this, static_cast<node_ptr>(&m_head)};
            }
            auto constexpr before_begin() const noexcept -> const_iterator
            {
                return const_iterator{*this, static_cast<node_cptr>(&m_head)};
            }

        private:
            empty_node m_head;
        };

        template <class T, class A>
        class reversed_list {
        public:
            using self_type = reversed_list;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;
        };

        template <class T, class A>
        class reversed_list;

        template <class T, class A>
        struct iterable_traits<reversed_list<T, A>> : iterable_traits_from_allocator<T, A> {};
        template <class T, class A>
        struct sequence_traits<reversed_list<T, A>> {
            using allocator_type = A;
        };

        template <class T, class A>
        class list;

        template <class T, class A>
        struct iterable_traits<list<T, A>> : iterable_traits_from_allocator<T, A> {
            using iterator = bidirectional_iterator<list<T, A>>;
            using const_iterator = bidirectional_iterator<list<T, A> const>;
        };

        template <class T, class A>
        struct sequence_traits<list<T, A>> {
            using allocator_type = A;
        };

        template <class T, class A = allocator>
        class list : private detail_::allocator_base<A> {
        public:
            using self_type = list;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;

        private:
            template <class>
            friend class detail_::bidirectional_policy;

            using base_type = detail_::allocator_base<A>;
            struct empty_node;
            struct node;
            using node_ptr = core::allocator_pointer<node, allocator_type>;
            using node_cptr = core::allocator_const_pointer<node, allocator_type>;

        private:
            struct empty_node {
                node_ptr prev;
                node_ptr next;
            };
            struct node : empty_node {
                template <class... Args>
                constexpr node(std::in_place_t, Args&&... args)
                    : value{static_cast<Args&&>(args)...}
                {}
                value_type value;
            };

        public:
            constexpr explicit list(allocator_type const& a = {}) noexcept
                : base_type{a}
            {
                m_head.next = m_head.prev = static_cast<node_ptr>(&m_head);
            }
            constexpr auto empty() const noexcept
            {
                return m_head.next == &m_head;
            }
            constexpr auto begin() noexcept
            {
                return iterator{*this, m_head.next};
            }
            constexpr auto end() noexcept
            {
                return iterator{*this, static_cast<node_ptr>(&m_head)};
            }
            constexpr auto begin() const noexcept
            {
                return const_iterator{*this, m_head.next};
            }
            constexpr auto end() const noexcept
            {
                return const_iterator{*this, static_cast<node_cptr>(&m_head)};
            }
            template <class... Args>
            auto constexpr insert(const_iterator p, Args&&... args) -> iterator
            {
                return iterator{*this, insert(get_node(p), static_cast<Args&&>(args)...)};
            }

            auto constexpr erase(const_iterator p) -> iterator
            {
                return iterator{*this, erase(get_node(p))};
            }
            template <class... Args>
            void push_front(Args&&... args)
            {
                insert(m_head.next, static_cast<Args&&>(args)...);
            }
            template <class... Args>
            void push_back(Args&&... args)
            {
                insert(m_head.prev, static_cast<Args&&>(args)...);
            }
            void pop_front()
            {
                erase(m_head.next);
            }
            void pop_back()
            {
                erase(m_head.prev);
            }

        private:
            template <class... Args>
            auto constexpr insert(node_ptr p, Args&&... args) -> node_ptr
            {
                node_ptr n = new_node(static_cast<Args&&>(args)...);
                n->next = p;
                n->prev = p->prev;
                p->prev->next = n;
                p->prev = n;
                return p;
            }
            auto constexpr erase(node_ptr p) -> node_ptr
            {
                node_ptr n = p->next;
                p->prev->next = n;
                n->prev = p->prev;
                delete_node(p);
                return n;
            }
            template <class... Args>
            auto constexpr new_node(Args&&... args) -> node_ptr
            {
                return allocate_and_construct<node>(base_type::alloc_ref(), std::in_place,
                                                    static_cast<Args&&>(args)...);
            }
            auto constexpr delete_node(node_ptr n)
            {
                core::destroy(n);
                base_type::alloc_ref().template deallocate<node>(n);
            }

        private:
            empty_node m_head;
        };

    } // namespace core
} // namespace azuik
#endif
