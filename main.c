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
 **/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <argp.h>
#include <errno.h>
#include <fcntl.h>
#include <mysql/mysql.h>

const char *argp_program_version = "\ndb2fs 0.0.1 Copyright 2015 zachwick\n\n\
zach@zachwick.com (Include db2fs somewhere in the subject)\n\
This program is free software; you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation; either version 3 of the License, or\n\
(at your option) any later version.\n\n\
This program is distributed with the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n\
GNU General Public License for more details.\n\n\
You should have received a copy of the GNU General Public License\n\
along with this program; if not, you can download a copy at\n\
https://www.gnu.org/licenses/gpl-3.0.txt\n";
const char *arpg_program_bug_address = "<zach@zachwick.com>";
static char doc[] = "Use your mysql db like a file system.";
static char args_doc[] = "db2fs -h [HOST] -u [USER] -p [PASSWORD] -d [DBNAME] -m [MOUNTPOINT]";

static struct argp_option options[] = {
	{"host",     'h', "HOST",   0, "MySQL host"},
	{"user",     'u', "USER",   0, "MySQL username"},
	{"password", 'p', "PASSWD", 0, "MySQL password"},
	{"dbname",   'b', "DBNAME", 0, "database name"},
	{ 0 }
};

struct arguments
{
	char *args[5];
	char *host;
	char *user;
	char *passwd;
	char *dbname;
};

static error_t
parse_opt(int key, char* arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;
	
	switch (key) {
	case 'h':
		arguments->host = arg;
		break;
	case 'u':
		arguments->user = arg;
		break;
	case 'p':
		arguments->passwd = arg;
		break;
	case 'b':
		arguments->dbname = arg;
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= 6)
			{
				printf("Too many arguments used\n");
				// Too many arguments used
				argp_usage(state);
			}
		arguments->args[state->arg_num] = arg;
		break;
		/*
	case ARGP_KEY_END:
		if (state->arg_num < 5)
			{
				printf("Too few arguments used: %d of 5\n",state->arg_num);
				// Too few arguments used
				argp_usage(state);
			}
		break;
		*/
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

static int
db2fs_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	return res;
}

static int
db2fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
              off_t offset, struct fuse_file_info *fi)
{
	return 0;
}

static int
db2fs_open(const char *path, struct fuse_file_info *fi)
{
	return 0;
}

static int
db2fs_read(const char *path, char *buf, size_t size, off_t offset,
           struct fuse_file_info *fi)
{
	return 0;
}

static struct fuse_operations db2fs_oper = {
	.getattr	= db2fs_getattr,
	.readdir	= db2fs_readdir,
	.open		= db2fs_open,
	.read		= db2fs_read,
};
	
int
mysql_sql_exec(MYSQL *mysql, const char *query)
{
	return mysql_real_query(mysql, query, strlen(query));
}

int
main(int argc, char **argv)
{
	// CLI Argument parsing default values
	struct arguments arguments;
	arguments.host   = "localhost";
	arguments.user   = "user";
	arguments.passwd = "passwd";
	arguments.dbname = "dbname";

	int fuse_argc = 2;
	char *fuse_argv[] = { argv[0], argv[1] };
	
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;

	unsigned int num_fields;
	unsigned int i;
	
	// Initialize the connection
	if (mysql_init(&mysql) == NULL)
		{
			printf("\nFailed to initialize MySQL connection");
			exit(1);
		}
	
	// Log into mysql server
	// mysql_real_connect( MYSQL mysql, char* host, char* user, char* db, , ,)
	if (!mysql_real_connect(&mysql, arguments.host, arguments.user,
	                        arguments.passwd, arguments.dbname, 0, NULL, 0))
		{
			printf("\nFailed to connect to the MySQL server: Error: %s\n",mysql_error(&mysql));
			exit(1);
		}

	// List DB Tables
	result = mysql_list_tables(&mysql, NULL);
	if (result)
		{
			num_fields = mysql_num_fields(result);
			while ((row = mysql_fetch_row(result)))
				{
					for (i = 0; i < num_fields; i++)
						{
							printf("%s\n",row[i]);
						}
				}
			mysql_free_result(result);
		}

	// Close the connection to MySQL
	mysql_close(&mysql);

	//return fuse_main(argc - 2, argv + 2, &db2fs_oper, NULL);
	return fuse_main(fuse_argc, fuse_argv, &db2fs_oper, NULL);
}
