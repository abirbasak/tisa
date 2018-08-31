#include <azuik/container/list.hpp>
#include <azuik/test/unit_test.hpp>
using azuik::core::list;

AZUIK_TEST_SUIT(list_test)
{
    AZUIK_TEST_CASE(empty)
    {
        list<int> l;
        AZUIK_TEST(l.empty() == true);
        l.push_back(1);
        AZUIK_TEST(l.empty() == false);
    }
}
