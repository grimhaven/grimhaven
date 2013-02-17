#ifndef SERVER_OBJ_MERGEABLE_H_
#define SERVER_OBJ_MERGEABLE_H_

// this is a base class for object types are that "mergeable"
// ex, components, commodities, talens, etc

#include "misc/obj.h"

class TMergeable : public virtual TObj {
 private:

 public:
  virtual bool willMerge(TMergeable *);
  virtual void doMerge(TMergeable *);

  TMergeable();
  TMergeable(const TMergeable &a);
  TMergeable & operator=(const TMergeable &a);
  virtual ~TMergeable();

};


#endif
