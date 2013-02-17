#ifndef SERVER_SYS_DATABASE_H_
#define SERVER_SYS_DATABASE_H_

#include <mysql/mysql.h>
#include <queue>
#include <map>
#include <cstdarg>

#include "util/sstring.h"
#include "core/logging.h"


// TDatabase is a class for interacting with the sql database.
//
// You should always use local instances of TDatabase, do not use a pointer
// and the new operator.  The local instance will clean up after itself in
// its destructor when it goes out of scope.  All of the functions are safe
// to use even on failures, so if you do not do any error checking, the worst
// than can happen is that you won't get any results.  On error, TDatabase
// will send LOG_DB vlogf's and return the appropriate value (false or NULL).
//
// Usage example:
//
// #include "sys/database.h"
//
// float weight=5.5;
// char name[]="blade";
// int vnum=10000;
//
// TDatabase db();
// db.query("select vnum, price, short_desc from obj where weight<%f and
// name like '%%%s%%' and vnum>%i", weight, name, vnum);
//
// while(db.fetchRow()){
//   if(atoi(db["price"]) > 10000){
//     vlogf(LOG_BUG, fmt("item %s had value of %s") %  db["vnum"] % db["price"]);
//   }
//   sendTo("%s %s", db["vnum"], db["short_desc"]);
// }
//
//
// Documentation:
//
// bool query(const char*,...) - This function sends a query to the database.
// It takes a printf style format sstring as the arguments.  The allowed
// specifiers are %s (char *), %i (int), %f (double) and %% (to print a %).
// The arguments that are passed are escaped for the query.  If the query
// does not expect results (insert, update, delete, etc) then the results are
// left as is.  You can do a select, then do an insert/update/delete and still
// access the select's results.
// Returns: TRUE if query was sent successfully, FALSE if there was an error
// Ex:
// float weight=5.5;
// char name[]="blade";
// int vnum=10000;
// db.query("select vnum, short_desc from obj where weight<%f and
// name like '%%%s%%' and vnum>%i", weight, name, vnum);
//
// bool fetchRow() - Makes the next row of results available via getColumn.
// Returns: FALSE if no results or no more rows available.
// Ex:
// while(db.fetchRow(){
//   printf("%s", db["vnum"]);
// }
//
// char *operator[](const sstring &) - returns the data associated with
// the specified column (by name)
// Ex:
// db.query("select vnum, short_desc from obj");
// db.fetchRow();
// sstring short_desc = db["short_desc"];
//
// bool isResults() - checks if there are results available
// Returns: TRUE if results are there, FALSE if not
//
// long rowCount() - added to return affected or retrieved row counts
// This should include affected counts for inserts, updates and deletes
// as well as standard result set sizes for select statements.
// Result of -1 means the query returned an error.
// Although, the docs claim that the my_ulonglong datatype is unsigned so who knows?
//
// const sstring escape(const sstring &) - escapes string for use in SQL
// Uses mysql_real_escape_string (mysql_escape_string is deprecated)

extern std::queue<sstring> queryqueue;

const sstring EMPTY("");

class TDatabase
{
 private:
  MYSQL_RES *res;
  MYSQL_ROW row;
  static MYSQL *db;
  long row_count;
  unsigned long row_columns;
  std::map<sstring, int> column_map;
  const sstring escapeQuery(const char *, va_list) const;
  const sstring escapeQuery(const char *, ...) const;
  void mapColumns(void);

 public:
  bool query(const char *, ...);
  const sstring escape(const sstring &) const;
  long rowCount() { return row_count; }
  bool isResults() { return res && mysql_num_rows(res); }
  long lastInsertId() { return db ? mysql_insert_id(db) : 0; }
  bool fetchRow() { return res && (row=mysql_fetch_row(res)); }
  const sstring operator[] (const unsigned int i) const
  {
    if (!(res && row))
      return NULL;

    if (i >= row_columns) {
      vlogf(LOG_DB, format("TDatabase::operator[%i] - invalid column index") %  i);
      return EMPTY;
    }
    return row[i];
  }
  const sstring operator[] (const sstring &s) const
  {
    if (!(res && row))
      return NULL;

    if (!column_map.count(s)) {
      vlogf(LOG_DB, format("TDatabase::operator[\"%s\"] - invalid column name") %  s);
      return EMPTY;
    }
    // use at() instead of [] - latter precludes function const status
    return row[column_map.at(s)];
  }

  TDatabase();
  ~TDatabase();
};

#endif
