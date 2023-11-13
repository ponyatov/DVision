/*------------------------------------------------------------*/
/* filename -       thstview.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  THistoryViewer member functions           */
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

#define Uses_TKeys
#define Uses_THistoryViewer
#define Uses_TScrollBar
#define Uses_TEvent
#include <tv.h>

#define cpHistoryViewer "\x06\x06\x07\x06\x06"

THistoryViewer::THistoryViewer( const TRect& bounds,
                                TScrollBar *aHScrollBar,
                                TScrollBar *aVScrollBar,
                                historyId_t aHistoryId
#ifndef _HISTORY_NO_VEC
                                , qstrvec_t *slist
#endif
                                ) :
    TListViewer(bounds, 1, aHScrollBar, aVScrollBar),
    historyId( aHistoryId )
#ifndef _HISTORY_NO_VEC
    ,strlist( slist )
#endif
{
    setRange( itemCount() );
//    if( range > 1 )
//        focusItem( 1 );
    hScrollBar->setRange( 0, historyWidth() - size.x + 3 );
}

int THistoryViewer::itemCount() const
{
#ifndef _HISTORY_NO_VEC
  return strlist == NULL ?
    historyCount( historyId ) :
    strlist->size();
#else
  return historyCount( historyId );
#endif
}

TPalette& THistoryViewer::getPalette() const
{
    static TPalette palette( cpHistoryViewer, sizeof( cpHistoryViewer )-1 );
    return palette;
}

void THistoryViewer::getText( char *dest, int item, size_t destsize )
{
  if ( ssize_t(destsize) > 0 )
  {
#ifndef _HISTORY_NO_VEC
    if ( strlist == NULL )
    {
#endif
      const char *str = historyStr( historyId, item );
      if( str != NULL )
        ustrncpy( dest, str, destsize );
      else
        *dest = EOS;
#ifndef _HISTORY_NO_VEC
    }
    else
    {
      if ( item < strlist->size() )
        ustrncpy( dest, strlist->at(item).c_str(), destsize );
      else
        *dest = EOS;
    }
#endif
  }
}

void THistoryViewer::handleEvent( TEvent& event )
{
    if( (event.what == evMouseDown && (event.mouse.eventFlags & meDoubleClick) ) ||
        (event.what == evKeyDown && event.keyDown.keyCode == kbEnter)
      )
        {
        endModal( cmOK );
        clearEvent( event );
        }
    else
        if( (event.what ==  evKeyDown && event.keyDown.keyCode == kbEsc) ||
            (event.what ==  evCommand && event.message.command ==  cmCancel)
          )
            {
            endModal( cmCancel );
            clearEvent( event );
            }
        else
            TListViewer::handleEvent( event );
}

int THistoryViewer::historyWidth()
{
    size_t width = 0;
    int count = itemCount();
#ifndef _HISTORY_NO_VEC
    if ( strlist == NULL )
    {
#endif
      for ( int i = 0; i < count; i++ )
      {
        size_t T = ustrlen( historyStr( historyId, i ) );
        width = qmax( width, T );
      }
#ifndef _HISTORY_NO_VEC
    }
    else
    {
      for ( int i = 0; i < count; i++ )
      {
        size_t T = ustrlen( strlist->at(i).c_str() );
        width = qmax( width, T );
      }
    }
#endif
    return (int)width;
}
