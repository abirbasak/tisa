#ifndef AZUIK_ALGORITHM_MATH_HPP
#define AZUIK_ALGORITHM_MATH_HPP
#include <azuik/core/mpl.hpp>
#include <limits.h>
#include <cmath>
#include <float.h>
#include <limits>
namespace azuik
{
    namespace core
    {
        template <class T, unsigned Ignore = 0>
        struct zero {
            static T const value;
        };
        template <class T, unsigned Ignore>
        T const zero<T, Ignore>::value = T();
        template <class T, unsigned Ignore = 0>
        struct one {
            static T const value;
        };
        template <class T, unsigned Ignore>
        T const one<T, Ignore>::value = T(1);

        template <class T, unsigned Ignore = 0>
        struct inf;
        template <class T, unsigned Ignore = 0>
        struct nan;
        template <class T, unsigned Ignore>
        struct eps;
#define AZUIK_SPECIALIZE(NAME, TYPE, VALUE)                                                        \
    template <unsigned Ignore>                                                                     \
    struct NAME<TYPE, Ignore> {                                                                    \
        static TYPE const value;                                                                   \
    };                                                                                             \
    template <unsigned Ignore>                                                                     \
    TYPE const NAME<TYPE, Ignore>::value = VALUE
        AZUIK_SPECIALIZE(inf, float, std::numeric_limits<float>::infinity());
        AZUIK_SPECIALIZE(inf, double, std::numeric_limits<double>::infinity());
        AZUIK_SPECIALIZE(inf, ldouble, std::numeric_limits<ldouble>::infinity());
        AZUIK_SPECIALIZE(nan, float, std::numeric_limits<float>::quiet_NaN());
        AZUIK_SPECIALIZE(nan, double, std::numeric_limits<double>::quiet_NaN());
        AZUIK_SPECIALIZE(nan, ldouble, std::numeric_limits<ldouble>::quiet_NaN());
        AZUIK_SPECIALIZE(eps, float, FLT_EPSILON);
        AZUIK_SPECIALIZE(eps, double, DBL_EPSILON);
        AZUIK_SPECIALIZE(eps, ldouble, LDBL_EPSILON);
#undef AZUIK_SPECIALIZE

#define AZUIK_SPECIALIZE_1(NAME)                                                                   \
    inline constexpr struct NAME##_fn {                                                            \
        template <class T>                                                                         \
        auto operator()(T x) const noexcept                                                        \
        {                                                                                          \
            return ::std::NAME(x);                                                                 \
        }                                                                                          \
    } const NAME {}

        AZUIK_SPECIALIZE_1(exp);
        AZUIK_SPECIALIZE_1(exp2);
        AZUIK_SPECIALIZE_1(expm1);

        AZUIK_SPECIALIZE_1(log);
        AZUIK_SPECIALIZE_1(log10);
        AZUIK_SPECIALIZE_1(log2);
        AZUIK_SPECIALIZE_1(log1p);

        AZUIK_SPECIALIZE_1(sin);
        AZUIK_SPECIALIZE_1(asin);
        AZUIK_SPECIALIZE_1(sinh);
        AZUIK_SPECIALIZE_1(asinh);

        AZUIK_SPECIALIZE_1(cos);
        AZUIK_SPECIALIZE_1(acos);
        AZUIK_SPECIALIZE_1(cosh);
        AZUIK_SPECIALIZE_1(acosh);

        AZUIK_SPECIALIZE_1(tan);
        AZUIK_SPECIALIZE_1(atan);
        AZUIK_SPECIALIZE_1(tanh);
        AZUIK_SPECIALIZE_1(atanh);

        AZUIK_SPECIALIZE_1(erf);
        AZUIK_SPECIALIZE_1(erfc);
        AZUIK_SPECIALIZE_1(tgamma);
        AZUIK_SPECIALIZE_1(lgamma);

        AZUIK_SPECIALIZE_1(logb);
        AZUIK_SPECIALIZE_1(ilogb);

        AZUIK_SPECIALIZE_1(abs);

        AZUIK_SPECIALIZE_1(ceil);
        AZUIK_SPECIALIZE_1(floor);
        AZUIK_SPECIALIZE_1(trunc);

        AZUIK_SPECIALIZE_1(round);
        AZUIK_SPECIALIZE_1(lround);
        AZUIK_SPECIALIZE_1(llround);

        AZUIK_SPECIALIZE_1(rint);
        AZUIK_SPECIALIZE_1(lrint);
        AZUIK_SPECIALIZE_1(llrint);

        AZUIK_SPECIALIZE_1(isnan);
        AZUIK_SPECIALIZE_1(isinf);

    } // namespace core
} // namespace azuik
#endif
