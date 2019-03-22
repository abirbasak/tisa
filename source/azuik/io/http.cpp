#include <azuik/io/http.hpp>
#include <cstddef>
#include <uv.h>
#include <http_parser.h>

namespace azuik
{
    namespace net
    {
        namespace http
        {
            struct checker {
                void operator>>(int r) const {}
            };
            inline constexpr checker checked{};

            struct helper {
                static ::http_parser_url* parser(url& u) noexcept
                {
                    return reinterpret_cast<::http_parser_url*>(&u.m_storage);
                }
                static ::http_parser_url const* parser(url const& u) noexcept
                {
                    return reinterpret_cast<::http_parser_url const*>(&u.m_storage);
                }
                template <http_parser_url_fields F>
                static auto field(url const& u) noexcept -> std::string_view
                {
                    return std::string_view{u.m_url}.substr(parser(u)->field_data[F].off,
                                                            parser(u)->field_data[F].len);
                }
            };

            url::url(std::string const& s)
                : m_url{s}
            {
                ::http_parser_url_init(helper::parser(*this));
                checked >> ::http_parser_parse_url(s.c_str(), s.size(), 0, helper::parser(*this));
            }
            url::url(std::string&& s)
                : m_url{std::move(s)}
            {
                ::http_parser_url_init(helper::parser(*this));
                checked >> ::http_parser_parse_url(s.c_str(), s.size(), 0, helper::parser(*this));
            }

            auto url::schema() const noexcept -> std::string_view
            {
                return helper::field<UF_SCHEMA>(*this);
            }
            auto url::host() const noexcept -> std::string_view
            {
                return helper::field<UF_HOST>(*this);
            }
            auto url::port() const noexcept -> std::string_view
            {
                return helper::field<UF_PORT>(*this);
            }
            auto url::path() const noexcept -> std::string_view
            {
                return helper::field<UF_PATH>(*this);
            }
            auto url::query() const noexcept -> std::string_view
            {
                return helper::field<UF_QUERY>(*this);
            }

            struct context {
                ::uv_tcp_t handle;
                ::http_parser parser;
                ::http_parser_settings settings;
            };

            struct server::implementation {
                implementation(char const* host, int port)
                    : m_host{host}
                    , m_port{port}
                    , m_event_loop{::uv_default_loop()}
                    , m_context{}
                {}
                void run() {}

            private:
                char const* m_host;
                int m_port;
                ::uv_loop_t m_event_loop;
                context m_context;
            };

            server::server(char const* host, int port)
                : m_impl{std::make_unique<implementation>(host, port)}
            {}
            void server::run()
            {
                m_impl->run();
            }
        } // namespace http
    }     // namespace net

} // namespace azuik
