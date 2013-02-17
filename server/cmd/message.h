#ifndef __CMD_MESSAGE_H
#define __CMD_MESSAGE_H

#define NEWBIE_PURGATORY_LENGTH 7257600

enum messageTypeT
{
  MSG_ERROR     = 0,
  MSG_MIN       = 1,
  MSG_IMM_TITLE = 1,
  MSG_PURGE,
  MSG_PURGE_TARG,
  MSG_RLOAD,
  MSG_LOAD_OBJ,
  MSG_LOAD_MOB,
  MSG_MEDIT,
  MSG_OEDIT,
  MSG_SWITCH_TARG,
  MSG_MOVE_IN,
  MSG_MOVE_OUT,
  MSG_SLAY,
  MSG_SLAY_TARG,
  MSG_FORCE,
  MSG_BAMFIN,
  MSG_BAMFOUT,
  MSG_LONGDESCR,
  MSG_NOTE,
  MSG_MAX
};

const int MSG_TYPE_MAX   = MSG_MAX;
const int MSG_REQ_GNAME  = (1 << 0); // God Name
const int MSG_REQ_ONAME  = (1 << 1); // Other Name [mobile/object/player]
const int MSG_REQ_STRING = (1 << 2); // Command sstring
const int MSG_REQ_DIR    = (1 << 3); // Direction of travel

struct messageBuffer
{
  char *msgImmTitle,
       *msgPurge,
       *msgPurgeTarg,
       *msgRLoad,
       *msgLoadObj,
       *msgLoadMob,
       *msgMEdit,
       *msgOEdit,
       *msgSwitchTarg,
       *msgMoveIn,
       *msgMoveOut,
       *msgSlay,
       *msgSlayTarg,
       *msgForce,
       *msgBamfin,
       *msgBamfout,
       *msgLongDescr,
       *msgNote;
};

class TMessages
{
  public:
    messageBuffer  tMessages;
    TBeing        *tPlayer;

    sstring getImmortalTitles(TBeing *);
    // Return the old default messages.
    sstring getDefaultMessage(messageTypeT, TBeing *);
    // ?(Has message type), also checks for appropriate power setting.
    bool operator== (messageTypeT);
    // Set message type to message
    void operator() (messageTypeT, sstring);
   // TMessages & operator() (messageTypeT, sstring);
    // Sets the fields in message  [Call this to actually Get the messages]
    sstring operator() (messageTypeT, TThing * = NULL, const char * = NULL, bool = true);
    // Used by  : Get message from type
    sstring operator[] (messageTypeT) const;
    void initialize();
    void savedown();

    TMessages();
    TMessages(const TMessages &);
    TMessages & operator == (const TMessages &);
    ~TMessages();
};

#endif
