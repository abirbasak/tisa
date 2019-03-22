#ifndef AZUIK_NET_HTTP_HPP
#define AZUIK_NET_HTTP_HPP
#include <string>
#include <string_view>
#include <map>
#include <cassert>
#include <memory>
#include <type_traits>
namespace azuik
{
    namespace net
    {
        namespace http
        {
            class url {
            public:
                explicit url(std::string const& s);
                explicit url(std::string&& s);
                auto schema() const noexcept -> std::string_view;
                auto host() const noexcept -> std::string_view;
                auto port() const noexcept -> std::string_view;
                auto path() const noexcept -> std::string_view;
                auto query() const noexcept -> std::string_view;

            private:
                friend struct helper;
                std::string m_url;
                std::aligned_storage_t<14 * sizeof(std::uint16_t)> m_storage;
            };

            enum class method_type { GET, POST, DELETE, PUT, HEAD, OPTIONS };

            struct request {
                std::string url;
                method_type method;
                std::string status_code;
                std::string body;
                std::map<std::string, std::string> headers;
            };
            class server {
            public:
                server(char const* host, int port);
                void run();

            private:
                struct implementation;
                std::unique_ptr<implementation> m_impl;
            };

        } // namespace http
    }     // namespace net
} // namespace azuik
#endif
