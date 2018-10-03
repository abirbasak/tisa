#ifndef AZUIK_CORE_MEMORY_HPP
#define AZUIK_CORE_MEMORY_HPP

#include <azuik/core/mpl.hpp>
#include <memory>

namespace azuik
{
    namespace core
    {
        struct for_each_iter_fn {
            template <class InIter, class Sentinel, class Fn>
            void operator()(InIter f, Sentinel l, Fn fn) const noexcept
            {
                for (; f != l; ++f)
                {
                    fn(f);
                }
            }
        } const for_each_iter{};

        struct construct_fn {
            template <class T, class... Args>
            void operator()(T* p, Args&&... x) const
            {
                ::new (static_cast<void*>(p)) T(static_cast<Args&&>(x)...);
            }
        } const construct{};

        struct destroy_fn {
            template <class InIter>
            auto constexpr operator()(InIter first, InIter last) const noexcept -> void
            {
                return std::destroy(first, last);
            }
            template <class Ptr>
            auto constexpr operator()(Ptr p) const noexcept -> void
            {
                return std::destroy_at(p);
            }
        };
        inline constexpr destroy_fn destroy = {};

        struct destroy_n_fn {
            template <class FwdIter, class Size>
            auto constexpr operator()(FwdIter first, Size n) const noexcept -> FwdIter
            {
                return std::destroy_n(first, n);
            }
        };

        inline constexpr destroy_n_fn destroy_n = {};

        struct uninitialized_fill_fn {
            template <class InIter, class T>
            void operator()(InIter first, InIter last, T const& x) const
            {
                return std::uninitialized_fill(first, last, x);
            }
        };
        inline constexpr uninitialized_fill_fn uninitialized_fill = {};

        struct uninitialized_fill_n_fn {
            template <class InIter, class Size, class T>
            auto operator()(InIter first, Size n, T const& x) const
            {
                return std::uninitialized_fill_n(first, n, x);
            }
            template <class InIter, class Size>
            auto operator()(InIter first, Size n) const
            {
                using T = core::value_type<InIter>;
                return std::uninitialized_fill_n(first, n, T{});
            }
        };
        inline constexpr uninitialized_fill_n_fn uninitialized_fill_n = {};

        struct uninitialized_value_construct_fn {
            template <class InIter>
            auto constexpr operator()(InIter first, InIter last) const -> void
            {
                return std::uninitialized_value_construct(first, last);
            }
        };
        inline constexpr uninitialized_value_construct_fn uninitialized_value_construct = {};

        struct uninitialized_value_construct_n_fn {
            template <class FwdIter, class Size>
            auto constexpr operator()(FwdIter first, Size n) const -> FwdIter
            {
                return std::uninitialized_value_construct_n(first, n);
            }
        };
        inline constexpr uninitialized_value_construct_n_fn uninitialized_value_construct_n = {};

        struct uninitialized_default_construct_fn {
            template <class InIter>
            auto constexpr operator()(InIter first, InIter last) const -> void
            {
                return std::uninitialized_default_construct(first, last);
            }
        };
        inline constexpr uninitialized_default_construct_fn uninitialized_default_construct = {};

        struct uninitialized_default_construct_n_fn {
            template <class FwdIter, class Size>
            auto constexpr operator()(FwdIter first, Size n) const -> FwdIter
            {
                return std::uninitialized_default_construct_n(first, n);
            }
        };
        inline constexpr uninitialized_default_construct_n_fn uninitialized_default_construct_n =
            {};

        struct uninitialized_copy_fn {
            template <class InIter, class FwdIter>
            auto constexpr operator()(InIter first, InIter last, FwdIter result) const -> FwdIter
            {
                return std::uninitialized_copy(first, last, result);
            }
        };
        inline constexpr uninitialized_copy_fn uninitialized_copy = {};

        struct uninitialized_copy_n_fn {
            template <class InIter, class Size, class FwdIter>
            auto constexpr operator()(InIter first, Size n, FwdIter result) const -> FwdIter
            {
                return std::uninitialized_copy_n(first, n, result);
            }
        };
        inline constexpr uninitialized_copy_n_fn uninitialized_copy_n = {};

        struct uninitialized_safe_move_fn {
            template <class InIter, class FwdIter>
            auto constexpr operator()(InIter first, InIter last, FwdIter result) const -> FwdIter
            {
                if constexpr (core::is_nothrow_move_constructible<core::value_type<InIter>>)
                {
                    return std::uninitialized_move(first, last, result);
                }
                else
                {
                    return std::uninitialized_copy(first, last, result);
                }
            }
        };
        inline constexpr uninitialized_safe_move_fn uninitialized_safe_move = {};

        struct uninitialized_safe_move_n_fn {
            template <class FwdIter1, class Size, class FwdIter2>
            auto constexpr operator()(FwdIter1 first, Size n, FwdIter2 result) const -> FwdIter2
            {
                if constexpr (core::is_nothrow_move_constructible<core::value_type<FwdIter1>>)
                {
                    return std::uninitialized_move_n(first, n, result);
                }
                else
                {
                    return std::uninitialized_copy_n(first, n, result);
                }
            }
        };
        inline constexpr uninitialized_safe_move_n_fn uninitialized_safe_move_n = {};

        template <class T>
        class ref {
        public:
            using self_type = ref;

        private:
            using object_ptr = core::pointer<T>;

        public:
            ref(T&& x)
                : m_ptr(&x)
            {}
            template <class Slot, class... A>
            void operator()(Slot s, A&&... x)
            {
                (*m_ptr)(s, static_cast<A&&>(x)...);
            }
            template <class Slot, class... A>
            void operator()(Slot s, A&&... x) const
            {
                (*m_ptr)(s, static_cast<A&&>(x)...);
            }

        private:
            object_ptr m_ptr;
        };

    } // namespace core
} // namespace azuik
#endif
