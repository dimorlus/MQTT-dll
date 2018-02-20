//---------------------------------------------------------------------------
#include <string.h>


#pragma hdrstop

#include "strnum.h"
#pragma warn -8060
#pragma package(smart_init)

//---------------------------------------------------------------------------
int strLen(char *str)
{
 int i=0;
 while(str[i]) i++;
 return i;
}
//---------------------------------------------------------------------------
//delete n chars from p position of the string
int dels(char *str, int p, int n)
{
 int res = 0;
 while(p--) if (!*str++) return 0;
 char *sstr = str;
 while(n--) if (!*++sstr) break;
 else res++;
 while(*str++ = *sstr++);
 *str = '\0';
 return res;
}
//----------------------------------
//insert char at the beginning of string
int ins(char *str, char c)
{
 int l = strlen(str)+1;
 while(l) {str[l] = str[l-1];l--;}
 str[0] = c;
 return 1;
}
//----------------------------------
//insert char at the position p of string
int pins(char *str, char c, int p)
{
 int l = strlen(str)+1;
 if (p < l)
  {
   while(l>p) {str[l] = str[l-1];l--;}
   str[p] = c;
   return 1;
  }
 else return 0;
}
//----------------------------------
//insert string at the beginning of string
int inss(char *str, const char *ins)
{
 int ll = strlen(str)+1;
 int li = strlen(ins);
 int res = li;
 while(li)
  {
   int l = ll++;
   while(l) {str[l] = str[l-1];l--;}
   str[0] = ins[li-- -1];
  }
 return res;
}
//----------------------------------
//insert string at the the position p of string
int pinss(char *str, const char *ins, int p)
{
 int ll = strlen(str)+1;
 int li = strlen(ins);
 int res = li;
 if (p < ll)
  {
   while(li)
    {
     int l = ll++;
     while(l>p) {str[l] = str[l-1];l--;}
     str[p] = ins[li-- -1];
    }
   return res;
  }
 else return 0;
}
//----------------------------------
//add char to the end of string
int add(char *str, char c)
{
 while(*str) str++;
 *str++ = c;
 *str = '\0';
 return 1;
}
//----------------------------------
//add new string to the end of exist
int adds(char *str, const char *add)
{
 int res=0;
 while(*str) str++;
 while ((*str++ = *add++)) res++;
 *str = '\0';
 return res;
}
//----------------------------------
//int to string
int itos(char *str, int i)
{
 int res = 0;
 str[0] = '\0';
 unsigned int u;
 if (i < 0) u = -i;
 else u = i;
 if (u)
  while(u)
   {
    char c = '0'+u%10;
    res += ins(str, c);
    u /= 10;
   }
 else res += ins(str, '0');
 if (i < 0) res += ins(str, '-');
 return res;
}
//----------------------------------
//int to string with adjustment
int itosa(char *str, int i, int adj)
{
 int res = itos(str, i);
 if (adj > 0) while (res < adj) res += ins(str, ' ');
 else
 if (adj < 0) while (res < -adj) res += add(str, ' ');
 return res;
}
//----------------------------------
//adjust string
int sadj(char *str, int adj)
{
 int res = strlen(str);
 if (adj > 0) while (res < adj) res += ins(str, ' ');
 else
 if (adj < 0) while (res < -adj) res += add(str, ' ');
 return res;
}
//----------------------------------
//int to hex
int itoh(char *str, int i)
{
 int res = 0;
 str[0] = '\0';
 unsigned int u;
 if (i < 0) u = -i;
 else u = i;
 if (u)
  while(u)
   {
    char c = '0'+u%16;
    if (c > '9') c += ('a'-'9'-1);//39
    res += ins(str, c);
    u /= 16;
   }
 else res += ins(str, '0');
 if (i < 0) res += ins(str, '-');
 return res;
}
//----------------------------------
//int to n hex digits
int itox(char *str, unsigned int u, int n)
{
 int res = 0;
 str[0] = '\0';
 while(n)
  {
   char c = '0'+u%16;
   if (c > '9') c += ('a'-'9'-1);//39
   res += ins(str, c);
   u /= 16;
   n--;
  }
 return res;
}
//----------------------------------
//floating point double to string
int dtostr(char *str, double d, int decimals)
{
  int res = 0;
  str[0] = '\0';
  char minus = 0;
  if (d < 0)
   {
    d = -d;
    str+=add(str, '-');
    minus = 1;
   }
  int whole = (int)d;
  double fract = (d - whole);
  res += itos(str, whole);
  int wlen = res;
  res+=add(str, '.');

//  while(decimals--) fract *= 10.0;
//  int ifract = ((int)(fract*10.0+5))/10;
//  res += itos(str+res, ifract);

  while(decimals)
   {
    decimals--;
    fract *= 10.0;
    int n = ((int)(fract*10.0+0.5))/10;
    char c = '0'+n%10;
    res+=add(str, c);
   }
  while((res>wlen)&&((str[res-1]=='0')||(str[res-1]=='.'))) str[--res] = '\0';
  return res+minus;
}
//----------------------------------
//return rounded value with n fract digits
double rnd(double d, int n)
{
 double Pow_10n = 1.0;
 while(n--) Pow_10n *= 10.0;
 return (int)(d * Pow_10n + 0.5) / Pow_10n;
}
//----------------------------------
//return rounded value with n significant digits
double nrnd(double d, int n)
{
 int exp = 0;
 double Pow_10n = 1.0;
 double dd = (d<0)?-d:d;
 if (dd > 0)
  {
   while (dd >= 1) {dd /= 10; exp++;}
   while (dd < 1)  {dd *= 10; exp--;}
  }
 if (d < 0) dd = -dd;
 if (n) n--;
 while(n--) Pow_10n *= 10.0;
 dd = (int)((dd * Pow_10n + 0.5)) / Pow_10n;
 while(exp)
  if (exp > 0) {dd *= 10.0; exp--;}
  else {dd /= 10.0; exp++;}
 return dd;
}
//---------------------------------------------------------------------------
//time in seconds to date and time string
int t2str1(char *str, __int64 sec, int adj)
{
 const unsigned __int64 dms[] =
   {(60i64*60*60*24*365.25*100i64),(60i64*60*24*365.25),
    (60i64*60*24), (60i64*60), 60i64, 1i64};
 const char * fmt[] =
   {":c ", ":y ", ":d ", ":h ", ":m ", ":s "};
 const char w[] =
   { 0,      3,     3,     2,     2,     2};
 unsigned int pt[6];
 int i, j, k;
 char *pc = str;

 for(i = 0, j = -1, k = 0; i < 6; i++)
  {
    pt[i] = (unsigned int)(sec / dms[i]);
    sec %= dms[i];
    if ((j == -1) && (pt[i] != 0)) j = i;
    if ((j != -1) && (pt[i] != 0)) k = i;
  }
 *str = '\0';
 if (j == -1) str += adds(str, "0:s");
 else
 for(i = j; i <= k; i++)
  {
   str += itosa(str, pt[i], w[i]);
   str += adds(str, fmt[i]);
  }
 int res = str-pc;
 if (adj > 0) while (res < adj) res += ins(str, ' ');
 else
 if (adj < 0) while (res < -adj) res += add(str, ' ');
 return res;
}
//---------------------------------------------------------------------------
//time in seconds to date and time string
int t2str(char *str, __int64 sec, int adj, bool full)
{
 const unsigned __int64 dms[] =
   {(60i64*60*60*24*365.25*100i64),(60i64*60*24*365.25),
    (60i64*60*24), (60i64*60), 60i64, 1i64};
 const char * fmt[] =
   {":c ", ":y ", ":d ", ":h ", ":m ", ":s "};
 const char w[] =
   { 0,      3,     3,     2,     2,     2};
 unsigned int pt[6];
 int i, j, k;
 char *pc = str;

 for(i = 0, j = -1, k = 0; i < 6; i++)
  {
    pt[i] = (unsigned int)(sec / dms[i]);
    sec %= dms[i];
    if ((j == -1) && (pt[i] != 0)) j = i;
    if ((j != -1) && (pt[i] != 0)) k = i;
  }
 if (full) k = 5;
 *str = '\0';
 if (j == -1) str += adds(str, "0:s");
 else
 for(i = j; i <= k; i++)
  {
   str += itosa(str, pt[i], w[i]);
   str += adds(str, fmt[i]);
  }
 int res = str-pc;
 if (adj > 0) while (res < adj) res += ins(pc, ' ');
 else
 if (adj < 0) while (res < -adj) res += add(str, ' ');
 return res;
}
//---------------------------------------------------------------------------
//floating point double to engineering string with prec significant digits
int d2scistrup(char *str, double d, const char* units, int prec, int adj)
{
 const char csci[] = {'y', 'z', 'a', 'f', 'p', 'n', 'u', 'm', ' ',
                      'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y'};
 enum {yocto, zepto, atto, femto, pico, nano, micro, milli, empty,
       kilo, mega, giga, tera, peta, exa, zetta, yotta};

 char *pc = str;
 double dd = (d<0)?-d:d;
 int rng = empty;
 int res;
 if (dd > 0)
  {
   while (dd >= 1000) {dd /= 1000; rng++;}
   while (dd < 1)     {dd *= 1000; rng--;}
  }
 if (d < 0) dd = -dd;
 if (rng == empty)
  {
   d =  nrnd(d, prec);
   res = dtostr(str, d, prec);
  }
 else
  {
   dd =  nrnd(dd, prec);
   res = dtostr(str, dd, prec);
   if ((rng >= yocto) && (rng <= yotta))
     res += add(str, csci[rng]);
   else
    {
     res += add(str, 'e');
     res += itos(str+res, (rng-empty)*3);
    }
  }
 res += adds(str, units);
 if (adj > 0) while (res < adj) res += ins(pc, ' ');
 else
 if (adj < 0) while (res < -adj) res += add(str, ' ');
 return res;
}
//---------------------------------------------------------------------------
//floating point double to engineering string
int d2scistru(char *str, double d, const char* units)
{
 return d2scistrup(str, d, units, 4, 0);
}
//---------------------------------------------------------------------------


