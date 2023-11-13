/*------------------------------------------------------------*/
/* filename -       thistwin.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  THistoryWindow member functions           */
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

#define Uses_THistoryWindow
#define Uses_THistoryViewer
#include <tv.h>

THistInit::THistInit( TListViewer *(*cListViewer)( TRect, TWindow *, historyId_t
#ifndef _HISTORY_NO_VEC
                                                 , qstrvec_t *
#endif
                                                  ) ) :
    createListViewer( cListViewer )
{
}

#define cpHistoryWindow "\x13\x13\x15\x18\x17\x13\x14"

THistoryWindow::THistoryWindow( const TRect& bounds,
                                historyId_t historyId
#ifndef _HISTORY_NO_VEC
                                , qstrvec_t *slist
#endif
                                ) :
    TWindowInit( THistoryWindow::initFrame ),
    THistInit( THistoryWindow::initViewer ),
    TWindow( bounds, 0, wnNoNumber),
    viewer(NULL)
{
    flags = wfClose;
    if( createListViewer != 0 &&
#ifndef _HISTORY_NO_VEC
        (viewer = createListViewer( getExtent(), this, historyId, slist )) != 0 )
#else
        (viewer = createListViewer( getExtent(), this, historyId )) != 0 )
#endif
        insert( viewer );
}

TPalette& THistoryWindow::getPalette() const
{
    static TPalette history_palette( cpHistoryWindow, sizeof( cpHistoryWindow )-1 );
    return history_palette;
}

void THistoryWindow::getSelection( char *dest, size_t destsize )
{
    viewer->getText( dest, viewer->focused, destsize );
}

TListViewer *THistoryWindow::initViewer( TRect r,
                                         TWindow * win,
                                         historyId_t historyId
#ifndef _HISTORY_NO_VEC
                                         , qstrvec_t *slist
#endif
                                          )
{
    r.grow( -1, -1 );
    return new THistoryViewer( r,
        win->standardScrollBar( sbHorizontal | sbHandleKeyboard ),
        win->standardScrollBar( sbVertical | sbHandleKeyboard ),
        historyId
#ifndef _HISTORY_NO_VEC
        , slist
#endif
        );
}
