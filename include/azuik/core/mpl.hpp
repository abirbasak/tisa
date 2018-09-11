#ifndef AZUIK_CORE_MPL_HPP
#define AZUIK_CORE_MPL_HPP
#include <cinttypes>
#include <type_traits>
namespace azuik
{
    namespace core
    {
        // a few named types
        using byte = unsigned char;
        using sbyte = signed char;

        using sshort = signed short; // same as short
        using ushort = unsigned short;

        using sint = signed int; // same as int
        using uint = unsigned int;

        using slong = signed long; // same as long
        using ulong = unsigned long;

        using llong = long long;
        using sllong = signed long long; // same as long long
        using ullong = unsigned long long;

        using ldouble = long double;

        using size_t = ::std::size_t;
        using ptrdiff_t = ::std::ptrdiff_t;
    } // namespace core

    // basic computational units
    namespace core
    {
        /// identity element
        template <class T>
        struct identity {
            using type = T;
        };

        /// returns nested type if T
        template <class T>
        struct nested_type {
            using type = typename T::type;
        };

        /// invoke the metafunction T.
        template <class T>
        using alias = typename T::type;

        /// a placeholder for default type.
        struct defaulted;
    } // namespace core

    namespace core
    {
        /// a void meta type.
        struct void_c {
            using type = void_c;
        };
        using nonesuch = void_c;

        template <class I, I x>
        struct integral_constant {
            using type = integral_constant;
            using value_type = I;
            static value_type const value = x;
        };

        template <bool x>
        using bool_c = integral_constant<bool, x>;
        using true_c = bool_c<true>;
        using false_c = bool_c<false>;

        template <byte x>
        using byte_c = integral_constant<byte, x>;
        template <sbyte x>
        using sbyte_c = integral_constant<sbyte, x>;

        template <short x>
        using short_c = integral_constant<short, x>;
        template <ushort x>
        using ushort_c = integral_constant<ushort, x>;

        template <int x>
        using int_c = integral_constant<int, x>;
        template <uint x>
        using uint_c = integral_constant<uint, x>;

        template <long x>
        using long_c = integral_constant<long, x>;
        template <ulong x>
        using ulong_c = integral_constant<ulong, x>;

        template <llong x>
        using llong_c = integral_constant<llong, x>;
        template <ullong x>
        using ullong_c = integral_constant<ullong, x>;

        template <size_t x>
        using size_c = integral_constant<size_t, x>;
        template <ptrdiff_t x>
        using ptrdiff_c = integral_constant<ptrdiff_t, x>;

        template <unsigned I, unsigned F>
        struct fixed;
    } // namespace core

    namespace core
    {
        template <class... Fs>
        struct overloaded;

        template <class F, class... Fs>
        struct overloaded<F, Fs...> : F, overloaded<Fs...> {
            explicit overloaded(F&& f, Fs&&... fs)
                : F{f}
                , Fs{fs}...
            {}
            using F::operator();
            using overloaded<Fs...>::operator();
        };
        template <class F>
        struct overloaded<F> : F {
            explicit overloaded(F&& f)
                : F{f}
            {}
        };

        struct overload_fn {
            template <class... Fs>
            overloaded<Fs...> operator()(Fs&&... fs) const
            {
                return overloaded<Fs...>(static_cast<Fs&&>(fs)...);
            }
        } const overload{};
    } // namespace core

    namespace core
    {
        template <class... T>
        struct type_list {};
        template <class T, T... x>
        struct value_list {};
        template <size_t... x>
        using index_list = value_list<size_t, x...>;

        template <class L, template <class...> class TPL>
        struct rebind_fn;
        template <template <class...> class L, template <class...> class TPL, class... Args>
        struct rebind_fn<L<Args...>, TPL> : identity<TPL<Args...>> {};
        template <class L, template <class...> class TPL>
        using rebind = alias<rebind_fn<L, TPL>>;

        template <class L>
        struct count_of;
        template <template <class...> class TMP, class... Args>
        struct count_of<TMP<Args...>> : size_c<sizeof...(Args)> {};

        template <class L, template <class...> class F>
        struct transform_fn;

        template <template <class...> class L, template <class...> class F, class... Args>
        struct transform_fn<L<Args...>, F> : identity<L<F<Args>...>> {};

        template <class L, template <class...> class F>
        using transform = alias<transform_fn<L, F>>;
    } // namespace core
    namespace core
    {
        template <bool C, class E = defaulted>
        struct Enable_if {};
        template <class E>
        struct Enable_if<true, E> : identity<E> {};

        template <bool C, class E = defaulted>
        using enable_if = alias<Enable_if<C, E>>;

        template <bool C, class E = defaulted>
        struct Disable_if {};
        template <class E>
        struct Disable_if<false, E> : identity<E> {};

        template <bool C, class E = defaulted>
        using disable_if = alias<Disable_if<C, E>>;

        template <class...>
        using when = defaulted;
    } // namespace core

    namespace core
    {
        namespace impl
        {
            template <class Default, class Enable, template <class...> class Expr, class... Args>
            struct detector : false_c {
                using detected_type = Default;
            };
            template <class Default, template <class...> class Expr, class... Args>
            struct detector<Default, when<Expr<Args...>>, Expr, Args...> : true_c {
                using detected_type = Expr<Args...>;
            };
        } // namespace impl
        template <bool... B>
        constexpr bool require = ::std::conjunction<bool_c<B>...>{};

        template <bool... B>
        constexpr bool either = ::std::disjunction<bool_c<B>...>{};

        template <bool... B>
        constexpr bool disallow = !require<B...>;

        template <template <class...> class Expr, class... Args>
        constexpr bool is_detected = impl::detector<void_c, when<>, Expr, Args...>{};

        template <template <class...> class Expr, class... Args>
        constexpr bool exists = is_detected<Expr, Args...>;

        template <template <class...> class Expr, class... Args>
        using detected_type =
            typename impl::detector<void_c, defaulted, Expr, Args...>::detected_type;

        template <class Expected, template <class...> class Expr, class... Args>
        constexpr bool is_detected_exact = ::std::is_same<Expected, detected_type<Expr, Args...>>{};

        template <class Expected, template <class...> class Expr, class... Args>
        constexpr bool identical_to = is_detected_exact<Expected, Expr, Args...>;

        template <class To, template <class...> class Expr, class... Args>
        constexpr bool is_detected_convertible =
            ::std::is_convertible<detected_type<Expr, Args...>, To>{};

        template <class To, template <class...> class Expr, class... Args>
        constexpr bool converts_to = is_detected_convertible<To, Expr, Args...>;

    } // namespace core

    namespace core
    {
        template <bool C, class T, class F>
        struct Cond : identity<T> {};
        template <class T, class F>
        struct Cond<false, T, F> : identity<F> {};
        template <class C, class T, class F>
        struct Cond_c : Cond<C::value, T, F> {};

        template <bool C, class T, class F>
        using cond = alias<Cond<C, T, F>>;
        template <class C, class T, class F>
        using cond_c = alias<Cond_c<C, T, F>>;

    } // namespace core

    namespace core
    {
        template <class T>
        using is_array = alias<::std::is_array<T>>;
        template <class T>
        using is_enum = alias<::std::is_enum<T>>;
        template <class T>
        using is_class = alias<::std::is_class<T>>;
        template <class T>
        using is_union = alias<::std::is_union<T>>;
        template <class T>
        using is_function = alias<::std::is_function<T>>;
        template <class T>
        using is_ptr = alias<::std::is_pointer<T>>;

        template <class T>
        using is_member_object_ptr = alias<::std::is_member_object_pointer<T>>;
        template <class T>
        using is_member_function_ptr = alias<::std::is_member_function_pointer<T>>;
        template <class T>
        using is_member_ptr = alias<::std::is_member_pointer<T>>;

        template <class T>
        using is_lvref = alias<::std::is_lvalue_reference<T>>;
        template <class T>
        using is_rvref = alias<::std::is_rvalue_reference<T>>;
        template <class T>
        using is_ref = alias<::std::is_reference<T>>;

    } // namespace core

    namespace core
    {
        /// add const qualifier to the type.
        template <class T>
        struct add_const_fn : ::std::add_const<T> {};
        template <class T>
        using add_const = alias<add_const_fn<T>>;

        /// remove const qualifier from the type.
        template <class T>
        struct remove_const_fn : ::std::remove_const<T> {};
        template <class T>
        using remove_const = alias<remove_const_fn<T>>;

        /// add volatile qualifier to the type.
        template <class T>
        struct add_volatile_fn : ::std::add_volatile<T> {};
        template <class T>
        using add_volatile = alias<add_volatile_fn<T>>;

        /// remove volatile qualifier from the type.
        template <class T>
        struct remove_volatile_fn : ::std::remove_volatile<T> {};
        template <class T>
        using remove_volatile = alias<remove_volatile_fn<T>>;

        /// add cv qualifier to the type.
        template <class T>
        struct add_cv_fn : ::std::add_cv<T> {};
        template <class T>
        using add_cv = alias<add_cv_fn<T>>;

        /// remove cv qualifier from the type.
        template <class T>
        struct remove_cv_fn : ::std::remove_cv<T> {};
        template <class T>
        using remove_cv = alias<remove_cv_fn<T>>;

        template <class T>
        inline constexpr bool is_const = ::std::is_const<T>{};
        template <class T>
        inline constexpr bool is_volatile = ::std::is_volatile<T>{};

        template <class T>
        using flip_const = cond<is_const<T>, remove_const<T>, add_const<T>>;
        template <class T>
        using flip_volatile = cond<is_const<T>, remove_volatile<T>, add_volatile<T>>;

        /// add lvalue reference to the type.
        template <class T>
        struct add_lv_ref_fn : bool_c<::std::add_lvalue_reference<T>::value> {};
        template <class T>
        using add_lv_ref = alias<add_lv_ref_fn<T>>;

        /// add rvalue reference to the type.
        template <class T>
        struct add_rv_ref_fn : bool_c<::std::add_rvalue_reference<T>::value> {};
        template <class T>
        using add_rv_ref = alias<add_rv_ref_fn<T>>;

        /// remove any reference from the type.
        template <class T>
        struct remove_ref_fn : ::std::remove_reference<T> {};
        template <class T>
        using remove_ref = alias<remove_ref_fn<T>>;

        template <class T>
        using remove_cv_ref = remove_cv<remove_ref<T>>;

        template <class T, class U>
        constexpr bool is_convertible = ::std::is_convertible<T, U>{};

        /// returns true if type T is constructible for Args
        template <class T, class... Args>
        constexpr bool is_constructible = ::std::is_constructible<T, Args...>{};

        /// retuns true if type T is copy constructible
        template <class T>
        constexpr bool is_copy_constructible = ::std::is_copy_constructible<T>{};

        /// returns true if type T is move constructible.
        template <class T>
        constexpr bool is_move_constructible = ::std::is_move_constructible<T>{};
        /// returns true if type T is assignable from U
        template <class T, class U>
        constexpr bool is_assignable = ::std::is_assignable<T, U>{};
        /// returns true if type T is copy assignable.
        template <class T>
        constexpr bool is_copy_assignable = ::std::is_copy_assignable<T>{};
        /// returns true if type T is move assignable.
        template <class T>
        constexpr bool is_move_assignable = ::std::is_move_assignable<T>{};
        /// returns true if type T is nothrow constructible from Args.
        template <class T, class... Args>
        constexpr bool is_nothrow_constructible = ::std::is_nothrow_constructible<T, Args...>{};
        /// returns true if type T is nothrow copy constructible.
        template <class T>
        constexpr bool is_nothrow_copy_constructible = ::std::is_nothrow_copy_constructible<T>{};
        /// returns true if type T is nothrow move constructible.
        template <class T>
        constexpr bool is_nothrow_move_constructible = ::std::is_nothrow_move_constructible<T>{};
        /// returns true if type T is nothrow assignable from U.
        template <class T, class U>
        constexpr bool is_nothrow_assignable = ::std::is_nothrow_assignable<T, U>{};
        /// returns true if type T us nothrow copy assignable.
        template <class T>
        constexpr bool is_nothrow_copy_assignable = ::std::is_nothrow_copy_assignable<T>{};
        /// returns true if type T is nothrow move assignable.
        template <class T>
        constexpr bool is_nothrow_move_assignable = ::std::is_nothrow_move_assignable<T>{};
        /// returns true if type T is destructible.
        template <class T>
        constexpr bool is_nothrow_destructible = ::std::is_nothrow_destructible<T>{};

        /// returns true if type T is trivially constructible from Args.
        template <class T, class... Args>
        constexpr bool is_trivially_constructible = ::std::is_trivially_constructible<T>{};
        /// returns true if type T is trivially copy constructible.
        template <class T>
        constexpr bool is_trivially_copy_constructible =
            ::std::is_trivially_copy_constructible<T>{};
        /// returns true if type T is trivially move constructible.
        template <class T>
        constexpr bool is_trivially_move_constructible =
            ::std::is_trivially_move_constructible<T>{};
        /// returns true if type T is trivially assignable.
        template <class T, class U>
        constexpr bool is_trivially_assignable = ::std::is_trivially_assignable<T, U>{};
        /// returns true if type T is trivially copy assignable.
        template <class T>
        constexpr bool is_trivially_copy_assgnable = ::std::is_trivially_copy_assignable<T>{};
        /// returns true if type T is trivially move assignable.
        template <class T>
        constexpr bool is_trivially_move_assignable = ::std::is_trivially_move_assignable<T>{};
        /// returns true if type T is trivially destructible.
        template <class T>
        constexpr bool is_trivially_destructible = ::std::is_trivially_destructible<T>{};

        /// returns true is type T & U are same
        template <class T, class U>
        constexpr bool is_same = ::std::is_same<T, U>{};

        /// returns true if type T & U are same without qualifier
        template <class T, class U>
        constexpr bool is_same_unqualified = is_same<T const volatile, U const volatile>;

        /// returns true if the type T is void
        template <class T>
        constexpr bool is_void = is_same_unqualified<T, void>;

        /// returns true if the type T is std::nullptr_t
        template <class T>
        constexpr bool is_nullptr = is_same_unqualified<T, ::std::nullptr_t>;

        /// returns true if the type T is bool
        template <class T>
        constexpr bool is_bool = is_same_unqualified<T, bool>;

        template <class T>
        constexpr bool is_char = is_same_unqualified<T, char>;

        template <class T>
        constexpr bool is_integral = std::is_integral<T>{};
        template <class T>
        constexpr bool is_floating_point = ::std::is_floating_point<T>{};

        namespace detail_
        {
            template <class F, class... Args>
            struct is_invocable_impl {
                template <class U>
                static auto test(U* p) -> decltype((*p)(std::declval<Args>()...), void(), true_c{});
                template <class U>
                static auto test(...) -> decltype(false_c{});
                static constexpr bool value = decltype(test<F>(0))::value;
            };
        } // namespace detail_

        template <class F, class... Args>
        constexpr bool is_invocable = detail_::is_invocable_impl<F, Args...>::value;

        template <class T, class = enable_if<is_integral<T>>>
        using Integral = T;

        template <class T, class = enable_if<is_floating_point<T>>>
        using Floating_point = T;

        template <class T>
        using size_of = size_c<sizeof(T)>;

        template <class T>
        struct alignment_of : bool_c<::std::alignment_of<T>::value> {};

        template <size_t N, size_t A>
        using aligned_storage = alias<::std::aligned_storage<N, A>>;

        template <class T, unsigned N = 1>
        using auto_storage = aligned_storage<N * size_of<T>::value, std::alignment_of<T>::value>;

        template <class T>
        struct count_of : size_c<1> {};
        template <class... T>
        struct count_of<type_list<T...>> : size_c<sizeof...(T)> {};
        template <class T, T... x>
        struct count_of<value_list<T, x...>> : size_c<sizeof...(x)> {};

        //template <template <class> class Pr, class... T>
        //struct any_of;
        //template <template <class> class Pr, class T>
        //struct any_of<Pr, T> : Pr<T> {};
        //template <template <class> class Pr, class F, class... R>
        //struct any_of<Pr, F, R...> : bool_c<(Pr<F>::value || any_of<Pr, R...>::value)> {};

        using int8_t = ::std::int8_t;
        using uint8_t = ::std::uint8_t;
        using int16_t = ::std::int16_t;
        using uint16_t = ::std::uint16_t;
        using int32_t = ::std::int32_t;
        using uint32_t = ::std::uint32_t;
        using int64_t = ::std::int64_t;
        using uint64_t = ::std::uint64_t;

        template <int8_t x>
        using int8_c = integral_constant<int8_t, x>;
        template <uint8_t x>
        using uint8_c = integral_constant<uint8_t, x>;
        template <int16_t x>
        using int16_c = integral_constant<int16_t, x>;
        template <uint16_t x>
        using uint16_c = integral_constant<uint16_t, x>;
        template <int32_t x>
        using int32_c = integral_constant<int32_t, x>;
        template <uint32_t x>
        using uint32_c = integral_constant<uint32_t, x>;
        template <int64_t x>
        using int64_c = integral_constant<int64_t, x>;
        template <uint64_t x>
        using uint64_c = integral_constant<uint64_t, x>;

    } // namespace core

    namespace core
    {
        template <class Pr, class T>
        constexpr bool is_unary_predicate = is_invocable<Pr, T>;

        template <class T, class U>
        using assignment_expression = decltype(std::declval<T&>() = std::declval<U&>());
        template <class T, class I>
        using index_expression = decltype(std::declval<T&>()[std::declval<I>()]);
        template <class T>
        using prefix_increment_expression = decltype(++std::declval<T&>());
        template <class T>
        using prefix_decrement_expression = decltype(--std::declval<T&>());
        template <class T>
        using postfix_increment_expression = decltype(std::declval<T&>()++);

        template <class T>
        constexpr bool is_prefix_incrementable = exists<prefix_increment_expression, T>;

        template <class T>
        using prefix_increment = decltype(++std::declval<T>());
        template <class T>
        using dereference = decltype(*std::declval<T>());

    } // namespace core

    // value_type traits
    namespace core
    {
        template <class S>
        struct iterable_traits {
            using value_type = typename S::value_type;
            using size_type = typename S::size_type;
            using difference_type = typename S::difference_type;
            using reference = typename S::reference;
            using pointer = typename S::pointer;
            using const_reference = typename S::const_reference;
            using const_pointer = typename S::const_pointer;
            using iterator = typename S::iterator;
            using const_iterator = typename S::const_iterator;
        };
        template <class S>
        struct iterable_traits<S&> : iterable_traits<S> {};
        template <class S>
        struct iterable_traits<S&&> : iterable_traits<S> {};

        template <class T>
        struct iterable_traits<T*> {
            using value_type = remove_const<T>;
            using size_type = size_t;
            using difference_type = ptrdiff_t;
            using reference = T&;
            using const_reference = add_const<T>&;
            using pointer = T*;
            using add_pointer = add_const<T>*;
            using iterator = T*;
            using const_iterator = add_const<T>*;
        };

        template <class S>
        struct ordered_iterable_traits : iterable_traits<S> {
            using key_type = typename S::key_type;
            using key_compare = typename S::key_compare;
        };

        template <class S>
        struct sequence_traits;

        template <class S>
        using allocator_type = typename sequence_traits<S>::allocator_type;

        template <class I>
        using value_type = typename iterable_traits<I>::value_type;

        template <class I>
        using size_type = typename iterable_traits<I>::size_type;

        template <class I>
        using difference_type = typename iterable_traits<I>::difference_type;

        template <class I>
        using pointer = typename iterable_traits<I>::pointer;

        template <class I>
        using reference = typename iterable_traits<I>::reference;

        template <class I>
        using const_pointer = typename iterable_traits<I>::const_pointer;

        template <class I>
        using const_reference = typename iterable_traits<I>::const_reference;

        template <class I>
        using iterator = typename iterable_traits<I>::iterator;

        template <class I>
        using const_iterator = typename iterable_traits<I>::const_iterator;

        template <class I>
        using key_type = typename ordered_iterable_traits<I>::key_type;

        template <class I>
        using key_compare = typename ordered_iterable_traits<I>::key_compare;

    } // namespace core
    namespace mpl
    {
        template <class I>
        constexpr bool is_power2 =
            core::bool_c<((I::value != 0) && !(I::value & (I::value - 1)))>{};
    }
    namespace core
    {
        template <class T>
        struct as_float_t;
        template <>
        struct as_float_t<float> : identity<float> {};
        template <>
        struct as_float_t<double> : identity<double> {};
        template <>
        struct as_float_t<ldouble> : identity<ldouble> {};

        template <>
        struct as_float_t<int> : identity<double> {};
        template <>
        struct as_float_t<long> : identity<double> {};
        template <>
        struct as_float_t<llong> : identity<ldouble> {};

        template <class T>
        using as_float = alias<as_float_t<T>>;
    } // namespace core
} // namespace azuik

#    define AZUIK_SLOT(name, call)                                                                 \
        struct name##_fn {                                                                         \
            template <class T, class... Args>                                                      \
            constexpr auto operator()(T&& x, Args&&... args) const                                 \
            {                                                                                      \
                return x.call(static_cast<Args&&>(args)...);                                       \
            }                                                                                      \
        } constexpr name {}

#    define AZUIK_HAS_NESTED_TYPE(NAME)                                                            \
        template <class T, class E = ::azuik::core::defaulted>                                     \
        struct has_type_##NAME##_c : ::azuik::core::false_c {};                                    \
        template <class T>                                                                         \
        struct has_type_##NAME##_c<T, ::azuik::core::when<typename T::NAME>>                       \
            : ::azuik::core::true_c {};                                                            \
        template <class T>                                                                         \
        constexpr bool has_type_##NAME = has_type_##NAME##_c<T>::value

#endif
