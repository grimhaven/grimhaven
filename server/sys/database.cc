#include <cstdarg>

#include "sys/database.h"
#include "misc/enum.h"
#include "misc/structs.h"
#include "sys/configuration.h"
#include "sys/timing.h"
#include "misc/toggle.h"

// log queries that take longer than this many seconds - comment out to disable
#define LONG_QUERY 1.0

MYSQL * TDatabase::db;

TDatabase::TDatabase() :
  res(NULL),
  row(NULL)
{
  row_count = 0;
  row_columns = 0;

  if (db) {
    if (!mysql_ping(db))
      return;
    vlogf(LOG_DB, format("mysql_ping failure: %s") % mysql_error(db));
    mysql_close(db);
  }
  db = mysql_init(db);
  vlogf(LOG_DB, "Connecting to database.");
  my_bool reconnect = 1;
  mysql_options(db, MYSQL_OPT_RECONNECT, &reconnect);
  if (!mysql_real_connect(db, Config::DbHost().c_str(), Config::DbUser().c_str(), NULL, Config::DbName().c_str(), 0, NULL, 0))
    vlogf(LOG_DB, format("Could not connect to database: %s") % mysql_error(db));
}

TDatabase::~TDatabase()
{
  if (res)
    mysql_free_result(res);
}

// execute a query
bool TDatabase::query(const char *query, ...)
{
  va_list ap;
  MYSQL_RES *restmp;
  TTiming t;

  t.start();

  // no db set??
  if (!db)
    return FALSE;

  va_start(ap, query);
  sstring buf = escapeQuery(query, ap);
  va_end(ap);

  if (mysql_query(db, buf.c_str())) {
    vlogf(LOG_DB, format("query failed: %s") % mysql_error(db));
    vlogf(LOG_DB, format("%s") % query);
    return FALSE;
  }

  // if there is supposed to be some results from this query,
  // free the previous results (if any) and assign the new results
  // if there aren't supposed to be results (update, insert, delete, etc)
  // then don't do anything with the results, they might still be used
  if ((restmp=mysql_store_result(db))) {
    if (res)
      mysql_free_result(res);
    res = restmp;
  }

  // capture rowcount here, because the db pointer state changes when
  // db timing is on
  row_count = (long) mysql_affected_rows(db);

  if (restmp)
    mapColumns();
  // store the column names and offsets
  t.end();

#ifdef LONG_QUERY
  if (t.getElapsed() > LONG_QUERY) {
    vlogf(LOG_DB, format("Query took %f seconds.") % t.getElapsed());
    vlogf(LOG_DB, format("%s") % query);
  }
#endif

  // this saves the queries (without args) and the execution time
  // it slows things down pretty significantly though
  if (toggleInfo.isLoaded() && toggleInfo[TOG_DBTIMING]->toggle) {
    mysql_query(db, escapeQuery("insert into querytimes (query, secs) values ('%s', %f)",
                                query, t.getElapsed()).c_str());
  }

  return TRUE;
}

void TDatabase::mapColumns(void)
{
  column_map.clear();
  row_columns = mysql_num_fields(res);
  MYSQL_FIELD *fields=mysql_fetch_fields(res);

  for (unsigned int i=0; i<row_columns; i++)
    column_map[fields[i].name] = i;
}

const sstring TDatabase::escapeQuery(const char *query, ...) const
{
  va_list ap;
  va_start(ap, query);
  sstring ret = escapeQuery(query, ap);
  va_end(ap);
  return ret;
}

const sstring TDatabase::escapeQuery(const char *query, va_list ap) const
{
  sstring buf="";
  const char *cur=query, *next, *arg;
  char fmt;

  while ((next=strchr(cur, '%'))) {
    buf.append(cur, next - cur);
    fmt = *(++next);
    cur = ++next;
    switch (fmt){
      case 'r':
      case 's':
        arg = va_arg(ap, char *);
        if (!arg){
          vlogf(LOG_DB, format("query - null argument for format specifier '%c'") % fmt);
          vlogf(LOG_DB, format("%s") % query);
        }
        if (fmt == 's')
          buf += escape(arg);
        else
          buf += arg;
        break;
      case 'i':
        buf += format("%i") % va_arg(ap, int);
        break;
      case 'f':
        buf += format("%f") % va_arg(ap, double);
        break;
      case '%':
        buf += "%";
        break;
      default:
        vlogf(LOG_DB, format("query - bad format specifier '%c'") % fmt);
        vlogf(LOG_DB, format("%s") % query);
    }
  }
  buf += cur;

  return buf;
}

const sstring TDatabase::escape(const sstring &str) const
{
    if (!db)
        return NULL;
    int len = str.length();
    char buf[len*2+1];
    mysql_real_escape_string(db, buf, str.c_str(), len);
    return buf;
}
