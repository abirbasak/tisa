#ifndef AZUIK_ALGORITHM_FUNCTIONAL_HPP
#define AZUIK_ALGORITHM_FUNCTIONAL_HPP
#include <azuik/core/mpl.hpp>
#include <azuik/container/iterable.hpp>
#include <numeric>
#include <utility>
#include <iterator>
#include <cctype>

namespace azuik
{
    namespace core
    {
        inline constexpr auto to_upper = [](char c) { return std::toupper(c);};
        inline constexpr auto to_lower = [](char c) { return std::tolower(c);};
        
        inline constexpr auto is_alpha = [](char c) { return std::isalpha(c);};
        inline constexpr auto is_alphanum = [](char c) { return std::isalnum(c);};
        inline constexpr auto is_space = [](char c) {return std::isspace(c);};
        inline constexpr auto is_blank = [](char c) { return std::isblank(c);};
        inline constexpr auto is_control = [](char c) { return std::iscntrl(c);};
        inline constexpr auto is_punct = [](char c) { return std::ispunct(c);};
        inline constexpr auto is_print = [](char c) { return std::isprint(c);};
        inline constexpr auto is_digit = [](char c) { return std::isdigit(c);};
        inline constexpr auto is_xdigit = [](char c) { return std::isxdigit(c);};
        inline constexpr auto is_upper = [](char c) { return std::isupper(c);};
        inline constexpr auto is_lower = [](char c) { return std::islower(c);};
        
        inline static constexpr struct plus_eq_fn {
            template <class T>
            constexpr T operator()(T& x, T const& y) const noexcept
            {
                return x += y;
            }
        } const plus_eq{};

        constexpr struct minus_eq_fn {
            template <class T>
            constexpr T operator()(T& x, T const& y) const noexcept
            {
                return x -= y;
            }
        } const minus_eq{};

        constexpr struct multiplies_eq_fn {
            template <class T>
            constexpr T operator()(T& x, T const& y) const noexcept
            {
                return x *= y;
            }
        } const multiplies_eq{};

        constexpr struct divides_eq_fn {
            template <class T>
            constexpr T operator()(T& x, T const& y) const noexcept
            {
                return x /= y;
            }
        } const divides_eq{};

        constexpr struct modulus_eq_fn {
            template <class T>
            constexpr T operator()(T& x, T const& y) const noexcept
            {
                return x %= y;
            }
        } const modulus_eq{};

        constexpr struct plus_fn {
            template <class T>
            constexpr T operator()(T const& x, T const& y) const noexcept
            {
                return x + y;
            }
        } const plus{};

        constexpr struct minus_fn {
            template <class T>
            constexpr T operator()(T const& x, T const& y) const noexcept
            {
                return x - y;
            }
        } const minus{};

        constexpr struct multiplies_fn {
            template <class T>
            constexpr T operator()(T const& x, T const& y) const noexcept
            {
                return x * y;
            }
        } const multiplies{};

        constexpr struct divides_fn {
            template <class T>
            constexpr T operator()(T const& x, T const& y) const noexcept
            {
                return x / y;
            }
        } const divides{};

        constexpr struct modulus_fn {
            template <class T>
            constexpr T operator()(T const& x, T const& y) const noexcept
            {
                return x % y;
            }
        } const modulus{};

        /// unary negation
        constexpr struct negate_fn {
            template <class T>
            constexpr bool operator()(T const& x) const noexcept
            {
                return -x;
            }
        } const negate = {};

        constexpr struct equal_to_fn {
            template <class T>
            constexpr bool operator()(T const& x, T const& y) const noexcept
            {
                return x == y;
            }
        } const equal_to = {};

        constexpr struct not_equal_to_fn {
            template <class T>
            constexpr bool operator()(T const& x, T const& y) const noexcept
            {
                return x != y;
            }
        } const not_equal_to = {};

        constexpr struct less_fn {
            template <class T>
            bool operator()(T const& x, T const& y) const noexcept
            {
                return x < y;
            }
        } const less = {};

        constexpr struct greater_fn {
            template <class T>
            bool operator()(T const& x, T const& y) const noexcept
            {
                return x > y;
            }
        } const greater{};

        constexpr struct less_eq_fn {
            template <class T>
            bool operator()(T const& x, T const& y) const noexcept
            {
                return x <= y;
            }
        } const less_eq{};

        constexpr struct greater_eq_fn {
            template <class T>
            bool operator()(T const& x, T const& y) const noexcept
            {
                return x >= y;
            }
        } const greater_eq{};

        constexpr struct logical_and_fn {
            template <class T>
            T operator()(T const& lhs, T const& rhs) const noexcept
            {
                return lhs && rhs;
            }
        } const logical_and{};

        constexpr struct logical_or_fn {
            template <class T>
            T operator()(T const& lhs, T const& rhs) const noexcept
            {
                return lhs || rhs;
            }
        } const logical_or{};

        constexpr struct logical_not_fn {
            template <class T>
            bool operator()(T const& x) const noexcept
            {
                return !x;
            }
        } const logical_not{};

        constexpr struct bitwise_and_fn {
            template <class T>
            T operator()(T const& lhs, T const& rhs) const noexcept
            {
                return lhs & rhs;
            }
        } const bitwise_and{};

        constexpr struct bitwise_or_fn {
            template <class T>
            T operator()(T const& lhs, T const& rhs) const noexcept
            {
                return lhs | rhs;
            }
        } const bitwise_or{};

        constexpr struct bitwise_xor_fn {
            template <class T>
            T operator()(T const& lhs, T const& rhs) const noexcept
            {
                return lhs ^ rhs;
            }
        } const bitwise_xor{};

        constexpr struct bitwise_not_fn {
            template <class T>
            T operator()(T const& x) const noexcept
            {
                return ~x;
            }
        } const bitwise_not{};

        struct hash_fn;

        template <class T>
        struct hash_combine;
    } // namespace core
} // namespace azuik
#endif
