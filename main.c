/**
 * db2fs - database to file system bridge
 *
 * Copyright 2015 zachwick <zach@zachwick.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Compile with (or with GNU Make):
 * gcc main.c -o db2fs -I/usr/local/include -L/usr/local/lib/mysql -lmysqlclient
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int
main(int argc, char **argv)
{	
	MYSQL mysql;

	// Initialize the connection
	if (mysql_init(&mysql) == NULL)
		{
			printf("\nFailed to initialize MySQL connection");
			exit(1);
		}
	else
		{
			printf("\nInitialized MySQL connection");	
		}
	
	// Log into mysql server
	// mysql_real_connect( MYSQL mysql, char* host, char* user, char* db, , ,)
	if (!mysql_real_connect(&mysql, "localhost","bareo","","bareo",0,NULL,0))
		{
			printf("\nFailed to connect to the MySQL server: Error: %s\n",mysql_error(&mysql));
			exit(1);
		}
	else
		{
			printf("\nLogged into the MySQL server successfully");
		}
	
	// Close the connection to MySQL
	mysql_close(&mysql);
}
