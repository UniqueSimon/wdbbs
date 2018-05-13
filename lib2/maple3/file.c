#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include "daom3.h"

void
f_cat(fpath, msg)
  char *fpath;
  char *msg;
{
  int fd;

  if ((fd = open(fpath, O_WRONLY | O_CREAT | O_APPEND, 0600)) >= 0)
  {
    strcat(msg,"\n");
    write(fd, msg, strlen(msg));
    close(fd);
  }
}


int
f_cp(src, dst, mode)
  char *src, *dst;
  int mode;			/* O_EXCL / O_APPEND / O_TRUNC */
{
  int fsrc, fdst, ret;

  ret = 0;

  if ((fsrc = open(src, O_RDONLY)) >= 0)
  {
    ret = -1;

    if ((fdst = open(dst, O_WRONLY | O_CREAT | mode, 0600)) >= 0)
    {
      char pool[BLK_SIZ];

      src = pool;
      do
      {
	ret = read(fsrc, src, BLK_SIZ);
	if (ret <= 0)
	  break;
      } while (write(fdst, src, ret) > 0);
      close(fdst);
    }
    close(fsrc);
  }
  return ret;
}

char *
f_img(fpath, fsize)
  char *fpath;
  int *fsize;
{
  int fd, size;
  struct stat st;

  if ((fd = open(fpath, O_RDONLY)) < 0)
    return NULL;

  fpath = NULL;

  if (!fstat(fd, &st) && S_ISREG(st.st_mode) && (size = st.st_size) > 0
    && (fpath = (char *) malloc(size)))
  {
    *fsize = size;
    if (read(fd, fpath, size) != size)
    {
      free(fpath);
      fpath = NULL;
    }
  }

  close(fd);
  return fpath;
}

/* ----------------------------------------------------- */
/* f_ln() : link() cross partition / disk		 */
/* ----------------------------------------------------- */

int
f_ln(src, dst)
  char *src, *dst;
{
  int ret;

  if ( ( ret = link(src, dst) ) )
  {
    if (errno != EEXIST)
      ret = f_cp(src, dst, O_EXCL);
  }
  return ret;
}

static struct flock fl = {
  .l_whence = SEEK_SET,
  .l_start = 0,
  .l_len = 0,
};

int
f_exlock(fd)
  int fd;
{
#if 0
  return flock(fd, LOCK_EX); 
#endif
  /* Thor.981205: �� fcntl ���Nflock, POSIX�зǥΪk */
  fl.l_type = F_WRLCK;
  /* Thor.990309: with blocking */
  return fcntl(fd, F_SETLKW /*F_SETLK*/, &fl);
}

int
f_unlock(fd)
  int fd;
{
#if 0
  return flock(fd, LOCK_UN); 
#endif
  /* Thor.981205: �� fcntl ���Nflock, POSIX�зǥΪk */
  fl.l_type = F_UNLCK;
  return fcntl(fd, F_SETLKW /*F_SETLK*/, &fl);
}



#ifdef MAP_FILE         /* 44BSD defines this & requires it to mmap files */
#  define DAO_MAP       (MAP_SHARED | MAP_FILE)
#else
#  define DAO_MAP       (MAP_SHARED)
#endif


char *
f_map(fpath, fsize)
  char *fpath;
  int *fsize;
{
  int fd, size;
  struct stat st;

  if ((fd = open(fpath, O_RDONLY)) < 0)
    return (char *) -1;

  if (fstat(fd, &st) || !S_ISREG(st.st_mode) || (size = st.st_size) <= 0)
  {
    close(fd);
    return (char *) -1;
  }

  fpath = (char *) mmap(NULL, size, PROT_READ, DAO_MAP, fd, 0);
  close(fd);
  *fsize = size;
  return fpath;
}


int
f_mode(fpath)
  char *fpath;
{
  struct stat st;

  if (stat(fpath, &st))
    return 0;

  return st.st_mode;
}

int
f_mv(src, dst)
  char *src, *dst;
{
  int ret;

  if ( ( ret = rename(src, dst) ) )
  {
    ret = f_cp(src, dst, O_TRUNC);
    if (!ret)
      unlink(src);
  }
  return ret;
}
/* ----------------------------------------------------- */
/* exclusively create file [*.n]			 */
/* ----------------------------------------------------- */

FILE *
f_new(fold, fnew)
  char *fold;
  char *fnew;
{
  int fd, try;
  extern int errno;

  try = 0;
  str_cat(fnew, fold, ".n");

  for (;;)
  {
    fd = open(fnew, O_WRONLY | O_CREAT | O_EXCL, 0600);

    if (fd >= 0)
      return fdopen(fd, "w");

    if (errno != EEXIST)
      break;

    if (!try++)
    {
      struct stat st;

      if (stat(fnew, &st) < 0)
	break;
      if (st.st_mtime < time(NULL) - 20 * 60)	/* ���] 20 ���������ӳB�z�� */
	unlink(fnew);
    }
    else
    {
      if (try > 24)		/* ���� 120 ���� */
	break;
      sleep(5);
    }
  }
  return NULL;
}

int
f_open(fpath)
  char *fpath;
{
  int fd;
  struct stat st;

  if ((fd = open(fpath, O_RDONLY)) >= 0)
  {
    if (fstat(fd, &st) || st.st_size <= 0 || !S_ISREG(st.st_mode))
    {
      close(fd);
      unlink(fpath);
      fd = -1;
    }
  }

  return fd;
}

/* ----------------------------------------------------- */
/* file structure : set file path for boards/user home	 */
/* ----------------------------------------------------- */

static void
mak_fpath(str, key, name)
  char *str;
  char *key;
  char *name;
{
  int cc;

  cc = '/';
  for (;;)
  {
    *str = cc;
    if (!cc)
      break;
    str++;
    cc = *key++;

#if 0
    if (cc >= 'A' && cc <= 'Z')
      cc |= 0x20;
#endif
  }

  if (name)
  {
    *str++ = '/';
    strcpy(str, name);
  }
}


void
brd_fpath(fpath, board, fname)
  char *fpath;
  char *board;
  char *fname;
{
  *fpath++ = 'b';
  *fpath++ = 'r';
  *fpath++ = 'd';
  mak_fpath(fpath, board, fname);
}


void
gem_fpath(fpath, board, fname)
  char *fpath;
  char *board;
  char *fname;
{
  *fpath++ = 'g';
  *fpath++ = 'e';
  *fpath++ = 'm';
  mak_fpath(fpath, board, fname);
}


void
usr_fpath(fpath, user, fname)
  char *fpath;
  char *user;
  char *fname;
{
  char buf[16];

  *fpath++ = 'u';
  *fpath++ = 's';
  *fpath++ = 'r';
  *fpath++ = '/';

  str_lower(buf, user);		/* lower case */
  *fpath++ = *buf;
  mak_fpath(fpath, buf, fname);
}

static int rm_dir();

int
f_rm(fpath)
  char *fpath;
{
  struct stat st;

  if (stat(fpath, &st))
    return -1;

  if (!S_ISDIR(st.st_mode))
    return unlink(fpath);

  return rm_dir(fpath);
}

static int
rm_dir(fpath)
  char *fpath;
{
  struct stat st;
  DIR *dirp;
  struct dirent *de;
  char buf[256], *fname;

  if (!(dirp = opendir(fpath)))
    return -1;

  for (fname = buf; (*fname = *fpath); fname++, fpath++)
    ;

  *fname++ = '/';

  readdir(dirp);
  readdir(dirp);

  while ( ( de = readdir(dirp) ) )
  {
    fpath = de->d_name;
    if (*fpath)
    {
      strcpy(fname, fpath);
      if (!stat(buf, &st))
      {
	if (S_ISDIR(st.st_mode))
	  rm_dir(buf);
	else
	  unlink(buf);
      }
    }
  }
  closedir(dirp);

  *--fname = '\0';
  return rmdir(buf);
}

void
f_suck(fp, fpath)
  FILE *fp;
  char *fpath;
{
  int fd;

  if ((fd = open(fpath, O_RDONLY)) >= 0)
  {
    char pool[BLK_SIZ];
    int size;

    fpath = pool;
    while ((size = read(fd, fpath, BLK_SIZ)) > 0)
    {
      fwrite(fpath, size, 1, fp);
    }
    close(fd);
  }
}
