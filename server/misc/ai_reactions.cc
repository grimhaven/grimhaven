//  return values are unimportant.  if char(s) are lost, return a -1

#include <cstdio>
#include <vector>

#include "core/logging.h"
#include "misc/room.h"
#include "misc/being.h"
#include "misc/extern.h"
#include "misc/monster.h"
#include "misc/combat.h"

int TMonster::aiGrinnedAt(TBeing *doer) {
  US(3);
  switch (::number(1,4)) {
    case 1:
      doSay("You're evil!");
      return TRUE;
      break;
    case 2:
      doAction(fname(doer->name),CMD_CRINGE);
      return TRUE;
      break;
    default:
  }
  return FALSE;
}

int TMonster::aiAccuseAvoid(TBeing *doer) {
  US(2);
  UA(1);
  switch (::number(1,4)) {
    case 1:
      doSay("I didn't do it, nobody saw me, you can't prove anything!");
      return TRUE;
      break;
    case 2:
      doSay("I'm innocent I tell ya, I'm innocent!");
      return TRUE;
      break;
    case 3:
      doSay("It wasn't me, you liar!  It was you!!");
      doAction(fname(doer->name),CMD_POINT);
      return TRUE;
      break;
    default:
      break;
  }
  return FALSE;
}

int TMonster::aiLoveSelf(TBeing *doer) {
  US(1);
  switch (::number(1,5)) {
    case 1:
      return aiStrangeThings(doer);
      break;
    case 2:
      doSay("A wee bit narcissistic, wouldn't you say?");
      return TRUE;
      break;
    case 3:
      doSay("Stop playing with yourself.");
      return TRUE;
      break;
    default:
      break;
  }
  return FALSE;
}

int TMonster::aiShutUp(TBeing *doer) {
  UA(1);
  switch (::number(1,8)) {
    case 1:
      doSay("Oh, shut up, fool");
      return TRUE;
      break;
    case 2:
      doSay("Look who's talking!");
      return TRUE;
      break;
    case 3:
      return aiUpset(doer);
      break;
    default:
      break;
  }
  return FALSE;
}

int TMonster::aiOtherInsulted(TBeing *, TBeing *other) {
  switch (::number(1,5)) {
    case 1:
      doSay("Hehehehe heh");
      break;
    case 2:
      if (canSpeak())
        act("$n says, \"Yep, $N sure is stupid.\"",TRUE,this,0,other,TO_ROOM);
      break;
    default:
      return false;
      break;
  }
  return true;
}

// proc referenced by social calls if homosexual act occurs
// SneezyMUD != politically correct  :)
// self = TRUE if ch was homo with mob
// self = FALSE if mob saw ch homo with other
int TMonster::aiFag(TBeing *homo,int self) {
  char buf[160];
  char sex[10], sex2[10];

  if ((!self && (GetMaxLevel() < 10)) || ::number(0,4))
    return FALSE;

  if (homo->getSex() == SEX_MALE)
    strcpy(sex,"men");
  else
    strcpy(sex,"women");
  if (getSex() == SEX_MALE)
    strcpy(sex2,"guy");
  else
    strcpy(sex2,"girl");
  int value;
  if (self)
    value = ::number(1,19);    // low numbered, ch sexed the mob
  else                         // high numbered, ch sexed someone else
    value = ::number(7,20);    // skip the ones assuming ch fag with mob

  switch (value) {
    case 20:
      act("$n averts $s eyes so $e doesn't have to watch this.",
            FALSE, this, 0, 0, TO_ROOM);
      break;
    case 5:
      doSay("I'd prefer not to get involved in your alternative lifestyle choice.");
      doAction(fname(homo->name),CMD_GLARE);
      break;
    case 17:
      doSay("I'm so glad you've come out of the closet and can express your true feelings like this now.");
      doAction(fname(homo->name),CMD_CLAP);
      break;
    case 16:
      doSay("I admire your courage in being so open about your sexuality.");
      doAction(fname(homo->name),CMD_PAT);
      break;
    case 1:
      sprintf(buf,"$n wonders about %s who do that to $m.",sex);
      act(buf,0, this, 0, 0, TO_ROOM);
      break;
    case 2:
      doAction(fname(homo->name),CMD_GROWL);
      break;
    case 19:
    case 18:
      doAction(fname(homo->name),CMD_SLAP);
      doSay("There won't be any of that!");
      break;
    case 3:
      sprintf(buf,"I'm not that kind of %s",sex2);
      doAction(fname(homo->name),CMD_POKE);
      doSay(buf);
      break;
    case 4:
      act("$n pushes you away quickly.",FALSE,this,0,homo,TO_VICT);
      act("$n pushes $N away quickly.",FALSE,this,0,homo,TO_NOTVICT);
      break;
    case 8:
      doSay("Hey now, let's not get carried away.");
      break;
    case 9:
    case 10:
      return aiInsultDoer(homo);
      break;
    case 11:
      doAction(fname(homo->name),CMD_SLAP);
      break;
    case 6:
      if (getSex() == SEX_MALE)
        doSay("I'm quite comfortable in my masculinity, and things like that don't phaze me.");
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

inline const char* choose(std::vector<const char *> v) {
  return v.at(::number(0, v.size()-1));
}

const std::vector<const char *> insults_bra = {
  "feeble",
  "flaccid",
  "frail",
  "limp",
  "pathetic",
  "pitiful",
  "puny",
  "weak",
  "wimpy",
};

const std::vector<const char *> insults_int = {
  "daft",
  "dim-witted",
  "dozy",
  "dumb",
  "fatuous",
  "feebleminded",
  "harebrained",
  "idiotic",
  "ignorant",
  "mindless",
  "moronic",
  "stupid",
  "vacuous",
  "witless",
};

const std::vector<const char *> insults_wis = {
  "barmy",
  "batty",
  "crazed",
  "cracked",
  "deranged",
  "foolish",
  "gormless",
  "inept",
  "juvenile",
  "psychotic",
  "puerile",
  "tedious",
  "thick-headed",
};

const std::vector<const char *> insults_agi = {
  "awkward",
  "blundering",
  "bumbling",
  "clumsy",
  "floundering",
  "gawky",
  "inept",
  "lumbering",
  "lumpish",
  "ponderous",
  "ungainly",
};

const std::vector<const char *> insults_con = {
  "cadaverous",
  "corpulent",
  "emaciated",
  "gaunt",
  "reedy",
  "scrawny",
  "spindly",
  "weedy",
  "wizened",
};

// includes both appearance and personality
const std::vector<const char *> insults_cha = {
  "degenerate",
  "ugly",
  "hideous",
  "loathesome",
  "foul",
  "repellent",
  "horrid",
  "barbaric",
  "vulgar",
  "churlish",
  "coarse",
  "ill-bred",
  "repulsive",
};

const std::vector<const char *> insults_misc = {
  "ankyloproctian", // tight anus
  "asinine",
  "bescumbering", // spray with feces
  "bloody",
  "cacopygian", // ugly butt
  "caprylic", "hircine", // smells like a goat
  "cheeky",
  "cowering",
  "cowhearted",
  "craven",
  "cretinous",
  "dasypygal", // hairy butt
  "depraved",
  "fatuous",
  "fimicolous", // live & grow on feces
  "flatulous",
  "froggy",
  "gnollish",
  "goatfaced",
  "greasy",
  "gutless",
  "helpless",
  "hircismic", // smelly armpits
  "impotent",
  "inbred",
  "janky",
  "mangy",
  "pusillanimous",
  "raddled",
  "ridiculous",
  "sleazy",
  "sordid",
  "spineless",
  "steatopygous", // fat butt
  "tacky",
  "timorous",
  "weak-kneed",
  "worthless",
  "wretched",
  "yellow",
  "yellow-bellied",
}

const char* adjective(TBeing *vict) {
  int bra = vict->getStat(STAT_CURRENT,STAT_BRA);
  int intel = vict->getStat(STAT_CURRENT,STAT_INT);
  int wis = vict->getStat(STAT_CURRENT,STAT_WIS);
  int agi = vict->getStat(STAT_CURRENT,STAT_AGI);
  int con = vict->getStat(STAT_CURRENT,STAT_CON);
  int cha = vict->getStat(STAT_CURRENT,STAT_CHA);
  if (bra <= intel && bra <= wis && bra <= agi && bra <= con && bra <= cha)
    return choose(insults_bra);
  if (intel <= wis && intel <= agi && intel <= con && intel <= cha)
    return choose(insults_int);
  if (wis <= agi && wis <= con && wis <= cha)
    return choose(insults_wis);
  if (agi <= con && agi <= cha)
    return choose(insults_agi);
  if (con <= cha)
    return choose(insults_con);
  return choose(insults_cha);
}

const char* adjective_random(void) {
  switch (::number(0, 5)) {
    case 0:
      return choose(insults_bra);
    case 1:
      return choose(insults_int);
    case 2:
      return choose(insults_wis);
    case 3:
      return choose(insults_agi);
    case 4:
      return choose(insults_con);
    case 5:
      return choose(insults_cha);
  }
}

static const std::vector<const char *> nouns = {
  "asshat",
  "arsehat",
  "bastich",
  "birdbrain",
  "blatherskite",
  "blob",
  "blockhead",
  "boob",
  "boogerdin",
  "boor",
  "braggart",
  "bumblenocker",
  "bugger",
  "clod",
  "coccydynia", // pain in the ass
  "cretin",
  "dimwit",
  "dinksmacker",
  "dope",
  "dolt",
  "dork",
  "fussock", // fat lady
  "freak",
  "fruitcake",
  "gasconader",
  "git",
  "goober",
  "halfwit",
  "humpertin",
  "imbecile",
  "idiot",
  "jackass",
  "jerk",
  "jobbernowl",
  "kook",
  "lackwit",
  "lout",
  "lunatic",
  "maniac",
  "muttonhead",
  "nancy",
  "nimrod",
  "ninnyhammer",
  "numbnuts",
  "numskull",
  "oaf",
  "pansy",
  "poltroon",
  "putz",
  "pucknut",
  "rat-bastard",
  "scumbag",
  "sleazeball",
  "smacktapper",
  "wanker",
  "weasel",
  "witling",
  "yutz",
}

inline const char *noun(void) {
  return choose(nouns);
}

sstring TBeing::getInsult(TBeing *vict) {
  sstring buf;
  ubyte insult = 1;

  switch (::number(1,50)) {
    case 1:
      buf = format("You %s %s") %
      break;
    case 2:
      buf = format("Cursed %s %s") % adverb(vict) % noun();
      break;
    case 3:
      buf = format("Damn %s") % buf3;
      break;
    case 4:
      buf = "Clump of feces";
      break;
    case 5:
      buf = format("Janky %s") % buf3;
      break;
    case 6:
      buf = "Dude, you got some heinous weenie-dog breath";
      break;
    case 7:
      buf = buf3;
      break;
    case 9:
      buf="Gnollish arsehat";
      break;
    case 10:
      buf="Hey dirt-for-brains, bite me";
      break;
    case 11:
      if (getRace() != vict->getRace()) {
        buf = format("All %s are idiots") % vict->getMyRace()->getProperName();
        break;
      } // else fall through
    case 12:
      buf = "You call that a face, the back of my horse is better looking";
      break;
    case 13:
      buf = "I bet your mother doesn't even know your father's name";
      break;
    case 14:
      buf = "Where'd you get that equipment?  I've seen better stuff in the dump";
      break;
    case 15:
      buf = format("Hey %s, is it true your IQ is the same as your shoe size") % fname(vict->name);
      break;
    case 16:
      buf = "The only reason you were born is your dad could run faster than your mom";
      break;
    case 17:
      buf = "You smell worse than a troll's outhouse";
      break;
    case 18:
      buf = "I could have been your father if I'd've had change for a talen";
      break;
    case 19:
      buf = "You are the finest woman to have ever walked the streets";
      break;
    case 20:
      buf = "The best part of you rolled down the back of a horse's leg!";
      break;
    case 21:
      buf = "Blah blah blah";
      break;
    case 22:
      buf = "PFFFfffffftttt";
      break;
    case 23:
      buf = "Feh! Meh! Plahhhh";
      break;
    case 24:
      buf = "Midos-loving crap-hugging piece of apple pie";
      break;
    case 25:
      buf = "Ha ha ha ha hahahahaha!! Hahahahahaha";
      break;
    case 26:
      buf = "Argle ripple yrk";
      break;
    case 27:
      buf = "Man the minefields! Secure the fritters! Batten down the pancakes";
      break;
    case 28:
      buf = "Ripple zarp rod, tool ftagn truck";
      break;
    case 29:
      buf = "Fish huzzah glyph frbble fnort";
      break;
    case 30:
      buf = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!!";
      break;
    case 31:
      buf = "What are you looking at, ugly?";
      break;
    case 32:
      buf = "Oh, run home to your mother, why don't you?";
      break;
    case 33:
      buf = "Take a picture, it'll last longer";
      break;
    case 34:
      buf = "Cthulhu johnson yerf cookies frob yuma gumma SUSHI";
      break;
    case 35:
      buf = "Put that in your hula hoop and smoke it";
      break;
    case 36:
      buf = "KILL EVERYTHING, KILL IT ALL RIGHT NOW DIE DIE DIE";
      break;
    case 37:
      buf = "What is your problem exactly, anyway?";
      break;
    case 38:
      buf = "Is there a point to all your talking?";
      break;
    case 39:
      buf = "When are you going to stop going in circles?";
      break;
    case 40:
      buf = format("We should go kill Mobatsu, %s is tanking") % fname(vict->name);
      break;
    case 41:
      buf = "Why? Why? Why? WHY? WHY??";
      break;
    case 42:
      buf = "Yoyodine mother hastur brob lram cookies";
      break;
    case 43:
      buf = "Big whoop";
      break;
    case 44:
      buf = "Here's a ladder, get over yourself";
      break;
    case 45:
      buf = "You're such a whiner";
      break;
    case 46:
      buf = "Stop it";
      break;
    case 47:
      buf = format("Stop wasting precious oxygen, you %s %s") %buf2 % buf3;
      break;
    case 48:
      buf = "Your arse is on fire";
      break;
    case 49:
      buf = "Look out behind you";
      break;
    case 50:
      buf = "Ugh, here's a tissue. Cover your face with it";
      break;
    default:
      return "I can't think of an insult right now";
  }

  for (insult=1; insult <= ::number(1,5); insult++)
    buf += "!";

  return buf;
}


int TMonster::aiInsultDoer(TBeing *vict)
{
  sstring buf, buf2;

  if (!isAngry() && !(::number(0,1)))
    return FALSE;

  if (canSpeak()) {
    buf=getInsult(vict);

    buf2 = format("$n looks at you and says, \"%s\"") %buf;
    act(buf2,TRUE,this,0,vict,TO_VICT);
    buf2 = format("$n looks at $N and says, \"%s\"") %buf;
    act(buf2,TRUE,this,0,vict,TO_NOTVICT);
  }

  return TRUE;
}

int TMonster::aiRudeNoise(TBeing *ch)
{
  US(1);

  switch (::number(1,9)) {
    case 1:
      act("$n is startled by your sudden outburst.",TRUE, this,0,ch,TO_VICT);
      act("$n is startled by the noise.",TRUE, this,0,ch,TO_NOTVICT);
      UA(1);
      break;
    case 2:
      act("$n jumps slightly in surprise.",TRUE, this,0,ch,TO_VICT);
      act("$n jumps slightly in surprise.",TRUE, this,0,ch,TO_NOTVICT);
      break;
    case 3:
      act("$n thinks you are strange.",TRUE,this,0,ch,TO_VICT);
      act("$n looks at $N strangely.",TRUE,this,0,ch,TO_NOTVICT);
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

int TMonster::aiBadManners(TBeing *ch)
{
  US(2);
  switch (::number(1,9)) {
    case 1:
      doSay("That was uncalled for.");
      break;
    case 2:
      doSay("Ehwww, gross!");
      break;
    case 3:
      doSay("Have you no pride?");
      break;
    case 4:
      doSay("Where are your manners?!?");
      break;
    case 5:
      if (canSpeak()) {
        act("$n looks at you and says, \"Well EXCUSE YOU.\"",TRUE,this,0,ch,TO_VICT);
        act("$n looks at $N and says, \"Well EXCUSE YOU.\"",TRUE,this,0,ch,TO_NOTVICT);
      }
      break;
    case 6:
      act("$n grimaces.",TRUE,this,0,0,TO_ROOM);
      break;
    case 7:
      doSay("Stop that crap will ya?!?");
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

int TMonster::aiUpset(TBeing *ch)
{
  UM(2);
  UA(5);
  US(2);
  if (::number(0,2))
    return FALSE;
  switch (::number(1,10)) {
    case 1:
      doAction(fname(ch->name),CMD_FUME);
      break;
    case 2:
      doAction(fname(ch->name),CMD_GROWL);
      break;
    case 3:
      doAction(fname(ch->name),CMD_SLAP);
      break;
    case 4:
      doAction(fname(ch->name),CMD_SNARL);
      break;
    case 5:
      if (canSpeak()) {
        doSay("Knock that stuff off!");
        break;
      } // fall thru for no speak
    case 6:
      if (canSpeak()) {
        act("$n says, \"Bite me, $N!\"",TRUE,this,0,ch,TO_ROOM);
        break;
      } // fall thru for no speak
    case 7:
      if (canSpeak()) {
        doSay("What's up with that?!?");
        break;
      } // fall thru for no speak
    case 8:
    case 9:
    case 10:
    default:
      return aiInsultDoer(ch);
      break;
  }
  return TRUE;
}

int TMonster::aiStrangeThings(TBeing *strange_target)
{
  US(1);
  switch (::number(1,20)) {
    case 1:
      act("$n thinks you are strange.",TRUE,this,0,strange_target,TO_VICT);
      act("$n looks at $N strangely.",TRUE,this,0,strange_target,TO_NOTVICT);
      break;
    case 2:
      doAction("",CMD_ARCH);
      break;
    case 3:
      doSay("weirdo.");
      break;
    case 4:
      act("$n thinks you're weird.",TRUE, this,0,strange_target,TO_VICT);
      act("$n looks at $N oddly.",TRUE, this,0,strange_target,TO_NOTVICT);
      break;
    case 5:
      doSay("bizarre");
      break;
    case 6:
      doSay("Uh oh, looks like another inmate escaped the looney bin.");
      break;
    case 7:
      act("$n questions your sanity.",TRUE, this,0,strange_target,TO_VICT);
      act("$n looks at $N strangely.",TRUE, this,0,strange_target,TO_NOTVICT);
      break;
    case 8:
      doSay("hmmmm");
      break;
    case 9:
      doSay("Have you ever considered professional help?");
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

// reaction for non humanoids that are shocked
int TMonster::aiMobShock(TBeing *doer)
{
  if (getRace() == RACE_CANINE) {
    switch (::number(1,5)) {
      case 1:
        act("$n jumps slightly in surprise and shock.", 0, this, 0, 0, TO_ROOM);
        break;
      case 2:
        act("$n yips slightly and makes a growling noise in the back of $s throat.",0, this, 0, 0, TO_ROOM);
        break;
      default:
        return FALSE;
        break;
    }
  } else if (getRace() == RACE_FELINE || getRace() == RACE_WYVELIN) {
    switch (::number(1,5)) {
      case 1:
        act("The hair on $n's back raises and $e hisses loudly.",
             TRUE, this, 0, 0, TO_ROOM);
        break;
      case 2:
        act("$n arches $s back and snarls in $N's direction.",
             TRUE, this, 0, doer, TO_NOTVICT);
        act("$n arches $s back and snarls in your direction.",
             TRUE, this, 0, doer, TO_VICT);
        break;
      default:
        return FALSE;
        break;
    }
  } else {
    switch (::number(1,5)) {
      case 1:
        act("The hair on $n's back raises and $e hisses loudly.",TRUE,this,0,
                     0,TO_ROOM);
        break;
      case 2:
        act("$n jumps slightly in shock as $N does that.",TRUE,this,0,
                    doer,TO_NOTVICT);
        act("$n jumps slightly in shock as you do that.",TRUE,this,0,
                    doer,TO_VICT);
        break;
      default:
        return FALSE;
        break;
    }
  }
  return TRUE;
}

// non humanoids made happy by something
int TMonster::aiMobHappy(TBeing *doer)
{
  DA(3);
  DMal(2);
  if (getRace() == RACE_CANINE) {
    switch (::number(1,3)) {
      case 1:
        doAction(fname(doer->name),CMD_LICK);
        act("$n yips happily.",TRUE,this,0,0,TO_ROOM);

        break;
      default:
        return FALSE;
        break;
    }
  } else if (getRace() == RACE_FELINE || getRace() == RACE_WYVELIN) {
    switch (::number(1,4)) {
      case 1:
        doAction(fname(doer->name),CMD_PURR);

        break;
      case 2:
        act("$n makes contented noises and rubs up against your leg.",
           TRUE,this,0,doer,TO_VICT);
        act("$n makes contented noises and rubs up against $N's leg.",
           TRUE,this,0,doer,TO_NOTVICT);

        break;
      default:
        return FALSE;
        break;
    }
  } else {
    switch (::number(1,3)) {
      case 1:
        act("$n makes contented noises and seems to smile.",TRUE,this,0,0,TO_ROOM);
        break;
      default:
        return FALSE;
        break;
    }
  }
  return TRUE;
}

// assumes opposite genders involved and no eunichs
// watch some porn flicks then fill some of these case statements out
int TMonster::aiMudSex(TBeing *doer)
{
  sstring buf;

  if (doer->isRealUgly()) {
    return aiMudSexRepulsed(doer);
  }

  US(3);
  DA(3);
  DMal(2);
  switch (::number(1,22)) {
    case 1:
      doSay("Oooooh, touch me like that again!");
      break;
    case 2:
      doAction("",CMD_MOAN);
      break;
    case 3:
      doSay("Uwmmmm, that feels SO good.");
      break;
    case 4:
      doSay("Come here lover, I've got something for you.");
      doAction(fname(doer->name),CMD_KISS);
      break;
    case 5:
      if (!::number(0,5)) {
        doSay("Hehe, Somebody SURE is bored and lonely to be trying to pleasure me  :)");
      }
      break;
    case 6:
      doAction(fname(doer->name),CMD_HUG);
      break;
    case 7:
      doAction(fname(doer->name),CMD_SNUGGLE);
      break;
    case 8:
      doAction(fname(doer->name),CMD_BEAM);
      break;
    case 9:
      doAction(fname(doer->name),CMD_FRENCH);
      break;
    case 10:
      doSay("Oooohh, I want you SO bad.");
      break;
    case 11:
      doSay("Come to me, I want you!");
      break;
    case 12:
      buf = format("%s Will you still respect me in the morning?") %fname(doer->name);
      doAsk(buf);
      break;
    case 13:
      buf = format("%s I love you for your body.") %fname(doer->name);
      doWhisper(buf);
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

// assumes doer and other are both opposite sexes and nobody's a eunich
int TMonster::aiMudSexOther(TBeing *doer, TBeing *)
{
  US(2);
  DA(1);
  switch (::number(1,10)) {
    case 1:
      doSay("Hey you two, get a room!");
      break;
    case 2:
      doAsk(format("%s Hey, how about a menage a trois?") % fname(doer->name));
      break;
    case 3:
      doSay("OK, no public displays of affection please!");
      break;
    case 4:
      doAction("",CMD_COUGH);
      doSay("You know, there could be kids watching.");
      break;
    case 5:
      doSay("If you two keep this up, we're bound to lose our PG-13 rating.");
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

int TMonster::aiMudSexRepulsed(TBeing *doer)
{
  UA(5);
  UM(3);
  switch (::number(1,7)) {
    case 1:
      doSay("Not tonight honey, I have a headache!");
      break;
    case 2:
      if (canSpeak()) {
        act("$n pushes you away and laughs, \"Yeah right.\"",
            TRUE,this,0,doer,TO_VICT);
        act("$n pushes $N away and laughs, \"Yeah right.\"",
            TRUE,this,0,doer,TO_NOTVICT);
        doSay("As if someone like me would be attracted to you.");
      }
      break;
    case 3:
      doAction(fname(doer->name),CMD_LAUGH);
      doSay("Yeah right, like someone like me would find you attractive!");
      break;
    case 4:
      act("$n is totally repulsed by your looks and pushes you away!",
            TRUE,this,0,doer,TO_VICT);
      act("$n is repulsed by $N's ugliness and pushes $M away.",
            TRUE,this,0,doer,TO_NOTVICT);
      break;
    case 5:
      act("$n pushes you away!",TRUE,this,0,doer,TO_VICT);
      act("$n pushes $M away.",TRUE,this,0,doer,TO_NOTVICT);
      doSay("I think not!");
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

int TMonster::aiClapReact(TBeing *, TBeing *)
{
  switch (::number(1,7)) {
    case 1:
      doAction("",CMD_CLAP);
      break;
    case 2:
      doAction("",CMD_APPLAUD);
      break;
    case 3:
      doAction("",CMD_WOO);
      break;
    case 4:
      doAction("",CMD_SMILE);
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

int TMonster::aiWimpCheck(TBeing *vict)
{
  switch (::number(1,5)) {
    case 1:
      doLook(fname(vict->name), CMD_LOOK);
      break;
    case 2:
      act("$n glances at you.", FALSE, this, 0, vict, TO_VICT);
      act("$n glances at $N.", FALSE, this, 0, vict, TO_NOTVICT);
      break;
    case 3:
      act("$n contemplates kicking your butt.", FALSE, this, 0, vict, TO_VICT);
      act("$n contemplates kicking $N's butt.", FALSE, this, 0, vict, TO_NOTVICT);
      doAction(fname(vict->name), CMD_THINK);
      if (isHumanoid())
        doSay("Hmm...");
      break;
    case 4:
      doLook(fname(vict->name), CMD_LOOK);
      doAction("",CMD_PONDER);
      break;
    case 5:
      act("$n glances at you.", FALSE, this, 0, vict, TO_VICT);
      act("$n glances at $N.", FALSE, this, 0, vict, TO_NOTVICT);
      doAction("",CMD_GRIN);
      break;
    default:
      return FALSE;
      break;
  }
  return TRUE;
}

int TMonster::aiWimpSwitch(TBeing *vict)
{
  TBeing *tank = NULL;
  int tSkill = 0, // Tank Rescue-Switch Skill Average
      mSkill = 0, // Monster Rescue-Switch Skill Average
      cLevel = 0;
  tank = fight();
  affectedData *hjp=NULL;


  // Lets see if the current tank is in a position, and if so would
  // Want to, to prevent the switch.  The mobs current target MUST be
  // in the 'to be switched to' persons Group.  And Both must be PCs.
  // Main Checks:
  //   1. Has tank(current monster target)
  //   2. tank is a pc and victim is a pc
  //   3. tank has session.amGroupTank set to true.
  //   4. tank is in the victims group
  //   5. tank is awake
  //   6. tank has the use of legs
  //   7. tank can see monster
  //   8. tank can see victim
  //   9. tank is at least Standing
  //  10. tank must also have a wait less than 4, allows for lagged blocking.
  //  11. tank has Neither AUTO_ENGAGE or AUTO_ENGAGE_ALWAYS set.
  if (tank                && tank->desc                      && tank->isPc()        &&
      vict->isPc()        && tank->desc->session.amGroupTank && vict->inGroup(*tank) &&
      tank->awake()       && !tank->bothLegsHurt()           && tank->canSee(this)  &&
      tank->canSee(vict)  && tank->getPosition() >= POSITION_STANDING               &&
      tank->getWait() < 4 && !IS_SET(tank->desc->autobits, AUTO_ENGAGE)             &&
      !IS_SET(tank->desc->autobits, AUTO_ENGAGE_ALWAYS)) {
    // [tank] Get Rescue Skill
    if (tank->doesKnowSkill(tank->getSkillNum(SKILL_RESCUE)))
      tSkill += tank->getSkillValue(tank->getSkillNum(SKILL_RESCUE));
    // [tank] Get Switch Skill [Detection]
    if (tank->doesKnowSkill(tank->getSkillNum(SKILL_SWITCH_OPP)))
      tSkill += tank->getSkillValue(tank->getSkillNum(SKILL_SWITCH_OPP));
    // [tank] Get Level for a difference, higher the better in favor of the preventor.
    cLevel = 40-tank->GetMaxLevel(); // l1 = 39,...,-39, l50 = -10,...,10
    tSkill += ::number(cLevel, -cLevel);
    if (tSkill > 0)
      // Cut it in half to make it easier to deduce a value, more strict this way.
      tSkill /= 2;
    else
      tSkill = 0;

    // [monster] Get Rescue Skill [detection] [to prevent block]
    if (doesKnowSkill(getSkillNum(SKILL_RESCUE)))
      mSkill += getSkillValue(getSkillNum(SKILL_RESCUE));
    // [monster] Get Switch Skill
    if (doesKnowSkill(getSkillNum(SKILL_SWITCH_OPP)))
      mSkill += getSkillValue(getSkillNum(SKILL_SWITCH_OPP));
    // [monster] Get Taunted affect
    if (affectedBySpell(SKILL_TAUNT)){
      for (hjp = affected; hjp; hjp = hjp->next) {
        if (hjp->type == SKILL_TAUNT)
          mSkill -= hjp->level;
      }
    }
    // [monster] Get Level for a difference, higher the better in favor of the switcher.
    cLevel = 40-GetMaxLevel(); // l1 = 39,...,-39, l50 = -10,...,10, l127 = -87,...,87
    mSkill += ::number(cLevel, -cLevel);
    if (mSkill > 0)
      // Cut it in half to make it easier to deduce a value, more strict this way.
      mSkill /= 2;
    else
      mSkill = 0;

    // Best/Worst Case examples:
    // Level 1: 0/0 rescue/switch
    //   0, 0, 39, (39, -39), (19, 0)
    // Level 50: 100/100 rescue/switch
    //   100, 200, -10, (190, 210), (95, 105)

    // Just check to see if tank got a higher score, if so he blocked it.
    // Random was added Earlier so we just want to check ending values here.
    if (tSkill > mSkill) {
      sstring oMessage;

      oMessage=format("%s attempts to switch to %s, but you block it.") %
        sstring(getName()).cap() % vict->getName();
      act(oMessage, TRUE, tank, 0, vict, TO_CHAR);

      oMessage=format("%s attempts to switch to %s, but %s blocks it.") %
        sstring(getName()).cap() % vict->getName() % tank->getName();
      act(oMessage, TRUE, tank, 0, vict, TO_NOTVICT);

      oMessage=format("%s attempts to switch to you, but %s blocks it.") %
        sstring(getName()).cap() % tank->getName();
      act(oMessage, TRUE, tank, 0, vict, TO_VICT);

      return FALSE;
    }

    // If they got here, they failed the block so the monster switched.
  }

  // remove taunt affect
  affectFrom(SKILL_TAUNT);

  switch (::number(1,7)) {
    case 1:
      act("$n senses that $N is a weaker opponent.",TRUE, this, 0, vict,
          TO_NOTVICT, ANSI_RED);
      act("$n senses that you are a weaker opponent.",TRUE, this, 0, vict,
               TO_VICT, ANSI_RED);
      break;
    case 2:
    case 4:
      if (!isDumbAnimal())
        doSay("Time to die, feeble one!");

      act("$n <R>switches to $N<Z>.", TRUE, this, 0, vict, TO_NOTVICT);
      act("$n <R>switches to you<Z>.", TRUE, this, 0, vict, TO_VICT);
      break;
    case 3:
    case 5:
      if (!isDumbAnimal())
        doSay("I think I'll just take care of you first!");

      act("$n <R>switches to $N<Z>.", TRUE, this, 0, vict, TO_NOTVICT);
      act("$n <R>switches to you<Z>.", TRUE, this, 0, vict, TO_VICT);
      break;
    case 6:
    case 7:
      if (!isDumbAnimal())
        doSay("You're all mine!");

      act("$n <R>switches to $N<Z>.", TRUE, this, 0, vict, TO_NOTVICT);
      act("$n <R>switches to you<Z>.", TRUE, this, 0, vict, TO_VICT);
      break;
  }

  addToWait(2);
  return TRUE;
}

// shove has already occurred, this and vict may not be in same room
// worked is whether the shove succeeded, dir is direction of shove
int TMonster::aiShoveReact(TBeing *doer, bool worked, dirTypeT dir)
{
  int rc;

  UM(3);
  UA(15);

  if (canSpeak() && !::number(0,2)) {
    if (worked && !sameRoom(*doer)) {
      act("You shake your head and say, \"Jeez, $N sure is pushy.\"",
              FALSE, this, 0, doer, TO_CHAR);
      act("$n shakes $s head and says, \"Jeez, $N sure is pushy.\"",
              FALSE, this, 0, doer, TO_ROOM);
    } else if (!worked) {
      return aiInsultDoer(doer);
    }
  }
  if (isSmartMob(40) && worked && !sameRoom(*doer)) {
    rc = goDirection(rev_dir[dir]);
    if (IS_SET_DELETE(rc, DELETE_THIS)) {
      // we're not checking for death, log an error
      vlogf(LOG_MOB_AI, format("error in shove react (%s shoving %d)") %
          doer->getName() % dir);
      return DELETE_THIS;
    }
  }
  if (sameRoom(*doer) && worked) {
    if (hasHands() && !bothArmsHurt()) {
      act("$n shoves $N.", TRUE, this, 0, doer, TO_NOTVICT);
      act("$n shoves you.", TRUE, this, 0, doer, TO_VICT);
      act("You shove $N.", TRUE, this, 0, doer, TO_CHAR);
    } else {
      act("$n nudges $N.", TRUE, this, 0, doer, TO_NOTVICT);
      act("$n nudges you.", TRUE, this, 0, doer, TO_VICT);
      act("You nudge $N.", TRUE, this, 0, doer, TO_CHAR);
    }
    if (isAngry())
      return takeFirstHit(*doer);
    else
      aiUpset(doer);
  }
  return FALSE;
}

// handles a variety of animal sounds
// presumes isDumbAnimal()
void TMonster::aiGrowl(const TBeing *tar) const
{
  if (getRace() == RACE_FELINE) {
    if (tar) {
      int tmp = ::number(0,1);
      switch(tmp) {
        case 0:
          act("$n hisses impatiently at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
        case 1:
          act("$n hisses angrily at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
      }
    } else {
      int tmp = ::number(0,1);
      switch(tmp) {
        case 0:
          act("$n hisses impatiently.", TRUE, this, 0, 0, TO_ROOM);
          return;
        case 1:
          act("$n hisses angrily.", TRUE, this, 0, 0, TO_ROOM);
          return;
      }
    }
    return;
  } else if (getRace() == RACE_CANINE) {
    if (tar) {
      int tmp = ::number(0,1);
      switch(tmp) {
        case 0:
          act("$n barks impatiently at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
        case 1:
          act("$n barks angrily at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
      }
    } else {
      int tmp = ::number(0,1);
      switch(tmp) {
        case 0:
          act("$n barks impatiently.", TRUE, this, 0, 0, TO_ROOM);
          return;
        case 1:
          act("$n barks angrily.", TRUE, this, 0, 0, TO_ROOM);
          return;
      }
    }
    return;
  } else if (getRace() == RACE_BAT) {
    if (tar) {
      int tmp = ::number(0,1);
      switch(tmp) {
        case 0:
          act("$n screeches impatiently at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
        case 1:
          act("$n screeches angrily at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
      }
    } else {
      int tmp = ::number(0,1);
      switch(tmp) {
        case 0:
          act("$n screeches impatiently.", TRUE, this, 0, 0, TO_ROOM);
          return;
        case 1:
          act("$n screeches angrily.", TRUE, this, 0, 0, TO_ROOM);
          return;
      }
    }
    return;
  } else if (getRace() == RACE_BIRD) {
    if (tar) {
      int tmp = ::number(0,5);
      switch(tmp) {
        case 0:
          act("$n squawks noisily at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
        case 1:
          act("$n squawks angrily at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
        case 2:
          act("$n chirps noisily at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
        case 3:
          act("$n chirps angrily at $N, and flaps $s wings menacingly.", TRUE, this, 0, tar, TO_ROOM);
          return;
        case 4:
          act("$n chirps noisily at $N, and flaps $s wings menacingly.", TRUE, this, 0, tar, TO_ROOM);
          return;
        case 5:
          act("$n chirps angrily at $N.", TRUE, this, 0, tar, TO_ROOM);
          return;
      }
    } else {
      int tmp = ::number(0,5);
      switch(tmp) {
        case 0:
          act("$n squawks noisily.", TRUE, this, 0, 0, TO_ROOM);
          return;
        case 1:
          act("$n squawks angrily.", TRUE, this, 0, 0, TO_ROOM);
          return;
        case 2:
          act("$n chirps noisily.", TRUE, this, 0, 0, TO_ROOM);
          return;
        case 3:
          act("$n chirps angrily.", TRUE, this, 0, 0, TO_ROOM);
          return;
        case 4:
          act("$n chirps noisily, and flaps $s wings meanicingly.", TRUE, this, 0, 0, TO_ROOM);
          return;
        case 5:
          act("$n chirps angrily, and flaps $s wings meanicingly.", TRUE, this, 0, 0, TO_ROOM);
          return;
      }
    }
    return;
  } else {
    // generic case
    if (tar) {
      act("$n growls at $N.",TRUE,this,0,tar,TO_ROOM);
    } else {
      int tmp = ::number(0,1);
      switch (tmp) {
        case 0:
          act("$n growls angrily!", FALSE, this, 0, 0, TO_ROOM);
          return;
        case 1:
          act("$n growls impatiently!", FALSE, this, 0, 0, TO_ROOM);
          return;
      }
    }
  }
}

bool TMonster::aiLoveNonHumanoid(TBeing *doer, aiTarg cond)
{
  if (isHumanoid())
    return false;

  US(1);
  if (cond != TARGET_MOB)
    return true;
  if (!pissed() && !isSusp())
    aiMobHappy(doer);
  else
    aiMobShock(doer);

  return true;
}

int TMonster::aiToastedAt(TBeing *doer)
{
  sstring bs;
  switch (::number(0,4)) {
    case 0:
      doSay("You are really too kind!");
      return TRUE;
      break;
    case 1:
    case 2:
      switch (::number(1, 4)) {
        case 1:
          bs = "Hoo-ee!";
          break;
        case 2:
          bs = "Oh my!";
          break;
        case 3:
          bs = fname(deities[::number(DEITY_NONE + 1, MAX_DEITIES - 1)]);
          bs += "'s wounds!";
          break;
        default:
          bs = "Aye!";
          break;
      }
      doSay(format("%s  Did you see me %s that %s?") % bs % RandomVerb() % RandomWord());
      return TRUE;
      break;
    default:
      break;
  }
  return FALSE;
}

// vim: ft=cpp:tw=79:sw=2:sts=2:ts=8:et
