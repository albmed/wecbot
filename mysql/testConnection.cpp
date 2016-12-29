#include <mysql++.h>

#include <iostream>
#include <iomanip>

using namespace std;

// Tested on cygwin 
// To compile (cygwin): 
//		g++ -o testConnection testConnection.cpp -I /usr/local/mysql++/include/mysql++ \
//	 	-L /usr/local/mysql++/lib -lmysqlpp $(/usr/local/mysql/bin/mysql_config --cflags --libs)

// Drawbacks (cygwin specific): 
// 	It's not able to locate cygmysqlpp-3.dll (don't know why). A workaround could be copy that file (under $MYSQL++_LIB) and 
//	copy it where is located executable file. This should not happen on a true Unix-like system 

int main(int argc, char *argv[]) {
	// Connect to the sample database.
	mysqlpp::Connection conn(false);
	if (conn.connect("ranking_old", "127.0.0.1", 
			"root", "Passwd")) {
		// Retrieve a subset of the sample stock table set up by resetdb
		// and display it.
		mysqlpp::Query query = conn.query("select username from player_login");
		if (mysqlpp::StoreQueryResult res = query.store()) {
			cout << "We have:" << endl;
			mysqlpp::StoreQueryResult::const_iterator it;
			for (it = res.begin(); it != res.end(); ++it) {
				mysqlpp::Row row = *it;
				cout << '\t' << row[0] << endl;
			}
		}
		else {
			cerr << "Failed to get item list: " << query.error() << endl;
			return 1;
		}
	
		return 0;
	}
	else {
			cerr << "DB connection failed: " << conn.error() << endl;
			return 1;
	}
}
