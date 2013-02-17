#include "logging.h"
#include "misc/being.h"
#include "misc/person.h"
#include "misc/monster.h"
#include "misc/account.h"
#include "misc/mail.h"
#include "sys/client.h"
#include "sys/connect.h"
#include "misc/room.h"
#include "sys/database.h"

void TBeing::doFeedback(const int cmd, const sstring &subject)
{
  sendTo("This command is unavailable for you.\n\r");
}

void TPerson::doFeedback(const int cmd, const sstring &subject)
{
  if (fight()) {
    sendTo("You cannot perform that action while fighting!\n\r");
    return;
  }

  sstring type = "request";
  int clientCmd = CLIENT_STARTEDIT;

  switch (cmd) {
    case CMD_BUG:
      type = "bug";
      clientCmd = CLIENT_BUG;
      break;
    case CMD_IDEA:
      type = "idea";
      clientCmd = CLIENT_IDEA;
      break;
    case CMD_TYPO:
      type = "typo";
      clientCmd = CLIENT_TYPO;
      break;
  }

  strncpy(desc->name, type.c_str(), cElements(desc->name));
  strncpy(desc->subject, subject.c_str(), cElements(desc->subject));

  if (!desc->m_bIsClient) {
    sendTo(format("Write your %s report. Use ~ when done, or ` to cancel.\n\r") % type);
    addPlayerAction(PLR_BUGGING);
    desc->connected = CON_WRITING;
    desc->str = new const char *('\0');
    desc->max_str = MAX_MAIL_SIZE;
  } else {
    desc->clientf(format("%d") % clientCmd);
  }
}

void Descriptor::send_feedback()
{
  TDatabase db;
  static const char *q = "INSERT INTO feedback "
      "(type, account, email, player, room, subject, message) VALUES "
      "(%s, %s, %s, %s, %i, %s, %s)";

  TBeing *player = (dynamic_cast<TMonster *>(character) && original) ? original : character;
  int room = player->roomp ? player->roomp->number : -1;

  if (!db.query(q, name, account->name.c_str(), account->email.c_str(), player->getName(), room, subject, *str)) {
    vlogf(LOG_BUG, format("Player feedback failed when posting message for %s.") % player->name);
    player->sendTo("There was an error filing your feedback.");
  }
}
