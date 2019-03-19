#include <azuik/io/sqlite.hpp>
#include <sqlite3.h>
namespace azuik::sql
{
    struct checked_type {
        explicit constexpr checked_type(int rc = SQLITE_OK) noexcept
            : rc{rc}
        {}
        int rc;
    };
    inline constexpr checked_type checked{};

    int operator/(checked_type c, int rc)
    {
        if (rc != c.rc)
        {
            throw exception{rc};
        }
        return rc;
    }

    statement::~statement()
    {
        ::sqlite3_finalize(m_handle);
    }

#define AZUIK_SQL_BIND_SPLZ(TYPE, TYPE2)                                                           \
    template <>                                                                                    \
    void statement::bind<TYPE>(char const* name, TYPE const& value)                                \
    {                                                                                              \
        int index = ::sqlite3_bind_parameter_index(m_handle, name);                                \
        checked / ::sqlite3_bind_##TYPE2(m_handle, index, value);                                  \
    }

#define AZUIK_SQL_BIND_SPLZ2(TYPE, TYPE2)                                                          \
    template <>                                                                                    \
    void statement::bind<TYPE>(char const* name, TYPE const& value)                                \
    {                                                                                              \
        int index = ::sqlite3_bind_parameter_index(m_handle, name);                                \
        checked / ::sqlite3_bind_##TYPE2(m_handle, index, value, -1, SQLITE_TRANSIENT);            \
    }

    AZUIK_SQL_BIND_SPLZ(double, double);
    AZUIK_SQL_BIND_SPLZ(int8_t, int);
    AZUIK_SQL_BIND_SPLZ(int16_t, int);
    AZUIK_SQL_BIND_SPLZ(int32_t, int);
    AZUIK_SQL_BIND_SPLZ(int64_t, int64);
    AZUIK_SQL_BIND_SPLZ2(char const*, text);

    void statement::clear()
    {
        checked / ::sqlite3_clear_bindings(m_handle);
    }
    void statement::reset()
    {
        checked / ::sqlite3_reset(m_handle);
    }
    result::result(sqlite3_stmt* stmt) noexcept
        : m_stmt{stmt}
    {}
    auto result::size() const noexcept -> int
    {
        return ::sqlite3_column_count(m_stmt);
    }
    auto result::name(int c) const noexcept -> char const*
    {
        return ::sqlite3_column_name(m_stmt, c);
    }

#define AZUIK_SQL_RESULT_SPLZ(TYPE, TYPE2)                                                         \
    template <>                                                                                    \
    auto result::value<TYPE>(int c) const noexcept->TYPE                                           \
    {                                                                                              \
        return (TYPE)(::sqlite3_column_##TYPE2(m_stmt, c));                                        \
    }

    AZUIK_SQL_RESULT_SPLZ(int8_t, int);
    AZUIK_SQL_RESULT_SPLZ(int16_t, int);
    AZUIK_SQL_RESULT_SPLZ(int32_t, int);
    AZUIK_SQL_RESULT_SPLZ(int64_t, int64);
    AZUIK_SQL_RESULT_SPLZ(double, double);
    AZUIK_SQL_RESULT_SPLZ(char const*, text);

    result_set::result_set(sqlite3_stmt* stmt) noexcept
        : m_stmt{stmt}
    {}
    auto result_set::begin() const noexcept -> const_iterator
    {
        return const_iterator{this};
    }
    auto result_set::end() const noexcept -> sentinel
    {
        return sentinel{};
    }
    result_set statement::exec_query()
    {
        return result_set{m_handle};
    }
    result_set::const_iterator::const_iterator() noexcept
        : m_view{nullptr}
        , m_status(SQLITE_DONE)
    {}
    result_set::const_iterator::const_iterator(result_set const* view) noexcept
        : m_view{view}
        , m_status{::sqlite3_step(view->m_stmt)}
    {}

    auto result_set::const_iterator::operator++() noexcept -> const_iterator&
    {
        assert(m_view != nullptr && "incrementing singular iterator");
        assert(m_status != SQLITE_DONE && "incrementing end iterator");
        m_status = ::sqlite3_step(m_view->m_stmt);
        return *this;
    }
    auto result_set::const_iterator::operator*() const noexcept -> result
    {
        assert(m_view != nullptr && "dereferencing singular iterator");
        assert(m_status != SQLITE_DONE && "dereferencing end iterator");
        return result{m_view->m_stmt};
    }
    auto result_set::const_iterator::operator==(sentinel) const noexcept -> bool
    {
        return m_status == SQLITE_DONE;
    }
    auto result_set::const_iterator::operator!=(sentinel) const noexcept -> bool
    {
        return m_status != SQLITE_DONE;
    }
    exception::exception(int code)
        : runtime_error(::sqlite3_errstr(code))
        , m_code(code)
    {}
    database::database()
        : m_handle(nullptr)
    {}
    database::~database()
    {
        close();
    }

    database::database(char const* file_name)
        : m_handle{nullptr}
    {
        open(file_name);
    }

    void database::open(char const* file_name)
    {
        int rc = ::sqlite3_open(file_name, &m_handle);
        if (rc != SQLITE_OK)
        {
            ::sqlite3_close(m_handle);
            throw exception{rc};
        }
    }
    void database::close()
    {
        if (m_handle)
        {
            ::sqlite3_close(m_handle);
            m_handle = nullptr;
        }
    }
    int database::exec_update(char const* q)
    {
        checked / ::sqlite3_exec(m_handle, q, nullptr, nullptr, nullptr);
        return ::sqlite3_changes(m_handle);
    }
    /*
    result_set database::exec_query(char const* q) {
        checked / ::sqlite3_exec(m_handle,q,)
    }*/

    statement database::prepare(char const* q)
    {
        sqlite3_stmt* stmt;
        checked / ::sqlite3_prepare_v2(m_handle, q, -1, &stmt, nullptr);
        return statement{stmt};
    }
    auto database::error_code() const noexcept -> int
    {
        return ::sqlite3_errcode(m_handle);
    }
    auto database::extended_error_code() const noexcept -> int
    {
        return ::sqlite3_extended_errcode(m_handle);
    }
    auto database::error_message() const noexcept -> char const*
    {
        return ::sqlite3_errmsg(m_handle);
    }
    auto database::get_error() const noexcept -> error
    {
        return {error_code(), extended_error_code(), error_message()};
    }
    auto database::last_row_id() const noexcept -> index_type
    {
        return ::sqlite3_last_insert_rowid(m_handle);
    }
    auto database::total_changes() const noexcept -> int
    {
        return ::sqlite3_total_changes(m_handle);
    }
    auto database::version() noexcept -> char const*
    {
        return ::sqlite3_libversion();
    }

    struct executor {
        static auto exec(sqlite3* db, char const* q) -> void
        {
            // db.exec("");
        }
    };
    transaction::transaction(database& db)
        : m_db{db}
        , m_committed{false}
    {
        executor::exec(db.native_handle(), "BEGIN");
    }
    transaction::~transaction()
    {
        if (!m_committed)
        {
            executor::exec(m_db.native_handle(), "ROLLBACK");
        }
    }

    auto transaction::commit() -> void
    {
        if (!m_committed)
        {
            executor::exec(m_db.native_handle(), "COMMIT");
            m_committed = true;
        }
    }
    template <>
    int column::get<int>()
    {
        return 0;
    }
} // namespace azuik::sql
