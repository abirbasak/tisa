#include <azuik/io/sqlite.hpp>
#include <iostream>
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    using namespace azuik;
    try
    {
        sql::database db{"test.db"};
        std::cout << "database created" << std::endl;
        int c = db.exec_update(R"(DROP TABLE IF EXISTS person)");
        c = db.exec_update(R"(CREATE TABLE IF NOT EXISTS person(
                               id INTEGER PRIMARY KEY AUTOINCREMENT,
                               name TEXT NOT NULL,
                               age INTEGER);)");

        std::cout << "CREATE TABLE " << c << std::endl;

        c = db.exec_update(R"(INSERT INTO person (name,age) VALUES
                           ('Justin',23),('Richard',29),('Jecob',27),('Lisa',25))");
        std::cout << "INSERT INTO " << c << std::endl;

        sql::statement stmt = db.prepare("SELECT * FROM person where id = :id");
        stmt.bind(":id", 2);
        for (auto const& r : stmt.exec_query())
        {
            std::cout << r.name(0) << " = " << r.value<int>(0) << std::endl;
            std::cout << r.name(1) << " = " << r.value<char const*>(1) << std::endl;
        }

        /*
         * this code desn't work as count_if doesn't take iterator/sentinel pair yet.
        auto r = stmt.exec_query();
        std::count_if(r.begin(), r.end(), [](sql::result const& r) -> bool { return true; });
        for (auto const& r : db.exec_query("SELECT * FROM person"))
        {
            std::cout << r.name(0) << " = " << r.value<int>(0) << std::endl;
            std::cout << r.name(1) << " = " << r.value<char const*>(1) << std::endl;
            std::cout << r.name(2) << " = " << r.value<int>(2) << std::endl;
        }
        */
    }
    catch (sql::exception& ex)
    {
        std::cout << "msg: " << ex.what() << " ,code : " << ex.code() << std::endl;
    }
}
