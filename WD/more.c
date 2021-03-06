/*-------------------------------------------------------*/
/* more.c       ( NTHU CS MapleBBS Ver 2.36 )            */
/*-------------------------------------------------------*/
/* target : simple & beautiful ANSI/Chinese browser      */
/* create : 95/03/29                                     */
/* update : 95/12/15                                     */
/*-------------------------------------------------------*/


#include "bbs.h"

#define MORE_BUFSIZE    4096
#define MORE_WINSIZE    4096    /* window size : 2048 or 4096 */

#define STR_ANSICODE    "[0123456789;,"

int beep = 0;
static uschar more_pool[MORE_BUFSIZE];
static int more_base, more_size, more_head;

        
char *
Ptt_prints(char* str,int mode)
{
  char *po , *px, strbuf[512];

  while (po = strstr(str, "\x1b[12"))
  {
    po[0] = 0;
  }
  while (po = strstr(str, "\x1b[10"))
  {
    po[0] = 0;
  }
  while (po = strstr(str, "\x1bn"))
  {
    po[0] = 0;
  }
  while (po = strstr(str, "\x1b]"))
  {
    po[0] = 0;
  }
  while (po = strstr(str, "\x1b[="))
  {
    po[0] = 0;
  }
  while (po = strstr(str, "\x1b*"))
  {
    switch(*(po+2))
    {
     case 'S' :
        *po = 0;
        px = po + 3;
        sprintf(strbuf, "%s%s", str, BOARDNAME);
        break;

     case 's' :
        *po = 0;
        px = po + 3;
        sprintf(strbuf, "%s%s", str, cuser.userid);
        break;

     case 't' :
       {
        time_t now = time(0);
        *po = 0;
        px = po + 3;
        sprintf(strbuf, "%s%s", str, Etime(&now));
        break;
       }

     case 'u' :
       {
        int attempts;
        extern struct UTMPFILE *utmpshm;
        attempts = utmpshm->number;
        *po = 0;
        px = po + 3;
        sprintf(strbuf,"%s%d", str, attempts);
        break;
       }

     case 'b' :
        *po = 0;
        px = po + 3;
        sprintf(strbuf,"%s%d/%d", str, cuser.month, cuser.day, px);
        break;

     case 'l' :
        *po = 0;
        px = po + 3;
        sprintf(strbuf, "%s%d", str, cuser.numlogins);
        break;

     case 'p' :
        *po = 0;
        px = po + 3;
        sprintf(strbuf, "%s%d",  str, cuser.numposts);
        break;

     case 'n' :
        *po = 0;
        px = po + 3;
        sprintf(strbuf, "%s%s", str, cuser.username);
        break;

     default :
        *po = 0;
        px = NULL;
        break;
   }

   if (px)
   {
     strcat(strbuf, px);
     strcpy(str, strbuf);
   }

  }
  return str;
}


static inline void
more_goto(fd, off)
  int fd;
  off_t off;
{
  int base = more_base;

  if (off < base || off >= base + more_size)
  {
    more_base = base = off & (-MORE_WINSIZE);
    lseek(fd, base, SEEK_SET);
    more_size = read(fd, more_pool, MORE_BUFSIZE);
  }
  more_head = off - base;
}


// alan.010503: maple 3.x的more_line
static int
readln(fd, buf)
  int fd;
  uschar *buf;
{
  int ch;
  uschar *data, *tail;
  int len, ansilen, bytes, in_ansi;
  int size, head;

  len = ansilen = bytes = in_ansi = 0;
  /* tail = buf + ANSILINELEN - 1; */
  /* Thor.991008: 為防止引言後, ">"要變色, 一行會超過ANSILINELEN, 故多留空間 */
  tail = buf + ANSILINELEN - 11;
  
  size = more_size;
  head = more_head;
  data = &more_pool[head];

  do
  {
    if (head >= size)
    {
      more_base += size;
      data = more_pool;
      more_size = size = read(fd, data, MORE_BUFSIZE);

      if (size == 0)		/* end of file */
	break;

      head = 0;
    }

    ch = *data++;
    head++;
    bytes++;
    if (ch == '\n')
    {
      break;
    }

    if (ch == '\t')
    {
      do
      {
	*buf++ = ' ';
      } while ((++len & 7) && len < 80);
    }
    else if (ch == '\a')
    {
      beep = 1;
    }
    else if (ch == KEY_ESC)
    {
      if (showansi)
	*buf++ = ch;
      in_ansi = 1;
    }
    else if (in_ansi)
    {
      if (showansi)
	*buf++ = ch;
      if (!strchr(STR_ANSICODE, ch))
	in_ansi = 0;
    }
    else if (isprint2(ch))
    {
      len++;
      *buf++ = ch;
    }
  } while (len < 80 && buf < tail);
  *buf = '\0';
  more_head = head;
  return bytes;
}


int
more(fpath, promptend)
  char *fpath;
  int promptend;
{
  extern char* strcasestr();
  static char *head[4] = 
    {"\x1b[1;36m╭ \x1b[46;33m作者\x1b[m", 
     "\x1b[1;36m│ \x1b[46;33m標題\x1b[m", 
     "\x1b[1;36m│ \x1b[46;33m時間\x1b[m" ,
     "\x1b[1;36m│ \x1b[46;33m轉信\x1b[m"};
  char *ptr, *word, buf[1024],*ch1;
  struct stat st;
  usint pagebreak[MAXPAGES], pageno, lino;
  int fd, fsize, line, ch, viewed, pos, numbytes;
  int header = 0;
  int local = 0;
  char search_char0=0;
  static char search_str[81]="";
  typedef char* (*FPTR)();
  static FPTR fptr;
  int searching = 0;
  int scrollup = 0;
  char *http[80]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
                  NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
                        /* Ptt */
  char pagemode = 0;
  char pagecount = 0;

  inmore = 1;
  memset(pagebreak, 0, sizeof(pagebreak));
  if (*search_str)
     search_char0 = *search_str;
  *search_str = 0;

  fd = open(fpath, O_RDONLY);
  if (fd < 0)
    return -1;

  if (fstat(fd, &st) || (fsize = st.st_size) <= 0)
  {
    close(fd);
    return -1;
  }

  pagebreak[0] = pageno = viewed = line = pos = more_base = more_head = more_size = 0;
  clear();

  while ((numbytes = readln(fd, buf)) || (line == t_lines))
  {
    if (scrollup) {
       rscroll();
       move(0, 0);
    }
    if (numbytes)               /* 一般資料處理 */
    {
      if (!viewed)              /* begin of file */
      {
        if (showansi)           /* header processing */
        {
          if (!strncmp(buf, str_author1, LEN_AUTHOR1))
          {
            line = 3;
            word = buf + LEN_AUTHOR1;
            local = 1;
          }
          else if (!strncmp(buf, str_author2, LEN_AUTHOR2))
          {
            line = 4;
            word = buf + LEN_AUTHOR2;
          }

          while (pos < line)
          {
            if (!pos && ((ptr = strstr(word, str_post1)) ||
                (ptr = strstr(word, str_post2))))
            {
              ptr[-1] = '\0';
              prints("%s%-52.52s",head[0], word);
              prints("\x1b[1;46;33m%.4s\x1b[0;1;33m%-13s\x1b[1;36m╮\x1b[0m\n", ptr, ptr + 5);
            }
            else if (pos < (local ? 3 : 4))
              prints("%s%-69.69s\x1b[1;36m│\x1b[0m\n", head[pos], word);

            viewed += numbytes;
            numbytes = readln(fd, buf);

            if(!pos && viewed >= 79)            /* 第一行太長了 */
            {
              if (memcmp( buf, head[1], 2))     /* 第二行不是 [標....] */
              {
                viewed += numbytes;             /* 讀下一行進來處理 */
                numbytes = readln(fd, buf);
              }
            }

            pos++;
          }
          if (pos)
          {
            header = 1;

            prints("\x1b[1;36m%s\x1b[m\n", msg_sep3);
//            prints("\n");
            line = pos = (local ? 4 : 5);
          }
        }
        lino = pos;
        word = NULL;
      }
//     if(strstr(buf,"This is a multi-part message in MIME format"))
//       decode = 1;
//     if(decode)
       str_decode(buf);

      /* ※處理引用者 & 引言 */

      if ((buf[1] == ' ') && (buf[0] == ':' || buf[0] == '>'))
        word = "\x1b[36m";
      if ((buf[1] == ' ') && (buf[0] == ':' || buf[0] == '>') && (buf[2] == ':' || buf[2] == '>'))
        word = "\x1b[33m";
      if ((buf[0] == '�' && buf[1] == '�') || !strncmp(buf, "==>", 3))
        word = "\x1b[1;36m";

      ch1 = buf ;
      while(1) /* Ptt */
        {
          int i;
          char e,*ch2;
          if(ch2 = strstr(ch1,"gopher://"));
          else if(ch2 = strstr(ch1,"mailto:"));
// 支援直接進入看板
          else if(ch2 = strstr(ch1,"board://"));
          else break;
          for(e=0; ch2[e] != ' ' && ch2[e] != '\n' && ch2[e] != '\0'
           && ch2[e] != '"' && ch2[e] != ';' && ch2[e] != ']'; e++);
          for(i=0;http[i] && i<80;i++)
                if(!strncmp(http[i],ch2,e) && http[e]==0) break;
          if(!http[i])
                {
                 http[i] = (char *) malloc( e+1 );
                 strncpy(http[i],ch2,e);
                 http[i][e]=0;
                 pagecount++;
                }
          ch1 = &ch2[7];
        }

      if (word)
        outs(word);
      {
/*
woju
*/
         char msg[500], *pos;

        if (*search_str && (pos = fptr(buf, search_str))) {
           char SearchStr[41];
           char buf1[100], *pos1;

           strncpy(SearchStr, pos, strlen(search_str));
           SearchStr[strlen(search_str)] = 0;
           searching = 0;
           sprintf(msg, "%.*s\x1b[7m%s\x1b[0m", pos - buf, buf,
              SearchStr);
           while (pos = fptr(pos1 = pos + strlen(search_str), search_str)) {
              sprintf(buf1, "%.*s\x1b[7m%s\x1b[0m", pos - pos1, pos1, SearchStr);
              strcat(msg, buf1);
           }
           strcat(msg, pos1);
           outs(Ptt_prints(msg,NO_RELOAD));
        }
        else {
           outs(Ptt_prints(buf,NO_RELOAD));
        }
      }
      if (word) {
        outs("\x1b[0m");
        word = NULL;
      }
      outch('\n');

      if (beep)
      {
        bell();
        beep = 0;
      }

      if (line < b_lines)       /* 一般資料讀取 */
        line++;

      if (line == b_lines && searching == -1) {
        if (pageno > 0)
          more_goto(fd, viewed=pagebreak[--pageno]);
        else
           searching = 0;
        lino = pos = line = 0;
        clear();
        continue;
      }

      if (scrollup) {
         move(line = b_lines, 0);
         clrtoeol();
         for (pos = 1; pos < b_lines; pos++)
            viewed += readln(fd, buf);
      }
      else if (pos == b_lines)  /* 捲動螢幕 */
        scroll();
      else
        pos++;


      if (!scrollup && ++lino >= b_lines && pageno < MAXPAGES - 1)
      {
        pagebreak[++pageno] = viewed;
        lino = 1;
      }

      if (scrollup) {
         lino = scrollup;
         scrollup = 0;
      }
      viewed += numbytes;       /* 累計讀過資料 */
    }
    else
      line = b_lines;           /* end of END */


    if (promptend && (!searching && line == b_lines || viewed == st.st_size))
    {
      /* Kaede 剛好 100% 時不停 */
/*
      if (viewed == st.st_size && viewed - numbytes == pagebreak[1])
        continue;
*/
      move(b_lines, 0);
      if (viewed == st.st_size) {
         if (searching == 1)
            searching = 0;
/*
woju
*/
      }
      else if (pageno == 1 && lino == 1) {
         if (searching == -1)
            searching = 0;
      }
      prints(COLOR2"  瀏覽 P.%d(%d%%)  ", pageno,(viewed * 100) / st.st_size);
#ifdef HYPER_BBS
      prints(COLOR1" \x1b[1m\x1b[33m(^Z)\x1b[37m求助 \x1b[33m→\x1b[200m\x1b[505m↓\x1b\x1b[201m");
      prints("\x1b[200m\x1b[500m[PgUp]\x1b[201m\x1b[200m\x1b[501m[PgDn]\x1b[201m\x1b[200m\x1b\x1b[502m[Home]\x1b[201m");
      prints("\x1b[200m\x1b[503m[End]\x1b[201m  \x1b[200m\x1b[506m\x1b[33m←\x1b[37m結束\x1b[201m  \x1b[m\n");
#else
      prints(COLOR1" \x1b[1m\x1b[33m (^Z)\x1b[37m求助 \
\x1b[33m→↓ [PgUp][PgDn][Home][End]\x1b[33m ←(q)\x1b[37m結束   \x1b[m\n");
#endif
      move(b_lines,0);
      while (line == b_lines || (line > 0 && viewed == st.st_size))
      {
        switch (ch = igetkey())
        {
        case ':': {
           char buf[10];
           int i = 0;

           getdata(b_lines - 1, 0, "Goto Page: ", buf, 5, DOECHO,0);
           sscanf(buf, "%d", &i);
           if (0 < i && i <  MAXPAGES && (i == 1 || pagebreak[i - 1]))
              pageno = i - 1;
           else if (pageno)
              pageno--;
           lino = line = 0;
           break;
        }

        case '/': {
           char ans[4] = "n";
           *search_str = search_char0;
           getdata(b_lines - 1, 0,"[搜尋]關鍵字:", search_str, 40, DOECHO,0);
           if (*search_str) {
              searching = 1;
              if (getdata(b_lines - 1, 0, "區分大小寫(Y/N/Q)? [N] ", ans, 4, LCECHO,0) && *ans == 'y')
                 fptr = strstr;
              else
                 fptr = strcasestr;
           }
           if (*ans == 'q')
              searching = 0;
           if (pageno)
              pageno--;
           lino = line = 0;
           break;
        }
        case 'n':
           if (*search_str) {
              searching = 1;
              if (pageno)
                 pageno--;
              lino = line = 0;
           }
           break;
        case 'N':
           if (*search_str) {
              searching = -1;
              if (pageno)
                 pageno--;
              lino = line = 0;
           }
           break;
        case 'r':
        case 'R':
        case 'Y':
           close(fd);
           inmore = 0;
           return 7;
        case 'y':
           close(fd);
           inmore = 0;
           return 8;
        case 'A':
           close(fd);
           inmore = 0;
           return 9;
        case 'a':
           close(fd);
           inmore = 0;
           return 10;
        case 'F':
           close(fd);
           inmore = 0;
           return 11;
        case 'B':
           close(fd);
           inmore = 0;
           return 12;
        case KEY_LEFT:
          close(fd);
          inmore = 0;
          return 6;
        case 'q':
          close(fd);
          inmore = 0;
          return 0;
        case 'b':
           close(fd);
           inmore = 0;
           return 1;
        case 'f':
           close(fd);
           inmore = 0;
           return 3;
        case ']':       /* Kaede 為了主題閱讀方便 */
           close(fd);
           inmore = 0;
           return 4;
        case '[':       /* Kaede 為了主題閱讀方便 */
           close(fd);
           inmore = 0;
           return 2;
        case '=':       /* Kaede 為了主題閱讀方便 */
           close(fd);
           inmore = 0;
           return 5;
        case Ctrl('F'):
        case KEY_PGDN:
          line = 1;
          break;
        case 't':
          if (viewed == st.st_size) {
             close(fd);
             inmore = 0;
             return 4;
          }
          line = 1;
          break;
        case ' ':
          if (viewed == st.st_size) {
             close(fd);
             inmore = 0;
             return 3;
          }
          line = 1;
          break;
        case KEY_RIGHT:
          if (viewed == st.st_size) {
             close(fd);
             inmore = 0;
             return 0;
          }
          line = 1;
          pagemode = 0;
          break;
        case '\r':
        case '\n':
          if (pagemode){
                 more_web(http[pagemode-1],YEA);
                 /*pagebreak[0] = pageno = viewed = line = pos = 0;*/
                pagemode = 0;
                *search_str = 0;
                 if (pageno)
                    pageno--;
                 lino = line = 0;
                 break;
                }

        case KEY_DOWN:
          if (viewed == st.st_size ||
              promptend == 2 && (ch == '\r' || ch == '\n')) {
             close(fd);
             inmore = 0;
             return 3;
          }
          line = t_lines - 2;
          break;

        case '$':
        case 'G':
        case KEY_END:
          line = t_lines;
          break;

        case '0':
        case 'g':
        case KEY_HOME:
          pageno = line = 0;
          break;

        case 'E':
          if (strcmp(fpath, "etc/ve.hlp")) {
             close(fd);
             inmore = 0;
             vedit(fpath, HAS_PERM(PERM_SYSOP) ? 0 : 2);
             return 0;
          }
          break;

        case KEY_ESC:
           if (KEY_ESC_arg == 'n') {
              edit_note();
              if (pageno)
                 pageno--;
              lino = line = 0;
           }
           else if (KEY_ESC_arg == 'c')
              capture_screen();
           break;

        case Ctrl('I'):
           if(!pagecount) break;
           pagemode = (pagemode % pagecount) + 1;
           strcpy(search_str,http[pagemode-1]);
           fptr = strstr;
           if (pageno)
              pageno--;
           lino = line = 0;
           break;

        case KEY_UP:
           line = -1;
           break;

        case Ctrl('B'):
        case KEY_PGUP:
          if (pageno > 1)
          {
            if (lino < 2)
               pageno -= 2;
            else
               pageno--;
            lino = line = 0;
          }
          else if (pageno && lino > 1)
            pageno = line = 0;
          break;
        case Ctrl('H'):
          if (pageno > 1)
          {
            if (lino < 2)
               pageno -= 2;
            else
               pageno--;
            lino = line = 0;
          }
          else if (pageno && lino > 1)
            pageno = line = 0;
/*
woju
*/
          else {
             close(fd);
             inmore = 0;
             return 1;
          }
        }
      }

      if (line > 0)
      {
        move(b_lines, 0);
        clrtoeol();
        refresh();
      }
      else if (line < 0) {                      /* Line scroll up */
         if (pageno <= 1) {
            if (lino == 1 || !pageno) {
               close(fd);
               inmore = 0;
               return 1;
            }
            if (header && lino <= 5) {
                more_goto(fd, viewed = pagebreak[scrollup = lino = pageno = 0] = 0);
               clear();
            }
         }
         if (pageno && lino > 1 + local) {
            line =  (lino - 2) - local;
            if (pageno > 1 && viewed == st.st_size)
               line += local;
            scrollup = lino - 1;
            more_goto(fd, viewed = pagebreak[pageno - 1]);
            while (line--)
               viewed += readln(fd, buf);
         }
         else if (pageno > 1) {
            scrollup = b_lines - 1;
            line = (b_lines - 2) - local;
            more_goto(fd, viewed = pagebreak[--pageno - 1]);
            while (line--)
               viewed += readln(fd, buf);
         }
         line = pos = 0;
      }
      else
      {
        pos = 0;
        more_goto(fd, viewed = pagebreak[pageno]);
        clear();
      }
    }
  }

  close(fd);
  if (promptend)
  {
    pressanykey(NULL);
    clear();
  }
  else
    outs(reset_color);
    
  inmore = 0;
  return 0;
}


int
more_web(fpath, promptend)
  char *fpath;
  int promptend;
{
  char *ch;
  char genbuf[512]; 

  if(ch=strstr(fpath,"mailto:"))
    {
        if(!HAS_PERM(PERM_LOGINOK))
           {
             move(b_lines - 1,0);
             outs("\x1b[41m 您的權限不足無法使用internet mail... \x1b[m");
             refresh();
             return 0;
           }
        if(!not_addr(&ch[7]) &&
               getdata(b_lines - 1, 0, "[寄信]主題：", genbuf, 40, DOECHO,0))
           {
                  do_send(&ch[7], genbuf);
           }
        else
           {
             move(b_lines - 1,0);
             outs("\x1b[41m 收信人email 或 標題 有誤... \x1b[m");
             refresh();
           }
        return 0;
    }
#if 0
  if(ch=strstr(fpath,"gopher://"))
    {
          ITEM item;
          strcpy(item.X.G.server, &ch[9]);
          strcpy(item.X.G.path, "1/");
          item.X.G.port = 70;
          gem(fpath , &item, 0);
        return 0;
    }
#endif
// wildcat : 支援直接進入看板 
  if(ch=strstr(fpath,"board://"))
  {
    char bname[20],bpath[60], oldch[STRLEN];
    struct stat st;
    int mode0 = currutmp->mode;
    int stat0 = currstat;
    int pos;
    boardheader *bhdr,*getbcache();

    strcpy(oldch, ch);
    strcpy(bname, strtok(oldch + 8, "#"));
    setbpath(bpath,bname);   
    if ((*bname == '\0') || (stat (bpath, &st) == -1))
    {
      pressanykey (err_bid);
      return RC_FULL;
    }

    if(bhdr = getbcache(bname))
    {
      if(Ben_Perm(bhdr) != 1)
      {
        pressanykey("你沒有進入該板的權限");
        return 0;
      }
    }
    else
    {
      pressanykey("你沒有進入該板的權限");
      return 0;
    }
/*
    setbfile (buf, bname, FN_LIST);
    
    if ((currbrdattr & BRD_HIDE) && belong_list (buf, cuser.userid) <= 0)
    {
      pressanykey(P_BOARD);
      return 0;
    }
*/
/* shakalaca.000123: 支援看某一篇 */    
    if (ch = strstr(fpath, "#"))
    {
      fileheader fhdr;
      
      pos = atoi(ch + 1);
      setbdir(bpath, bname);
      rec_get(bpath, &fhdr, sizeof(fileheader), pos);
      setbfile(bpath, bname, fhdr.filename);
      more(bpath);
    }
    else
    {
    /* shakalaca.000124: 解決 "未讀" 問題.. */
      brc_initial (bname);
      Read();  
    }

    currutmp->mode = mode0;
    currstat = stat0;
    return 0;
  }
}
