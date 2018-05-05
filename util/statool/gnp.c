#include <sys/param.h>
#include "bbs.h"


#define  PASSWD BBSHOME"/.PASSWDS"
#define  OUT BBSHOME"/etc/GNP"

int coun=0,count=0;
userec cuser;

char *
Cdate(clock)
  time_t *clock;
{
  static char foo[22];
  struct tm *mytm = localtime(clock);

  strftime(foo, 22, "%D %T %a", mytm);
  return (foo);
}



int
bad_user_id()
{
  register char ch;
  int j;

  if (strlen(cuser.userid) < 2 || !isalpha(cuser.userid[0]))
    return 1;

  if (cuser.numlogins==0)
    return 1;

  for(j=1;ch = cuser.userid[j];j++)
  {
    if (!isalnum(ch))
      return 1;
  }

  return 0;
}

main()
{
  FILE *fp1=fopen(PASSWD,"r");
  FILE *fp2=fopen(OUT,"w");
  double total1=0,total2=0;
  float gnp1,gnp2;
  float tgnp1=0;
  float tgnp2=0;
  time_t now = time(0);
  int days=0;
  while( (fread( &cuser, sizeof(cuser), 1, fp1))>0 ) 
  {
   count ++;
   if(bad_user_id())
   {
    printf("<%d> userid:%s  silver:%d  gold: %d\n",
            count,cuser.userid,cuser.silvermoney,cuser.goldmoney);
   }         
   else	
   {
      coun ++;
      total1+=(cuser.goldmoney);    
      total2+=(cuser.silvermoney);    
      days+=cuser.numlogins;
      gnp1= (cuser.goldmoney)/cuser.numlogins;
      tgnp1+=gnp1;  
      gnp2= (cuser.silvermoney)/cuser.numlogins;
      tgnp2+=gnp2;  
   } 
  }
 fclose(fp1);
 fprintf(fp2,"    [1m-----[[32m%s[37m]-----[m\n",Cdate(&now)); 
 fprintf(fp2,"    本站目前總人口   : [1m%11d[0;36m 人\n",coun);
 fprintf(fp2,"    [1;33m本站人民總金幣   : [1m%11.0f [0;36m元\n",total1);
 fprintf(fp2,"    [1;33m平均人民金幣     : [1m%11d[0;36m 元[m\n", (long long)total1/coun);
 fprintf(fp2,"    [1;33m國民生產毛額(金) : [1m%11f[0;36m 元[m\n", tgnp1/days);
 fprintf(fp2,"    [1;37m本站人民總銀幣   : [1m%11.0f [0;36m元\n",total2);
 fprintf(fp2,"    [1;37m平均人民銀幣     : [1m%11d[0;36m 元[m\n", (long long)total2/coun);
 fprintf(fp2,"    [1;37m國民生產毛額(銀) : [1m%11f[0;36m 元[m\n", tgnp2/days);
 fprintf(fp2,"    [1m---------------------------------[m\n");
 fclose(fp2);
}