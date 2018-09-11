#ifndef AZUIK_CONTAINER_SET_HPP
#define AZUIK_CONTAINER_SET_HPP
#include <azuik/algorithm/functional.hpp>
#include <azuik/core/allocator.hpp>
#include <azuik/container/vector.hpp>
namespace azuik
{
    namespace core
    {
        template <class T, class Pr = less_fn, class A = allocator>
        class tree_set {
        private:
        };
        template <class T, class Pr = less_fn, class A = allocator>
        class tree_multiset {};

        template <class V, class Pr = less_fn>
        class linear_set;

        template <class V, class Pr>
        struct ordered_iterable_traits<linear_set<V, Pr>> : ordered_traits_from_sequence<V, Pr> {};
        template <class V, class Pr>
        struct sequence_traits<linear_set<V, Pr>> {
            using allocator_type = core::allocator_type<V>;
        };
        template <class V, class Pr>
        class linear_set : private assoc_vector<V, Pr> {
        private:
            using base_type = assoc_vector<V, Pr>;

        public:
            using self_type = linear_set;

            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using size_type = core::size_type<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;
            using key_compare = core::key_compare<self_type>;
            using key_type = core::key_type<self_type>;

        public:
            linear_set(key_compare const& comp = {})
                : base_type{comp}
            {}
            template <class FwdIter>
            linear_set(FwdIter first, FwdIter last, key_compare const& comp = {})
                : base_type{comp, first, last}
            {}

        public:
            using base_type::begin;
            using base_type::capacity;
            using base_type::empty;
            using base_type::end;
            using base_type::equal_range;
            using base_type::find;
            using base_type::lower_bound;
            using base_type::upper_bound;

            using base_type::size;
        };
        template <class V, class Pr>
        class linear_multiset {};
        template <class T, class Hash = hash_fn, class Eq = equal_to_fn, class A = allocator>
        class hash_set {};
        template <class T, class Hash = hash_fn, class Eq = equal_to_fn, class A = allocator>
        class hash_multiset {};
    } // namespace core
} // namespace azuik
#endif
