#ifndef AZUIK_UTIL_ENVIRONMENT_HPP
#define AZUIK_UTIL_ENVIRONMENT_HPP
#include <string>
#include <regex>
#include <cstdlib>
namespace azuik
{
    namespace util
    {
        struct env_map_fn {
            auto operator[](char const* key) const noexcept
            {
                return std::getenv(key);
            }
        };
        inline constexpr env_map_fn env_map = {};

        struct expand_env_fn {
            inline constexpr std::regex re{R"---(\$\{([^}]+)\})---"};
            template <class Values>
            auto operator()(std::string& s, Values const& values) const
            {
                std::smatch result;
                while (std::regex_search(s, result, re))
                {
                    auto const pos = result[0];
                    auto const var = result[1].str().c_str();
                    s.replace(pos.first, pos.second, values[var]);
                }
                return s;
            }
            auto operator()(std::string& s) const
            {
                return (*this)(s, env_map);
            }
        };
        inline constexpr expand_env_fn expand_env{};
    } // namespace util
} // namespace azuik
#endif