#ifndef SERVER_SYS_SSTRING_H_
#define SERVER_SYS_SSTRING_H_

#include <cstdlib>
#include <typeinfo>
#include <sstream>

#include <boost/format.hpp>

const char * const WHITESPACE = " \f\n\r\t\v";

inline boost::format format(const std::string &f_string) {
  boost::format fmter(f_string);
#if 0 // throw format exceptions
  fmter.exceptions(boost::io::all_error_bits);
#else
  fmter.exceptions(boost::io::no_error_bits);
#endif
  return fmter;
}

class sstring : public std::string {
public:
  // constructors
  sstring() : std::string(){}
  sstring(const char *str) : std::string(str?str:"") {}
  sstring(const std::string &str) : std::string(str) {}
  sstring(boost::format &a) : std::string(a.str()) {}

  const sstring & operator=(const boost::format &a) { assign(a.str()); return *this; }

  const sstring & operator+=(const boost::format &a) { append(a.str()); return *this; }
  const sstring & operator+=(const char &a) { push_back(a); return *this; }
  const sstring & operator+=(const char *a) { append(a); return *this; }
  const sstring & operator+=(const std::string &a) { append(a); return *this; }

  // these functions return modified copies of the string
  const sstring toCRLF() const;
  const sstring lower() const;
  const sstring upper() const;
  const sstring cap() const;
  const sstring uncap() const;
  const sstring word(int) const;
  const sstring comify() const;
  const sstring replaceString(sstring, sstring) const;
  const sstring trim() const;
  const sstring matchCase(const sstring match) const;

  // call this function with NULL data to get alloc size, then agian with alloc'd array
  int split(const char delimit, sstring *data) const;

  // other functions
  const bool hasDigit() const;
  const bool isNumber() const;
  const bool isWord() const;
  const bool startsVowel() const;

  // string mungers
  void convertStringColor(const sstring);
  void inlineReplaceString(const std::string, const std::string);
  void inlineTrimWhiteLines();
  void inlineRemoveBetween(const sstring, const sstring, bool, bool);
  void inlineReplaceMarkup(const sstring, const sstring, const sstring, const sstring);
  void inlinePad(const char pad, int num) { resize(length()+num, pad); }
};

extern bool isvowel(const char c);

// used for easily defining buffer sizes
#ifndef cElements
#define cElements(x) (sizeof(x)/sizeof(x[0]))
#endif

template<class T> T convertTo(const sstring &s) {
  T x;
  if (typeid(x) == typeid(int)) {
    return (T) strtol(s.c_str(), NULL, 10);
  } else if (typeid(x) == typeid(unsigned int)) {
    return (T) strtoll(s.c_str(), NULL, 10);
  } else if (typeid(x) == typeid(float)) {
    return (T) strtof(s.c_str(), NULL);
  } else if (typeid(x) == typeid(double)) {
    return (T) strtod(s.c_str(), NULL);
  } else {
    std::istringstream is(s);
    if (!(is >> x)) // let failure convert to 0 with no warning.  we relied on
      x=0;          // this (undefined) behavior with atoi, so we need it now
    return x;
  }
}

#endif

// vim: ft=cpp:tw=79:sw=2:sts=2:ts=8:et
