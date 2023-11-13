// edits.cpp defines functions previously found in edits.asm

#define Uses_TEditor
#include <tv.h>


TSymbol TV_CDECL TEditor::bufChar( size_t p ) const
{
#ifndef __UTF8__
    return buffer[p + ((p >= curPtr) ? gapLen : 0)];
#else
    size_t len;
    return getUtf8symbol(&buffer[p + ((p >= curPtr) ? gapLen : 0)], len);
#endif
}


size_t TV_CDECL TEditor::bufPtr( size_t p ) const
{
    return (p >= curPtr) ? p + gapLen : p;
}


// WARNING: This function is depended on format of TDrawBuffer

void TV_CDECL TEditor::formatLine( void *DrawBuf, size_t LinePtr,
                          int Width, ushort Color ) const
{
  register int i = 0;       // index in the DrawBuf
  register size_t p = LinePtr; // index in the Buffer
  ushort curColor;

  while (i <= Width) {
    register size_t pp = p;
    if (p >= curPtr) pp += gapLen;
    if (buffer[pp] == '\n' ||
        pp >= bufSize) break;

    curColor = (p >= selStart && p < selEnd) ? (Color & 0xFF00) : ((Color & 0xFF) << 8);
#ifndef __UTF8__
    if (buffer[pp] == '\t') {
      do {
        ((ushort *) DrawBuf) [i] = ' ' | curColor;
        i++;
      } while ((i % EDIT_TAB_SIZE) && (i <= Width));
    } else {
      ((ushort *) DrawBuf) [i++] = (unsigned char)buffer[pp] | curColor;
    }
    p++;
#else
    curColor >>= 8;
    if (buffer[pp] == '\t') {
      do {
        ((TCharHolder *) DrawBuf) [i] = (' ' << 8) | curColor;
        i++;
      } while ((i % EDIT_TAB_SIZE) && (i <= Width));
      p++;
    } else {
      size_t l;
      TSymbol c = getUtf8symbol(&buffer[pp], l);
      ((TCharHolder *) DrawBuf) [i++] = (c << 8) | curColor;
      p += l;
    }
#endif
  }

  curColor = (p >= selStart && p < selEnd) ? (Color & 0xFF00) : ((Color & 0xFF) << 8);
#ifndef __UTF8__
  while (i <= Width)
    ((ushort *) DrawBuf) [i++] = ' ' | curColor;
#else
  curColor >>= 8;
  while (i <= Width)
    ((TCharHolder *) DrawBuf) [i++] = (' ' << 8) | curColor;
#endif
}


size_t TV_CDECL TEditor::lineEnd( size_t p ) const
{
    if (p < curPtr)
    {
        while (p < curPtr)
            if (buffer[p] == '\n')
                return p;
            else
                p++;

        if (curPtr == bufLen)
            return bufLen;
    }
    else
    {
        if (p == bufLen)
            return bufLen;
    }

    while (p + gapLen < bufSize)
        if (buffer[p + gapLen] == '\n')
            return p;
        else
            p++;

    return p;

}


size_t TV_CDECL TEditor::lineStart( size_t p ) const
{
    while (p > curPtr)
        if (buffer[--p + gapLen] == '\n')
            return p + 1;

    if (curPtr == 0)
        return 0;

    while (p > 0)
        if (buffer[--p] == '\n')
            return p + 1;

    return 0;
}


size_t TV_CDECL TEditor::nextChar( size_t p ) const
{
    if (p == bufLen)   return p;

#ifndef __UTF8__
    if (++p == bufLen) return p;
#else
    size_t t = (p >= curPtr) ? p + gapLen : p;
    p += getUtf8Len(buffer[t]);
#endif

    return p;
}


size_t TV_CDECL TEditor::prevChar( size_t p ) const
{
    if (p == 0)   return p;
#ifndef __UTF8__
    if (--p == 0) return p;
#else
    size_t t;
    do {
        p--;
        t = (p >= curPtr) ? p + gapLen : p;
    } while (!isUtf8head(buffer[t]));
#endif

    return p;
}
