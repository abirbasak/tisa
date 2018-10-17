#ifndef AZUIK_CONTAINER_HEAP_HPP
#define AZUIK_CONTAINER_HEAP_HPP
#include <cstddef>
namespace azuik
{
    namespace core
    {
        template <class T>
        class leftist_heap {
        public:
            using self_type = leftist_heap;
            using value_type = T;
            using reference = T&;

        public:
            leftist_heap()
                : m_root{nullptr}
            {}
            leftist_heap(self_type const& that);
            leftist_heap(self_type&& that) noexcept;
            ~leftist_heap();
            leftist_heap& operator=(self_type const& that);
            leftist_heap& operator=(self_type&& that) noexcept;
            auto constexpr empty() const noexcept
            {
                return m_root == nullptr;
            }
            auto constexpr top() const noexcept;
            template <class... Args>
            auto constexpr push(Args&&... args)
            {
                m_root = merge(new node{static_cast<Args&&>(args)..., nullptr, nullptr}, m_root);
            }
            auto constexpr pop()
            {
                assert(!empty() && "empty heap");
                node_ptr oldRoot = m_root;
                m_root = merge(m_root->left, m_root->right);
                delete oldRoot;
            }
            auto constexpr clear();
            void merge(self_type& that)
            {
                if (this == &that)
                {
                    return;
                }
                m_root = merge(m_root, that.m_root);
                that.m_root = nullptr;
            }

        private:
            struct node;
            using node_ptr = node*;
            struct node {
                value_type value;
                node_ptr* left;
                node_ptr* right;
                int npl;
                template <class... Args>
                node(Args&&... args, node_ptr left, node_ptr right, int npl = 0)
                    : value{static_cast<Args&&>(args)...}
                    , left{left}
                    , right{right}
                    , npl{npl}
                {}
            };
            node_ptr m_root;
        };
        template <class T>
        class binomial_heap {
        public:
            using value_type = T;
            using size_type = std::size_t;
            using differene_type = std::ptrdiff_t;

        private:
            struct node;
            using node_ptr = node*;
            struct node {
                value_type value;
                node_ptr next;
                node_ptr child;
            };

        public:
            auto constexpr empty() const noexcept;

            template <class... Args>
            void push(Args&&... args);
            void pop();
            void clear();
        };

    } // namespace core
} // namespace azuik
#endif
