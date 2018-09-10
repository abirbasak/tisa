#ifndef AZUIK_CONTAINER_MAP_HPP
#define AZUIK_CONTAINER_MAP_HPP
#include <azuik/container/iterable.hpp>
#include <azuik/container/tree.hpp>
#include <azuik/algorithm/functional.hpp>
#include <tuple>
#include <vector>
namespace azuik
{
    namespace core
    {
        template <class K, class V, class Pr = less_fn>
        class tree_map : private bs_tree<K, V> {
        private:
            using base_type = bs_tree<K, V>;

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

        public:
            constexpr explicit tree_map(allocator_type const& a = {}) noexcept
                : base_type{a}
            {}
            template <class InIter>
            constexpr tree_map(InIter first, InIter last, allocator_type const& a = {})
                : base_type{a}
            {
                base_type::insert_unique(first, last);
            }
            auto constexpr operator[](key_type const& k) -> mapped_type&
            {
                auto p = base_type::lower_bound(k);
                if (p == base_type::end() || key_comp()(k, std::get<0>(*p)))
                {
                    p = base_type::insert(p, k, mapped_type{});
                }
                return std::get<1>(*p);
            }
            template <class... Args>
            auto constexpr insert(Args&&... args)
            {
                return base_type::insert_unique(static_cast<Args&&>(args)...);
            }
            template <class... Args>
            auto constexpr insert(const_iterator p, Args&&... args)
            {
                return base_type::insert_unique(p, static_cast<Args&&>(args)...);
            }
            template <class InIter>
            auto constexpr insert(InIter first, InIter last)
            {
                base_type::insert_unique(first, last);
            }
            using base_type::begin;
            using base_type::clear;
            using base_type::empty;
            using base_type::end;
            using base_type::erase;
            using base_type::get_allocator;
            using base_type::size;
            //
            using base_type::equal_range;
            using base_type::find;
            using base_type::lower_bound;
            using base_type::upper_bound;
        };

        template <class K, class V, class Pr = less_fn>
        class tree_multimap;

        template <class K, class V, class Pr = less_fn>
        class linear_map : private std::vector<std::tuple<K, V>> {
        private:
            using base_type = std::vector<std::tuple<K, V>>;

        public:
            using self_type = linear_map;
            using allocator_type = core::allocator_type<base_type>;
            using value_type = core::value_type<base_type>;
            using size_type = core::size_type<base_type>;
            using difference_type = core::difference_type<base_type>;
            using reference = core::reference<base_type>;

            using iterator = core::iterator<base_type>;
            using const_iterator = core::const_iterator<base_type>;

        public:
            explicit linear_map(allocator_type const& a = {})
                : base_type{a}
            {}
            template <class InIter>
            linear_map(InIter first, InIter last, allocator_type const& a = {})
                : base_type{a}
            {
                insert(first, last);
            }
            explicit linear_map(std::initializer_list<value_type> init,
                                allocator_type const& a = {})
                : base_type{a}
            {
                insert(init.begin(), init.end());
            }
            auto insert(std::initializer_list<value_type> init) -> void
            {
                insert(init.begin(), init.end());
            }

            template <class InIter>
            auto constexpr insert(InIter first, InIter last) -> void
            {
                for (; first != last; ++first)
                {
                    insert(*first);
                }
            }
            using base_type::erase;
            auto constexpr erase(key_type const& k)
            {
                auto pos = find(k);
                if (pos != end())
                {
                    erase(pos);
                    return 1;
                }
                return 0;
            }
            auto constexpr count(key_type const& k) const noexcept -> size_type
            {
                auto pos = find(k);
                return (pos != end()) ? (size_type)1 : (size_type)0;
            }
            template <class Key>
            auto constexpr lower_bound(Key const& k) noexcept -> iterator
            {
                return std::lower_bound(begin(), end(), k, mValueCompare);
            }
            template <class Key>
            auto constexpr lower_bound(Key const& k) const noexcept -> const_iterator
            {
                return std::lower_bound(begin(), end(), k, mValueCompare);
            }
            template <class Key>
            auto constexpr upper_bound(Key const& k) noexcept -> iterator
            {
                return std::upper_bound(begin(), end(), k, mValueCompare);
            }
            template <class Key>
            auto constexpr upper_bound(Key const& k) const noexcept -> const_iterator
            {
                return std::upper_bound(begin(), end(), k, mValueCompare);
            }

            using base_type::begin;
            using base_type::empty;
            using base_type::end;
            using base_type::get_allocator;
            using base_type::size;
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
