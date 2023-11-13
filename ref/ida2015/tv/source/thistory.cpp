/*------------------------------------------------------------*/
/* filename -       thistory.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  THistory member functions                 */
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

#define Uses_THistory
#define Uses_TKeys
#define Uses_TRect
#define Uses_TEvent
#define Uses_TInputLine
#define Uses_THistoryWindow
#define Uses_THistoryViewer
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

#define cpHistory "\x16\x17"

THistory::THistory( const TRect& bounds,
                    TInputLine *aLink,
                    historyId_t aHistoryId
#ifndef _HISTORY_NO_VEC
                    , qstrvec_t *slist
#endif
                    ) :
    TView(bounds),
    link( aLink ),
    historyId( aHistoryId ),
#ifndef _HISTORY_NO_VEC
    strlist( slist ),
#endif
    curIndex( -1 ),
    historyWindow( NULL )
{
    options |= ofPostProcess;
    eventMask |= evBroadcast;
}

void THistory::shutDown()
{
    link = 0;
    TView::shutDown();
}

void THistory::draw()
{
    TDrawBuffer b;

    b.moveCStr( 0, icon, getColor(0x0102) );
    writeLine( 0, 0, ushort(size.x), ushort(size.y), b );
}

TPalette& THistory::getPalette() const
{
    static TPalette palette( cpHistory, sizeof( cpHistory )-1 );
    return palette;
}

void THistory::handleEvent( TEvent& event )
{
    TRect  r, p;
    ushort c;

    TView::handleEvent( event );
    if( event.what == evMouseDown ||
          ( event.what == evKeyDown &&
            ctrlToArrow( event.keyDown.keyCode ) ==  kbDown &&
            (link->state & sfFocused) != 0
          )
      )
        {
        link->select();
#ifndef _HISTORY_NO_VEC
        if ( strlist == NULL )
#endif
          if (link->str[0] != '\0')
            historyAdd( historyId, link->str.c_str() );
        r = link->getBounds();
        r.a.x--;
        r.b.x++;
        r.b.y += 7;
        r.a.y--;
        p = owner->getExtent();
        r.intersect( p );
        r.b.y--;
        historyWindow = initHistoryWindow( r );
        if( historyWindow != 0 )
            {
            c = owner->execView( historyWindow );
            if( c == cmOK )
                {
                char rslt[maxViewWidth];
                historyWindow->getSelection( rslt, sizeof(rslt) );
                link->str = rslt;
                curIndex = historyWindow->viewer->focused;
                link->selectAll( True );
                link->drawView();
                if ( link->onchange != NULL )
                     link->onchange->changed(this);
                }
            destroy( historyWindow );
            historyWindow = NULL;
            }
        clearEvent( event );
        }
    else
        if( event.what == evBroadcast )
        {
          if( (event.message.command == cmReleasedFocus &&
               event.message.infoPtr ==  link) ||
              event.message.command ==  cmRecordHistory
            )
          {
#ifndef _HISTORY_NO_VEC
              if ( strlist == NULL )
#endif
                if ( !link->str.empty() )
                {
                  historyAdd( historyId, link->str.c_str() );
                  if (historyWindow != 0) {
                      THistoryViewer *v = (THistoryViewer *) historyWindow->viewer;
                      v->setRange(historyCount(historyId));
                  }
                }
          }
        }
}

THistoryWindow *THistory::initHistoryWindow( const TRect& bounds )
{
#ifndef _HISTORY_NO_VEC
    THistoryWindow *p = new THistoryWindow( bounds, historyId, strlist );
#else
    THistoryWindow *p = new THistoryWindow( bounds, historyId );
#endif
    p->helpCtx = link->helpCtx;
    return p;
}

int THistory::currentIndex() const
{
  return curIndex;
}

void THistory::setCurrentIndex(int i)
{
  curIndex = i;
#ifndef _HISTORY_NO_VEC
  if ( strlist != NULL )
  {
    link->str = strlist->at(curIndex);
    link->selectAll(True);
    link->drawView();
  }
#endif
}

#ifndef NO_TV_STREAMS
void THistory::write( opstream& os )
{
    TView::write( os );
    os << link << historyId;
}

void *THistory::read( ipstream& is )
{
    TView::read( is );
    is >> link >> historyId;
    return this;
}

TStreamable *THistory::build()
{
    return new THistory( streamableInit );
}

THistory::THistory( StreamableInit ) : TView( streamableInit )
{
}
#endif  // ifndef NO_TV_STREAMS


