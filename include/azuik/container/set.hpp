#ifndef AZUIK_CONTAINER_SET_HPP
#define AZUIK_CONTAINER_SET_HPP
#include <azuik/algorithm/functional.hpp>
#include <azuik/core/allocator.hpp>
#include <azuik/container/vector.hpp>
#include <vector>
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
        struct ordered_iterable_traits<linear_set<V, Pr>>
            : ordered_traits_from_sequence<std::vector<V>, Pr> {};
        template <class V, class Pr>
        struct sequence_traits<linear_set<V, Pr>> {
            using allocator_type = core::allocator_type<std::vector<V>>;
        };
        template <class V, class Pr>
        class linear_set : public assoc_vector<std::vector<V>, Pr> {
        private:
            using base_type = assoc_vector<std::vector<V>, Pr>;

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
            using key_type = core::key_type<self_type>;
            using key_compare = core::key_compare<self_type>;

        public:
            constexpr linear_set(allocator_type const& alloc = {})
                : base_type{alloc}
            {}
            constexpr linear_set(key_compare const& comp, allocator_type const& alloc = {})
                : base_type{comp, alloc}
            {}
            template <class FwdIter>
            constexpr linear_set(FwdIter first, FwdIter last, allocator_type const& alloc = {})
                : base_type{first, last, alloc}
            {}
            template <class FwdIter>
            constexpr linear_set(FwdIter first, FwdIter last, key_compare const& comp,
                                 allocator_type const& alloc)
                : base_type{first, last, comp, alloc}
            {}
            constexpr linear_set(std::initializer_list<value_type> ilist,
                                 allocator_type const& alloc = {})
                : base_type{ilist, alloc}
            {}
            constexpr linear_set(std::initializer_list<value_type> ilist, key_compare const& comp,
                                 allocator_type const& alloc = {})
                : base_type{ilist, comp, alloc}
            {}

        public:
            using base_type::get_allocator;
            using base_type::key_comp;

            using base_type::begin;
            using base_type::end;

            using base_type::empty;
            using base_type::size;
            using base_type::capacity;

            using base_type::find;
            using base_type::equal_range;
            using base_type::lower_bound;
            using base_type::upper_bound;

            template <class... Args>
            auto constexpr insert(Args&&... args) -> std::tuple<iterator, bool>
            {
                return base_type::insert_unique(static_cast<Args&&>(args)...);
            }
            template <class FwdIter>
            auto constexpr insert(FwdIter first, FwdIter last) -> void
            {
                return base_type::insert_unique(first, last);
            }
            using base_type::erase;
            using base_type::clear;
        };

        template <class V, class Pr = less_fn>
        class linear_multiset;

        template <class V, class Pr>
        struct ordered_iterable_traits<linear_multiset<V, Pr>>
            : ordered_traits_from_sequence<V, Pr> {};

        template <class V, class Pr>
        struct sequence_traits<linear_multiset<V, Pr>> {
            using allocator_type = core::allocator_type<V>;
        };
        template <class V, class Pr>
        class linear_multiset : private assoc_vector<V, Pr> {
        private:
            using base_type = assoc_vector<V, Pr>;

        public:
            using self_type = linear_multiset;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_tupe = core::difference_type<self_type>;
            using reference = core::reference<self_type>;
            using const_reference = core::const_reference<self_type>;
            using pointer = core::pointer<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::const_iterator<self_type>;
            using key_type = core::key_type<self_type>;
            using key_compare = core::key_compare<self_type>;

        public:
            using base_type::get_allocator;
            using base_type::key_comp;

            using base_type::begin;
            using base_type::end;

            using base_type::empty;
            using base_type::size;
            using base_type::capacity;

            using base_type::find;
            using base_type::equal_range;
            using base_type::lower_bound;
            using base_type::upper_bound;

            template <class... Args>
            auto constexpr insert(Args&&... args) -> std::tuple<iterator, bool>
            {
                return base_type::insert_equal(static_cast<Args&&>(args)...);
            }
            template <class FwdIter>
            auto constexpr insert(FwdIter first, FwdIter last) -> void
            {
                return base_type::insert_equal(first, last);
            }
            using base_type::erase;
            using base_type::clear;
        };
        template <class T, class Hash = hash_fn, class Eq = equal_to_fn, class A = allocator>
        class hash_set {};
        template <class T, class Hash = hash_fn, class Eq = equal_to_fn, class A = allocator>
        class hash_multiset {};
    } // namespace core
} // namespace azuik
#endif
