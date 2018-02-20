//---------------------------------------------------------------------------

#ifndef strnumH
#define strnumH

#define STRBUF  256     // bufer size for string operations
#define FMTBUF  16
#define MAXARGS 32
//---------------------------------------------------------------------------

//delete n chars from p position of the string
extern int dels(char *str, int p, int n);

//insert char at the beginning of string
extern int ins(char *str, char c);

//----------------------------------
//insert char at the position p of string
extern int pins(char *str, char c, int p);

//insert string at the beginning of string
extern int inss(char *str, const char *ins);

//insert string at the the position p of string
extern int pinss(char *str, const char *ins, int p);

//add char to the end of string
extern int add(char *str, char c);

//add new string to the end of exist
extern int adds(char *str, const char *add);

//int to string
extern int itos(char *str, int i);

//int to string with adjustment
extern int itosa(char *str, int i, int adj);

//adjust string
extern int sadj(char *str, int adj);

//int to hex
extern int itoh(char *str, int i);

//int to n hex digits
extern int itox(char *str, unsigned int u, int n);

//floating point double to string
extern int dtostr(char *str, double d, int decimals);

//calculate round value
extern double rnd(double d, int n);

//return rounded value with n significant digits
extern double nrnd(double d, int n);

//time in seconds to date and time string
extern int t2str(char *str, __int64 sec, int adj, bool full);

//floating point double to engineering string
extern int d2scistru(char *str, double d, const char* units);

//floating point double to engineering string with prec significant digits
extern int d2scistrup(char *str, double d, const char* units, int prec, int adj);


//---------------------------------------------------------------------------
#endif
