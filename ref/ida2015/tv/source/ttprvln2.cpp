/*
 *              TTerminal::prevLines()
 *
 *              Rewritten on C++ by Ilfak Guilfanov
 *                      26.06.95
 *
 */

#define Uses_TTerminal
#include <tv.h>

#define DECPTR                          \
{                                       \
  if ( ptr <= buffer ) ptr += bufSize;  \
  ptr--;                                \
}

#define INCPTR                          \
{                                       \
  ptr++;                                \
  if ( ptr >= buffer+bufSize ) ptr = buffer; \
}

//------------------------------------------------------------------------
size_t TV_CDECL TTerminal::prevLines(size_t pos, int lines)
{
  char *ptr = buffer + pos;
  if ( lines != 0 )
  {
    if ( buffer+queBack == ptr )
      return queBack;
    DECPTR
    while ( 1 )
    {
      size_t counter;
      if ( ptr <= buffer+queBack )
        counter = ptr - buffer;
      else
        counter = ptr - (buffer+queBack);
      counter++;
      while ( 1 )
      {
        // Added a check not avoid references past the allocated buffer.
        // Howver, I have no idea if this is the correct way of fixing it.
        if ( ptr == buffer+bufSize )
        {
          --ptr;
        }
        else if ( *ptr-- == '\n' )
        {
          lines--;
          if ( lines == 0 )
            goto ret;
          break;
        }
        counter--;
        if ( counter == 0 )
        {
          if ( ptr-buffer+1 == queBack )
            return queBack;
          ptr = buffer + bufSize;
          break;
        }
      }
    }
  }
ret:
  INCPTR
  INCPTR
  return ptr - buffer;
}
