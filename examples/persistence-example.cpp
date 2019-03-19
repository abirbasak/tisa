#include <azuik/io/persistence.hpp>
#include <iostream>

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

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    person p{"abir basak", 36};
    azuik::io::write_xml(p, std::cout);
    std::cout << std::endl;
}
