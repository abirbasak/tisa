#include <azuik/core/error.hpp>
namespace azuik
{
    namespace core
    {
        exception::exception() : m_msg{'\0'} {}
        char const* exception::what() const noexcept
        {
            return m_msg;
        }

    } // namespace core

} // namespace azuik
