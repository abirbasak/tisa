#ifndef AZUIK_ALGORITHM_SEQUENTIAL_HPP
#define AZUIK_ALGORITHM_SEQUENTIAL_HPP
#include <azuik/core/mpl.hpp>
#include <azuik/algorithm/functional.hpp>
#include <azuik/container/view.hpp>
#include <algorithm>
namespace azuik
{
    namespace core
    {
#define AZUIK_ALGORITHM_PRED(NAME, STD_NAME)                                                       \
    struct NAME##_fn {                                                                             \
        template <class View, class Pred,                                                          \
                  core::enable_if<is_unary_predicate<Pred, value_type<View>>, int> = 0>            \
        auto constexpr operator()(View&& v, Pred p) const noexcept                                 \
        {                                                                                          \
            return ::std::STD_NAME(core::begin(v), core::end(v), p);                               \
        }                                                                                          \
        template <class View, class T, class BiPred = equal_to_fn,                                 \
                  core::disable_if<is_unary_predicate<T, value_type<View>>, int> = 0>              \
        auto constexpr operator()(View&& v, T const& x, BiPred pr = {}) const noexcept             \
        {                                                                                          \
            return ::std::STD_NAME(core::begin(v), core::end(v),                                   \
                                   [&x, pr](auto const& e) { return pr(x, e); });                  \
        }                                                                                          \
    };                                                                                             \
    inline constexpr NAME##_fn NAME

#define AZUIK_ALGORITHM_BIPRED(NAME, STD_NAME)                                                     \
    struct NAME##_fn {                                                                             \
        template <class View>                                                                      \
        auto constexpr operator()(View&& v) const noexcept                                         \
        {                                                                                          \
            return ::std::STD_NAME(core::begin(v), core::end(v));                                  \
        }                                                                                          \
        template <class View, class BiPred>                                                        \
        auto constexpr operator()(View&& v, BiPred p) const noexcept                               \
        {                                                                                          \
            return ::std::STD_NAME(core ::begin(v), core::end(v), p);                              \
        }                                                                                          \
    };                                                                                             \
    inline constexpr NAME##_fn NAME

#define AZUIK_ALGORITHM_SEQ(NAME, STD_NAME)                                                        \
    struct NAME##_fn {                                                                             \
        template <class Iter1, class Iter2>                                                        \
        auto constexpr operator()(Iter1 f1, Iter1 l1, Iter2 f2) const noexcept                     \
        {                                                                                          \
            return ::std::STD_NAME(f1, l1, f2);                                                    \
        }                                                                                          \
        template <class Iter1, class Iter2>                                                        \
        auto constexpr operator()(Iter1 f1, Iter1 l1, Iter2 f2, Iter2 l2) const noexcept           \
        {                                                                                          \
            return ::std::STD_NAME(f1, l1, f2, l2);                                                \
        }                                                                                          \
        template <class View1, class Iter2>                                                        \
        auto constexpr operator()(View1&& v1, Iter2 first) const noexcept                          \
        {                                                                                          \
            return ::std::STD_NAME(core::begin(v1), core::end(v1), first);                         \
        }                                                                                          \
        template <class View1, class View2>                                                        \
        auto constexpr operator()(View1&& v1, View2&& v2) const noexcept                           \
        {                                                                                          \
            return ::std::STD_NAME(core::begin(v1), core::end(v1), core::begin(v2),                \
                                   core::end(v2));                                                 \
        }                                                                                          \
        template <class View1, class Iter2, class BiPred>                                          \
        auto constexpr operator()(View1&& v1, Iter2 first, BiPred p) const noexcept                \
        {                                                                                          \
            return ::std::STD_NAME(core::begin(v1), core::end(v1), first, p);                      \
        }                                                                                          \
        template <class View1, class View2, class BiPred>                                          \
        auto constexpr operator()(View1&& v1, View2&& v2, BiPred p) const noexcept                 \
        {                                                                                          \
            return ::std::STD_NAME(core::begin(v1), core::end(v1), core::begin(v2), core::end(v2), \
                                   p);                                                             \
        }                                                                                          \
    };                                                                                             \
    inline constexpr NAME##_fn NAME

        AZUIK_ALGORITHM_PRED(all_of, all_of);
        AZUIK_ALGORITHM_PRED(none_of, none_of);
        AZUIK_ALGORITHM_PRED(any_of, any_of);
        AZUIK_ALGORITHM_PRED(count, count_if);

        // search
        AZUIK_ALGORITHM_PRED(find, find_if);
        AZUIK_ALGORITHM_PRED(find_not, find_if_not);
        AZUIK_ALGORITHM_PRED(remove, remove_if);

        // partition
        AZUIK_ALGORITHM_PRED(is_partitioned, is_partitioned);
        //TBD: is_partitioned_until
        AZUIK_ALGORITHM_PRED(partition, partition);
        AZUIK_ALGORITHM_PRED(stable_partition, stable_partition);
        AZUIK_ALGORITHM_PRED(partition_point, partition_point);
        //TBD: partition_copy(InView,Pred,Out1,Out2)

        //sorting
        AZUIK_ALGORITHM_BIPRED(is_sorted, is_sorted);
        AZUIK_ALGORITHM_BIPRED(is_sorted_until, is_sorted_until);
        AZUIK_ALGORITHM_BIPRED(sort, sort);
        AZUIK_ALGORITHM_BIPRED(stable_sort, stable_sort);
        // AZUIK_ALGORITHM_BPRED(nth_element, nth_element);
        // AZUIK_ALGORITHM_BPRED(partial_sort, partial_sort);
        // AZUIK_ALGORITHM_BPRED(partial_sort_copy, partial_sort_copy);

        // heap
        AZUIK_ALGORITHM_BIPRED(is_heap, is_heap);
        AZUIK_ALGORITHM_BIPRED(is_heap_until, is_heap_until);
        AZUIK_ALGORITHM_BIPRED(make_heap, make_heap);
        AZUIK_ALGORITHM_BIPRED(push_heap, push_heap);
        AZUIK_ALGORITHM_BIPRED(pop_heap, pop_heap);
        AZUIK_ALGORITHM_BIPRED(sort_heap, sort_heap);

        // rank
        AZUIK_ALGORITHM_BIPRED(min_element, min_element);
        AZUIK_ALGORITHM_BIPRED(max_element, max_element);
        AZUIK_ALGORITHM_BIPRED(minmax_element, minmax_element);
        AZUIK_ALGORITHM_SEQ(equal, equal);
        AZUIK_ALGORITHM_SEQ(lexicographical_compare, lexicographical_compare);

        // permutations
        AZUIK_ALGORITHM_SEQ(is_permutation, is_permutation);
        AZUIK_ALGORITHM_BIPRED(next_permutation, next_permutation);
        AZUIK_ALGORITHM_BIPRED(prev_permutation, prev_permutation);

    } // namespace core

    namespace core
    {
        struct for_each_fn {
            template <class Iterable, class Fn>
            void operator()(Iterable&& r, Fn fn) const noexcept
            {
                auto f = core::begin(r);
                auto l = core::end(r);
                for (; f != l; ++f)
                {
                    fn(core::source(f));
                }
            }
        } const for_each{};

        inline struct find_range_fn {
            template <class FwdIterable, class T, class Pred>
            core::range_view<FwdIterable> operator()(FwdIterable&& v, T const& x,
                                                     Pred pr = {}) const noexcept
            {
                auto first = core::find(static_cast<FwdIterable&&>(v), pr);
                auto last = core::find_not(static_cast<FwdIterable&&>(v), pr);
                return {first, last};
            }
        } const find_range{};

        struct push_sorted_fn {
            template <class FwdIter, class Comp>
            auto constexpr operator()(FwdIter first, FwdIter last, Comp cmp) const noexcept
                -> FwdIter
            {
                auto i = std::lower_bound(first, std::prev(last), *last, cmp);
                if (i == last || cmp(*last * i))
                {
                    std::rotate(first, last, i);
                }
            }
            template <class FwdView, class Comp>
            auto constexpr operator()(FwdView&& v, Comp c) const noexcept
            {
                return (*this)(core::begin(v), core::end(v), c);
            }
        } inline constexpr push_sorted{};

        struct find_sorted_fn {
            template <class FwdIter, class T, class Comp>
            auto constexpr operator()(FwdIter first, FwdIter last, T&& x, Comp cmp) const noexcept
                -> FwdIter
            {
                auto i = std::lower_bound(first, last, x, cmp);
                return i == last || cmp(x, *i) ? last : i;
            }
            template <class FwdView, class T, class Comp>
            auto constexpr operator()(FwdView&& v, T&& x, Comp cmp) const noexcept
            {
                return (*this)(core::begin(v), core::end(v), x, cmp);
            }
        } inline constexpr find_sorted{};

        struct copy_fn {
            //copy
            template <class InIter, class OutIter>
            auto constexpr operator()(InIter first, InIter last, OutIter result) const -> OutIter
            {
                return ::std::copy(first, last, result);
            }
            template <class InView, class OutIter>
            auto constexpr operator()(InView&& in, OutIter result) const -> OutIter
            {
                return (*this)(core::begin(in), core::end(in), result);
            }
            //copy_n
            template <class FwdIter, class Size, class OutIter,
                      core::enable_if<core::is_integral<Size>, int> = 0>
            auto constexpr operator()(FwdIter first, Size n, OutIter result) const -> OutIter
            {
                return ::std::copy_n(first, n, result);
            }
            //copy_bounded
            template <class InIter, class OutIter>
            auto constexpr operator()(InIter first, InIter last, OutIter dfirst,
                                      OutIter dlast) const -> std::pair<InIter, OutIter>
            {
                for (; first != last && dfirst != dlast; ++first, void(), ++dfirst)
                {
                    *dfirst = *first;
                }
                return {first, dfirst};
            }
        };
        inline constexpr copy_fn copy = {};

        struct copy_if_fn {
            //copy_if
            template <class InIter, class OutIter, class Pred>
            auto constexpr operator()(InIter first, InIter last, OutIter result, Pred p) const
                -> OutIter
            {
                return ::std::copy_if(first, last, result, p);
            }
            template <class InView, class OutIter, class Pred>
            auto constexpr operator()(InView&& in, OutIter result, Pred p) const -> OutIter
            {
                return ::std::copy_if(core::begin(in), core::end(in), result, p);
            }
            //copy_n_if
            template <class FwdIter, class Size, class OutIter, class Pred>
            auto constexpr operator()(FwdIter first, Size n, OutIter result, Pred p) const
                -> OutIter
            {
                for (; n != 0; ++first, void(), --n)
                {
                    if (p(*first))
                    {
                        *result++ = *first;
                    }
                }
                return result;
            }
            //copy_bounded_if
            template <class InIter, class OutIter, class Pred>
            auto constexpr operator()(InIter first, InIter last, OutIter dfirst, OutIter dlast,
                                      Pred p) const -> std::pair<InIter, OutIter>
            {
                for (; first != last && dfirst != dlast; ++first, void(), ++dfirst)
                {
                    if (p(*first))
                    {
                        *dfirst = *first;
                    }
                }
                return {first, dfirst};
            }
        };
        inline constexpr copy_if_fn copy_if = {};

        struct copy_backward_fn {
            template <class BiIter1, class BiIter2>
            auto constexpr operator()(BiIter1 first, BiIter1 last, BiIter1 result) const -> BiIter2
            {
                return ::std::copy_backward(first, last, result);
            }
        };
        inline constexpr copy_backward_fn copy_backward = {};

        struct copy_while_fn {
            template <class InIter, class OutIter, class Pred>
            auto constexpr operator()(InIter first, InIter last, OutIter result, Pred p) const
                -> std::pair<InIter, OutIter>
            {
                for (; first != last && p(*first); ++first)
                {
                    *result++ = *first;
                }
                return {first, result};
            }
            template <class InView, class OutIter, class Pred>
            auto constexpr operator()(InView&& in, OutIter result, Pred p) const
                -> std::pair<core::iterator<InView>, OutIter>
            {
                return (*this)(core::begin(in), core::end(in), result, p);
            }
        };
        inline constexpr copy_while_fn copy_while = {};

        struct copy_until_fn {
            template <class InIter, class OutIter, class Pred>
            auto constexpr operator()(InIter first, InIter last, OutIter result, Pred p) const
                -> std::pair<InIter, OutIter>
            {
                for (; first != last && !p(*first); ++first)
                {
                    *result++ = *first;
                }
                return {first, result};
            }
            template <class InView, class OutIter, class Pred>
            auto constexpr operator()(InView&& in, OutIter result, Pred p) const
                -> std::pair<core::iterator<InView>, OutIter>
            {
                return (*this)(core::begin(in), core::end(in), result, p);
            }
        };

        inline constexpr copy_until_fn copy_until = {};

        struct shift_right_fn {
            template <class BiIter>
            auto constexpr operator()(BiIter first, BiIter last, difference_type<BiIter> n) const
                noexcept -> BiIter
            {
                return ::std::move_backward(first, std::prev(last, n), last);
            }
        };
        inline constexpr shift_right_fn shift_right = {};

        struct shift_left_fn {
            template <class FwdIter>
            auto constexpr operator()(FwdIter first, FwdIter last, difference_type<FwdIter> n) const
                noexcept -> FwdIter
            {
                return ::std::move(first, last, std::prev(first, n));
            }
        };
        inline constexpr shift_left_fn shift_left = {};

    } // namespace core
} // namespace azuik
#endif
