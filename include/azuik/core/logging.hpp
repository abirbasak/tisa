#ifndef AZUIK_CORE_LOGGING_HPP
#define AZUIK_CORE_LOGGING_HPP
namespace azuik
{
    namespace core
    {
        enum class log_level { info, warning, error, critical };
        class logger {
            template <class T>
            logger& operator<<(T const& x);
            static void set_log_level(log_level level);
            static logger& get();
        };
    } // namespace core
} // namespace azuik
#endif