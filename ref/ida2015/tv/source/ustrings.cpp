#ifdef __UTF8__

#define Uses_TScreen

#include <tv.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <strings.h>

//=============================================================================
// returns true if UTF-8 support is detected
bool
isUtf8mode(void)
{
    return TScreen::useUTF8;
}

//=============================================================================

static const uint8 utf8_mblen_tbl[256] = {
  /*        0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F          */
  /* 00 */  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 00 */
  /* 10 */  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 10 */
  /* 20 */  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 20 */
  /* 30 */  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 30 */
  /* 40 */  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 40 */
  /* 50 */  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 50 */
  /* 60 */  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 60 */
  /* 70 */  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,   /* 70 */

  /* 80 */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /* 80 */
  /* 90 */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /* 90 */
  /* A0 */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /* A0 */
  /* B0 */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   /* B0 */
  /* C0 */  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   /* C0 */
  /* D0 */  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,   /* D0 */
  /* E0 */  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,   /* E0 */
  /* F0 */  4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 0, 0,   /* F0 */
  /*        0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F          */
};
static const ushort uni_low[0x20] = {
    0x2248, 0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022, // 00..07
    0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C, // 08..0F
    0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x2310, 0x21A8, // 10..17
    0x2191, 0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC  // 18..1F
};
static const ushort uni_high[0x30] = {
    0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, // B0..B7
    0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510, // B8..BF
    0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F, // C0..C7
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567, // C8..CF
    0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B, // D0..D7
    0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580  // D8..DF
};

static locale_t cnvLocale;

//=============================================================================
// returns size of UTF-8 sequence
// argument - first byte of sequence

size_t getUtf8Len(uchar firstByte)
{
    if ( !TScreen::useUTF8 )
      return 1;
    size_t len = utf8_mblen_tbl[firstByte];
    return len == 0 ? 1 : len;
}

//=============================================================================
// returns size of UTF-8 sequence. argument - first byte of sequence
// if argument is not valid returns 0

size_t getValidUtf8Len(uchar firstByte)
{
    if ( !TScreen::useUTF8 || firstByte == 0 )
      return 1;

    return utf8_mblen_tbl[firstByte];
}

//=============================================================================
// returns true if string is valid UTF-8 with symbol lenght no more 3 byte

bool isValidUtf8string(const char *s)
{
    if ( !TScreen::useUTF8 )
      return true;

    for (uchar *p = (uchar *) s; *p != '\0'; )
    {
      uint8 l = utf8_mblen_tbl[*p];
      if (l == 0 || l > 3)
        return false;
      for ( p++; --l > 0; )
        if ( (*p++ & 0xC0) != 0x80 )
          return false;
    }
    return true;
}

//=============================================================================
// strlen for UTF-8
// returns string lenght in symbols

size_t ustrlen(const char *s)
{
    size_t  len = 0;

    for (uchar *p = (uchar *) s; *p != '\0'; ) {
        uint8 l = utf8_mblen_tbl[*p];
        if (l == 0) l = 1;
        p += l;
        ++len;
    }
    return len;
}

//=============================================================================
// cstrlen for UTF-8

int ucstrlen(const char *s)
{
    int     len = 0;

    for (uchar *p = (uchar *) s; *p != '\0'; ) {
        if (*p == '~') {
            p++;
            continue;
        }
        uint8 l = utf8_mblen_tbl[*p];
        if (l == 0) l = 1;
        p += l;
        ++len;
    }
    return len;
}

//=============================================================================
// convert TSymbol to upper wchar_t
wchar_t utoupper(TSymbol s)
{
    if (!TScreen::useUTF8)
        return toupper(int(s));

    wchar_t c = symbol2wchar(s);
    return toupper_l(c, cnvLocale);
}

//=============================================================================
// case insensitive compare of two symbols

bool uisiequal(TSymbol s1, TSymbol s2)
{
    if (!TScreen::useUTF8)
        return tolower(int(s1)) == tolower(int(s2));

    wchar_t c1 = symbol2wchar(s1);
    wchar_t c2 = symbol2wchar(s2);
    return towlower_l(c1, cnvLocale) == towlower_l(c2, cnvLocale);
}

//=============================================================================
// compare two UTF-8 strings

int ustrnicmp(const char *s1, const char *s2, size_t n)
{
    if (!TScreen::useUTF8)
        return strncasecmp(s1, s2, n);

    char *p1 = (char *) s1;
    char *p2 = (char *) s2;
    while (p1 < s1+n && p2 < s2+n) {
        wchar_t c1, c2;
        if (*p1 == '\0' && *p2 == '\0')
            return 0;
        p1 = getWcharSymbol(p1, c1);
        c1 = towlower_l(c1, cnvLocale);
        p2 = getWcharSymbol(p2, c2);
        c2 = towlower_l(c2, cnvLocale);
        if (c1 != c2)
            return c1 - c2;
    }
    return 0;
}

//=============================================================================
// convert one symbol to WCHAR

wchar_t symbol2wchar(TSymbol s)
{
    wchar_t c;
    size_t  len = utf8_mblen_tbl[uchar(s)];

    switch (len) {
    case 2:
        c = ((s & 0x1F) << 6) | ((s >>8) & 0x3F);
        break;
    case 3:
        c = ((s & 0x0F) << 12) | ((s & 0x3F00) >> 2) | ((s >> 16) & 0x3F);
        break;
    default:
        c = uchar(s);
        break;
    }

    return c;
}

//=============================================================================
// gets WCHAR symbol from UTF-8 string

char *getWcharSymbol(char *str, wchar_t &sym)
{
    size_t  len = utf8_mblen_tbl[uchar(*str)];
    switch (len) {
    case 2:
        sym = ((str[0] & 0x1F) << 6) | (str[1] & 0x3F);
        str += 2;
        break;
    case 3:
        sym = ((str[0] & 0x0F) << 12) | ((str[1] & 0x3F) << 6) | (str[2] & 0x3F);
        str += 3;
        break;
    default:
        sym = uchar(*str++);
        break;
    }
    return str;
}

//=============================================================================
// convert WCHAR (2-byte unicode) to UTF-8

uchar *wchar2utf8(ushort wc, uchar *out)
{
    if (wc <= 0x7F) *out++ = uchar(wc);
    else if (wc <= 0x7FF) {
      *out++ = uchar(0xC0 | (0x1F & (wc >> 6)));
      *out++ = uchar(0x80 | (0x3F & wc));
    } else {
      *out++ = uchar(0xE0 | (0x0F & (wc >> 12)));
      *out++ = uchar(0x80 | (0x3F & (wc >> 6)));
      *out++ = uchar(0x80 | (0x3F & wc));
    }
    return out;
}

//=============================================================================
// gets one UTF-8 symbol as TSymbol and return its lenght

TSymbol getUtf8symbol(const char *str, size_t &len)
{
    TSymbol out = 0;
    len = 1;

    if (!TScreen::useUTF8)
        return uchar(*str);

    len = utf8_mblen_tbl[uchar(*str)];
    if (len == 0) len = 1;
    if (len <=3)
        for (size_t k = 0; k < len; k++)
            out |= uchar(*str++) << 8*k;
    else
        out = '?';
    return out;
}

//=============================================================================

void uCreateCnvLocale(void)
{
    cnvLocale = newlocale(LC_CTYPE_MASK, "", NULL);
}

//=============================================================================
// convert WCHAR string to TSymbol array

void wchar2utf8(const ushort *u16, TCharHolder *out, size_t len)
{
    while (len-- != 0) {
        if (*u16 < 0x200)
            *out++ = *u16++;
        else if (*u16 < 0x800) {
            *out++ = (0xC0 | (*u16 >> 6)) | ((0x80 | (*u16 & 0x3F)) << 8);
            u16++;
        } else {
            *out++ = (0x0000E0 |  (*u16 >> 12)) |
                     (0x008000 | ((*u16 <<  2) & 0x3F00)) |
                     (0x800000 | ((*u16 << 16) & 0x3F0000));
            u16++;
        }
    }
}

//=============================================================================
// convert pseudo graphics string to malloc'ed UTF-8 string

const char *pGraph2utf8(const char *in)
{
    size_t l = 0;

    for (uchar *p = (uchar *) in; *p != '\0'; p++) {
        ushort c = *p;
        if (c < 0x20)                    c = uni_low[c];
        else if (c == 0x7F)              c = 0x2302;
        else if (c == 0xFE)              c = 0x2022;
        else if (0xB0 <= c && c <= 0xDF) c = uni_high[c - 0xB0];
        if (c < 0x200)      l += 1;
        else if (c < 0x800) l += 2;
        else                l += 3;
    }
    const char *out = (const char *) qalloc(l+1);
    uchar *po = (uchar *) out;
    for (uchar *p = (uchar *) in; *p != '\0'; p++) {
        ushort c = *p;
        if (c < 0x20)                    c = uni_low[c];
        else if (c == 0x7F)              c = 0x2302;
        else if (c == 0xFE)              c = 0x2022;
        else if (0xB0 <= c && c <= 0xDF) c = uni_high[c - 0xB0];
    	po = wchar2utf8(c, po);
    }
    *po = '\0';

    return out;
}

//=============================================================================
// convert pseudo graphics symbols in buffer to UTF-8

void pGraph2utf8buf(const TCharHolder *b, size_t len)
{
    TCharHolder *p = (TCharHolder *) b;
    for ( ; len-- != 0; p++) {
        TSymbol c = *p >> 8;
        if (c < 0x20)                    c = uni_low[c];
        else if (c == 0x7F)              c = 0x2302;
        else if (c == 0xFE)              c = 0x2022;
        else if (0xB0 <= c && c <= 0xDF) c = uni_high[c - 0xB0];
        else continue;
        *p = (*p & 0xFF) | (_UTF8_3(c) << 8);
    }
}

//=============================================================================
// return address of string symbol by index

char *ustraddr(const char *s, size_t i)
{
    if ( !TScreen::useUTF8 )
      return (char *) (s + i);

    for ( ; i > 0 && *s != '\0'; i--) {
        uint8 l = utf8_mblen_tbl[uchar(*s)];
        if (l == 0)
          l = 1;
        s += l;
    }
    return (char *) s;
}

//=============================================================================
// return size of string part in bytes (len in symbols)

static size_t ustrpartlen(const char *s, size_t len)
{
    if ( !TScreen::useUTF8 )
        return len;

    size_t rlen = 0;
    for ( ; len > 0 && *s != '\0'; len--) {
        uint8 l = utf8_mblen_tbl[uchar(*s)];
        if (l == 0) l = 1;
        rlen += l;
        s += l;
    }
    return rlen;
}

//=============================================================================
// trim uncomplete tail of UTF-8 string

static void utrimuncomplete(char *s, size_t slen)
{
    if ( !TScreen::useUTF8 )
      return;

    if ( slen == 0 )
      slen = strlen(s);
    size_t idx = slen;
    while (idx > 0 && (s[idx-1] & 0xC0) == 0x80)
      idx--;
    if ( idx-- == 0 )
      s[0] = '\0';
    else if ( idx + utf8_mblen_tbl[uchar(s[idx])] > slen )
      s[idx] = '\0';
    return idx;
}

//=============================================================================
// strncat version for UTF-8; trim tail of string if needed

char *ustrncat(char *dst, const char *src, size_t len)
{
    char *s = dst += strlen(dst);
    ustrncpy(s, src, len);
    return dst;
}

//=============================================================================
// copy no more than len bytes

char *ustrncpy(char *dst, const char *src, size_t len)
{
    qstrncpy(dst, src, len);
    utrimuncomplete(dst);
    return dst;
}

//=============================================================================
// copy no more than len symbols

char *ustrNcpy(char *dst, const char *src, size_t len)
{
    size_t slen = ustrpartlen(src, len);
    memmove(dst, src, slen);
    dst[slen] = '\0';
    return dst;
}

//=============================================================================

#endif // __UTF8__
