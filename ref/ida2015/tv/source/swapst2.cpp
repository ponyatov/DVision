#define Uses_TSystemError
#define Uses_TScreen
#include <tv.h>

void TV_CDECL TSystemError::swapStatusLine( TDrawBuffer& tb )
{
  TCharHolder *scr = (TCharHolder *) (TScreen::screenBuffer + TScreen::screenWidth * (TScreen::screenHeight-1));
  TCharHolder *buf = (TCharHolder *) &tb;
  for ( int i=0; i < TScreen::screenWidth; i++ ) {
    TCharHolder x = *scr;
    *scr++ = *buf;
    *buf++ = x;
  }
}
