#include <azuik/container/view.hpp>
#include <azuik/tool/unit_test.hpp>
#include <vector>
AZUIK_TEST_SUIT(view)
{
    using namespace azuik;
    AZUIK_TEST_CASE(empty)
    {
        using S = std::vector<int>;
        S v{};
        using RV = core::range_view<S>;
        RV rv{v};
        AZUIK_TEST(rv.empty() == true);
    }
    AZUIK_TEST_CASE(size)
    {
        using S = std::vector<int>;
        S v{1, 3, 5, 7};
        using RV = core::range_view<S>;
        RV rv{v};
        AZUIK_TEST(rv.size() == v.size());
    }
}
