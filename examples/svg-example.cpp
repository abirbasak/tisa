#include <azuik/io/svg.hpp>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    using namespace azuik::io;
    svg sample1{0, 0, 100, 100};
    sample1.push_back(svg::circle{50.0, 50.0, 40.0});
    sample1.push_back(svg::ellipse{50.0, 50.0, 50.0, 20.0});
    sample1.push_back(svg::line{0.0, 50.0, 100.0, 50.0});
    sample1.push_back(svg::rect{5.0, 90.0, 90.0, 5.0});
    sample1.write(std::cout);
    std::cout << std::endl;
}
