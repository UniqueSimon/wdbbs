/*-------------------------------------------------------*/
/* config.h     ( WD-BBS Ver 2.3 )                         */
/*-------------------------------------------------------*/
/* target : site-configurable settings                   */
/* create : 95/03/29                                     */
/* update : 98/12/09                                     */
/*-------------------------------------------------------*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "../wdbbs.conf"                              /*r2.20180502: �զ�󦳼u�ʪ��]�w�� */

/* ----------------------------------------------------- */
/* �w�q BBS ���W��}                                     */
/* ------------------------------------------------------*/

#ifndef BOARDNAME
#define BOARDNAME       "���P�ЮJ����� fork ����"   /* ���寸�W */
#endif

#ifndef BBSNAME
#define BBSNAME         "WD-fork BBS"                /* �^�寸�W */
#endif

#ifndef MYHOSTNAME
#define MYHOSTNAME      "wdbbs.net"                  /* ���} */
#endif

#ifndef MYIP
#define MYIP            "140.129.79.234"             /* IP */
#endif

#define MYVERSION       "Fork WD"                    /* �{������ */

#ifndef MYBBSID
#define MYBBSID         "WD-F"                       /* ��H�N�X */
#endif

#ifndef BBSHOME
#define BBSHOME         "/home/bbs"                  /* BBS ���a */
#endif

#ifndef BBSUSER
#define BBSUSER         "bbs"
#endif

#ifndef BBSUID
#define BBSUID          9999
#endif

#ifndef BBSGID
#define BBSGID          99
#endif

#ifndef TAG_VALID
#define TAG_VALID        "[WD-F BBS]To "        
                        /* shakalaca.000814: ��� MagicKey �{��
                                             ���N���γo�Ӫ��N�F :p */
#endif

/* ----------------------------------------------------- */
/* �պA�W��                                              */
/* ------------------------------------------------------*/

#ifndef __CYGWIN__
#define HAVE_CHKLOAD                /* check cpu load */
                                    /* r2.20180502: cygwin may not check cpu load */
#endif // __CYGWIN__

#ifdef HAVE_CHKLOAD
  #define MAX_CPULOAD     (10)            /* CPU �̰�load */
  #define MAX_SWAPUSED    (10)            /* SWAP�̰��ϥβv */
#endif

#define WITHOUT_CHROOT                /* ���ݭn root set-uid */
#define HAVE_MOVIE                    /* ��ܰʺA�i�ܪO */
#define INTERNET_PRIVATE_EMAIL        /* �i�H�H�p�H�H��� Internet */
#define BIT8                          /* �䴩����t�� */
#define GB_SUPPORT                    /* �䴩��� �c��->²�餤��X */
#define DEFAULTBOARD "SYSOP"          /* �w�]�ݪO */
#define LOGINATTEMPTS (3)             /* �̤j�i�����~���� */
#define LOGINASNEW                    /* �ĥΤW���ӽбb����� */
#define INTERNET_EMAIL                /* �䴩 InterNet Email �\��(�t Forward) */
#undef  REG_MAGICKEY                  /* �o�X MagicKey eMail �����{�ҫH�� */
#define REG_FORM                      /* shakalaca: ����U�� */
                                      /* shakalaca.000813: �{�Ҥ覡�оܤ@ */
#undef  NEWUSER_LIMIT                 /* �s��W�����T�ѭ��� */
#undef  POSTNOTIFY                    /* �s�峹�q���\�� */
#undef  INVISIBLE                     /* ���� BBS user �Ӧۦ�B */
#define MULTI_NUMS        (1)         /* �̦h���ƤW���H�� (guest���~) */
#define INITIAL_SETUP                 /* ��}���A�٨S�إ߹w�]�ݪO[SYSOP] */
#define HAVE_MMAP                     /* �ĥ� mmap(): memory mapped I/O */
#define HAVE_ANONYMOUS                /* ���� Anonymous �O */
#define HAVE_ORIGIN                   /* ��� author �Ӧۦ�B */
#define HAVE_MAILCLEAN                /* �M�z�Ҧ��ϥΪ̭ӤH�H�c */
#define WHERE                         /* �O�_���G�m�\�� */
#define HAVE_NOTE_2                   /* wildcat:�p�ݪO�\�� */
#define HAVE_GEM_GOPHER               /* shakalaca: �s�u��ذ� */
#define FORM_REG                      /* shakalaca: ����U�� */

/* ----------------------------------------------------- */
/* �H BBS ���W�Ҧ������X�W                               */
/* ----------------------------------------------------- */

#ifndef MAXUSERS
#define MAXUSERS        (65536)         /* �̰����U�H�� */
#endif

#ifndef MAXBOARD
#define MAXBOARD        (512)           /* �̤j�}���Ӽ� */
#endif

#ifndef MAXACTIVE
#define MAXACTIVE       (256)           /* �̦h�P�ɤW���H�� */
#endif

#define MAX_FRIEND      (128)           /* ���J cache ���̦h�B�ͼƥ� */
#define MAX_REJECT      (32)            /* ���J cache ���̦h�a�H�ƥ� */
#define MAX_MOVIE       (256)           /* �̦h�ʺA�ݪ��� */
#define MAX_FROM        (512)           /* �̦h�G�m�� */
#define MAX_REVIEW      (10)            /* �̦h���y�^�U */

/* ----------------------------------------------------- */
/* ��L�t�ΤW���Ѽ�                                      */
/* ----------------------------------------------------- */

#define MAX_HISTORY     8                /* �ʺA�ݪO�O�� 12 �����v�O�� */
#define MAXKEEPMAIL     (100)            /* �̦h�O�d�X�� MAIL�H */
#define MAXEXKEEPMAIL   (400)            /* �̦h�H�c�[�j�h�֫� */
#define MAX_NOTE        (32)             /* �̦h�O�d�X�g�d���H */
#define MAXSIGLINES     (16)             /* ñ�W�ɤޤJ�̤j��� */
#define MAXQUERYLINES   (16)             /* ��� Query/Plan �T���̤j��� */
#define MAXPAGES        (999)            /* more.c ���峹���ƤW�� (lines/22) */
#define MOVIE_INT       (8)              /* �����ʵe���g���ɶ� 12 �� */
#define MAXTAGS         (256)                /* post/mail reader ���Ҽƥؤ��W�� */

/* ----------------------------------------------------- */
/* �o�b�L�[�۰�ñ�h                                      */
/* ------------------------------------------------------*/

#define LOGIN_TIMEOUT        (30)                /* login �ɦh�[�S���\ñ�J�N�_�u */

#define DOTIMEOUT

#ifdef  DOTIMEOUT
#define IDLE_TIMEOUT    (100*60)        /* �@�뱡�p�� timeout (��)*/
#define SHOW_IDLE_TIME                  /* ��ܶ��m�ɶ� */
#endif

/* ----------------------------------------------------- */
/* chat.c & xchatd.c ���ĥΪ� port �� protocol           */
/* ------------------------------------------------------*/

#define CHATPORT        3838

#define MAXROOM         16              /* �̦h���X���]�[�H */

#define EXIT_LOGOUT     0
#define EXIT_LOSTCONN   -1
#define EXIT_CLIERROR   -2
#define EXIT_TIMEDOUT   -3
#define EXIT_KICK       -4

#define CHAT_LOGIN_OK       "OK"
#define CHAT_LOGIN_EXISTS   "EX"
#define CHAT_LOGIN_INVALID  "IN"
#define CHAT_LOGIN_BOGUS    "BG"

#define BADCIDCHARS " *:/\,;.?"        /* Chat Room ���T�Ω� nick ���r�� */


/* ----------------------------------------------------- */
/* �t�ΰѼ�      cache                                   */
/* ----------------------------------------------------- */
#define MAGIC_KEY       1999   /* �����{�ҫH��s�X */

#define SEM_ENTER      -1      /* enter semaphore */
#define SEM_LEAVE      1       /* leave semaphore */
#define SEM_RESET      0       /* reset semaphore */

#define BRDSHM_KEY      1215
#define UIDSHM_KEY      1217
#define UTMPSHM_KEY     1219
#define FILMSHM_KEY     1221    /* �ʺA�ݪ� , �`�� */
#define FROMSHM_KEY     1223    /* whereis, �̦h�ϥΪ� */

#define BRDSEM_KEY      2005
#define FILMSEM_KEY     2000    /* semaphore key */
#define FROMSEM_KEY     2003    /* semaphore key */

/* ----------------------------------------------------- */
/* �ӽбb���ɭn�D�ӽЪ̯u����                          */
/* ----------------------------------------------------- */

#define SHOWUID                 /* �ݨ��ϥΪ� UID */
#define SHOWTTY                 /* �ݨ��ϥΪ� TTY */
#define SHOWPID                 /* �ݨ��ϥΪ� PID */

#define REALINFO                /* �u��m�W */

#ifdef  REALINFO
#undef  ACTS_REALNAMES          /* �D�ؿ��� (U)ser ��ܯu��m�W */
#undef  POST_REALNAMES          /* �K���ɪ��W�u��m�W */
#undef  MAIL_REALNAMES          /* �H�����H��ɪ��W�u��m�W */
#undef  QUERY_REALNAMES         /* �Q Query �� User �i���u��m�W */
#endif

/* ----------------------------------------------------- */
/* WD �ۭq�w�q�                                          */
/* ----------------------------------------------------- */

#define ANNOUNCE_BRD        "Announce"
#define VOTEBOARD           "VoteBoard"

#define DEF_MAXP        5000        /* �ݪO�峹�򥻤W���ƶq */
#define DEF_MAXT        365        /* �ݪO�峹�򥻫O�d�Ѽ� */

#define COLOR1        "\x1b[46;37m"
#define COLOR2        "\x1b[1m\x1b[44;33m"

#endif //_CONFIG_H_
