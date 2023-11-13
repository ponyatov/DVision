/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   DRAWBUF.H                                                             */
/*                                                                         */
/*   Copyright (c) Borland International 1991                              */
/*   All Rights Reserved.                                                  */
/*                                                                         */
/*   defines the class TDrawBuffer, which provides the high-level          */
/*   interface to the Screen Manager.                                      */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#if defined( Uses_TDrawBuffer ) && !defined( __TDrawBuffer )
#define __TDrawBuffer

class TDrawBuffer
{

    friend class TSystemError;
    friend class TView;
    friend void genRefs();
    friend class TFrame;

public:

    void moveChar( ushort indent, TSymbol c, ushort attr, ushort count );
    void moveStr( ushort indent, const char *str, ushort attrs );
    void moveCStr( ushort indent, const char *str, ushort attrs );
    void moveBuf( ushort indent, const void *source,
                  ushort attr, ushort count );
#ifdef __UTF8__
    void moveBuf( ushort indent, const TSymbol *source,
                  ushort attr, ushort count );
#endif // __UTF8__

    void putAttribute( ushort indent, ushort attr );
    void putChar( ushort indent, TCharHolder c );

protected:

    TCharHolder data[maxViewWidth];

};

#ifndef __UTF8__
#define loByte(w)    (((uchar *)(&w))[0])
#define hiByte(w)    (((uchar *)(&w))[1])

inline void TDrawBuffer::putAttribute( ushort indent, ushort attr )
{
    hiByte(data[indent]) = (uchar)attr;
}

inline void TDrawBuffer::putChar( ushort indent, ushort c )
{
    loByte(data[indent]) = (uchar)c;
}
#else
#define attrPart(w)         (((uchar *)(&w))[0])

inline void TDrawBuffer::putAttribute( ushort indent, ushort attr )
{
    attrPart(data[indent]) = (uchar)attr;
}

inline void TDrawBuffer::putChar( ushort indent, TSymbol c )
{
    data[indent] = (data[indent] & 0xFF) | (c << 8);
}
#endif // __UTF8__

#endif  // Uses_TDrawBuffer


