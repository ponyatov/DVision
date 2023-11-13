/*------------------------------------------------------------------------*/
/* filename - drivers.cpp                                                 */
/*                                                                        */
/* function(s)                                                            */
/*        moveBuf  --   moves a buffer of char/attribute pairs            */
/*        moveChar --   sets a buffer with a char/attribute pair          */
/*        moveCStr --   moves a char array into a buffer & adds an        */
/*                      attribute to each char                            */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/*                                                                        */
/*    Turbo Vision -  Version 1.0                                         */
/*                                                                        */
/*                                                                        */
/*    Copyright (c) 1991 by Borland International                         */
/*    All Rights Reserved.                                                */
/*                                                                        */
/*------------------------------------------------------------------------*/

#define Uses_TDrawBuffer
#define Uses_TKeys
#include <tv.h>

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveBuf                                                  */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent - character position within the buffer where the data      */
/*               is to go                                                 */
/*                                                                        */
/*      source - far pointer to string                                    */
/*                                                                        */
/*      attr   - attribute to be used for all characters (0 to retain     */
/*               the attribute from 'source')                             */
/*                                                                        */
/*      count   - number of character/attribute pairs to move             */
/*                                                                        */
/*------------------------------------------------------------------------*/

void TDrawBuffer::moveBuf( ushort indent, const void *_source,
                           ushort attr, ushort _count )
{
  ssize_t   count = _count;
  size_t    offset = indent;

  if ( offset+count > qnumber(data) )
    count = ssize_t(qnumber(data)-offset);

#ifndef __UTF8__
  unsigned char* source = (unsigned char*) _source;
  if (attr)
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++] = (source[i] & 0xFF)+(attr << 8);
  }
  else
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++] = source[i]+(data[indent+i] & 0xFF00);
  }
#else
  const char* source = (const char*) _source;
  if (attr)
  {
    for ( ssize_t i=0; i<count; i++) {
      size_t k;
      data[offset++] = (getUtf8symbol(source, k) << 8) | (attr & 0xFF);
      source += k;
    }
  }
  else
  {
    for ( ssize_t i=0; i<count; i++) {
      size_t k;
      data[offset++] = (getUtf8symbol(source, k) << 8) | (data[indent+i] & 0xFF);
      source += k;
    }
  }
#endif // __UTF8__
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveBuf (TSymbol string)                                 */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent - character position within the buffer where the data      */
/*               is to go                                                 */
/*                                                                        */
/*      source - pointer to TSymbol string                                */
/*                                                                        */
/*      attr   - attribute to be used for all characters (0 to retain     */
/*               the attribute from 'source')                             */
/*                                                                        */
/*      count   - number of character/attribute pairs to move             */
/*                                                                        */
/*------------------------------------------------------------------------*/

#ifdef __UTF8__
void TDrawBuffer::moveBuf( ushort indent, const TSymbol *source,
                           ushort attr, ushort _count )
{
  ssize_t   count = _count;
  size_t    offset = indent;

  if ( offset+count > qnumber(data) )
    count = ssize_t(qnumber(data)-offset);

  if (attr)
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++] = (*source++ << 8) | (attr & 0xFF);
  }
  else
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++] = (*source++ << 8) | (data[indent+i] & 0xFF);
  }
}
#endif // __UTF8__

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveChar                                                 */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent  - character position within the buffer where the data     */
/*                is to go                                                */
/*                                                                        */
/*      c       - character to be put into the buffer                     */
/*                                                                        */
/*      attr    - attribute to be put into the buffer                     */
/*                                                                        */
/*      count   - number of character/attribute pairs to put into the     */
/*                buffer                                                  */
/*                                                                        */
/*------------------------------------------------------------------------*/

void TDrawBuffer::moveChar( ushort indent, TSymbol c, ushort attr, ushort _count )
{
  ssize_t count = _count;
  size_t offset = indent;

  if ( offset+count > qnumber(data) )
    count = ssize_t(qnumber(data)-offset);

#ifndef __UTF8__
  if (attr && c)
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++]= ((unsigned char) c)+(attr << 8);
  }
  else if (c)
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++]= ((unsigned char) c)+(data[indent+i] & 0xFF00);
  }
  else  // change attribute byte only
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++]= (attr << 8)+(data[indent+i] & 0x00FF);
  }
#else
  TCharHolder att = attr & 0xFF;
  if (attr && c)
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++] = (c << 8) | att;
  }
  else if (c)
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++] = (c << 8) | (data[indent+i] & 0xFF);
  }
  else  // change attribute byte only
  {
    for ( ssize_t i=0; i<count; i++)
      data[offset++] = att | (data[indent+i] & ~0xFF);
  }
#endif // __UTF8__
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveCStr                                                 */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent  - character position within the buffer where the data     */
/*                is to go                                                */
/*                                                                        */
/*      str     - pointer to a 0-terminated string of characters to       */
/*                be moved into the buffer                                */
/*                                                                        */
/*      attrs   - pair of text attributes to be put into the buffer       */
/*                with each character in the string.  Initially the       */
/*                low byte is used, and a '~' in the string toggles       */
/*                between the low byte and the high byte.                 */
/*                                                                        */
/*------------------------------------------------------------------------*/

void TDrawBuffer::moveCStr( ushort indent, const char *str, ushort attrs )
{
  for ( size_t i=0; indent < qnumber(data) && str[i]; i++ )
  {
    if ( str[i]=='~' )
      attrs = (attrs>>8)+((attrs & 0xFF)<<8);
    else
#ifndef __UTF8__
      data[indent++] = ((unsigned char) (str[i]) ) + ((attrs&0xFF)<<8);
#else
    {
      size_t k;
      data[indent++] = (getUtf8symbol(&str[i], k) << 8) | (attrs & 0xFF);
      i += k - 1;
    }
#endif // __UTF8__
  }
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  TDrawBuffer::moveStr                                                  */
/*                                                                        */
/*  arguments:                                                            */
/*                                                                        */
/*      indent  - character position within the buffer where the data     */
/*                is to go                                                */
/*                                                                        */
/*      str     - pointer to a 0-terminated string of characters to       */
/*                be moved into the buffer                                */
/*                                                                        */
/*      attr    - text attribute to be put into the buffer with each      */
/*                character in the string.                                */
/*                                                                        */
/*------------------------------------------------------------------------*/

//-----------------------------------------------------------------------
void TDrawBuffer::moveStr( ushort indent, const char *str, ushort attr )
{
#ifndef __UTF8__
    attr <<= 8;
    while ( indent < qnumber(data) && *str != '\0' )
      data[indent++] = (unsigned char)(*str++) + attr;
#else
    attr &= 0xFF;
    while ( indent < qnumber(data) && *str != '\0' ) {
      size_t k;
      data[indent++] = (getUtf8symbol(str, k) << 8) | attr;
      str += k;
    }
#endif // __UTF8__
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  ctrlToArrow                                                           */
/*                                                                        */
/*  argument:                                                             */
/*                                                                        */
/*      keyCode - scan code to be mapped to keypad arrow code             */
/*                                                                        */
/*  returns:                                                              */
/*                                                                        */
/*      scan code for arrow key corresponding to Wordstar key,            */
/*      or original key code if no correspondence exists                  */
/*                                                                        */
/*------------------------------------------------------------------------*/
ushort ctrlToArrow(ushort keyCode)
{

static const uchar ctrlCodes[] =
    {
    kbCtrlS, kbCtrlD, kbCtrlE, kbCtrlX, kbCtrlA,
    kbCtrlF, kbCtrlG, kbCtrlV, kbCtrlR, kbCtrlC, kbCtrlH
    };

static const ushort arrowCodes[] =
    {
    kbLeft, kbRight, kbUp, kbDown, kbHome,
    kbEnd,  kbDel,   kbIns,kbPgUp, kbPgDn, kbBack
    };

    for( size_t i = 0; i < sizeof(ctrlCodes); i++ )
        if( (keyCode & 0x00ff) == ctrlCodes[i] )
            return arrowCodes[i];
    return keyCode;
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/*  cstrlen                                                               */
/*                                                                        */
/*  argument:                                                             */
/*                                                                        */
/*      s       - pointer to 0-terminated string                          */
/*                                                                        */
/*  returns                                                               */
/*                                                                        */
/*      length of string, ignoring '~' characters.                        */
/*                                                                        */
/*  Comments:                                                             */
/*                                                                        */
/*      Used in determining the displayed length of command strings,      */
/*      which use '~' to toggle between display attributes                */
/*                                                                        */
/*------------------------------------------------------------------------*/

int cstrlen( const char *s )
{
    int len = 0;
    while( *s != EOS )
        {
        if( *s++ != '~' )
            len++;
        }
    return len;
}
