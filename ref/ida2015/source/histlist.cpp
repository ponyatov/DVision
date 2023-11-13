/*------------------------------------------------------------*/
/* filename - histlist.cpp                                    */
/*                                                            */
/* function(s)                                                */
/*          startId                                           */
/*          historyCount                                      */
/*          historyAdd                                        */
/*          historyStr                                        */
/*          clearHistory                                      */
/*          initHistory                                       */
/*          doneHistory                                       */
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

#define USE_DANGEROUS_FUNCTIONS
#define Uses_TApplication
#include <tv.h>
class HistList : public qvector<qstrvec_t> {};
static HistList histList;

#ifdef _MSC_VER
#pragma warning(disable:4291)   // no placement delete
#endif

static void insertString( historyId_t id, const char *str )
{
  if ( id >= histList.size() )
    histList.resize(id+1);
  qstrvec_t &h = histList[id];
  h.insert(h.begin(), str);
}

ushort historyCount( historyId_t id )
{
  if ( id >= histList.size() )
    return 0;;
  qstrvec_t &h = histList[id];
  return h.size();
}

void historyAdd( historyId_t id, const char *str )
{
    if( str[0] == EOS )
        return;
    if ( id >= histList.size() )
      histList.resize(id+1);
    qstrvec_t &h = histList[id];
    for ( qstrvec_t::iterator ptr = h.begin(); ptr < h.end(); ++ptr )
    {
      if ( *ptr == str )
      {
        h.erase(ptr);
        break;
      }
    }
    insertString(id, str);
}

const char *historyStr( historyId_t id, int index )
{
  if ( id >= histList.size() )
    return NULL;
  qstrvec_t &h = histList[id];
  if ( index >= h.size() )
    return NULL;
  return h[index].c_str();
}

void initHistory()
{
  histList.clear();
}

void doneHistory()
{
  initHistory();
}
