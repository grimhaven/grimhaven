#include <cstring>

#include "util/sstring.h"

inline bool isvowel(const char c) {
  return strchr("AEIOUaeiou", c);
}

// puts commas every 3rd char, for formatting number strings
const sstring sstring::comify() const {
  sstring tString=*this;
  unsigned int  strCount, charIndex = 0;

  tString=format("%.0f") % convertTo<float>(*this);
  strCount = tString.length();
  tString="";

  for (; charIndex < strCount; charIndex++) {
    // put commas every 3rd char EXCEPT if next char is '-'
    // that is, want "123456" to become "123,456"
    // but don't want "-123" to become "-,123"
    if (!((strCount - charIndex) % 3) && charIndex != 0 &&
        !(charIndex == 1 && (*this)[0] == '-'))
      tString += ",";

    tString += (*this)[charIndex];
  }

  for (; charIndex != this->length(); charIndex++)
    tString += (*this)[charIndex];

  return tString;
}

// converts newlines in the string to CRLF if possible
// this is for preparation for sending out to a player
// for cross platform compatibility
const sstring sstring::toCRLF() const {
  sstring dosstr = "";
  unsigned int len;

  len = (*this).length();
  for (unsigned int loc=0; loc < len; ++loc){
    dosstr += (*this)[loc];
    if (loc>0 && (*this)[loc] == '\n' && (*this)[loc-1] != '\r' &&
      (loc+1) < len && (*this)[loc+1] != '\r') {
      dosstr += '\r';
    }
  }
  return dosstr;
}

// converts A-Z to lower case a-z
const sstring sstring::lower() const {
  sstring s=*this;

  std::transform(s.begin(), s.end(), s.begin(), ::tolower);

  return s;
}

// converts a-z to upper case A-Z
const sstring sstring::upper() const {
  sstring s=*this;

  std::transform(s.begin(), s.end(), s.begin(), ::toupper);

  return s;
}

// capitalizes first letter, skipping color codes
const sstring sstring::cap() const {
  int counter = 0;
  sstring s=*this;

  if (s.length() <= 0)
    return s;

  if(s[0] != '<'){
    s[0]=toupper(s[0]);
  } else {
    // Accounting for Items with color strings and % as first character
    for(sstring::size_type i=0;i<s.length();++i){
      if (s[i] == '<')
        counter = 0;
      else
        counter++;

      if (counter == 3) {
        s[i] = toupper(s[i]);
        return s;
      }
    }
  }

  return s;
}

// uncapitalizes first letter, skipping color codes
const sstring sstring::uncap() const {
  int counter = 0;
  sstring s=*this;

  if (s.length() <= 0)
    return s;

  if (s[0] != '<') {
    s[0] = tolower(s[0]);
  } else {
    // Accounting for Items with color sstrings and % as first character
    for(sstring::size_type i=0;i<s.length();++i){
      if (s[i] == '<')
              counter = 0;
      else
              counter++;

      if (counter == 3) {
              s[i] = tolower(s[i]);
        return s;
      }
    }
  }

  return s;
}

// splits the string up by whitespace and returns the i'th "word"
const sstring sstring::word(int i) const {
  size_t copy_begin=0, copy_end=0;

  while (1){
    // find first non-whitespace past our last working point
    copy_begin=find_first_not_of(WHITESPACE, copy_end);

    // if nothing found, no more words, return
    if(copy_begin == sstring::npos)
      return "";

    // find our first whitespace past last non-whitespace
    copy_end=find_first_of(WHITESPACE, copy_begin);

    if(!i--){
      // if nothing found, we're on the last word, no trailing whitespace
      if(copy_end == sstring::npos)
        return substr(copy_begin);
      else
        return substr(copy_begin, copy_end-copy_begin);
    }
  }

  return "";
}

// returns true if string has a digit in it
const bool sstring::hasDigit() const {
  for (unsigned int i = 0, s = size(); i < s; i++) {
    if (isdigit(at(i)))
      return true;
  }
  return false;
}


// returns true if string has only digits in it
const bool sstring::isNumber() const {
  for (unsigned int i = 0, s = size(); i < s; i++){
    if (!isdigit(at(i)))
      return false;
  }

  return true;
}

const bool sstring::isWord() const {
  for (unsigned int i = 0, s = size(); i < s; i++) {
    if (!isalpha(at(i)))
      return false;
  }
  return true;
}

const bool sstring::startsVowel() const {
  size_t i = find_first_not_of(WHITESPACE);
  return i != sstring::npos && isvowel(at(i));
}

const sstring sstring::replaceString(sstring find, sstring replace) const {
  sstring str = *this;
  str.inlineReplaceString(find, replace);
  return str;
}

void sstring::inlineReplaceString(const std::string f, const std::string r) {
  size_t start;
  while(std::string::npos != (start = find(f, start))) {
    replace(start, f.length(), r);
    start += r.length();
  }
}

// inline: trims to crlf those lines which consist only of whitespace
void sstring::inlineTrimWhiteLines() {
  size_t len = length();
  size_t start, spaces;

  while (1) {
    spaces = 0;
    start = find("\n\r", start);
    if (start == sstring::npos)
      return;
    start += 2;

    while (start+spaces < len && at(start+spaces) == ' ')
      spaces++;
    if (start >= len)
      return;

    if (spaces && start+spaces+1 < len && at(start+spaces) == '\n' &&
        at(start+spaces+1) == '\r')
      erase(start, spaces);
    else
      start += spaces;
  }
}

// removes all text between start and end (inclusive or exclusive)
void sstring::inlineRemoveBetween(const sstring start, const sstring end,
                                  bool inclusive, bool stopNewline = false) {
  size_t iStart, iEnd, newLine;

  while (1) {
    iStart = find(start, iStart);
    if (iStart == sstring::npos)
      return;
    if (!inclusive)
      iStart += start.length();
    iEnd = find(end, iStart);
    if (iEnd == sstring::npos)
      return;
    newLine = stopNewline ? find("\n", iStart) : sstring::npos;
    if (newLine != sstring::npos && newLine < iEnd) {
      iStart = newLine;
      continue;
    }
    if (inclusive)
      iEnd += end.length();
    if (iEnd > iStart)
      replace(iStart, iEnd-iStart, "", 0);
  }
}

// takes one set of markup tags if they exist in order, and replaces then with
// new tags in the same order
void sstring::inlineReplaceMarkup(const sstring markupStart,
                                  const sstring markupEnd,
                                  const sstring replaceStart,
                                  const sstring replaceEnd) {
  size_t markSLen = markupStart.length();
  size_t markELen = markupEnd.length();
  size_t repSLen = replaceStart.length();
  size_t repELen = replaceEnd.length();
  size_t start = find(markupStart);
  size_t end;

  while (start != sstring::npos) {
    end = find(markupEnd, start + markSLen);
    if (end == sstring::npos)
      break;
    replace(start, markSLen, replaceStart);
    end -= markSLen - repSLen;
    replace(end, markELen, replaceEnd);
    start = find(markupStart, end + repELen);
  }
}

// removes leading and trailing whitespace
const sstring sstring::trim() const {
  size_t start = find_first_not_of(WHITESPACE);
  if (start == sstring::npos)
    return "";
  size_t end = find_last_not_of(WHITESPACE); // can't be ::npos if start isn't
  return substr(start, end - start + 1);
}


// splits a string into an array of strings, given a delimiter
// pass NULL for data to get the amount of split strings
// like this:
//   int c = s.split(';', NULL);
//   sstring *commands = new sstring[c];
//   s.split(';', commands);
//   delete[] commands;
int sstring::split(const char delimit, sstring *data) const {
  int iFound = 0;
  size_t iPos = 0, iPosLast = 0;

  while (sstring::npos != (iPos = find(delimit, iPos))) {
    size_t len = iPos-iPosLast;
    if (len) {
      if (data)
        data[iFound] = substr(iPosLast, len);
      iFound++;
    }
    iPosLast = ++iPos;
  }
  if (iPosLast < length()) {
    if (data)
      data[iFound] = substr(iPosLast, length());
    iFound++;
  }

  return iFound;
}

// given a sentence, try to match to the same case structure
const sstring sstring::matchCase(const sstring match) const {
  sstring out = *this;
  size_t iOut = 0, iMatch = 0;

  while (iMatch < match.length() && iOut < out.length()) {
    // skip to next word to match case on
    if (match[iMatch] == ' ') {
      while (iMatch < match.length() && match[iMatch] == ' ') iMatch++;
      if (out[iOut] != ' ')
        while (iOut < out.length() && out[iOut] != ' ') iOut++;
      while (iOut < out.length() && out[iOut] == ' ') iOut++;
      continue;
    }

    // we're done with our word, skip to see next match
    if (out[iOut] == ' ') {
      while (iOut < out.length() && out[iOut] == ' ') iOut++;
      if (out[iOut] != ' ')
        while (iMatch < match.length() && match[iMatch] != ' ') iMatch++;
      while (iMatch < match.length() && match[iMatch] == ' ') iMatch++;
      continue;
    }

    // match yon case
    if (isupper(match[iMatch]) && !isupper(out[iOut]))
      out[iOut] = toupper(out[iOut]);
    else if (islower(match[iMatch]) && !islower(out[iOut]))
      out[iOut] = tolower(out[iOut]);
    iMatch++;
    iOut++;
  }

  return out;
}

// many of the talk features colorize the says/tells/etc for easier viewing
// If I do "say this <r>color<z> is cool", I would expect to see color in
// red, and "this ", " is cool" be the 'normal' say color.
// unfortunately, turning off red (<z>) makes everything go back to
// normal, and we lose the 'normal' color.
// To get around this, we parse the say statement, and convert any <z>, <Z>,
// or <1> to a 'replacement' color sstring and then send it out.
// unfortunately, we also need to "unbold", so we need to send both the
// normal <z> as well as the replacement
void sstring::convertStringColor(const sstring replacement) {
  // we use <tmpi> to represent a dummy placeholder which we convert to
  // <z> at the end
  sstring repl = "<tmpi>";
  repl += replacement;

  while (find("<z>") != sstring::npos)
    replace(find("<z>"), 3, repl);

  while (find("<Z>") != sstring::npos)
    replace(find("<Z>"), 3, repl);

  while (find("<1>") != sstring::npos)
    replace(find("<1>"), 3, repl);

  while (find("<tmpi>") != sstring::npos)
    replace(find("<tmpi>"), 6, "<z>");
}

// vim: ft=cpp:tw=79:sw=2:sts=2:ts=8:et
