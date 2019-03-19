#ifndef AZUIK_IO_FILE_HPP
#define AZUIK_IO_FILE_HPP
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
namespace azuik
{
    namespace io
    {
        struct read_file_fn {
            std::string operator()(std::ifstream& in) const
            {
                return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
            }
        };
        inline constexpr read_file_fn read_file{};

        struct split_fn {
            std::vector<std::string> operator()(std::string const& s, char delimiter) const
            {
                std::vector<std::string> values;
                std::string value;
                std::istringstream ss{s};
                while (std::getline(ss, value, delimiter))
                {
                    values.push_back(value);
                }
                return values;
            }
        };
        inline constexpr split_fn split{};

        struct read_csv_fn {
            template <class S>
            S& operator()(std::ifstream& in, S& out, char separator = ',') const
            {
                std::string line;
                while (std::getline(in, line))
                {
                    if (line[0] != '#')
                    {
                        auto values = split(line, separator);
                        out(values);
                    }
                }
                return out;
            }
        };
    } // namespace io
} // namespace azuik
#endif
