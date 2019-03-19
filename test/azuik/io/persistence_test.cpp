#include <azuik/tool/unit_test.hpp>
#include <azuik/io/persistence.hpp>
#include <iostream>
AZUIK_TEST_SUIT(persist)
{
    struct person {
        std::string name;
        int age;

        struct visit {
            template <class Self, class F>
            void operator()(Self&& self, F f)
            {
                f("name", self.name);
                f("age", self.age);
            }
        };
    };

    AZUIK_TEST_CASE(xml)
    {
        person p{"abir basak", 36};
        azuik::io::write_xml(p, std::cout);
    }
}