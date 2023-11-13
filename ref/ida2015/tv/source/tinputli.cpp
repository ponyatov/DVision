/*------------------------------------------------------------*/
/* filename -       tinputli.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  TInputLine member functions               */
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
#define Uses_TInputLine
#define Uses_TDrawBuffer
#define Uses_TEvent
#define Uses_opstream
#define Uses_ipstream
#ifdef __NT__
#define Uses_TThreaded
#elif defined(__LINUX__)
#define Uses_TScreen
#else
#define Uses_TEditor
#endif
#include <tv.h>

static uint32 qstrulen(const qstring &str)
{
#ifdef __UTF8__
  return str.empty() ? 0 : ustrlen(str.c_str());
#else
  return str.length();
#endif
}

static uint32 str_idx(const qstring &str, int32 ch_num)
{
  if ( ch_num < 0 || str.empty() )
    return 0;

#ifdef __UTF8__
  const char *ptr = ustraddr(str.c_str(), ch_num);
  return ptr - &str[0];
#else
  qnotused(str);
  return ch_num;
#endif
}

const int CONTROL_Y = 25;

char hotKey( const char *s )
{
    const char *p;

    if( (p = strchr( s, '~' )) != 0 )
        return qtoupper(p[1]);
    else
        return 0;
}

#define cpInputLine "\x13\x13\x14\x15\x21\x01\x01\x01\x01\x01\x01\x01"

TInputLine::TInputLine( const TRect& bounds, int aMaxLen ) :
    TView(bounds),
    maxLen( aMaxLen-1 ),
    curPos( 0 ),
    firstPos( 0 ),
    selStart( 0 ),
    selEnd( 0 ),
    readOnly( false ),
    anchor( -1 )
{
    state |= sfCursorVis;
    options |= ofSelectable | ofFirstClick;
}

TInputLine::~TInputLine()
{
}

Boolean TInputLine::canScroll( int delta ) const
{
    if( delta < 0 )
        return Boolean( firstPos > 0 );
    else
        if( delta > 0 )
            return Boolean(qstrulen(str) - firstPos + 2 > size.x);
        else
            return False;
}

size_t TInputLine::dataSize()
{
    return str.size();
}

void TInputLine::draw()
{
    int l, r;
    TDrawBuffer b;

    uchar cdis = uchar(getColor(5) + 0x50); // Make it darkgray
    uchar carr = uchar(getColor(4));
    uchar cfill = uchar(getColor(3));
    uchar color = uchar((state & sfFocused) ? getColor( 2 ) : getColor( 1 ));
    if ( state & sfDisabled )
      carr = cfill = color = cdis;

    b.moveChar( 0, ' ', color, ushort(size.x) );
    char buf[MAXSTR];
    ustrNcpy( buf, str.c_str() + str_idx(str, firstPos), size.x-1 );
    b.moveStr( 1, buf, color );

    if( canScroll(1) )
        b.moveChar( ushort(size.x-1), *rightArrow, carr, 1 );
    if( (state & sfSelected) != 0 )
        {
        if( canScroll(-1) )
            b.moveChar( 0, *leftArrow, carr, 1 );
        l = selStart - firstPos;
        r = selEnd - firstPos;
        l = qmax( 0, l );
        r = qmin( size.x - 2, r );
        if (l <  r)
            b.moveChar( ushort(l+1), 0, cfill, ushort(r - l) );
        }
    writeLine( 0, 0, ushort(size.x), ushort(size.y), b );
    setCursor( ushort(curPos-firstPos+1), 0);
}

void TInputLine::getData( void *rec, size_t recsize )
{
    size_t dsize = dataSize();
    memcpy( rec, str.c_str(), qmin(recsize, dsize) );
}

TPalette& TInputLine::getPalette() const
{
    static TPalette palette( cpInputLine, sizeof( cpInputLine )-1 );
    return palette;
}

int TInputLine::mouseDelta( const TEvent& event ) const
{
    TPoint mouse = makeLocal( event.mouse.where );

    if( mouse.x <= 0 )
        return -1;
    else
        if( mouse.x >= size.x - 1 )
            return 1;
        else
            return 0;
}

int TInputLine::mousePos( const TEvent& event ) const
{
    TPoint mouse = makeLocal( event.mouse.where );
    mouse.x = qmax( mouse.x, 1 );
    size_t dsize = qstrulen(str);
    size_t pos = mouse.x + firstPos - 1;
    pos = qmax( pos, 0 );
    pos = qmin( pos, dsize );
    return (int)pos;
}

void  TInputLine::deleteSelect()
{
    if( selStart < selEnd )
    {
      uint32 start = str_idx(str, selStart);
      uint32 end = str_idx(str, selEnd);
      str.remove(start, end-start);
      curPos = selStart;
    }
}

void TInputLine::adjustSelectBlock()
{
#ifndef __UNPATCHED
    if(anchor < 0)
        selEnd = selStart = 0;
    else
#endif
    if (curPos < anchor)
        {
        selStart = curPos;
        selEnd =  anchor;
        }
    else
        {
        selStart = anchor;
        selEnd = curPos;
        }
}

#ifdef __LINUX__
static char localClipboard[MAXSTR];

bool localClipboardPut(const char *str, size_t from, size_t to)
{
    size_t len = to - from;
    if (to <= from || len > sizeof(localClipboard) - 1)
        return false;

    qstrncpy(localClipboard, str + from, len);
    return true;
}

char *localClipboardGet(size_t &sz)
{
    size_t len = strlen(localClipboard);
    if (len == 0)
        return NULL;
    if ( sz > 0 && len > sz )
      len = sz;
    sz = len;
    char *ans = (char *) qalloc(len + 1);
    memcpy(ans, localClipboard, len);
    ans[len] = '\0';
    return ans;
}
#endif

bool TInputLine::clip_put(void) const
{
#ifdef __NT__
    return TThreads::clipboard_put(str.c_str(), selStart, selEnd);
#elif defined(__LINUX__)
    if (selStart < selEnd) {
        uint32 start = str_idx(str, selStart);
        size_t len = str_idx(str, selEnd)-start;
        if ( x11_clipboard_accessible() )
            return x11_clipboard_put(&str[start], 0, len);
        else
            return localClipboardPut(&str[start], 0, len);
    }
    return false;
#else
    if ( TEditor::clipboard == NULL )
        return false;
    return TEditor::clipboard->insertBuffer(str.c_str(), selStart, selEnd,
                                            False, True) == True;
#endif
}

char *TInputLine::clip_get(size_t &clipsz) const
{
#ifdef __NT__
    return TThreads::clipboard_get(clipsz, true);
#elif defined(__LINUX__)
    if ( x11_clipboard_accessible() )
        return x11_clipboard_get(clipsz, false);
    else
        return localClipboardGet(clipsz);
#else
    char *answer = NULL;
    if ( TEditor::clipboard != NULL )
        {
        char *ptr = TEditor::clipboard->buffer;
        char *end = ptr + TEditor::clipboard->selEnd;
        ptr += TEditor::clipboard->selStart;
        if ( end > ptr )
            {
            size_t  tot = end - ptr;
            if ( tot > clipsz )
                tot = clipsz;
            answer = (char *)qalloc(tot + 1);
            if ( answer != NULL )
                {
                char *p = answer;
                while ( ptr < end && *ptr != 0 )  // second - paranoya
                    {
                    *p = *ptr++;
                    if ( *p < ' ' || *p == 127 )
                        {
                        if ( p == answer ) continue;
                        *p = ' ';
                        }
                        ++p;
                    }
                for ( ; p > answer; --p )
                   if ( p[-1] > ' ' ) break;
                if ( p == answer )
                    {
                        free ( answer);
                        answer = NULL;
                    }
                    else
                    {
                        *p = '\0';
                        clipsz = p - answer;
                    }
                }
            }
        }
    return answer;
#endif
}

void  TInputLine::handleEvent( TEvent& event )
{
    /* Home, Left Arrow, Right Arrow, End, Ctrl-Left Arrow, Ctrl-Right Arrow */
    static const char padKeys[] = {0x47,0x4b,0x4d,0x4f,0x73,0x74, 0};
    TView::handleEvent(event);

    int delta, i;
    bool changed = false;
    if( (state & sfSelected) != 0 )
        switch( event.what )
            {
            case  evMouseDown:
                if( canScroll(delta = mouseDelta(event)) )
                    do  {
                        if( canScroll(delta) )
                            {
                            firstPos += delta;
                            drawView();
                            }
                        } while( mouseEvent( event, evMouseAuto ) );
                else if (event.mouse.eventFlags & meDoubleClick)
                    selectAll(True);
                else
                    {
                    anchor =  mousePos(event);
                    do  {
                        if( event.what == evMouseAuto)
                            {
                            delta = mouseDelta(event);
                            if (canScroll(delta))
                                firstPos += delta;
                            }
                        curPos = mousePos(event);
                        adjustSelectBlock();
                        drawView();
                        }
                        while (mouseEvent(event,evMouseMove | evMouseAuto));
                    }
                clearEvent(event);
                break;
            case  evKeyDown:
                int oldKeyCode = event.keyDown.keyCode;
                event.keyDown.keyCode = ctrlToArrow(event.keyDown.keyCode);

                /* SS: scanCode must be non zero */

                if (event.keyDown.charScan.scanCode != 0 &&
                    strchr(padKeys, event.keyDown.charScan.scanCode ) &&
                    (event.keyDown.controlKeyState & kbShift) != 0
                  )
                    {
                    event.keyDown.charScan.charCode = 0;
#ifndef __UNPATCHED
                    if(anchor < 0)
                        anchor = curPos;
                }
                else
                    anchor = -1;
#else
                    if (curPos == selEnd)
                        anchor = selStart;
                    else
                        anchor = selEnd;
                    extendBlock = True;
                    }
                else
                    extendBlock = False;
#endif
                switch( event.keyDown.keyCode )
                    {
                    case kbLeft:
                        if( curPos > 0 )
                            curPos--;
                        break;
                    case kbRight:
                        if( curPos < qstrulen(str) )
                            curPos++;
                        break;
                    case kbHome:
                        curPos =  0;
                        break;
                    case kbEnd:
                        curPos = qstrulen(str);
                        break;
                    case kbCtrlLeft:
                        while ( curPos > 0 && isspace(str[str_idx(str, curPos-1)]) )
                          curPos--;
                        while ( curPos > 0 && !isspace(str[str_idx(str, curPos-1)]) )
                          curPos--;
                        break;
                    case kbCtrlRight:
                        {
                          int len = qstrulen(str);
                          while ( curPos < len && !isspace(str[str_idx(str, curPos-1)]) )
                            curPos++;
                          while ( curPos < len && isspace(str[str_idx(str, curPos-1)]) )
                            curPos++;
                        }
                        break;
                    case kbBack:
                        if( curPos > 0 && !readOnly )
                        {
                          curPos--;
                          uint32 start = str_idx(str, curPos);
                          size_t sl = getUtf8Len(uchar(str[start]));
                          str.remove(start, sl);
                          if( firstPos > 0 )
                            firstPos--;
                          changed = true;
                        }
                        break;
                    case kbDel:
                        if ( !readOnly )
                        {
                          if( selStart == selEnd )
                              if( curPos < qstrulen(str) )
                              {
                                selStart = curPos;
                                selEnd = curPos + 1;
                              }
                          deleteSelect();
                          changed = true;
                        }
                        break;
                    case kbIns:
                        setState(sfCursorIns, Boolean(!(state & sfCursorIns)));
                        break;
                    case kbShiftIns:
                        {
                          if ( !readOnly )
                          {
                            size_t len = qstrulen(str);
                            if ( selEnd > selStart )
                                len -= (selEnd - selStart);
                            //              signed comparision (see above)
                            if ( (int)len < 0 || (maxLen > 0 && (int)len >= maxLen) )
                                break;
                            size_t clipsz = maxLen > 0 ? maxLen - len : 0;
                            char *pcl = clip_get(clipsz);
                            if ( pcl == NULL )
                              break;
                            deleteSelect();
                            if( firstPos > curPos )
                              firstPos = curPos;
                            uint32 start = str_idx(str, curPos);
                            str.insert(start, pcl);
                            qfree(pcl);
                            changed = true;
                          }
                        }
                        break;
                    case kbCtrlIns:
                    case kbShiftDel:
                        if (   !clip_put()
                            || event.keyDown.keyCode == kbCtrlIns
                            || readOnly )
                            {
                              clearEvent ( event );
                              return; // do not remove selection
                            }
                        deleteSelect();
                        changed = true;
                        break;
                    default:
                        if( event.keyDown.charScan.charCode >= ' ' )
                        {
                          if ( !readOnly )
                          {
                            deleteSelect();
#ifndef __UTF8__
                            TSymbol sym = event.keyDown.charScan.charCode;
#else
                            TSymbol sym = event.keyDown.utfCode;
#endif
                            size_t slo = getUtf8Len(uchar(*dataCurPos));
                            size_t sln = getUtf8Len(uchar(sym));
                            ssize_t delta2 = (state & sfCursorIns) ? sln - slo : sln;
                            if ( maxLen > 0 || qstrulen(str) + delta2 <= maxLen )
                            {
                              uint32 start = str_idx(str, curPos);
                              if ( state & sfCursorIns )
                                str.remove(start, slo);
                              str.insert(start, ((char *)&sym), sln);
                              curPos++;
                              changed = true;
                            }
                          }
                        }
                        else if( event.keyDown.charScan.charCode == CONTROL_Y)
                            {
                              if ( !readOnly )
                              {
                                curPos = 0;
                                str.clear();
                                changed = true;
                              }
                            }
                            else
                            {
                                /* SS: restore the old value before exit */

                                event.keyDown.keyCode = (ushort)oldKeyCode;
                                return;
                            }
                    }
#ifndef __UNPATCHED
                adjustSelectBlock();
#else
                if (extendBlock)
                    adjustSelectBlock();
                else
                    {
                    selStart = 0;
                    selEnd = 0;
                    }
#endif
                    if( firstPos > curPos )
                        firstPos = curPos;
                    i = curPos - size.x + 2;
                    if( firstPos < i )
                        firstPos = i;
                    drawView();
                    clearEvent( event );
                    if ( changed )
                    {
                        if ( onchange != NULL )
                          onchange->changed(this);
                    }
                    break;
            }
}

bool TInputLine::disableReselect = false; // for 'ovelapped' wait_boxes
void TInputLine::selectAll( Boolean enable )
{
    if( !disableReselect )
    {
        selStart = 0;
        if( enable )
            curPos = selEnd = qstrulen(str);
        else
            curPos = selEnd = 0;
        firstPos = qmax( 0, curPos-size.x+2 );
#ifndef __UNPATCHED
        anchor = 0;               //<----- This sets anchor to avoid deselect
#endif
    }
    drawView();
}

void TInputLine::setData( void *rec )
{
  str = (char *) rec;
  selectAll( True );
}

void TInputLine::setState( ushort aState, Boolean enable )
{
    TView::setState( aState, enable );
    if( aState == sfSelected ||
        ( aState == sfActive && (state & sfSelected) != 0 )
      )
        selectAll( enable );
    if( aState == sfDisabled )
      drawView();
}

void TInputLine::setReadOnly(bool b)
{
  readOnly = b;
}

#ifndef NO_TV_STREAMS
void TInputLine::write( opstream& os )
{
    TView::write( os );
    os << maxLen << str.size() << curPos << firstPos
       << selStart << selEnd;
    os.writeString(str.c_str());
}

void *TInputLine::read( ipstream& is )
{
    TView::read( is );
    uint32 strsize;
    is >> maxLen >> strsize >> curPos >> firstPos
       >> selStart >> selEnd;
    str.resize(strsize);
    is.readString(&str[0], strsize);
    state |= sfCursorVis;
    options |= ofSelectable | ofFirstClick;
    return this;
}

TStreamable *TInputLine::build()
{
    return new TInputLine( streamableInit );
}

TInputLine::TInputLine( StreamableInit ) : TView( streamableInit )
{
}
#endif  // ifndef NO_TV_STREAMS

