
#include <azuik/algorithm/sequential.hpp>
#include <azuik/test/unit_test.hpp>
#include <vector>
AZUIK_TEST_SUIT(sequential)
{
    using namespace azuik;
    AZUIK_TEST_CASE(all_of)
    {
        std::vector<int> v1{1, 2, 3, 4, 5};
        std::vector<int> v2{2, 2, 2, 2, 2};
        AZUIK_TEST(core::all_of(v1, 3) == false);
        AZUIK_TEST(core::all_of(v2, 2) == true);
        AZUIK_TEST(core::all_of(v2, 3) == false);
    }
    AZUIK_TEST_CASE(none_of)
    {
        std::vector<int> v1{1, 2, 3, 4, 5};
        std::vector<int> v2{2, 2, 2, 2, 2};
        AZUIK_TEST(core::none_of(v1, 3) == false);
        AZUIK_TEST(core::none_of(v2, 20) == true);
        AZUIK_TEST(core::none_of(v2, 3) == true);
        AZUIK_TEST(core::none_of(v2, 2) == false);
    }
    AZUIK_TEST_CASE(any_of)
    {
        std::vector<int> v1{1, 2, 3, 4, 5};
        std::vector<int> v2{2, 2, 2, 2, 2};
        AZUIK_TEST(core::any_of(v1, 3) == true);
        AZUIK_TEST(core::any_of(v2, 6) == false);
        AZUIK_TEST(core::any_of(v2, 3) == false);
        AZUIK_TEST(core::any_of(v2, 2) == true);
    }
    AZUIK_TEST_CASE(count)
    {
        std::vector<int> v1{1, 2, 3, 2, 5, 2, 5};
        AZUIK_TEST(core::count(v1, 1) == 1);
        AZUIK_TEST(core::count(v1, 2) == 3);
        AZUIK_TEST(core::count(v1, 5) == 2);
        AZUIK_TEST(core::count(v1, [](auto x) { return x % 2 == 0; }) == 3);
        AZUIK_TEST(core::count(v1, [](auto x) { return x > 2; }) == 3);
    }
    AZUIK_TEST_CASE(find)
    {
        std::vector<int> v{0, 1, 2, 3, 4, 5};

        AZUIK_TEST(*core::find(v, 3) == 3);
        AZUIK_TEST(core::find(v, 10) == std::end(v));

        AZUIK_TEST(*core::find(v, [](auto x) { return x == 3; }) == 3);
        AZUIK_TEST(core::find(v, [](auto x) { return x == 10; }) == std::end(v));
    }
    AZUIK_TEST_CASE(find_not)
    {
        std::vector<int> v0{1, 1, 1, 3, 4, 5};

        AZUIK_TEST(*core::find_not(v0, 1) == 3);
        std::vector<int> v1{3, 3, 3, 3, 3, 3};
        AZUIK_TEST(core::find_not(v1, 3) == std::end(v1));
        std::vector<int> v2{0, 1, 2, 3, 4, 5};
        AZUIK_TEST(*core::find_not(v2, [](auto x) { return x != 3; }) == 3);
        AZUIK_TEST(core::find_not(v2, [](auto x) { return x != 10; }) == std::end(v2));
    }
}
