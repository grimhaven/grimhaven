#ifndef SERVER_MISC_DIRSORT_H_
#define SERVER_MISC_DIRSORT_H_

class dirlistSort {
  public:
    bool operator() (const sstring &, const sstring &) const;
};

#endif
