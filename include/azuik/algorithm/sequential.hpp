#ifndef AZUIK_ALGORITHM_SEQUENTIAL_HPP
#define AZUIK_ALGORITHM_SEQUENTIAL_HPP
#include <azuik/core/mpl.hpp>
#include <azuik/algorithm/functional.hpp>
#include <algorithm>
namespace azuik
{
    namespace core
    {
#define AZUIK_ALGORITHM_PRED(NAME, STD_NAME)                                                       \
    struct NAME##_fn {                                                                             \
        template <class View, class Pred,                                                          \
                  core::enable_if<is_unary_predicate<Pred, value_type<View>>::value, int> = 0>     \
        auto constexpr operator()(View&& v, Pred p) const noexcept                                 \
        {                                                                                          \
            return ::std::STD_NAME(begin(v), end(v), p);                                           \
        }                                                                                          \
        template <class View, class T, class BiPred = equal_to_fn,                                 \
                  core::disable_if<is_unary_predicate<T, value_type<View>>::value, int> = 0>       \
        auto constexpr operator()(View&& v, T const& x, BiPred pr = {}) const noexcept             \
        {                                                                                          \
            return ::std::STD_NAME(begin(v), end(v),                                               \
                                   [&x, pr](auto const& e) { return pr(x, e); });                  \
        }                                                                                          \
    };                                                                                             \
    inline constexpr NAME##_fn NAME

#define AZUIK_ALGORITHM_BIPRED(NAME, STD_NAME)                                                     \
    struct NAME##_fn {                                                                             \
        template <class View>                                                                      \
        auto constexpr operator()(View&& v) const noexcept                                         \
        {                                                                                          \
            return ::std::STD_NAME(begin(v), end(v));                                              \
        }                                                                                          \
        template <class View, class BiPred>                                                        \
        auto constexpr operator()(View&& v, BiPred p) const noexcept                               \
        {                                                                                          \
            return ::std::STD_NAME(begin(v), end(v), p);                                           \
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

    } // namespace core
} // namespace azuik
#endif
