#ifndef __USTRINGS_H__
#define __USTRINGS_H__

#if !defined( __TTYPES_H )
#ifdef __UTF8__
typedef unsigned int    TCharHolder;  // Symbol + attributes
typedef unsigned int    TSymbol;      // Symbol
#else
typedef unsigned short  TCharHolder;  // Symbol + attributes
typedef char            TSymbol;      // Symbol
#endif
#endif // __TTYPES_H

#ifndef __UTF8__

#define ustrlen(s)          strlen(s)
#define ustraddr(s,i)       ((s)+(i))
#define ustrnicmp(a,b,n)    strnicmp(a,b,n)
#define ucstrlen(s)         cstrlen(s)
#define ustrncat(t,f,n)     qstrncat(t,f,n)
#define ustrncpy(t,f,n)     qstrncpy(t,f,n)
#define ustrNcpy(t,f,n)     qstrncpy(t,f,(n)+1)
#define getUtf8Len(c)       1
#define getValidUtf8Len(c)  1

#else

#define _UTF8_2(c)  TSymbol((0x0000C0 | ((c >>  6) & 0x001F)) |  \
                            (0x008000 | ((c <<  8) & 0x3F00)))

#define _UTF8_3(c)  TSymbol((0x0000E0 | ((c >> 12) & 0x000F)) |  \
                            (0x008000 | ((c <<  2) & 0x3F00)) |  \
                            (0x800000 | ((c << 16) & 0x3F0000)))

#define isUtf8head(c)       (((c) & 0xC0) != 0x80)

// returns true if UTF-8 support is detected
bool    isUtf8mode(void);

// returns size of UTF-8 sequence. argument - first byte of sequence
size_t  getUtf8Len(uchar firstByte);

// returns size of UTF-8 sequence. argument - first byte of sequence
// if argument is not valid returns 0
size_t  getValidUtf8Len(uchar firstByte);

// returns true if string is valid UTF-8 with symbol lenght no more 3 byte
bool    isValidUtf8string(const char *s);

// returns string lenght in symbols
size_t  ustrlen(const char *s);

// cstrlen for UTF-8 (TV)
int     ucstrlen(const char *s);

// convert TSymbol to upper wchar_t
wchar_t utoupper(TSymbol s);

// case insensitive compare of two symbols
bool    uisiequal(TSymbol s1, TSymbol s2);

// compare two UTF-8 strings
int     ustrnicmp(const char*, const char*, size_t);

// convert one symbol to WCHAR
wchar_t symbol2wchar(TSymbol s);

// gets WCHAR symbol from UTF-8 string
char   *getWcharSymbol(char *str, wchar_t &sym);

// convert WCHAR (2-byte unicode) to UTF-8
uchar  *wchar2utf8(ushort wc, uchar *out);

// gets one UTF-8 symbol as TSymbol and return its lenght
TSymbol getUtf8symbol(const char *str, size_t &len);

// convert WCHAR string to TSymbol array
void    wchar2utf8(const ushort *u16, TCharHolder *out, size_t len);

// convert pseudo graphics string to malloc'ed UTF-8 string
const char *pGraph2utf8(const char *in);

// convert pseudo graphics symbols in buffer to UTF-8
void    pGraph2utf8buf(const TCharHolder *b, size_t len);

// return address of string symbol by index
char   *ustraddr(const char *s, size_t i);

// strncat version for UTF-8; trim tail of string if needed
char   *ustrncat(char *dst, const char *src, size_t len);

// copy no more than len bytes
char   *ustrncpy(char *dst, const char *src, size_t len);

// copy no more than len symbols
char   *ustrNcpy(char *dst, const char *src, size_t len);

// service
void    updatePseudoGraphics(void);
void    uCreateCnvLocale(void);

#endif // __UTF8__

#endif // __USTRINGS_H__
