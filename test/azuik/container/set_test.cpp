#include <azuik/container/set.hpp>
#include <azuik/test/unit_test.hpp>

using namespace azuik;

AZUIK_TEST_SUIT(linear_set)
{
    AZUIK_TEST_CASE(constructor_default)
    {
        core::linear_set<int> s;
    }
}
AZUIK_TEST_SUIT(linear_multiset) {}

AZUIK_TEST_SUIT(tree_set) {}
AZUIK_TEST_SUIT(tree_multiset) {}

AZUIK_TEST_SUIT(hash_set) {}

AZUIK_TEST_SUIT(hash_multiset) {}
