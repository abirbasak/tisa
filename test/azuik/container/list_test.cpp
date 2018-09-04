#include <azuik/container/list.hpp>
#include <azuik/test/unit_test.hpp>
using namespace azuik;

AZUIK_TEST_SUIT(list_test)
{
    AZUIK_TEST_CASE(empty)
    {
        core::forward_list<int> l;
        AZUIK_TEST(l.empty() == true);
        l.push_front(1);
        AZUIK_TEST(l.empty() == false);
    }
}
