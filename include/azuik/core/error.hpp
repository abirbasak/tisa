#ifndef AZUIK_CORE_ERROR_HPP
#define AZUIK_CORE_ERROR_HPP
#include <azuik/core/mpl.hpp>
#include <cstdarg>
#include <exception>
namespace azuik
{
    namespace core
    {
        class exception : ::std::exception
        {
          private:
            static unsigned const SIZE = 128;

          public:
            exception() : m_msg{'\0'}
            {
            }
            char const *what() const noexcept
            {
                return m_msg;
            }

          private:
            char m_msg[SIZE];
        };

        namespace exception_
        {
            AZUIK_HAS_NESTED_TYPE(size_type);
            template <class Base, class Enable = defaulted>
            struct exception_base : public Base
            {
              public:
                using size_type = size_t;
                static size_type const msg_size = 256;

              protected:
                char msg_[msg_size];
            };
            template <class Base>
            struct exception_base<Base, enable_if<has_type_size_type<Base>>> : public Base
            {
            };
        } // namespace exception_
        /**
         *  a factory to create exceptions from a base class.
         *  derived exception class contains the data
         */
        template <class Tag, class Base = exception>
        class exception_factory : public exception_::exception_base<Base>
        {
          private:
            using exception_base = exception_::exception_base<Base>;

          public:
            exception_factory() noexcept : exception_base()
            {
            }
            explicit exception_factory(const char *fmt, ...) noexcept : exception_base()
            {
                va_list args;
                va_start(args, fmt);
                vsnprintf(exception_base::msg_, exception_base::msg_size, fmt, args);
                va_end(args);
            }
            exception_factory(const char *fmt, va_list args) noexcept : exception_base()
            {
                vsnprintf(exception_base::msg_, exception_base::msg_size, fmt, args);
            }
            char const *what() const noexcept
            {
                return exception_base::msg_;
            }
        };

        template <class E>
        void throw_if(bool c, const char *fmt, ...)
        {
            if (c) { throw E{}; }
        }
    } // namespace core
} // namespace azuik
#endif
