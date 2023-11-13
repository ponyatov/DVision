/*-------------------------------------------------------------------*/
/* filename -       tapplica.cpp                                     */
/*                                                                   */
/* function(s)                                                       */
/*          TApplication member functions (constructor & destructor) */
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*                                                                   */
/*    Turbo Vision -  Version 1.0                                    */
/*                                                                   */
/*                                                                   */
/*    Copyright (c) 1987,1988,1990 by Borland International          */
/*    All Rights Reserved.                                           */
/*                                                                   */
/*-------------------------------------------------------------------*/

#define Uses_TSystemError
#define Uses_TEventQueue
#define Uses_TScreen
#define Uses_TObject
#define Uses_TApplication
#include <tv.h>

//lint -esym(528, tsc, teq, sysErr) not referenced
TMouse TEventQueue::mouse;
static TScreen tsc;
static TEventQueue teq;
static TSystemError sysErr;

TApplication::TApplication() :
    TProgInit( TApplication::initStatusLine,
                  TApplication::initMenuBar,
                  TApplication::initDeskTop
                )
{
    initHistory();
}

TApplication::~TApplication()
{
    doneHistory();
}

void TApplication::suspend()
{
#ifdef __MSDOS__
    TSystemError::suspend();
    TEventQueue::suspend();
    TScreen::suspend();
#else
    TSystemInit::suspend();
#endif
}

void TApplication::resume()
{
#ifdef __MSDOS__
    TScreen::resume();
    TEventQueue::resume();
    TSystemError::resume();
#else
    TSystemInit::resume();
#endif
}
