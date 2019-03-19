#ifndef AZUIK_IO_SQLITE_HPP
#define AZUIK_IO_SQLITE_HPP

#include <stdexcept>
#include <string>
#include <sqlite3.h>
namespace azuik
{
    namespace sql
    {
        class exception : public std::runtime_error {
        public:
            explicit exception(int code);
            auto code() const noexcept -> int
            {
                return m_code;
            }

        private:
            int m_code;
        };

        struct error {
            int code;
            int extended_code;
            char const* message;
        };
        struct result {
            explicit result(sqlite3_stmt* stmt) noexcept;
            int size() const noexcept;
            template <class T>
            T value(int c) const noexcept;
            char const* name(int c) const noexcept;

        private:
            sqlite3_stmt* m_stmt;
        };

        struct result_set {
            struct sentinel {};
            struct const_iterator {
                const_iterator() noexcept;
                explicit const_iterator(result_set const* view) noexcept;
                auto operator++() noexcept -> const_iterator&;
                auto operator==(sentinel that) const noexcept -> bool;
                auto operator!=(sentinel that) const noexcept -> bool;
                auto operator*() const noexcept -> result;

            private:
                int m_status;
                result_set const* m_view;
            };
            using iterator = const_iterator;

        public:
            explicit result_set(sqlite3_stmt* stmt) noexcept;
            auto begin() const noexcept -> const_iterator;
            auto end() const noexcept -> sentinel;

        private:
            sqlite3_stmt* m_stmt;
        };

        class statement {
        public:
            using native_handle_type = sqlite3_stmt*;
            statement(statement const&) = delete;
            statement& operator=(statement const&) = delete;

        public:
            explicit statement(native_handle_type handle) noexcept
                : m_handle{handle}
            {}
            ~statement();
            native_handle_type native_handle() noexcept
            {
                return m_handle;
            }
            template <class T>
            void bind(char const* name, T const& value);
            void clear();
            void reset();
            auto exec_update() -> int;
            auto exec_query() -> result_set;

        private:
            native_handle_type m_handle;
        };

        using index_type = sqlite3_int64;

        class database {
        public:
            using native_handle_type = sqlite3*;

        public:
            database();
            explicit database(char const* file_name);
            ~database();
            database(database const&) = delete;
            void operator=(database&) = delete;
            void open(char const* file_name);
            void close();
            native_handle_type native_handle() noexcept
            {
                return m_handle;
            }
            int exec_update(char const* q);
            result_set exec_query(char const* q);
            statement prepare(char const* query);
            auto error_code() const noexcept -> int;
            auto extended_error_code() const noexcept -> int;
            auto error_message() const noexcept -> char const*;
            auto get_error() const noexcept -> error;
            auto last_row_id() const noexcept -> index_type;
            auto total_changes() const noexcept -> int;
            static auto version() noexcept -> char const*;

        private:
            native_handle_type m_handle;
        };

        class transaction {
        public:
            explicit transaction(database& db);
            void commit();
            ~transaction();

        private:
            database& m_db;
            bool m_committed;
        };

        enum class column_type { INTEGER, FLOAT, TEXT, BLOB, Null };

        class column {
            template <class T>
            T get();
        };
    } // namespace sql
} // namespace azuik
#endif
