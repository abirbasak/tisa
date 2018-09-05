#include <azuik/container/list.hpp>
#include <azuik/test/unit_test.hpp>
using namespace azuik;

AZUIK_TEST_SUIT(list_test)
{
    AZUIK_TEST_CASE(empty1)
    {
        core::forward_list<int> l;
        AZUIK_TEST(l.empty() == true);
        l.push_front(1);
        AZUIK_TEST(l.empty() == false);
    }
    AZUIK_TEST_CASE(empty2)
    {
        core::list<int> l;
        AZUIK_TEST(l.empty() == true);
        l.push_front(1);
        AZUIK_TEST(l.empty() == false);
    }
    AZUIK_TEST_CASE(count1)
    {
        core::forward_list<int> l;
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 0);
        l.push_front(10);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 1);
        l.push_front(15);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 2);
        l.push_front(20);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 3);
        l.push_front(25);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 4);

        l.pop_front();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 3);
        l.pop_front();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 2);
        l.pop_front();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 1);
        l.pop_front();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 0);
    }
    AZUIK_TEST_CASE(count2)
    {
        core::list<int> l;
        l.begin();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 0);
        l.push_front(10);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 1);
        l.push_front(15);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 2);
        l.push_front(20);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 3);
        l.push_front(25);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 4);

        l.pop_front();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 3);
        l.pop_front();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 2);
        l.pop_front();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 1);
        l.pop_front();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 0);
    }
    AZUIK_TEST_CASE(count3)
    {
        core::list<int> l;
        l.begin();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 0);
        l.push_back(10);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 1);
        l.push_back(15);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 2);
        l.push_back(20);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 3);
        l.push_back(25);
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 4);

        l.pop_back();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 3);
        l.pop_back();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 2);
        l.pop_back();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 1);
        l.pop_back();
        AZUIK_TEST(std::distance(l.begin(), l.end()) == 0);
    }
    AZUIK_TEST_CASE(constructor)
    {
        core::forward_list<int> l0{1, 2};
        AZUIK_TEST(std::distance(l0.begin(), l0.end()) == 2);

        core::forward_list<int> l1{1, 2, 3};
        AZUIK_TEST(std::distance(l1.begin(), l1.end()) == 3);

        core::forward_list<int> l2{1, 2, 3, 4};
        AZUIK_TEST(std::distance(l2.begin(), l2.end()) == 4);
    }
}
