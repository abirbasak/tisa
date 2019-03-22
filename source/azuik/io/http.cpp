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
                void run()
                {
                    checked >> ::uv_tcp_init(&m_event_loop, &m_context.handle);
                    struct sockaddr_in addr;
                    checked >> ::uv_ip4_addr(m_host, m_port, &addr);
                    checked >> ::uv_tcp_bind(&m_context.handle, (struct sockaddr*)&addr, 0);
                    checked >> ::uv_listen(as_stream(&m_context.handle), 128, on_connect);
                }
                static void on_connect(::uv_stream_t* stream, int status)
                {
                    auto* ctx = new context{};
                    checked >> ::uv_tcp_init(stream->loop, &ctx->handle);
                    ctx->handle.data = ctx;
                    int r = ::uv_accept(stream, as_stream(&ctx->handle));
                    if (r != 0)
                    {
                        auto* shutdown_request = new uv_shutdown_t{};
                        checked >> ::uv_shutdown(shutdown_request, as_stream(&ctx->handle),
                                                 on_shutdown);
                    }
                    ::http_parser_init(&ctx->parser, HTTP_REQUEST);
                    ctx->parser.data = ctx;
                    checked >> ::uv_read_start(as_stream(&ctx->handle), on_alloc, on_read);
                }
                static void on_alloc(::uv_handle_t* handle, std::size_t suggested_size,
                                     ::uv_buf_t* buffer)
                {}
                static void on_read(::uv_stream_t* stream, ssize_t nread, ::uv_buf_t const* buffer)
                {}
                static void on_write(uv_write_t* write_request, int status)
                {
                    (void)status;
                    ::uv_close(as_handle(write_request->handle), on_close);
                    delete write_request;
                }
                static void on_shutdown(uv_shutdown_t* shutdown_request, int status)
                {
                    (void)status;
                    ::uv_close(as_handle(shutdown_request->handle), on_close);
                    delete shutdown_request;
                }
                static void on_close(uv_handle_t* handle)
                {
                    delete get_context(handle);
                }
                static ::uv_handle_t* as_handle(::uv_stream_t* s)
                {
                    return reinterpret_cast<::uv_handle_t*>(s);
                }
                static ::uv_stream_t* as_stream(::uv_tcp_t* socket)
                {
                    return reinterpret_cast<::uv_stream_t*>(socket);
                }
                template <class Handle>
                static context* get_context(Handle* handle)
                {
                    return static_cast<context*>(handle->data);
                }

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
