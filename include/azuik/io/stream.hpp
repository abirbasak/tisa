#ifndef AZUIK_IO_STREAM_HPP
#define AZUIK_IO_STREAM_HPP
namespace azuik
{
    namespace io
    {
        template <class S>
        struct front_inserter {
        public:
            explicit front_inserter(S& out)
                : m_out{&out}
            {}
            template <class... Args>
            S& operator()(Args&&... args)
            {
                (*m_out).emplace_front(static_cast<Args&&>(args)...);
                return *m_out;
            }

        private:
            S* m_out;
        };

        template <class S>
        struct back_inserter {
        public:
            explicit back_inserter(S& out)
                : m_out{&out}
            {}
            template <class... Args>
            S& operator()(Args&&... args)
            {
                (*m_out).emplace_back(static_cast<Args&&>(args)...);
                return *m_out;
            }

        private:
            S* m_out;
        };

        template <class Stream>
        struct stream_inserter {
        public:
            explicit stream_inserter(Stream& s)
                : m_stream{&s}
            {}
            template <class... Args>
            Stream& operator()(Args&&... args)
            {
                ((*m_stream) << ... << args);
                return *m_stream;
            }

        private:
            Stream* m_stream;
        };

    } // namespace io
} // namespace azuik
#endif
