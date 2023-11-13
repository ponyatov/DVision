/*------------------------------------------------------------*/
/* filename -       tmenubox.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  TMenuBox member functions                 */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*                                                            */
/*                                                            */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*------------------------------------------------------------*/

#define Uses_TRect
#define Uses_TMenu
#define Uses_TMenuItem
#define Uses_TMenuBox
#include <tv.h>

#ifndef __UTF8__
#define SUBMENU_MARKER  0x10
#else
#define SUBMENU_MARKER  _UTF8_3(0x25BA)
#endif

static TRect getRect( const TRect& bounds, const TMenu *aMenu )
{
    short w =  10;
    short h =  2;
    if( aMenu != 0 )
        {
        for( TMenuItem *p = aMenu->items; p != 0; p = p->next )
            {
            if( p->name != 0 )
                {
                short l = short(ucstrlen(p->name) + 6);
                if( p->command == 0 )
                    l += 3;
                else
                    if( p->param != 0 )
                        l = short(l + ucstrlen(p->param) + 2);
                w = qmax( l, w );
                }
            h++;
            }
        }

    TRect r( bounds );

    if( r.a.x + w < r.b.x )
        r.b.x = r.a.x + w;
    else
        r.a.x = r.b.x - w;

    if (r.a.y + h < r.b.y)
        r.b.y = r.a.y + h;
    else
        r.a.y = r.b.y - h;

    return r;
}

TMenuBox::TMenuBox( const TRect& bounds,
                    TMenu *aMenu,
                    TMenuView *aParentMenu) :
    TMenuView( getRect( bounds, aMenu ), aMenu, aParentMenu )
{
    state |= sfShadow;
    options |= ofPreProcess;
}

static ushort cNormal, color;

void TMenuBox::frameLine( TDrawBuffer& b, short n ) const
{
    b.moveChar( 0, frameChars[n+0], cNormal, 1 );
    b.moveChar( 1, frameChars[n+1], cNormal, 1 );
    b.moveChar( 2, frameChars[n+2], color, ushort(size.x - 4) );
    b.moveChar( ushort(size.x-2), frameChars[n+3], cNormal, 1 );
    b.moveChar( ushort(size.x-1), frameChars[n+4], cNormal, 1 );
}

void TMenuBox::draw()
{
    TDrawBuffer    b;

    cNormal = getColor(0x0301);
    ushort cSelect = getColor(0x0604);
    ushort cNormDisabled = getColor(0x0202);
    ushort cSelDisabled = getColor(0x0505);
    short y = 0;
    color =  cNormal;
    frameLine( b, 0 );
    writeBuf( 0, y++, ushort(size.x), 1, b );
    if( menu != 0 )
        {
        for( TMenuItem *p = menu->items; p != 0; p = p->next )
            {
            color = cNormal;
            if( p->name == 0 )
                frameLine( b, 15 );
            else
                {
                if( p->disabled )
                    if( p ==  current )
                        color = cSelDisabled;
                    else
                        color = cNormDisabled;
                else if( p == current )
                    color = cSelect;
                frameLine( b, 10 );
                b.moveCStr( 3, p->name, color );
                if( p->command == 0 )
                    b.putChar( ushort(size.x-4), SUBMENU_MARKER );
                else if( p->param != 0 )
                    b.moveStr( ushort(size.x-3-ustrlen(p->param)),
                               p->param,
                               color);
                }
            writeBuf( 0, y++, ushort(size.x), 1, b );
            }
        }
    color = cNormal;
    frameLine( b, 5 );
    writeBuf( 0, y, ushort(size.x), 1, b );
}

TRect TMenuBox::getItemRect( TMenuItem *item ) const
{
    short  y = 1;
    TMenuItem *p = menu->items;

    while( p != item )
        {
        y++;
        p =  p->next;
        }
    return TRect( 2, y, size.x-2, y+1 );
}

#ifndef NO_TV_STREAMS
TStreamable *TMenuBox::build()
{
    return new TMenuBox( streamableInit );
}

TMenuBox::TMenuBox( StreamableInit ) : TMenuView( streamableInit )
{
}
#endif  // ifndef NO_TV_STREAMS


