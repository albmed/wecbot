#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

// To compile: g++ -o testMySqlVersion testMySqlVersion.c `mysql_config --cflags --libs`
// Tested on cygwin

int main(int argc, char **argv)
{
	printf("MySQL client version: %s\n", mysql_get_client_info());
	exit(0);
}
