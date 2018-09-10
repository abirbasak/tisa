#ifndef AZUIK_CONTAINER_TREE_HPP
#define AZUIK_CONTAINER_TREE_HPP
#include <azuik/core/allocator.hpp>
#include <azuik/algorithm/functional.hpp>
#include <algorithm>
#include <utility>
#include <iterator>
namespace azuik
{
    namespace core
    {
        namespace detail_
        {
            template <class S>
            struct tree_policy {
                using node_ptr = cond<is_const<S>, typename S::node_cptr, typename S::node_ptr>;
                using iterator_category = std::bidirectional_iterator_tag;
                constexpr tree_policy(S& s, node_ptr p) noexcept
                    : m_ptr{p}
                {}
                auto constexpr increment() noexcept {}
                auto constexpr decrement() noexcept {}
                auto constexpr deref() const noexcept
                {
                    return (*m_ptr).value;
                }
                template <class That>
                auto constexpr equal(That const& that) const noexcept -> bool
                {
                    return m_ptr = that.m_ptr;
                }
                node_ptr m_ptr;
            };
        } // namespace detail_
        template <class T, class A = allocator>
        class bs_tree {
        public:
            using self_type = bs_tree;
            using allocator_type = A;
            using value_type = T;

        private:
            struct node;
            using node_ptr = core::allocator_pointer<node, allocator_type>;
            using node_cptr = core::allocator_const_pointer<node, allocator_type>;
            struct sentinel {
                node_ptr up;
                node_ptr left;
                node_ptr right;
            };
            struct node : sentinel {
                template <class... Args>
                constexpr node(std::in_place_t, Args&&... args)
                    : value{static_cast<Args&&>(args)...}
                {}
                value_type value;
            };

        public:
            explicit bs_tree(allocator_type const& a = {})
                : m_sentinel{}
            {
                m_sentinel.left = static_cast<node_ptr>(&m_sentinel);
                m_sentinel.right = static_cast<node_ptr>(&m_sentinel);
                m_sentinel.up = nullptr;
            }
            ~bs_tree()
            {
                clear(root());
            }

        private:
            auto constexpr root() noexcept -> node_ptr
            {
                return m_sentinel.up;
            }
            auto constexpr root() const noexcept -> node_cptr
            {
                return m_sentinel.up;
            }
            auto constexpr leftmost() noexcept -> node_ptr
            {
                return m_sentinel.left;
            }
            auto constexpr leftmost() const noexcept -> node_cptr
            {
                return m_sentinel.left;
            }
            auto constexpr rightmost() noexcept -> node_ptr
            {
                return m_sentinel.left;
            }
            auto constexpr rightmost() const noexcept -> node_cptr
            {
                return m_sentinel.left;
            }
            auto constexpr shoot() noexcept -> node_ptr
            {
                return static_cast<node_ptr>(&m_sentinel);
            }
            auto constexpr shoot() const noexcept -> node_cptr
            {
                return static_cast<node_cptr>(&m_sentinel);
            }
            static auto constexpr minimum(node_ptr p) noexcept -> node_ptr
            {
                for (; p->left != nullptr; p = p->left) {}
                return p;
            }
            static auto constexpr maximum(node_ptr p) noexcept -> node_ptr
            {
                for (; p->right != nullptr; p = p->right) {}
                return p;
            }
            auto constexpr clear(node_ptr p) noexcept {}

        private:
            sentinel m_sentinel;
        };
        template <class T, class A = allocator>
        class splay_tree {};
        template <class T, class A = allocator>
        class red_black_tree {};
        template <class T, class A = allocator>
        class avl_tree {};
        template <class T, class A = allocator>
        class treap {
        public:
            using value_type = T;
            using reference = T&;
            using const_reference = T const&;
            using pointer = T*;
            using const_pointer = T const*;
            class node {
            private:
                using node_ptr = node*;
                using value_type = T;
                using size_type = std::size_t;

            private:
                value_type m_value;
                node_ptr m_left;
                node_ptr m_right;
                size_type m_cost;
            };

        private:
        };

    } // namespace core
} // namespace azuik
#endif
