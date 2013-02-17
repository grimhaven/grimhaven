#ifndef SERVER_OBJ_BOOK_H_
#define SERVER_OBJ_BOOK_H_

#include "misc/obj.h"


class TBook : public TObj {
  private:
  public:
    virtual void assignFourValues(int, int, int, int);
    virtual void getFourValues(int *, int *, int *, int *) const;
    virtual sstring statObjInfo() const;
    virtual itemTypeT itemType() const { return ITEM_BOOK; }
    virtual void lookAtObj(TBeing *, const char *, showModeT) const;

    TBook();
    TBook(const TBook &a);
    TBook & operator=(const TBook &a);
    virtual ~TBook();
};


#endif
