/*-------------------------------------------------------*/
/* util/account.c       ( NTHU CS MapleBBS Ver 2.36 )    */
/*-------------------------------------------------------*/
/* target : �W���H���έp�B�t�θ�Ƴƥ�                   */
/* create : 95/03/29                                     */
/* update : 95/12/15                                     */
/*-------------------------------------------------------*/

#include "bbs.h"
#include "proto.h"

/* ------------------------------------------------------------------------- */

// from WD/record.c
// todo: move to common library

void
stampfile(fpath, fh)
  char *fpath;
  fileheader *fh;
{
  register char *ip = fpath;
  time_t dtime;
  struct tm *ptime;
  int fp;

#if 1
  if (access(fpath, X_OK | R_OK | W_OK))
    mkdir(fpath, 0755);
#endif

  time(&dtime);
  while (*(++ip));
  *ip++ = '/';
  do
  {
    sprintf(ip, "M.%d.A", ++dtime );
  } while ((fp = open(fpath, O_CREAT | O_EXCL | O_WRONLY, 0644)) == -1);
  close(fp);
  memset(fh, 0, sizeof(fileheader));
  strcpy(fh->filename, ip);
  ptime = localtime(&dtime);

#if 0// !defined(_BBS_UTIL_C_)
  {//shakalaca.000428: find out the BBSHOME/M.* problem.. :p
    char genbuf[80];
    
    sprintf(genbuf, "MODE:%d", currstat);
    debug(genbuf);
  }
#endif
  sprintf(fh->date, "%2d/%02d", ptime->tm_mon + 1, ptime->tm_mday);
}

// from WD/record.c
// todo: move to common library

/* ------------------------------------------------------------------------- */

#define MAX_LINE        16
#define ADJUST_M        6       /* adjust back 5 minutes */

/* Ptt about share memory */
struct UCACHE *uidshm;
struct FILMCACHE *ptt;
struct FROMCACHE *fcache;

static void
attach_err(shmkey, name)
  int shmkey;
  char *name;
{
  fprintf(stderr, "[%s error] key = %x\n", name, shmkey);
  exit(1);
}
static void *
attach_shm(shmkey, shmsize)
  int shmkey, shmsize;
{
  void *shmptr;
  int shmid;

  shmid = shmget(shmkey, shmsize, 0);
  if (shmid < 0)
  {
    shmid = shmget(shmkey, shmsize, IPC_CREAT | 0600);
    if (shmid < 0)
      attach_err(shmkey, "shmget");
    shmptr = (void *) shmat(shmid, NULL, 0);
    if (shmptr == (void *) -1)
      attach_err(shmkey, "shmat");
    memset(shmptr, 0, shmsize);
  }
  else
  {
    shmptr = (void *) shmat(shmid, NULL, 0);
    if (shmptr == (void *) -1)
      attach_err(shmkey, "shmat");
  }
  return shmptr;
}
void
resolve_ucache()
{
  if (uidshm == NULL)
  {
    uidshm = attach_shm(UIDSHM_KEY, sizeof(*uidshm));
  }
}
void
reset_garbage()
{
  if (ptt == NULL)
  {
    ptt = attach_shm(FILMSHM_KEY, sizeof(*ptt));
    if (ptt->touchtime == 0)
      ptt->touchtime = 1;
  }
  ptt->uptime =0;
}

void
resolve_fcache()
{
  if (fcache == NULL)
  {
    fcache = attach_shm(FROMSHM_KEY, sizeof(*fcache));
    if (fcache->touchtime == 0)
      fcache->touchtime = 1;
  }
  fcache->uptime =0;
}

void
keeplog(fpath, board, title)
char *fpath;
char *board;
char *title;
{
  fileheader fhdr;
  char genbuf[256];
  char *flog;

  if(!board)
    board = "Record";

  sprintf(genbuf, "boards/%s", board);
  stampfile(genbuf, &fhdr);
  f_mv(fpath, genbuf);

  strcpy(fhdr.title, title);
  strcpy(fhdr.owner, "[���v������]");
  sprintf(genbuf, "boards/%s/.DIR", board);
  rec_add(genbuf, &fhdr, sizeof(fhdr));
}


void
account_outs(fp, buf, mode)
  FILE *fp;
  char buf[], mode;
{
  static char state = '0';

  if (state != mode)
    fprintf(fp, "[3%cm", state = mode);
  if (buf[0])
  {
    fprintf(fp, buf);
    buf[0] = 0;
  }
}


void
gzip(source, target, stamp)
  char *source, *target, *stamp;
{
  char buf[128];
  sprintf(buf, "/bin/gzip -9n adm/%s%s", target, stamp);
  rename(source, &buf[14]);
  system(buf);
}

main()
{
  int hour, max, item, total, i,
       j,mo,da,max_user=0,max_login=0,max_reg=0,mahour,k;
  char *act_file = ".act";
  char *log_file = "usies";
  char buf[256],buf1[256], *p;
  FILE *fp,*fp1;
  int act[27];                  /* ����/�֭p�ɶ�/pointer */
  time_t now;
  struct tm *ptime;
  chdir(BBSHOME);
  now = time(NULL) - ADJUST_M * 60;     /* back to ancent */
  ptime = localtime(&now);

  memset(act, 0, sizeof(act));
  printf("����/�֭p�ɶ�\n");
  if ((ptime->tm_hour != 0) && (fp = fopen(act_file, "r")))
  {
    fread(act, sizeof(act), 1, fp);
    fclose(fp);
  }
  if ((fp = fopen(log_file, "r")) == NULL)
  {
    printf("cann't open usies\n");
    if (fp = fopen(act_file, "w"))
    {
      memset(act, 0, sizeof(act));
      fwrite(act, sizeof(act), 1, fp);
      fclose(fp);
    }
    return 1;
  }
  if (act[26])
    fseek(fp, (off_t)(act[26]), 0);
  while (fgets(buf, 256, fp))
  {
    hour = atoi(buf + 9);
    if (hour < 0 || hour > 23)
    {
      continue;
    }
    if (!strncmp(buf + 22, "ENTER", 5))
    {
      act[hour]++;
      continue;
    }
    if (p = (char *) strstr(buf + 40, "Stay:"))
    {
      if (hour = atoi(p + 5))
      {
        act[24] += hour;
        act[25]++;
      }
      continue;
    }
  }
  act[26] = ftell(fp);
  fclose(fp);
  for (i = max = total = 0; i < 24; i++)
  {
    total += act[i];
    if (act[i] > max)
     {
      max_user = max = act[i];
      mahour = i;
     }
  }
  item = max / MAX_LINE + 1;

  if (!ptime->tm_hour)
    keeplog("etc/today", "Record", "[�ЮJ����] �W���H���έp");

  if ((fp = fopen("etc/today", "w")) == NULL)
  {
    printf("cann't open etc/today\n");
    return 1;
  }
  fprintf(fp, "\t\t\t[1m[33;46m �C�p�ɤW���H���έp [%02d/%02d/%02d] [40m\n\n", ptime->tm_year % 100, ptime->tm_mon + 1, ptime->tm_mday);
  for (i = MAX_LINE + 1; i > 0; i--)
  {
    strcpy(buf, "   ");
    for (j = 0; j < 24; j++)
    {
      max = item * i;
      hour = act[j];
      if (hour && (max > hour) && (max - item <= hour))
      {
        account_outs(fp, buf, '7');
        fprintf(fp, "%-3d", hour);
      }
      else if (max <= hour)
      {
        account_outs(fp, buf, '6');
        fprintf(fp, "�i ");
      }
      else
        strcat(buf, "   ");
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "   [33m"
    "0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23\n\n"
    "\t[33m�`�@�W���H���G[37m%-9d[33m�����ϥΤH�ơG[37m%-5d[33m�����ϥήɶ��G[37m %d [33m��[m \n"
    ,total, total/24,act[24]/act[25]+1);
  fclose(fp);

  if (fp = fopen(act_file, "w"))
  {
    fwrite(act, sizeof(act), 1, fp);
    fclose(fp);
  }

  /* -------------------------------------------------------------- */

    sprintf(buf, "-%02d%02d%02d",
      ptime->tm_year % 100, ptime->tm_mon + 1, ptime->tm_mday);

  now += ADJUST_M * 60;         /* back to future */



   /* Ptt ���v�ƥ�B�z */
    if(fp = fopen("etc/history.data","r"))
     {                                  /*�̦h�P�ɤW�u*/
      if(fscanf(fp,"%d %d %d %d",&max_login,&max,&max_reg,&k))
        {
	 int a;
         resolve_fcache();
         printf("���ɬq�̦h�P�ɤW�u:%d �L�h:%d",a=fcache->max_user,k);
         fclose(fp);
         if(a > k)
                {
                  time_t now = time(0); 
                  ptime=localtime(&fcache->max_time);
//                  ptime=localtime(&now);
                  if(fp1=fopen("etc/history", "a"))
                    {
                      fprintf(fp1,
                      "�� �i%02d/%02d/%02d %02d:%02d�j"
                      "[32m�P�ɦb�����H��[m�����F�� [1;36m%d[m �H��\n",
                        ptime->tm_mon+1, ptime->tm_mday, ptime->tm_year % 100,
                        ptime->tm_hour,ptime->tm_min,a);
                      fclose(fp1);
                    }
                   if(fp=fopen("etc/history.data","w"))
                    {
                     fprintf(fp,"%d %d %d %d",max_login,max,max_reg,a);
                    }
                }
        }
        fclose(fp);
     }
  ptime = localtime(&now);

if(ptime->tm_hour)
  {
  printf("�h�Ӹ`��B�z\n");
   /* Ptt �h�Ӹ`��B�z */

      if(fp1 = fopen("etc/today_is","r"));
        {
          char tod[20][100];
          for(i=0;fgets(tod[i],20,fp1) && i<100;i++);
          fclose(fp1);

          fp1 = fopen("etc/today_is","w");
          for(j=1;j<=i;j++)
             {
               fputs(tod[j%i],fp1);
             }
          fclose(fp1);
        }
  /* Ptt reset Ptt's share memory */
  reset_garbage();
  }

if(!ptime->tm_hour)
  {
    keeplog(BBSHOME"/.note", "Record", "[�ЮJ����] �߱��d����");
    keeplog(BBSHOME"/etc/GNP","Record","[���Ĥ���] �Ͳ����B�έp");
    keeplog(BBSHOME"/log/counter/�W���H��","Record","[�t�γ��i] ����H�ƭp�ƾ�");


    keeplog("usies", "Security", "[�t�γ��i] �ϥΪ̤W�u����");
/*    keeplog("log/admin.log", "Security","[�t�γ��i] ����t�ά���"); */
    keeplog(BBSHOME"/log/bm_check", "Record","[�t�γ��i] �O�D�쯸����");
    keeplog(BBSHOME"/log/bank.log", "Security","[���Ĥ���] �����y�ʬ���");
    keeplog(BBSHOME"/log/board_edit", "Security","[�t�γ��i] �ݪO��ʬ���");
    gzip("usboard", "usboard", buf);
    gzip("dlog", "dlog", buf);
    gzip("usies", "usies", buf);

    system("/bin/cp etc/today etc/yesterday");
    system("rm -f note.dat");

/* Ptt ���v�ƥ�B�z */
    now = time(NULL) - ADJUST_M * 60;     /* back to ancent */
    ptime = localtime(&now);

    resolve_ucache();
    if(fp = fopen("etc/history.data","r"))
     {                                  /* ���̦h���H��,�P�ɤW�u,���U*/
      if(fscanf(fp,"%d %d %d %d",&max_login,&max,&max_reg,&k))
       {
        fp1 = fopen("etc/history","r+");
        fseek(fp1,(off_t)0,2);
        if ( max_user > max )
          {
      fprintf(fp1,"�� �i%02d/%02d/%02d %02d�j   "
            "[1;32m��@�p�ɤW�u�H��[m�����F�� [1;35m%d[m �H�� \n"
             ,ptime->tm_mon+1,ptime->tm_mday,ptime->tm_year % 100,mahour,max_user);
             max = max_user;
          }
        if ( total > max_login )
          {
      fprintf(fp1,"�� �i%02d/%02d/%02d�j      "
            "[1;32m���W�u�H��[m�����F��[1;33m %d[m �H��   \n"
             ,ptime->tm_mon+1,ptime->tm_mday,ptime->tm_year % 100,total);
              max_login = total;
          }

        if ( uidshm->number > max_reg + max_reg/10)
          {
      fprintf(fp1,"�� �i%02d/%02d/%02d�j      "
            "[1;32m�`���U�H��[m���ɨ�[1;31m %d[m �H   \n"
             ,ptime->tm_mon+1,ptime->tm_mday,ptime->tm_year % 100,uidshm->number);
              max_reg = uidshm->number;
          }

        fclose(fp1);
       }
      fclose(fp);
      fp = fopen("etc/history.data","w");
      fprintf(fp,"%d %d %d %d",max_login,max,max_reg,k);
      fclose(fp);
     }
  now += ADJUST_M * 60;         /* back to future */
  ptime = localtime(&now);

/* Ptt �`��B�z */
    i=0;
  if(fp1 = fopen("etc/today_is","w"))
    {
     if(fp = fopen("etc/feast","r"))
      {
       while (fscanf(fp,"%d %d %s\n",&mo,&da,buf1) != EOF)
        {
         if(ptime->tm_mday == da && ptime->tm_mon + 1 == mo)
           {
              i =1;
              fprintf(fp1,"%-14s\n",buf1);
           }
       }
      fclose(fp);
     }
   if(i==0)
     {
       if(fp = fopen("etc/today_boring","r"))
       {
         while (fgets(buf1,250,fp))
         {
          if(strlen(buf)>3)
           {
            buf1[strlen(buf1)-1]=0;
            fprintf(fp1,"%-14s\n",buf1);
           }
         }
         fclose(fp);
       }
       else
       {
         fprintf(fp1,"����`��x�D��");
       }
     }
     fclose(fp1);
    }

/* Ptt �w��e���B�z */
/*
    if(fp = fopen("etc/Welcome.date","r"))
    {
     char temp[50];
      while (fscanf(fp,"%d %d %s\n",&mo,&da,buf1) != EOF)
      {
        if(ptime->tm_mday == da && ptime->tm_mon + 1 == mo)
          {
                strcpy(temp,buf1);
                sprintf(buf1,"cp -f etc/Welcomes/%s etc/Welcome",temp);
                system(buf1);
                break;
          }
      }
     fclose(fp);
    }
*/
    if (ptime->tm_wday == 0)
    {
      keeplog("log/week", "Record", "[�ЮJ����] ���g�������D");
      keeplog("log/trade.log", "Record","[���Ĥ���] �ӫ~�c�檬�p");

      keeplog("log/race.log", "LocalGame","[�C�ֳ�] ���g�ɰ�����");
      keeplog("log/pip.log", "LocalGame","[�C�ֳ�] ���g�q�l���}�i����");
      keeplog("log/bingo.log", "LocalGame","[�C�ֳ�] ���g���G����");
      keeplog("log/stock.log", "LocalGame","[�C�ֳ�] ���g�ѥ�����");
      keeplog("log/gp.log", "LocalGame","[�C�ֳ�] ���g�����J����");
      keeplog("log/dice.log", "LocalGame","[�C�ֳ�] ���g��K�Ԭ���");
      keeplog("log/marie.log", "LocalGame","[�C�ֳ�] ���g�p���R����");
      keeplog("log/nine.log", "LocalGame","[�C�ֳ�] ���g�E�E����");
      keeplog("log/ab.log", "LocalGame","[�C�ֳ�] ���g�q�Ʀr����");
      keeplog("log/bj.log", "LocalGame","[�C�ֳ�] ���g�³ǧJ����");
      keeplog("log/chessmj.log", "LocalGame","[�C�ֳ�] ���g�H�ѳ±N����");
      keeplog("log/seven.log", "LocalGame","[�C�ֳ�] ���g�䫰�C�i����");

      gzip("bbslog", "bntplink", buf);
      gzip("innd/bbslog", "innbbsd", buf);
      gzip("log/bbsmail.log", "mailog", buf);
    }

    if (ptime->tm_mday == 1)
    {
      keeplog("log/month", "Record", "[�ЮJ����] ����������D");
      keeplog("etc/topboard", "Record", "[�ЮJ����] ����ݪO�Ʀ�");
    }
    if (ptime->tm_yday == 1)
      keeplog("log/year", "Record", "[�ЮJ����] �~�׼������D");
  }
  else if (ptime->tm_hour == 3 && ptime->tm_wday == 6)
  {
    char *fn1 = "tmp";
    char *fn2 = "suicide";
    rename(fn1, fn2);
    mkdir(fn1, 0755);
    sprintf(buf, "/bin/gtar cfz adm/%s-%02d%02d%02d.tgz %s",
      fn2, ptime->tm_year % 100, ptime->tm_mon + 1, ptime->tm_mday, fn2);
    system(buf);
    sprintf(buf, "/bin/rm -fr %s", fn2);
    system(buf);
  }
}