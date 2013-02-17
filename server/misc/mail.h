#ifndef SERVER_MISC_MAIL_H_
#define SERVER_MISC_MAIL_H_

const int MIN_MAIL_LEVEL=2;
const int STAMP_PRICE=50;
const int FACTION_STAMP_PRICE=5000;
const int MAX_MAIL_SIZE=4000;
const int NAME_SIZE =15;


extern bool has_mail(const sstring);

extern void autoMail(TBeing *, const char *, const char *, int m = 0, int r = 0);

#endif
