int
is_alnum(ch)
  int ch;
{
  return ((ch >= '0' && ch <= '9') ||
    (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}
int
is_alpha(ch)
  int ch;
{
  return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}
#include <string.h>
#include "daoop.h"
	
int is_fname(str)
  char *str;
{
  int ch;

  ch = *str;
  if (ch == '/')
    return 0;

  do
  {
    if (!is_alnum(ch) && !strchr("-._/+@", ch))
      return 0;
  } while ( (ch = *++str) );
  return 1;
}
/* ----------------------------------------------------- */
/* transform to real path & security check		 */
/* ----------------------------------------------------- */


int
is_fpath(path)
  char *path;
{
  int ch, level;
  char *source, *target;

  level = 0;
  source = target = path;


  for (;;)
  {
    ch = *source;

    if (ch == '/')
    {
      int next;

      next = source[1];

      if (next == '/')
      {
	return 0;		/* [//] */
      }
      else if (next == '.')
      {
	next = source[2];

	if (next == '/')
	  return 0;		/* [/./] */

	if (next == '.' && source[3] == '/')
	{
	  /* -------------------------- */
	  /* abc/xyz/../def ==> abc/def */
	  /* -------------------------- */

	  for (;;)
	  {
	    if (target <= path)
	      return 0;

	    target--;
	    if (*target == '/')
	      break;
	  }

	  source += 3;
	  continue;
	}
      }

      level++;
    }

    *target = ch;

    if (ch == 0)
      return level;

    target++;
    source++;
  }
}
#include "daoop.h"

#define	STRICT_FQDN_EMAIL

int
not_addr(addr)
  char *addr;
{
  int ch, mode;

  mode = -1;

  while ( (ch = *addr) )
  {
    if (ch == '@')
    {
      if (++mode)
	break;
    }

#ifdef	STRICT_FQDN_EMAIL
    else if ((ch != '.') && (ch != '-') && (ch != '_') && !is_alnum(ch))
#else
    else if (!is_alnum(ch) && !strchr(".-_[]%!:", ch))
#endif

      return 1;

    addr++;
  }

  return mode;
}
