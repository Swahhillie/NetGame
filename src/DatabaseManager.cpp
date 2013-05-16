#include "DatabaseManager.hpp"
#include "ConfigLoader.hpp"
#include "sqlite3.h"
#include <cstring>
#include <assert.h>
#include <sstream>
#include "StringOperations.hpp"

DatabaseManager::DatabaseManager():db(NULL)
{
    //ctor
    open();
    //sqlite3_stmt * statement = makeQuery(std::string("select * from main.users;"));

    //doStatementAndPrintResult(statement);

    //statement = queryForUsername("David");

    //doStatementAndPrintResult(statement);

//    sqlite3_stmt * statement = queryAddGameResult("koekje", 1337);
//    doStatement(statement);


}

DatabaseManager::~DatabaseManager()
{
    //dtor
    close();
}

bool DatabaseManager::login(const std::string & name, const std::string & password, std::string & reason)
{
	sqlite3_stmt * statement = queryUsernameAndPassword(name, password);

	int rowCount = 0;
	while(sqlite3_step(statement) == SQLITE_ROW)
	{

		rowCount ++;

	}
	if(rowCount == 1)
		return true;
	else{
		assert(rowCount == 0);
		reason = "Invalid username / password ";
		return false;

	}


}
bool DatabaseManager::registerUser(const std::string & name, const std::string & password, std::string & reason)
{
	sqlite3_stmt * statement = queryForUsername(name);

	int rowCount = 0;
	while(sqlite3_step(statement) == SQLITE_ROW)
	{

		rowCount ++;

	}
	if(rowCount == 0){
		sqlite3_stmt * addStatement = queryAddUsername(name, password);
		doStatement(addStatement);
		reason = "Registration successful, you can login now";
		return true;
	}

	else{
		assert(rowCount == 1);
		reason = "Username is taken";
		return false;

	}
}
void DatabaseManager::open()
{
    ConfigLoader & config = ConfigLoader::instance();
    std::string dbPath = config.getString("database,path");
    int result = sqlite3_open(dbPath.c_str(), &db);
    assert(result == SQLITE_OK);
    std::cout << "Database opened " << std::endl;

}
void DatabaseManager::close()
{

}
void DatabaseManager::doStatementAndPrintResult(sqlite3_stmt * statement)
{
    std::stringstream stream;
    stream << "Prepare statement" << std::endl;
    int columns = sqlite3_column_count( statement );
    stream << "Getting columns : " << columns << std::endl;

    while ( sqlite3_step( statement ) == SQLITE_ROW )   // print every row
    {
        stream << "Row : " ;
        for ( int c = 0; c < columns; c++ )   // print every column of this row
        {
            const char * cell = (const char *)sqlite3_column_text( statement, c );
            stream << cell << " ";
        }
        stream << std::endl;
    }
    std::cout << stream.str();
}
void DatabaseManager::doStatement(sqlite3_stmt * statement)
{
    while ( sqlite3_step( statement ) == SQLITE_ROW )
    {

    }
}
sqlite3_stmt * DatabaseManager::queryForUsername(const std::string & name)
{
    //std::string printf()
    std::string usernameQuery = ConfigLoader::instance().getString("database,usernameQuerySelect");
    StringOperations::myReplace(usernameQuery, "USERNAME", name);
    return makeQuery(usernameQuery);
}
sqlite3_stmt * DatabaseManager::queryUsernameAndPassword(const std::string & name, const std::string & password)
{
	std::string uAndPQuery = ConfigLoader::instance().getString("database,usernameAndPasswordQuery");
	StringOperations::myReplace(uAndPQuery, "USERNAME", name);
	StringOperations::myReplace(uAndPQuery, "PASSWORD", password);
	return makeQuery(uAndPQuery);
}
sqlite3_stmt * DatabaseManager::queryAddGameResult(const std::string & winnerName, int score)
{
    std::string scoreString = "";
    std::stringstream stream;
    stream << score;
    stream >> scoreString;

    std::string insertToGamesQuery = ConfigLoader::instance().getString("database,insertGame");
    StringOperations::myReplace(insertToGamesQuery, "USERNAME", winnerName);
    StringOperations::myReplace(insertToGamesQuery, "TOTALSCORE", scoreString);

    return makeQuery(insertToGamesQuery);


}
sqlite3_stmt * DatabaseManager::queryAddUsername(const std::string & name, const std::string & password)
{
    std::string usernameQuery = ConfigLoader::instance().getString("database,usernameQueryAdd");
    StringOperations::myReplace(usernameQuery, "USERNAME", name);
    StringOperations::myReplace(usernameQuery, "PASSWORD", password);
    std::cout << "making queury " << usernameQuery <<std::endl;
    return makeQuery(usernameQuery);
}
sqlite3_stmt * DatabaseManager::makeQuery(const std::string & queury)
{
    //prepare the statement
    std::cout << "Querying: " << queury << std::endl;
    const char * q = queury.c_str();
    sqlite3_stmt * statement = NULL;
    const char * errorMessage = NULL;
    int result = sqlite3_prepare_v2(db, q, strlen(q)+1, &statement, &errorMessage);
    if(result == SQLITE_OK)
    {
        return statement;
    }
    else
    {
        std::cout << "DB Error " << sqlite3_errmsg(db) << std::endl;
        assert(result == SQLITE_OK);
        return NULL;
    }


}
/*
std::cout << "Hello world!" << std::endl;

	sqlite3 * db;
	int result;
	result = sqlite3_open( "game_db", &db );

	std::cout << " open result " << result << " " << SQLITE_OK << std::endl;
	if ( result == SQLITE_OK ) {
		std::cout << "Opened game_db" << std::endl;
		const char * errorMessage = NULL;

		// do query
//		const char * query = "SELECT * FROM players";
		const char * query = "SELECT * FROM players WHERE username='Eelco'  ";
		sqlite3_stmt * statement;
		result = sqlite3_prepare_v2( db, query, strlen( query ) + 1, & statement, & errorMessage );

		if ( result == SQLITE_OK ) {
			std::cout << "Prepare statement" << std::endl;
			int columns = sqlite3_column_count( statement );
			std::cout << "Getting columns : " << columns << std::endl;

			while ( sqlite3_step( statement ) == SQLITE_ROW ) { // print every row
				std::cout << "Row : " ;
				for ( int c = 0; c < columns; c++ ) { // print every column of this row
					const char * cell = (const char *)sqlite3_column_text( statement, c );
					std::cout << cell << " ";
				}
				std::cout << std::endl;
			}
		}



//		result = sqlite3_exec( db, "SELECT * FROM players", show_table, NULL, &errorMessage);

	} else {
		std::cout << "Could not open game_db" << sqlite3_errmsg(db) << std::endl;

	}
	sqlite3_close( db );

*/
