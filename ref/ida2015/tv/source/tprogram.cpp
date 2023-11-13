
/*------------------------------------------------------------*/
/* filename -       tprogram.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  TProgram member functions                 */
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

#define Uses_TApplication
#define Uses_TKeys
#define Uses_TProgram
#define Uses_TEvent
#define Uses_TScreen
#define Uses_TStatusLine
#define Uses_TMenu
#define Uses_TGroup
#define Uses_TDeskTop
#define Uses_TEventQueue
#define Uses_TSystemError
#define Uses_TMenuBar
#define Uses_TStatusDef
#define Uses_TStatusItem
#define Uses_TThreaded
#include <tv.h>
#include <tvhelp.h>
#include <stdio.h>
#include <time.h>

// Public variables

TStatusLine *TProgram::statusLine = 0;
TMenuBar *TProgram::menuBar = 0;
TDeskTop *TProgram::deskTop = 0;
TProgram *TProgram::application = 0;
int TProgram::appPalette = apColor;
TEvent TProgram::pending;

#ifndef __MSDOS__
int TProgram::event_delay =        // time to wait for an event
        TV_DEFAULT_EVENT_DELAY;    // in milliseconds. default: 1000
                                   // the idle() function may change
                                   // this value to lower values
                                   // and will be called more frequently
#endif

TProgInit::TProgInit( TStatusLine *(*cStatusLine)( TRect ),
                            TMenuBar *(*cMenuBar)( TRect ),
                            TDeskTop *(*cDeskTop )( TRect )
                          ) :
    createStatusLine( cStatusLine ),
    createMenuBar( cMenuBar ),
    createDeskTop( cDeskTop )
{
}

#ifndef __MSDOS__

static int inited = 0;

static void NT_CDECL SystemSuspend(void)
{
  if ( inited ) {
    inited = 0;
    TSystemError::suspend();
    TEventQueue::suspend();
    TScreen::suspend();
#ifndef __UNIX__
    TThreads::suspend();
#endif
  }
}

//---------------------------------------------------------------------------
#if defined(__NT__) && defined(__IDA__)
static qsemaphore_t request_ready;
static qmutex_t request_mutex;
static qvector<void *> requests;
static void (*request_collector)(void *);
bool doswin32;

static void init_functor_objects(void)
{
  if ( GetVersion() >= 0xF0000000 )
  {
    doswin32 = true;
    return;
  }

  request_mutex = qmutex_create();
  if ( request_mutex == NULL )
  {
FAILURE:
    fprintf(stderr, "tv: failed to create syncronization objects\n");
    abort();
  }
  request_ready = qsem_create(NULL, 0);
  if ( request_ready == NULL )
    goto FAILURE;
}

static void term_functor_objects(void)
{
  if ( doswin32 )
    return;
  qmutex_free(request_mutex);
  qsem_free(request_ready);
}

// Post an asynchronous request
void send_request_to_main_thread(void *req)
{
  if ( doswin32 )
    goto FAILURE;
  // tell the main thread we have a request
  if ( !qmutex_lock(request_mutex) )
FAILURE: abort();

  requests.push_back(req);
  if ( !qsem_post(request_ready) )
    goto FAILURE;
  if ( !qmutex_unlock(request_mutex) )
    goto FAILURE;
}

static void store_functor_request(void)
{
  void *req = NULL;
  if ( !qmutex_lock(request_mutex) )
    abort();

  if ( !requests.empty() )
  {
    req = requests[0];
    requests.erase(requests.begin());
  }
  if ( !qmutex_unlock(request_mutex) )
    abort();

  request_collector(req);
}

// Clear the request queue without executing the requests
void flush_exec_requests(void)
{
  if ( !qmutex_lock(request_mutex) )
    abort();

  requests.clear();

  if ( !qmutex_unlock(request_mutex) )
    abort();
}

// Register a callback for asycnhronious exec requests
void register_request_collector(void (*func)(void *))
{
  request_collector = func;
}

#else
#if defined(__NT__)
bool doswin32;
static void init_functor_objects(void)
{
  if ( GetVersion() >= 0xF0000000 )
    doswin32 = true;
}
#else
inline void init_functor_objects(void) {}
#endif
inline void term_functor_objects(void) {}
#endif
//---------------------------------------------------------------------------

TSystemInit::TSystemInit()
{
  resume();
  static bool is_atexit_set = false;
  if ( !is_atexit_set )
  {
    atexit(SystemSuspend);
    is_atexit_set = true;
  }
  init_functor_objects();
}

TSystemInit::~TSystemInit()
{
  term_functor_objects();
  suspend();
}

void TSystemInit::suspend()
{
  SystemSuspend();
}

void TSystemInit::resume()
{
  if ( !inited ) {
    inited = 1;
#ifndef __UNIX__
    TThreads::resume();
#endif
    TScreen::resume();
    TEventQueue::resume();
    TSystemError::resume();
  }
}

#else // !__MSDOS__
#ifdef __DOS32__

extern "C" int __dll_terminate(void)
{
  TProgram::application->suspend();
  return 1;
}

#endif // __DOS32__

#endif  // !__MSDOS__

TProgram::TProgram() :
    TProgInit( TProgram::initStatusLine,
               TProgram::initMenuBar,
               TProgram::initDeskTop),
#ifndef __MSDOS__
    TSystemInit(),
#endif  // !__MSDOS__
    TGroup( TRect( 0,0,TScreen::screenWidth,TScreen::screenHeight ) )
{
    application = this;
    initScreen();
    state = sfVisible | sfSelected | sfFocused | sfModal | sfExposed;
    options = 0;
    buffer = TScreen::screenBuffer;

    if( createDeskTop != 0 &&
        (deskTop = createDeskTop( getExtent() )) != 0
      )
        insert(deskTop);

    if( createStatusLine != 0 &&
        (statusLine = createStatusLine( getExtent() )) != 0
      )
        insert(statusLine);

    if( createMenuBar != 0 &&
        (menuBar = createMenuBar( getExtent() )) != 0
      )
        insert(menuBar);
}

TProgram::~TProgram()
{
    application = 0;
}

void TProgram::shutDown()
{
    statusLine = 0;
    menuBar = 0;
    deskTop = 0;
    TGroup::shutDown();
}

//lint -e{818} Pointer parameter could be declared as pointing to const
inline Boolean hasMouse2( TView *p, void *s )
{
    return Boolean( (p->state & sfVisible) != 0 &&
                     p->mouseInView( ((TEvent *)s)->mouse.where ));
}


void TProgram::getEvent(TEvent& event)
{
    if( pending.what != evNothing )
        {
        event = pending;
        pending.what = evNothing;
        }
    else
        {
#if defined(__MSDOS__)
        event.getMouseEvent();
        if( event.what == evNothing )
            {
            event.getKeyEvent();
            if( event.what == evNothing )
                idle();
            }
#elif defined(__OS2__)
        ULONG which;
        int delay = TEventQueue::lastMouse.buttons != 0
                        ? TEventQueue::autoDelay
                        : event_delay;
        APIRET rc = DosWaitMuxWaitSem(TThreads::hmuxMaster,delay,&which);
        if (rc==0 || TEventQueue::lastMouse.buttons != 0) {
          if (which==0 || TEventQueue::lastMouse.buttons != 0) {
            event.getMouseEvent();
          } else if (which==1) {
            event.getKeyEvent();
          }
        } else {
          event.what = evNothing;
          idle();
        }
#elif defined(__NT__)
        DWORD c;
#define hCin  TThreads::chandle[cnInput]
        int code = TThreads::ispending()
                || TThreads::macro_playing
                || GetNumberOfConsoleInputEvents(hCin, &c) && c;
        if ( !code && !doswin32 )
        {
          HANDLE handles[2];
          handles[0] = hCin;
          handles[1] = request_ready;
          int timeout = TEventQueue::lastMouse.buttons != 0
                      ? TEventQueue::autoDelay
                      : event_delay;
          code = WaitForMultipleObjects(qnumber(handles), handles, false, timeout);
          if ( code == WAIT_OBJECT_0 )
            code = 1; // console input
          else if ( code == WAIT_OBJECT_0+1 )
            code = 2; // functor piper
          else
            code = 0;
        }
#undef hCin
        switch ( code )
        {
          case 2:               // functor
            store_functor_request();
            break;
          case 1:               // console input
            event.getMouseEvent();
            if ( event.what == evNothing )
              event.getKeyEvent();
            break;
          case 0:               // nothing
          default:
            event.what = evNothing;
            if ( TEventQueue::lastMouse.buttons != 0 )
              event.getMouseEvent();
            break;
        }

        if ( changed_nt_console_size != 0 )
        { ushort x = ushort(changed_nt_console_size);
          ushort y = ushort(changed_nt_console_size >> 16);
          if (x > 255) x = 255;
          if (y > 255) y = 255;
          TProgram::application->setScreenMode((x<<8) | y);
          changed_nt_console_size = 0;
        }
        if ( event.what == evNothing ) idle();
#elif defined(__UNIX__)
        TScreen::getEvent(event);
        if ( event.what == evNothing )
          idle();
        else if (event.what == evCommand)
          switch ( event.message.command )
        {
          case cmSysRepaint:
                /*
                 * This command redraws the screen.  Useful when the
                 * user restarts the process after a ctrl-z.
                 */
            redraw();
            clearEvent(event);
            break;
          case cmSysResize:
                /*
                 * Generated after a SIGWINCH signal.
                 */
            buffer = TScreen::screenBuffer;
            changeBounds(TRect(0, 0, TScreen::screenWidth,
                TScreen::screenHeight));
            setState(sfExposed, False);
            setState(sfExposed, True);
            redraw();
            clearEvent(event);
            break;
          case cmSysWakeup:
            idle();
            clearEvent(event);
        }
#else
#error Unknown platform!
#endif
        }
    if( statusLine != 0 )
        {
        if( (event.what & evKeyDown) != 0 ||
            ( (event.what & evMouseDown) != 0 &&
              firstThat( hasMouse2, &event ) == statusLine
            )
          )
            statusLine->handleEvent( event );
        }
}

TPalette& TProgram::getPalette() const
{
    static TPalette color ( cpColor cHelpColor, sizeof(cpColor cHelpColor)-1 );
    static TPalette blackwhite( cpBlackWhite cHelpBlackWhite, sizeof(cpBlackWhite cHelpBlackWhite)-1 );
    static TPalette monochrome( cpMonochrome cHelpMonochrome, sizeof(cpMonochrome cHelpMonochrome)-1 );
    static TPalette *const palettes[] =
        {
        &color,
        &blackwhite,
        &monochrome
        };
    return *(palettes[appPalette]);
}

void TProgram::handleEvent( TEvent& event )
{
    if( event.what == evKeyDown )
        {
        char c = getAltChar( event.keyDown.keyCode );
        if( c >= '1' && c <= '9' )
            {
            if( message( deskTop,
                         evBroadcast,
                         cmSelectWindowNum,
                         (void *)(c - '0')
                       ) != 0 )
                clearEvent( event );
            }
        }

    TGroup::handleEvent( event );
    if( event.what == evCommand && event.message.command == cmQuit )
        {
        endModal( cmQuit );
        clearEvent( event );
        }
}

void TProgram::idle()
{
    if( statusLine != 0 )
        statusLine->update();

    if( commandSetChanged == True )
        {
        message( this, evBroadcast, cmCommandSetChanged, 0 );
        commandSetChanged = False;
        }
    set_event_delay(TV_DEFAULT_EVENT_DELAY);
}

TDeskTop *TProgram::initDeskTop( TRect r )
{
    r.a.y++;
    r.b.y--;
    return new TDeskTop( r );
}

TMenuBar *TProgram::initMenuBar( TRect r )
{
    r.b.y = r.a.y + 1;
    return new TMenuBar( r, (TMenu *)0 );
}

void TProgram::initScreen()
{
#if defined(__OS2__)
    short mode = TScreen::screenMode;
    if ( mode != TDisplay::smMono &&
         mode != TDisplay::smBW80 &&
         mode != TDisplay::smCO80    ) {
      VIOMODEINFO info;
      info.cb=sizeof(VIOMODEINFO);
      VioGetMode(&info,0);
      if ( info.color == COLORS_2 ) mode = TDisplay::smMono;
      else mode = TDisplay::smCO80;
    }
#elif defined(__NT__)
    short mode = TDisplay::smCO80;
#elif defined(__MSDOS__) || defined(__UNIX__)
    short mode = TScreen::screenMode;
#else
#error  Unknown platform!
#endif
    if( (mode & 0x00FF) != TDisplay::smMono )
        {
        if( (mode & TDisplay::smFont8x8) != 0 )
            shadowSize.x = 1;
        else
            shadowSize.x = 2;
        shadowSize.y = 1;
        showMarkers = False;
        if( (mode & 0x00FF) == TDisplay::smBW80 )
            appPalette = apBlackWhite;
        else
            appPalette = apColor;
        }
    else
        {
        shadowSize.x = 0;
        shadowSize.y = 0;
        showMarkers = True;
        appPalette = apMonochrome;
        }
}

TStatusLine *TProgram::initStatusLine( TRect r )
{
    r.a.y = r.b.y - 1;
    return new TStatusLine( r,
        *new TStatusDef( 0, 0xFFFF ) +
            *new TStatusItem( exitText, kbAltX, cmQuit ) +
            *new TStatusItem( 0, kbF10, cmMenu ) +
            *new TStatusItem( 0, kbAltF3, cmClose ) +
            *new TStatusItem( 0, kbF5, cmZoom ) +
            *new TStatusItem( 0, kbCtrlF5, cmResize )
            );
}

void TProgram::outOfMemory()
{
}

void TProgram::putEvent( TEvent & event )
{
    pending = event;
}

void TProgram::run()
{
    execute();
}

void TProgram::setScreenMode( ushort mode )
{
    TRect  r;

#ifndef __NT__
    TEventQueue::mouse.hide(); //HideMouse();
#endif
    TScreen::setVideoMode( mode );
    initScreen();
    buffer = TScreen::screenBuffer;
    r = TRect( 0, 0, TScreen::screenWidth, TScreen::screenHeight );
    changeBounds( r );
//    setState(sfExposed, False);
//    setState(sfExposed, True);
    redraw();
#ifndef __NT__
    TEventQueue::mouse.show(); //ShowMouse();
#endif
}

TView* TProgram::validView(TView* p)
{
    if( p == 0 )
        return 0;
    if( lowMemory() )
        {
        destroy( p );
        outOfMemory();
        return 0;
        }
    if( !p->valid( cmValid ) )
        {
        destroy( p );
        return 0;
        }
    return p;
}
