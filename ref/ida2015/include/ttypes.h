/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   TTYPES.H                                                              */
/*                                                                         */
/*   Copyright (c) Borland International 1991                              */
/*   All Rights Reserved.                                                  */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if !defined( __TTYPES_H )
#define __TTYPES_H

enum Boolean { False, True };

typedef unsigned short ushort;
typedef unsigned char uchar;

#ifdef __UTF8__
typedef unsigned int    TCharHolder;  // Symbol + attributes
typedef unsigned int    TSymbol;      // Symbol
#else
typedef unsigned short  TCharHolder;  // Symbol + attributes
typedef char            TSymbol;      // Symbol
#endif

const char EOS = '\0';

enum StreamableInit { streamableInit };

class ipstream;
class opstream;
class TStreamable;
class TStreamableTypes;

typedef int ccIndex;
typedef Boolean (*ccTestFunc)( void *, void * );
typedef void (*ccAppFunc)( void *, void * );

const int ccNotFound = -1;

extern const TSymbol *specialChars;


#endif  // __TTYPES_H
