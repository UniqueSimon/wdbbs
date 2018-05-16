/*-------------------------------------------------------*/
/* modes.h      ( NTHU CS MapleBBS Ver 2.36 )            */
/*-------------------------------------------------------*/
/* target : user operating mode & status                 */
/* create : 95/03/29                                     */
/* update : 95/12/15                                     */
/*-------------------------------------------------------*/

#ifndef _MODES_H_
#define _MODES_H_

/* ----------------------------------------------------- */
/* strip ansi mode   Ptt                                 */
/* ----------------------------------------------------- */

enum
{STRIP_ALL, ONLY_COLOR, NO_RELOAD};

/* ----------------------------------------------------- */
/* money mode   wildcat                                  */
/* ----------------------------------------------------- */

enum
{SILVER,GOLD};

/* ----------------------------------------------------- */
/* �s�զW��Ҧ�   Ptt                                    */
/* ----------------------------------------------------- */

#define FRIEND_OVERRIDE 0
#define FRIEND_REJECT   1
#define FRIEND_ALOHA    2
#define FRIEND_POST     3         
#define FRIEND_SPECIAL  4
#define FRIEND_CANVOTE  5
#define BOARD_WATER     6
#define BOARD_VISABLE   7 
#define FRIEND_MAILLIST	8
#define FRIEND_MAILREPLYLIST	9



/* ----------------------------------------------------- */
/* user �ާ@���A�P�Ҧ�                                   */
/* ----------------------------------------------------- */

#define IDLE            0
#define MMENU           1       /* menu mode */
#define ADMIN           2
#define MAIL            3
#define TMENU           4
#define UMENU           5
#define XMENU           6
#define CLASS           7
#define PMENU           8
#define NMENU           9
#define POSTING         10      /* boards & class */
#define READBRD         11
#define READING         12
#define READNEW         13
#define SELECT          14
#define RMAIL           15      /* mail menu */
#define SMAIL           16
#define CHATING         17      /* talk menu */
#define XMODE           18
#define FRIEND          19
#define LAUSERS         20
#define LUSERS          21
#define MONITOR         22
#define PAGE            23
#define QUERY           24
#define TALK            25
#define EDITPLAN        26      /* user menu */
#define EDITSIG         27
#define VOTING          28
#define XINFO           29
#define MSYSOP          30
#define LOG		31
#define REPLY           32
#define HIT             33
#define DBACK           34
#define NOTE            35
#define EDITING         36
#define MAILALL         37
#define LOGIN           38       /* main menu */
#define DICT            39
#define EDITEXP         40
#define ANNOUNCE        41       /* announce */
#define EDNOTE          42 
#define	POWERBOOK	43
#define	PRECORD		44
#define	MSGMENU		45
#define NoteMoney	46
#define LISTMAIN	47
#define LISTEDIT	48

/* ----------------------------------------------------- */
/* menu.c �����Ҧ�                                       */
/* ----------------------------------------------------- */

#define XEASY   0x333           /* Return value to un-redraw screen */
#define QUIT    0x666           /* Return value to abort recursive functions */


/* ----------------------------------------------------- */
/* read.c �����Ҧ�                                       */
/* ----------------------------------------------------- */

/* Read menu command return states */

#define RC_NONE		0	/* nothing to do */
#define RC_FULL		0x0100	/* Entire screen was destroyed in this oper */
#define RC_BODY		0x0200	/* Only the top three lines not destroyed */
#define RC_DRAW		0x0400
#define RC_FOOT		0x0800
#define RC_ITEM		0x1000
#define RC_CHDIR	0x2000	/* Index file was changed */
#define RC_NEWDIR	0x4000	/* Directory has changed, re-read files */

#define RS_FORWARD      0x01    /* backward */
#define RS_TITLE        0x02    /* author/title */
#define RS_RELATED      0x04
#define RS_FIRST        0x08    /* find first article */
#define RS_CURRENT      0x10    /* match current read article */
#define RS_THREAD       0x20    /* search the first article */
#define RS_AUTHOR       0x40    /* search author's article */
#define RS_SCORE	0x80	/* article score */

#define RS_NEXT         0x8000  /* Direct read next file */
#define RS_PREV         0x10000 /* Direct read prev file */

#define CURSOR_FIRST    (RS_RELATED | RS_TITLE | RS_FIRST)
#define CURSOR_NEXT     (RS_RELATED | RS_TITLE | RS_FORWARD)
#define CURSOR_PREV     (RS_RELATED | RS_TITLE)
#define RELATE_FIRST    (RS_RELATED | RS_TITLE | RS_FIRST | RS_CURRENT)
#define RELATE_NEXT     (RS_RELATED | RS_TITLE | RS_FORWARD | RS_CURRENT)
#define RELATE_PREV     (RS_RELATED | RS_TITLE | RS_CURRENT)
#define THREAD_NEXT     (RS_THREAD | RS_FORWARD)
#define THREAD_PREV     (RS_THREAD)
#define AUTHOR_NEXT     (RS_AUTHOR | RS_FORWARD)
#define AUTHOR_PREV     (RS_AUTHOR)

/*
woju
*/
#define POS_NEXT        101     /* cursor_pos(locmem, locmem->crs_ln + 1, 1);*/


/* for currmode */

#define MODE_STARTED    0x01    /* �O�_�w�g�i�J�t�� */
#define MODE_MENU	0x02    /* �O�_�b MENU */
#define MODE_TAG	0x04	/* */
#define	MODE_POST	0x08	/* �O�_�i�H�b currboard �o��峹 */
#define	MODE_BOARD	0x10	/* �O�_�i�H�b currboard �R���Bmark�峹 */
#define	MODE_SECRET	0x20	/* �O�_�� secret board */
#define	MODE_DIGEST	0x40	/* �O�_�� digest mode */
#define	MODE_DIRTY	0x80	/* �O�_��ʹL userflag */
#define MODE_SELECT     0x100 
#define MODE_FAVORITE	0x200	/* �O�_���ߦn�ݪO */
#define MODE_TINLIKE	0x400	/* �O�_�� tin-like �\Ū */

/* for curredit */

#define EDIT_MAIL       1       /* �ثe�O mail/board ? */
#define EDIT_LIST       2       /* �O�_�� mail list ? */
#define EDIT_BOTH       4       /* both reply to author/board ? */
#define EDIT_ITEM	8
#ifdef HAVE_ANONYMOUS
#define EDIT_ANONYMOUS 10       /* �ΦW�Ҧ� */
#endif

/* for Tag List */
#define TAG_COMP        0       /* ��� TagList */
#define TAG_TOGGLE      1       /* ���� TagList */
#define TAG_INSERT      2       /* �[�J TagList */

/* for announce.c */
#define GEM_FMODE         1       /* �ɮ׼Ҧ� */
#define GEM_PERM          2       /* �i�s��� */
#define GEM_TAG           4       /* �O���аO */
#define GEM_LOCK_PATH     8       /* ��w��ذϸ��| */
#define GEM_NET           16      /* ��ذϳs�u�� */
#define GEM_RELOAD        32      /* ��ذ� �j�� Reload */
#define GEM_BM            64	  /* �p�O�D */

#endif                          /* _MODES_H_ */
