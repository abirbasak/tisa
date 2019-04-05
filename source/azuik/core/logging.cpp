#include <azuik/core/logging.hpp>
namespace azuik
{
    namespace core
    {
        logger& logger::get()
        {
            static logger l;
            return l;
        }
        void logger::set_log_level(log_level l) {
            
        }
    } // namespace core
} // namespace azuik