/*-------------------------------------------------------*/
/* config.h     ( WD-BBS Ver 2.3 )                         */
/*-------------------------------------------------------*/
/* target : site-configurable settings                   */
/* create : 95/03/29                                     */
/* update : 98/12/09                                     */
/*-------------------------------------------------------*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "../wdbbs.conf"                              /*r2.20180502: 試行更有彈性的設定檔 */

/* ----------------------------------------------------- */
/* 定義 BBS 站名位址                                     */
/* ------------------------------------------------------*/

#ifndef BOARDNAME
#define BOARDNAME       "風與塵埃的對話 fork 版本"   /* 中文站名 */
#endif

#ifndef BBSNAME
#define BBSNAME         "WD-fork BBS"                /* 英文站名 */
#endif

#ifndef MYHOSTNAME
#define MYHOSTNAME      "wdbbs.net"                  /* 網址 */
#endif

#ifndef MYIP
#define MYIP            "140.129.79.234"             /* IP */
#endif

#define MYVERSION       "Fork WD"                    /* 程式版本 */

#ifndef MYBBSID
#define MYBBSID         "WD-F"                       /* 轉信代碼 */
#endif

#ifndef BBSHOME
#define BBSHOME         "/home/bbs"                  /* BBS 的家 */
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
                        /* shakalaca.000814: 改用 MagicKey 認證
                                             其實就不用這個玩意了 :p */
#endif

/* ----------------------------------------------------- */
/* 組態規劃                                              */
/* ------------------------------------------------------*/

#ifndef __CYGWIN__
#define HAVE_CHKLOAD                /* check cpu load */
                                    /* r2.20180502: cygwin may not check cpu load */
#endif // __CYGWIN__

#ifdef HAVE_CHKLOAD
  #define MAX_CPULOAD     (10)            /* CPU 最高load */
  #define MAX_SWAPUSED    (10)            /* SWAP最高使用率 */
#endif

#ifndef PLATFORM_NAME

  #if defined(__FreeBSD__)
    #define PLATFORM_NAME "FreeBSD"
  #elif defined (__CYGWIN__)
    #define PLATFORM_NAME "Cygwin on MS Windows"
  #elif defined (__linux__)
    #define PLATFORM_NAME "Linux"
  #else
    #define PLATFORM_NAME "Unknown"
  #endif

#endif

#if defined(__CYGWIN__)
#define NO_SO  //不支援 DSO
#endif

#define WITHOUT_CHROOT                /* 不需要 root set-uid */
#define HAVE_MOVIE                    /* 顯示動態告示板 */
#define INTERNET_PRIVATE_EMAIL        /* 可以寄私人信件到 Internet */
#define BIT8                          /* 支援中文系統 */
#define GB_SUPPORT                    /* 支援顯示 繁體->簡體中文碼 */
#define DEFAULTBOARD "SYSOP"          /* 預設看板 */
#define LOGINATTEMPTS (3)             /* 最大進站失誤次數 */
#define LOGINASNEW                    /* 採用上站申請帳號制度 */
#define INTERNET_EMAIL                /* 支援 InterNet Email 功能(含 Forward) */
#undef  REG_MAGICKEY                  /* 發出 MagicKey eMail 身份認證信函 */
#define REG_FORM                      /* shakalaca: 填註冊單 */
                                      /* shakalaca.000813: 認證方式請擇一 */
#undef  NEWUSER_LIMIT                 /* 新手上路的三天限制 */
#undef  POSTNOTIFY                    /* 新文章通知功能 */
#undef  INVISIBLE                     /* 隱藏 BBS user 來自何處 */
#define MULTI_NUMS        (1)         /* 最多重複上站人次 (guest除外) */
#define INITIAL_SETUP                 /* 剛開站，還沒建立預設看板[SYSOP] */
#define HAVE_MMAP                     /* 採用 mmap(): memory mapped I/O */
#define HAVE_ANONYMOUS                /* 提供 Anonymous 板 */
#define HAVE_ORIGIN                   /* 顯示 author 來自何處 */
#define HAVE_MAILCLEAN                /* 清理所有使用者個人信箱 */
#define WHERE                         /* 是否有故鄉功能 */
#define HAVE_NOTE_2                   /* wildcat:小看板功能 */
#define HAVE_GEM_GOPHER               /* shakalaca: 連線精華區 */
#define FORM_REG                      /* shakalaca: 填註冊單 */

/* ----------------------------------------------------- */
/* 隨 BBS 站規模成長而擴增                               */
/* ----------------------------------------------------- */

#ifndef MAXUSERS
#define MAXUSERS        (65536)         /* 最高註冊人數 */
#endif

#ifndef MAXBOARD
#define MAXBOARD        (512)           /* 最大開版個數 */
#endif

#ifndef MAXACTIVE
#define MAXACTIVE       (256)           /* 最多同時上站人數 */
#endif

#define MAX_FRIEND      (128)           /* 載入 cache 之最多朋友數目 */
#define MAX_REJECT      (32)            /* 載入 cache 之最多壞人數目 */
#define MAX_MOVIE       (256)           /* 最多動態看版數 */
#define MAX_FROM        (512)           /* 最多故鄉數 */
#define MAX_REVIEW      (10)            /* 最多水球回顧 */

/* ----------------------------------------------------- */
/* 其他系統上限參數                                      */
/* ----------------------------------------------------- */

#define MAX_HISTORY     8                /* 動態看板保持 12 筆歷史記錄 */
#define MAXKEEPMAIL     (100)            /* 最多保留幾封 MAIL？ */
#define MAXEXKEEPMAIL   (400)            /* 最多信箱加大多少封 */
#define MAX_NOTE        (32)             /* 最多保留幾篇留言？ */
#define MAXSIGLINES     (16)             /* 簽名檔引入最大行數 */
#define MAXQUERYLINES   (16)             /* 顯示 Query/Plan 訊息最大行數 */
#define MAXPAGES        (999)            /* more.c 中文章頁數上限 (lines/22) */
#define MOVIE_INT       (8)              /* 切換動畫的週期時間 12 秒 */
#define MAXTAGS         (256)                /* post/mail reader 標籤數目之上限 */

/* ----------------------------------------------------- */
/* 發呆過久自動簽退                                      */
/* ------------------------------------------------------*/

#define LOGIN_TIMEOUT        (30)                /* login 時多久沒成功簽入就斷線 */

#define DOTIMEOUT

#ifdef  DOTIMEOUT
#define IDLE_TIMEOUT    (100*60)        /* 一般情況之 timeout (秒)*/
#define SHOW_IDLE_TIME                  /* 顯示閒置時間 */
#endif

/* ----------------------------------------------------- */
/* chat.c & xchatd.c 中採用的 port 及 protocol           */
/* ------------------------------------------------------*/

#ifndef CHATPORT
#define CHATPORT        3838
#endif

#define MAXROOM         16              /* 最多有幾間包廂？ */

#define EXIT_LOGOUT     0
#define EXIT_LOSTCONN   -1
#define EXIT_CLIERROR   -2
#define EXIT_TIMEDOUT   -3
#define EXIT_KICK       -4

#define CHAT_LOGIN_OK       "OK"
#define CHAT_LOGIN_EXISTS   "EX"
#define CHAT_LOGIN_INVALID  "IN"
#define CHAT_LOGIN_BOGUS    "BG"

#define BADCIDCHARS " *:/\,;.?"        /* Chat Room 中禁用於 nick 的字元 */


/* ----------------------------------------------------- */
/* 系統參數      cache                                   */
/* ----------------------------------------------------- */

#define MAGIC_KEY       1999   /* 身分認證信函編碼 */

#define SEM_ENTER      -1      /* enter semaphore */
#define SEM_LEAVE      1       /* leave semaphore */
#define SEM_RESET      0       /* reset semaphore */

#ifndef BRDSHM_KEY
#define BRDSHM_KEY      1215 
#endif

#ifndef UIDSHM_KEY
#define UIDSHM_KEY      1217 
#endif

#ifndef UTMPSHM_KEY
#define UTMPSHM_KEY     1219 
#endif

#ifndef FILMSHM_KEY
#define FILMSHM_KEY     1221    /* 動態看版 , 節日 */ 
#endif

#ifndef FROMSHM_KEY
#define FROMSHM_KEY     1223    /* whereis, 最多使用者 */ 
#endif

#ifndef BRDSEM_KEY
#define BRDSEM_KEY      2005 
#endif

#ifndef FILMSEM_KEY
#define FILMSEM_KEY     2000    /* semaphore key */ 
#endif

#ifndef FROMSEM_KEY
#define FROMSEM_KEY     2003    /* semaphore key */ 
#endif


/* ----------------------------------------------------- */
/* 申請帳號時要求申請者真實資料                          */
/* ----------------------------------------------------- */

#define SHOWUID                 /* 看見使用者 UID */
#define SHOWTTY                 /* 看見使用者 TTY */
#define SHOWPID                 /* 看見使用者 PID */

#define REALINFO                /* 真實姓名 */

#ifdef  REALINFO
#undef  ACTS_REALNAMES          /* 主目錄的 (U)ser 顯示真實姓名 */
#undef  POST_REALNAMES          /* 貼文件時附上真實姓名 */
#undef  MAIL_REALNAMES          /* 寄站內信件時附上真實姓名 */
#undef  QUERY_REALNAMES         /* 被 Query 的 User 告知真實姓名 */
#endif

/* ----------------------------------------------------- */
/* WD 自訂定義�                                          */
/* ----------------------------------------------------- */

#define ANNOUNCE_BRD        "Announce"
#define VOTEBOARD           "VoteBoard"

#define DEF_MAXP        5000        /* 看板文章基本上限數量 */
#define DEF_MAXT        365        /* 看板文章基本保留天數 */

#define COLOR1        "\x1b[46;37m"
#define COLOR2        "\x1b[1m\x1b[44;33m"

#ifndef BANNER
#define BANNER  "【"BOARDNAME"】◎ 電子布告欄系統 ◎ ("MYIP")\r\nPowered by "PLATFORM_NAME"\r\n\n"
#endif

#endif //_CONFIG_H_
