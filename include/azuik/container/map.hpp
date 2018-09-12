#ifndef AZUIK_CONTAINER_MAP_HPP
#define AZUIK_CONTAINER_MAP_HPP
#include <azuik/container/iterable.hpp>
#include <azuik/container/tree.hpp>
#include <azuik/container/vector.hpp>
#include <azuik/algorithm/functional.hpp>
#include <tuple>

namespace azuik
{
    namespace core
    {
        template <class K, class V, class Pr = less_fn>
        class tree_map : private bs_tree<std::tuple<K, V>, Pr> {
        private:
            using base_type = bs_tree<std::tuple<K, V>, Pr>;

        public:
            using self_type = tree_map;
            using allocator_type = core::allocator_type<self_type>;
            using value_type = core::value_type<self_type>;
            using size_type = core::size_type<self_type>;
            using difference_type = core::difference_type<self_type>;
            using pointer = core::pointer<self_type>;
            using reference = core::reference<self_type>;
            using const_pointer = core::const_pointer<self_type>;
            using const_reference = core::const_reference<self_type>;
            using iterator = core::iterator<self_type>;
            using const_iterator = core::iterator<self_type>;
            using key_type = core::key_type<self_type>;
            using key_compare = core::key_compare<self_type>;

        public:
            constexpr explicit tree_map(allocator_type const& a = {}) noexcept
                : base_type{a}
            {}
            constexpr explicit tree_map(key_compare const& c, allocator_type const& a = {}) noexcept
                : base_type{c, a}
            {}

            template <class InIter>
            constexpr tree_map(InIter first, InIter last, allocator_type const& a = {})
                : base_type{first, last, a}
            {}
            template <class InIter>
            constexpr tree_map(InIter first, InIter last, key_compare const& comp,
                               allocator_type const& a = {})
                : base_type{first, last, comp, a}
            {}
            constexpr explicit tree_map(std::initializer_list<value_type> ilist,
                                        allocator_type const& a = {})
                : base_type{ilist, a}
            {}
            constexpr tree_map(std::initializer_list<value_type> ilist, key_compare const& comp,
                               allocator_type const& a = {})
                : base_type{ilist, comp, a}
            {}

        public:
            using base_type::get_allocator;
            using base_type::key_comp;

            using base_type::begin;
            using base_type::end;

            using base_type::empty;
            using base_type::size;

            using base_type::find;
            using base_type::equal_range;
            using base_type::lower_bound;
            using base_type::upper_bound;

            template <class... Args>
            auto constexpr insert(Args&&... args) -> std::tuple<iterator, bool>
            {
                return base_type::insert_unique(static_cast<Args&&>(args)...);
            }
            template <class InIter>
            auto constexpr insert(InIter first, InIter last) -> void
            {
                base_type::insert_unique(first, last);
            }
            using base_type::erase;
            using base_type::clear;
        };

        template <class K, class V, class Pr = less_fn>
        class tree_multimap;

        template <class K, class V, class Pr = less_fn>
        class linear_map : private assoc_vector<std::tuple<K, V>, Pr> {
        private:
            using base_type = assoc_vector<std::tuple<K, V>, Pr>;

        public:
            using self_type = linear_map;
            using allocator_type = core::allocator_type<base_type>;
            using value_type = core::value_type<base_type>;
            using size_type = core::size_type<base_type>;
            using difference_type = core::difference_type<base_type>;
            using reference = core::reference<base_type>;

            using iterator = core::iterator<base_type>;
            using const_iterator = core::const_iterator<base_type>;

            using key_type = core::key_type<self_type>;
            using key_compare = core::key_compare<self_type>;

        public:
            constexpr explicit linear_map(allocator_type const& a = {})
                : base_type{a}
            {}
            constexpr explicit linear_map(key_compare const& comp, allocator_type const& a = {})
                : base_type{comp, a}
            {}
            template <class InIter>
            constexpr linear_map(InIter first, InIter last, allocator_type const& a = {})
                : base_type{first, last, a}
            {}
            template <class InIter>
            constexpr linear_map(InIter first, InIter last, key_compare const& comp,
                                 allocator_type const& a = {})
                : base_type{first, last, comp, a}
            {}
            constexpr explicit linear_map(std::initializer_list<value_type> ilist,
                                          allocator_type const& a = {})
                : base_type{ilist, a}
            {}
            constexpr linear_map(std::initializer_list<value_type> ilist, key_compare const& comp,
                                 allocator_type const& a = {})
                : base_type{ilist, comp, a}
            {}

        public:
            using base_type::get_allocator;
            using base_type::key_comp;

            using base_type::begin;
            using base_type::end;

            using base_type::empty;
            using base_type::size;
            using base_type::capacity;

            template <class... Args>
            auto constexpr insert(Args&&... args) -> std::tuple<iterator, bool>
            {
                return base_type::insert_unique(static_cast<Args&&>(args)...);
            }
            template <class InIter>
            auto constexpr insert(InIter first, InIter last) -> void
            {
                return base_type::insert_unique(first, last);
            }
            using base_type::erase;
            using base_type::clear;
        };

        template <class K, class V, class Pr = less_fn>
        class linear_multimap {
        private:
            using base_type = std::vector<std::tuple<K, V>>;

        public:
            using self_type = linear_multimap;
            using allocator_type = core::allocator_type<base_type>;
            using value_type = core::value_type<base_type>;
            using size_type = core::size_type<base_type>;
            using difference_type = core::difference_type<base_type>;
            using reference = core::reference<base_type>;

        public:
        };

        template <class K, class V>
        class hash_map;

        template <class K, class V>
        class hash_multimap;

    } // namespace core

} // namespace azuik
#endif
