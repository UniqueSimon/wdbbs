/*-------------------------------------------------------*/
/* menu.c       ( NTHU CS MapleBBS Ver 2.36 )            */
/*-------------------------------------------------------*/
/* target : menu/help/movie routines                     */
/* create : 95/03/29                                     */
/* update : 95/12/15                                     */
/*-------------------------------------------------------*/


#include "bbs.h"

/* ------------------------------------- */
/* help & menu processring               */
/* ------------------------------------- */
int refscreen = NA;
extern char *boardprefix;


void
showtitle(title, mid)
  char *title, *mid;
{
  char buf[40];
  int spc, pad;

  spc = strlen(mid);

  if (title[0] == 0)
    title++;
   else if (chkmail(0))
  {
    mid = "\x1b[41;33;5m   �H�c�̭����s�H��I  \x1b[m\x1b[1m"COLOR1;

/*
 * CyberMax:
 *           spc �O�ǰt mid ���j�p.
 */
    spc = 22;
  }
  else if (dashf(BBSHOME"/register.new") && HAS_PERM(PERM_ACCOUNTS))
  {
    mid = "\x1b[45;33;5m  ���s���ϥΪ̵��U�o!  \x1b[m\x1b[1m"COLOR1;
    spc = 22;
  }

  spc = 66 - strlen(title) - spc - strlen(currboard);
/*
woju
*/
  if (spc < 0)
     spc = 0;
  pad = 1 - spc & 1;
  memset(buf, ' ', spc >>= 1);
  buf[spc] = '\0';

  move(0,0);
  clrtobot();
//  clear();
  prints(COLOR2"  \x1b[1;37m%s  "COLOR1"%s\x1b[33m%s%s%s\x1b[3%s\x1b[1m "COLOR2"  \x1b[37m%s  \x1b[m\n",
    title, buf, mid, buf, " " + pad,
    currmode & MODE_SELECT ? "1m�t�C" :
    currmode & MODE_DIGEST ? "5m��K" : "7m�ݪO", currboard);

}

// wildcat : ����u�Ϊ��ſ�� :p
int
null_menu()
{
  pressanykey("�o�O�@�Ӫſ�� :p ");
  return 0;
}


/* ------------------------------------ */
/* �ʵe�B�z                              */
/* ------------------------------------ */


#define FILMROW 11
unsigned char menu_row = 3;
unsigned char menu_column = 4;
char mystatus[512];


/* wildcat 1998.8.7 */

void
movie(i)
  int i;
{
  extern struct FILMCACHE *film;
  static short history[MAX_HISTORY];
  static char myweek[] = "��@�G�T�|����";
  static char buf[512],pbuf[512];
  char *msgs[] = {"��", "�}", "��", "��","��"};
  time_t now = time(NULL);
  struct tm *ptime = localtime(&now);

  resolve_garbage(); /* get film cache */

  if (currstat == GAME) 
    return; 
  if (HAVE_HABIT(HABIT_MOVIE))
  {
//    film->max_film=31;
    if((!film->busystate) && film->max_film) /* race condition */
    {
      do{
        if (i != 1 || i != 0 || i != 999)
          i = 1 + (rand()%film->max_film);

        for (now = film->max_history; now >= 0; now--)
        if (i == history[now])
        {
          i = 0;
          break;
        }
      } while (i == 0);
    }
//    else i = 1;
    memcpy(history, &history[1], film->max_history * sizeof(short));
    history[film->max_history] = now = i;

    if (i == 999)       /* Goodbye my friend */
      i = 0;
    setapath(pbuf, "Note");
    sprintf(buf, "%s/%s", pbuf, film->notes[i]);
    if(film->notes[i][0])
      show_file(buf,13,FILMROW,NO_RELOAD);
  }
  i = ptime->tm_wday << 1;
  update_data();
  sprintf(mystatus, "\x1b[1;36m %d �I %02d �� (%c%c) %0d �� %0d ��"
"\x1b[1;37m �m�W: %-13s \x1b[32m[�I�s��]%-2.2s \x1b[m",
    ptime->tm_hour, ptime->tm_min, myweek[i], myweek[i + 1],
    ptime->tm_mon + 1, ptime->tm_mday, cuser.userid, msgs[currutmp->pager]);
  move(1,0);
  clrtoeol();
  outs(mystatus);
  refresh();
}


/* ===== end ===== */

static int
show_menu(p)
  MENU *p;
{
  register int n = 0, m = 0;
  int i = 0;
  register char *s;
          char buf[512],fn[512];
  char buf2[11][512] = {"\0","\0","\0","\0","\0","\0","\0","\0","\0","\0","\0"};
//  char buf2[][];
  FILE *fp;
#ifdef HAVE_NOTE_2
  sprintf(fn,BBSHOME"/m2/%d",(rand()%10)+1);
  if(fp = fopen(fn,"r"))
  {
    while(fgets(buf2[i],512,fp) != NULL)
      i++;
    fclose(fp);
  }
#endif
  movie(0);
  move(2,0);
#ifdef HYPER_BBS
  prints(COLOR1"\x1b[1m"HB_BACK" �\\ ��       ��    ��                 �� \x1b[1;33m\x1b[200m\x1b[444m\x1b[626m[Ctrl-Z]\x1b[201m\x1b[37m \x1b[31m�D�U               \x1b[m");
#else
  prints(COLOR1"\x1b[1m         �\\  ��        ��    ��                 �� [\x1b[1;33mCtrl-Z\x1b[37m] \x1b[31m�D�U               \x1b[m");
#endif
  move(menu_row, 0);
  while ((s = p[n].desc)!=NULL || buf2[m][0]!='\0')
  {
    if ( s != NULL )
    {
      if (HAS_PERM(p[n].level))
      {
        sprintf(buf,s+2);
#ifdef HAVE_NOTE_2
        if(currstat == GAME || buf2[m][0]=='\0')
#endif
          prints("%*s  [\x1b[1;36m%c\x1b[m]%s\n", 
            menu_column, "", s[1], buf);
#ifdef HAVE_NOTE_2
        else
          prints("%*s  [\x1b[1;36m%c\x1b[m]%-28s%s",
            menu_column, "", s[1], buf,buf2[m++]);
#endif
      }
      n++;
    }
#ifdef HAVE_NOTE_2
    else
      prints("%37s%-s", "", buf2[m++] );
#endif
  }
  return n - 1;
}


void
domenu(cmdmode, cmdtitle, cmd, cmdtable)
  char *cmdtitle;
  int cmdmode, cmd;
  MENU *cmdtable;
{

  int lastcmdptr;
  int n, pos, total, i;
  int err;
  int chkmailbox();
  static char cmd0[LOGIN];

  if (cmd0[cmdmode])
     cmd = cmd0[cmdmode];

  setutmpmode(cmdmode);
  sprintf(tmpbuf,"%s [�u�W %d �H]",BOARDNAME,count_ulist());

  showtitle(cmdtitle, tmpbuf);
  total = show_menu(cmdtable);
  move(1,0);
  outs(mystatus);

  lastcmdptr = pos = 0;

  do
  {
    i = -1;

    switch (cmd)
    {
    case KEY_ESC:
       if (KEY_ESC_arg == 'c')
          capture_screen();
       else if (KEY_ESC_arg == 'n') {
          edit_note();
          refscreen = YEA;
       }
       i = lastcmdptr;
       break;
    case Ctrl('N'):
       New();
       refscreen = YEA;
       i = lastcmdptr;
       break;
    case Ctrl('A'):
    {
      int stat0 = currstat;
      currstat = RMAIL;
      if (man() == RC_FULL)
        refscreen = YEA;
      i = lastcmdptr;
      currstat = stat0;
      break;
    }
    case KEY_DOWN:
      i = lastcmdptr;

    case KEY_HOME:
    case KEY_PGUP:
      do
      {
        if (++i > total)
          i = 0;
      } while (!HAS_PERM(cmdtable[i].level));
      break;

    case KEY_END:
    case KEY_PGDN:
      i = total;
      break;

    case KEY_UP:
      i = lastcmdptr;
      do
      {
        if (--i < 0)
          i = total;
      } while (!HAS_PERM(cmdtable[i].level));
      break;

    case KEY_LEFT:
    case 'e':
    case 'E':
      if (cmdmode == MMENU)
        cmd = 'G';
      else if ((cmdmode == MAIL) && chkmailbox())
        cmd = 'R';
      else return;
    default:
       if ((cmd == Ctrl('G') || cmd == Ctrl('S')) && (currstat == MMENU || currstat == TMENU || currstat == XMENU))  {
          if (cmd == Ctrl('S'))
             ReadSelect();
          else if (cmd == Ctrl('G'))
            Read();
          refscreen = YEA;
          i = lastcmdptr;
          break;
        }
      if (cmd == '\n' || cmd == '\r' || cmd == KEY_RIGHT)
      {

        boardprefix = cmdtable[lastcmdptr].desc;

        if(cmdtable[lastcmdptr].mode && DL_get(cmdtable[lastcmdptr].cmdfunc))
        {
          void *p = (void *)DL_get(cmdtable[lastcmdptr].cmdfunc);
          if(p) cmdtable[lastcmdptr].cmdfunc = p;
          else break;
        }

        currstat = XMODE;

        {
          int (*func)() = 0;

          func = cmdtable[lastcmdptr].cmdfunc;
          if(!func) return;
          if ((err = (*func)()) == QUIT)
            return;
        }

        currutmp->mode = currstat = cmdmode;

        if (err == XEASY)
        {
          refresh();
          sleep(1);
        }
        else if (err != XEASY + 1 || err == RC_FULL)
          refscreen = YEA;

        if (err != -1)
          cmd = cmdtable[lastcmdptr].desc[0];
        else
          cmd = cmdtable[lastcmdptr].desc[1];
        cmd0[cmdmode] = cmdtable[lastcmdptr].desc[0];
      }

      if (cmd >= 'a' && cmd <= 'z')
        cmd &= ~0x20;
      while (++i <= total)
      {
        if (cmdtable[i].desc[1] == cmd)
          break;
      }
    }

    if (i > total || !HAS_PERM(cmdtable[i].level))
    {
      continue;
    }

    if (refscreen)
    {
      showtitle(cmdtitle, tmpbuf);
      show_menu(cmdtable);
      move(1,0);
      outs(mystatus);
      refscreen = NA;
    }

    cursor_clear(menu_row + pos, menu_column);
    n = pos = -1;
    while (++n <= (lastcmdptr = i))
    {
      if (HAS_PERM(cmdtable[n].level))
        pos++;
    }
    cursor_show(menu_row + pos, menu_column);
  } while (((cmd = igetkey()) != EOF) || refscreen);

  abort_bbs();
}
/* INDENT OFF */


/* ----------------------------------------------------- */
/* administrator's maintain menu                         */
/* ----------------------------------------------------- */

int m_user(), m_newbrd(), m_board(), m_register(),x_reg(),XFile(),
    search_key_user(),search_bad_id,reload_cache();
/*    ,search_bad_id();*/

#ifdef  HAVE_MAILCLEAN
int m_mclean();
#endif

static struct MENU adminlist[] = {
  m_user,       PERM_ACCOUNTS,  "UUser          [�ϥΪ̸��]",0,
  search_key_user,PERM_ACCOUNTS,"FFind User     [�j�M�ϥΪ�]",0,
  m_newbrd,     PERM_BOARD,     "NNew Board     [�}�ҷs�ݪO]",0,
  m_board,      PERM_BOARD,     "SSet Board     [ �]�w�ݪO ]",0,
  m_register,   PERM_ACCOUNTS,  "RRegister      [�f�ֵ��U��]",0,
  XFile,        PERM_SYSOP,     "XXfile         [�ק�t����]",0,
  reload_cache, PERM_SYSOP,     "CCache Reload  [ ��s���A ]",0,
/*
  m_mclean, PERM_BBSADM, "MMail Clean    �M�z�ϥΪ̭ӤH�H�c",0,
#endif */
  x_reg,        PERM_ACCOUNTS,  "MMerge         [�f�֭ײz��]",0,

NULL, 0, NULL,0};

int
rpg_menu()
{
  //domenu(RMENU, "�����t�C��", 'U', rpglist);
  return 0;
}

/* ----------------------------------------------------- */
/* class menu                                            */
/* ----------------------------------------------------- */

int board(); //todo: refine variable name in util/mandex.c ,and move this to include/proto.h .

static struct MENU classlist[] = {
   voteboard, 0,      "VVoteBoard    [�ݪO�s�p�t��]",0,
   board, 0,          "CClass        [���������ݪO]",0,
   New, 0,            "NNew          [�Ҧ��ݪO�C��]",0,
   local_board,0,     "LLocal        [�����ݪO�C��]",0,
   good_board,0,      "GGoodBoard    [  �u�}�ݪO  ]",0,
   Favor,PERM_BASIC,  "BBoardFavor   [�ڪ��̷R�ݪO]",0,
favor_edit,PERM_BASIC,"FFavorEdit    [�s��ڪ��̷R]",0,
   ReadSelect, 0,     "SSelect       [  ��ܬݪO  ]",0,
   NULL, 0, NULL,0};

#ifdef HAVE_GAME

/* ----------------------------------------------------- */
/* NetGame menu                                          */
/* ----------------------------------------------------- */

struct MENU netgame_list[] = {
  "SO/xyz.so:x_mj",0,
    "QQkmj      �� �����±N��",1,
  "SO/xyz.so:x_big2",0,
    "BBig2      �� �����j�ѤG",1,
  "SO/xyz.so:x_chess",PERM_LOGINOK,
    "CChess     �� �����U�H��",1,
NULL, 0, NULL,0};

int
netgame_menu()
{
  //domenu(NETGAME, "�����s�u�C��", 'Q', netgame_list);
  return 0;
}

/* ----------------------------------------------------- */
/* Game menu                                             */
/* ----------------------------------------------------- */

static struct MENU gamelist[] = {
  rpg_menu,0,
    "RRPG        �� �����t�C��           �I�u��",0,
  netgame_menu,0,
    "NNetGame    �� �����s�u�C��           $100s/��",0,
#if 0
  "SO/gamble.so:ticket_main",PERM_BASIC,
    "GGamble     �� ���ֽ�L             $100s/�i",1,
  "SO/marie.so:mary_m",0,
    "MMarie      �� �p���R�ֶ�             �̧C���O $1s",1,
  "SO/race.so:race",PERM_BASIC,
    "HHorseRace  �� �c�P�䰨��             �̧C���O $1s",1,
  "SO/bingo.so:bingo",PERM_BASIC,
    "BBingo      �� �դ뻫�G��             �̧C���O $1s",1,
#endif
  "SO/gagb.so:gagb",0,
    "G?A?B       �� �q�q�q�Ʀr             �̧C���O $1s",1,
#if 0
  "SO/guessnum.so:fightNum",0,
    "FFightNum   �� ��Բq�Ʀr             �̧C���O $1s",1,
#endif
  "SO/bj.so:BlackJack",0,
    "JJack       �� �դ�³ǧJ             �̧C���O $1s",1,
  "SO/nine.so:p_nine",PERM_BASIC,
    "999         �� �Ѧa�[�E�E             �̧C���O $1s",1,
  "SO/dice.so:x_dice",0,
    "DDice       �� ��K�Խ��             �̧C���O $1s",1,
#if 0
  "SO/gp.so:p_gp",0,
    "PPoke       �� �����J����             �̧C���O $1s",1,
  "SO/pip.so:p_pipple",PERM_LOGINOK,
    "CChicken    �� ���йq�l��             �K�O���A��!!",1,
  "SO/xyz.so:x_tetris",0,
    "TTetris     �� �Xù�����             �K�O���A��",1,
  "SO/mine.so:Mine",0,
    "LLandMine   �� �l�z��a�p             �K�O���A��",1,
  "SO/poker.so:p_dragon",0,
    "11���s      �� ���դ������s           �K�O���A��",1,
#endif
  "SO/chessmj.so:p_chessmj",0,
    "22ChessMJ   �� �H�ѳ±N               �̧C���O $1s",1,
#if 0
  "SO/seven.so:p_seven",0,
    "33Seven     �� �䫰�C�i               �̧C���O $1s",1,
#endif
  "SO/bet.so:p_bet",0,
    "44Bet       �� �ƨg��L               �̧C���O $1s",1,
#if 0
  "SO/stock.so:p_stock",PERM_BASIC,
    "SStock      �� ���Ъѥ�",1,
  x_bridgem,PERM_LOGINOK,"OOkBridge    �i ���P�v�� �j",0,
#endif
NULL, 0, NULL,0};

#endif //HAVE_GAME

/* ----------------------------------------------------- */
/* Talk menu                                             */
/* ----------------------------------------------------- */

int t_users(), t_idle(), t_query(), t_pager();
// t_chat(), t_list(), t_talk();
/* Thor: for ask last call-in message */
int t_display();

static struct MENU talklist[] = {

  t_users,      0,              "LList          [�u�W�W��]",0,
  t_pager,      PERM_BASIC,     "PPager         [�������A]",0,
  t_idle,       0,              "IIdle          [��w�ù�]",0,
  t_query,      0,              "QQueryUser     [�d��User]",0,
/*
  t_talk,       PERM_PAGE,      "TTalk          [��H���]",0,
 */
#ifdef NO_SO
  t_chat, PERM_CHAT,"CChatRoom      [�s�u���]",1,
#else
  "SO/chat.so:t_chat",PERM_CHAT,"CChatRoom      [�s�u���]",1,
#endif
  t_display,    0,              "DDisplay       [���y�^�U]",0,
NULL, 0, NULL,0};

/* ----------------------------------------------------- */
/* User menu                                             */
/* ----------------------------------------------------- */

static struct MENU userlist[] = {
  u_info,       0,              "IInfo          [�ק���]",0,
  u_habit,      PERM_BASIC,     "HHabit         [�ߦn�]�w]",0,
  ListMain,     PERM_LOGINOK,   "LList          [�]�w�W��]",0, 

#ifdef POSTNOTIFY
  re_m_postnotify,PERM_BASIC,   "PPostNotify    [�f�֤峹�q��]",0,
#endif

  u_editfile,   PERM_LOGINOK,   "FFileEdit      [�ӤH�ɮ�]",0,
  u_cloak,      PERM_CLOAK,     "CCloak         [���αK�k]",0,

#ifdef REG_FORM
  u_register,   PERM_POST,      "RRegister      [����U��]",0,
#endif

#ifdef REG_MAGICKEY
  u_verify, 	PERM_BASIC,	"VVerify        [����U�X]",0,
#endif

  u_list,       PERM_BASIC,     "UUsers         [���U�W��]",0,
NULL, 0, NULL,0};

/* ----------------------------------------------------- */
/* Service menu                                          */
/* ----------------------------------------------------- */

#ifdef HAVE_GAME
int
game_list()
{
  domenu(GAME, "�����C�ֳ�", 'R', gamelist);
  return 0;
}
#endif

static struct MENU servicelist[] = {
#ifdef HAVE_GAME
  game_list,    0,              "PPlay          [�|�֤���]",0,
#endif
#ifdef NO_SO
   all_vote,    PERM_LOGINOK,   "VVote          [�벼����]",1,
#else
  "SO/vote.so:all_vote",
                PERM_LOGINOK,   "VVote          [�벼����]",1,
#endif
  note,         PERM_LOGINOK,   "NNote          [�g�d���O]",0,
  show_hint_message,0,          "HHint          [�оǺ��F]",0,

  NULL, 0, NULL,0};

/* ----------------------------------------------------- */
/* mail menu                                             */
/* ----------------------------------------------------- */
int m_new(), m_read(), m_send(),m_sysop(),mail_mbox(),mail_all(),mail_list();

#ifdef INTERNET_PRIVATE_EMAIL
int m_internet();
#endif

static struct MENU maillist[] = {
  m_new,        PERM_READMAIL,  "RNew           [�\\Ū�s�H]",0,
  m_read,       PERM_READMAIL,  "RRead          [�H��C��]",0,
  m_send,       PERM_BASIC,     "SSend          [�����H�H]",0,
  mail_list,    PERM_BASIC,     "MMailist       [�s�ձH�H]",0,
  m_internet,   PERM_INTERNET,  "IInternet      [�����l��]",0,
  m_sysop,      0,              "OOp Mail       [�ԴA����]",0,
  mail_mbox,    PERM_INTERNET,  "ZZip           [���]���]",0,
  mail_all,     PERM_SYSOP,     "AAll           [�t�γq�i]",0,
NULL, 0, NULL,0};



/* ----------------------------------------------------- */
/* main menu                                             */
/* ----------------------------------------------------- */

static int
admin()
{
  domenu(ADMIN, "�����Ѥj", 'X', adminlist);
  return 0;
}

static int
BOARD()
{
  domenu(CLASS, "�ݪO�C��", 'G', classlist);
  return 0;
}

static int
Mail()
{
  domenu(MAIL, "�l����", 'R', maillist);
  return 0;
}

int
static Talk()
{
  domenu(TMENU, "��ѿ��", 'L', talklist);
  return 0;
}

static int
User()
{
  domenu(UMENU, "�ӤH�]�w", 'H', userlist);
  return 0;
}


static int
Service()
{
  domenu(PMENU, "�U�تA��", 'H', servicelist);
  return 0;
}


int Announce(), Boards(), Goodbye(),Log(),board();


struct MENU cmdlist[] = {
  admin,        PERM_ADMIN,     "00Admin        [�t�κ޲z]",0,
  Announce,     0,              "AAnnounce      [�Ѧa���]",0,
  BOARD,        0,              "BBoard         [�ݪO�\\��]",0,
  board,        0,              "CClass         [�����ݪO]",0,
  Mail,         PERM_BASIC,     "MMail          [���жl��]",0,
  Talk,         0,              "TTalk          [�ͤѻ��a]",0,
  User,         PERM_BASIC,     "UUser          [�ӤH�u��]",0,
  Service,	PERM_LOGINOK,	"SService       [�U�تA��]",0,
  Log,          0,              "HHistory       [���v�y��]",0,
  Goodbye,      0,              "GGoodbye       [���t�d��]",0,
NULL, 0, NULL,0};
/* INDENT ON */
