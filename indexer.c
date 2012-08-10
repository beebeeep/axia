#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#include <db.h>

#include "misc.h"



DB* ax_prepare_db(char *db_name) {
	DB *dbp;

	u_int32_t flags;
	int ret;

	ret = db_create(&dbp, NULL, 0);
	if(ret != 0) {
		ax_log("Cannot create DB handler!\n");
		return NULL;
	}

	flags = DB_CREATE;

	ret = dbp->open(dbp, NULL, db_name, NULL, DB_HASH, flags, 0);
	if (ret != 0) {
		ax_log("Cannot create DB!\n");
		return NULL;
	}

	if(dbp != NULL) {
		dbp->close(dbp, 0);
	}
	
	return dbp;
}

int ax_build_index(char *path, char *db_name) 
{

	return NULL;
}
