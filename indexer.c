#include <stdlib.h>
#include <stdio.h>

#include <db.h>


void ax_log(char *str) 
{
	fprintf(stderr, str);
}

int ax_build_index(char *path, char *db_name) 
{

	DB *dbp;

	u_int32_t flags;
	int ret;

	ret = db_create(&dbp, NULL, 0);
	if(ret != 0) {
		ax_log("Cannot create DB handler!\n");
		exit(-1);
	}

	flags = DB_CREATE;

	ret = dbp->open(dbp, NULL, db_name, NULL, DB_HASH, flags, 0);
	if (ret != 0) {
		ax_log("Cannot create DB!\n");
		exit(-2);
	}

	if(dbp != NULL) {
		dbp->close(dbp, 0);
	}
	
	return 0;
}
