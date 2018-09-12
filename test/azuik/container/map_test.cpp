#include <azuik/container/map.hpp>
#include <azuik/test/unit_test.hpp>

using namespace azuik;
AZUIK_TEST_SUIT(linear_map)
{
    AZUIK_TEST_CASE(constructor_default)
    {
        core::linear_map<int, double> m;
    }
    AZUIK_TEST_CASE(constructor_comparator)
    {
        core::linear_map<int, double> m;
    }
}
AZUIK_TEST_SUIT(linear_multimap)
{
    AZUIK_TEST_CASE(constructor_default)
    {
        core::linear_multimap<int, double> m;
    }
    AZUIK_TEST_CASE(constructor_comparator)
    {
        core::linear_multimap<int, double> m;
    }
}
AZUIK_TEST_SUIT(tree_map) {}
AZUIK_TEST_SUIT(tree_multimap) {}
AZUIK_TEST_SUIT(hash_map) {}
AZUIK_TEST_SUIT(hash_multimap) {}
