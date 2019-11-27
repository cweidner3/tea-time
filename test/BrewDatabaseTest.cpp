/**
 * \file BrewDatabaseTest.cpp
 */

#define BOOST_TEST_MODULE BrewDatabase Test
#define BOOST_TEST_NO_MAIN	// So we can define a custom entry point
#include <boost/test/unit_test.hpp>

#include "BrewDatabase.h"
#include "BrewItem.h"

#include <iostream>

#include <QDebug>

#include <sstream>
#include <string>

#undef BOOST_CHECK_NO_THROW_IMPL
#define BOOST_CHECK_NO_THROW_IMPL( S, TL )                                      \
    do {                                                                        \
        try {                                                                   \
            S;                                                                  \
            BOOST_TEST_TOOL_IMPL( 2, true, "no exceptions thrown by " BOOST_STRINGIZE( S ), \
                                  TL, CHECK_MSG, _ );                           \
        } catch( std::exception &e ) {                                          \
            std::stringstream stream;                                           \
            stream << "unexpected exception thrown by ";                        \
            stream << BOOST_STRINGIZE( S );                                     \
            stream << ": " << e.what();                                         \
            BOOST_TEST_TOOL_IMPL( 2, false, stream.str(),                       \
                                  TL, CHECK_MSG, _ );                           \
        }                                                                       \
    } while( ::boost::test_tools::tt_detail::dummy_cond() )

#define BOOST_WARN_NO_THROW( S )            BOOST_CHECK_NO_THROW_IMPL( S, WARN )
#define BOOST_CHECK_NO_THROW( S )           BOOST_CHECK_NO_THROW_IMPL( S, CHECK )
#define BOOST_REQUIRE_NO_THROW( S )         BOOST_CHECK_NO_THROW_IMPL( S, REQUIRE )

namespace fs = std::filesystem;
namespace but = boost::unit_test;

BOOST_AUTO_TEST_CASE(TestOpenDatabase)
{
    BrewDatabase bdb;
    qDebug() << ">> Test Open Database";
    qDebug() << ">>>> Opening";
    BOOST_REQUIRE_NO_THROW(bdb.openDatabase());
    qDebug() << ">>>> Closing";
    BOOST_REQUIRE_NO_THROW(bdb.closeDatabase());
}

BOOST_AUTO_TEST_CASE(TestAddABrew, * but::depends_on("TestOpenDatabase"))
{
    int count;
    qDebug() << ">> Test Add a Brew";
    BrewDatabase bdb;
    BrewItem item("Test Item", 10, 2, 90, 4);
    qDebug() << ">>>> Opening";
    BOOST_REQUIRE_NO_THROW(bdb.openDatabase());
    qDebug() << ">>>> Adding brew item";
    BOOST_REQUIRE_NO_THROW(bdb.addBrewToTable(item));
    qDebug() << ">>>> Checking table count";
    BOOST_REQUIRE_NO_THROW(count = bdb.getBrewsCount());
    BOOST_TEST(1 == count);
    qDebug() << ">>>> Clearing brew table";
    BOOST_REQUIRE_NO_THROW(bdb.clearBrewsTable());
    qDebug() << ">>>> Checking table count";
    BOOST_REQUIRE_NO_THROW(count = bdb.getBrewsCount());
    BOOST_TEST(0 == count);
}

BOOST_AUTO_TEST_CASE(TestAddManyBrews, * but::depends_on("TestAddABrew"))
{
    int count;
    qDebug() << ">> Test Add Many Brews";
    BrewDatabase bdb;
    QList<BrewItem> items = {
        BrewItem("Test Item 1", 10, 2, 90, 4),
        BrewItem("Test Item 2", 10, 2, 90, 4),
        BrewItem("Test Item 3", 10, 2, 90, 4),
        BrewItem("Test Item 4", 10, 2, 90, 4),
        BrewItem("Test Item 5", 10, 2, 90, 4),
    };
    qDebug() << ">>>> Adding brew item";
    BOOST_REQUIRE_NO_THROW(bdb.addBrewsToTable(items));
    qDebug() << ">>>> Checking table count";
    BOOST_REQUIRE_NO_THROW(count = bdb.getBrewsCount());
    BOOST_TEST(items.count() == count);
}

BOOST_AUTO_TEST_CASE(TestReadBrews, * but::depends_on("TestAddManyBrews"))
{
    int count;
    QList<BrewItem> items;
    qDebug() << ">> Test Read Brews";
    BrewDatabase bdb;
    qDebug() << ">>>> Checking table count";
    BOOST_REQUIRE_NO_THROW(count = bdb.getBrewsCount());
    qDebug() << ">>>> Count = " << count;
    qDebug() << ">>>> Reading brew table";
    BOOST_REQUIRE_NO_THROW(items = bdb.getBrewsFromTable());
    for (BrewItem item : items) {
        qDebug() << ">>>> " << item;
    }
}

/*----------------------------------------------------------------------------*/
/*
 * Test Setup
 */

struct db_path_s
{
        fs::path full_path; // Full path to the db file
        fs::path top_dir; // Folder in /tmp for us to use
        fs::path sub_dir; // A subdirectory to make sure we have to create folders
};

static struct db_path_s setup_test_db_path()
{
    struct db_path_s path_struct;
    path_struct.top_dir = fs::temp_directory_path() / "tea-time";
    path_struct.sub_dir = path_struct.top_dir / "tmp-db-path";
    path_struct.full_path = path_struct.sub_dir / "test.db";
    return path_struct;
}

int main(int argc, char *argv[], char *envp[])
{
    (void) envp;
    /* Setup the temporary database */
    struct db_path_s test_db_path = setup_test_db_path();
    BrewDatabase::setDatabasePath(test_db_path.full_path);
    /* Run tests */
    int status = boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
    /* Remove temp paths */
    fs::remove_all(test_db_path.top_dir);
    return status;
}
