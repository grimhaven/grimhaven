#ifndef _SKILL_SORT_H
#define _SKILL_SORT_H

// this class serves two related purposes
// first, it provides an operator that can be sent to the sort() algorithm
// second, it stores the data that will be needed by that sorting routine.
class skillSorter {
  public:
    TBeing *ch;
    spellNumT theSkill;
    skillSorter(TBeing *c2, spellNumT ts) :
      ch(c2), theSkill(ts) {}

    skillSorter() : ch(NULL), theSkill(TYPE_UNDEFINED) {}

    bool operator() (const skillSorter &, const skillSorter &) const;
};

#endif
