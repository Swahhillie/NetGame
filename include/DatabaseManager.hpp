#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "sqlite3.h"
#include <string>

class DatabaseManager
{
	public:
		DatabaseManager();
		virtual ~DatabaseManager();
        /** \brief Attempt a log in
         *
         * \param username of the player to log in
         * \param pw of the player to log in
         * \param if failed, reason why it has failed
         * \return true is successfull, false if failed,
         *
         */

		bool login(const std::string & name, const std::string & pw, std::string & rejectionReason);
		bool registerUser(const std::string & name, const std::string & pw, std::string & rejectionReason);

	protected:
	private:

		sqlite3 * db;

	private:
		void open();
		void close();
		sqlite3_stmt * queryForUsername(const std::string & username);
		sqlite3_stmt * queryAddGameResult(const std::string & winnerName, int score);
		sqlite3_stmt * queryAddUsername(const std::string & username, const std::string & password);
		sqlite3_stmt * queryUsernameAndPassword(const std::string & name, const std::string & password);
		sqlite3_stmt * makeQuery(const std::string & queury);

		void doStatementAndPrintResult(sqlite3_stmt * statement);
		void doStatement(sqlite3_stmt * statement);
};

#endif // DATABASEMANAGER_H
