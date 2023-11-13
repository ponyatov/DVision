#ifndef __status_codes_h__
#define __status_codes_h__

const int
    DST_MOVE    = 0,
    DST_RESIZE  = 1,
    DST_MENU    = 2,
    DST_RESTORE = -1;

void setDragStatus(int status);

#endif // __status_codes_h__
