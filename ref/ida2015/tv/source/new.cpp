/*------------------------------------------------------------*/
/* filename -       new.cpp                                   */
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

#ifdef __BORLANDC__
#include <alloc.h>
#endif
#ifdef __WATCOMC__
#include <malloc.h>
#endif

#define Uses_TVMemMgr
#include <tv.h>

TBufListEntry *TBufListEntry::bufList = NULL;

TBufListEntry::TBufListEntry( void*& o ) : owner( o )
{
    next = bufList;
    prev = NULL;
    bufList = this;
    if( next != NULL )
        next->prev = this;
}

TBufListEntry::~TBufListEntry()
{
    owner = 0;
    if( prev == NULL )
        bufList = next;
    else
        prev->next = next;
    if( next != NULL )
        next->prev = prev;
    next = NULL;
    prev = NULL;
}

void *TBufListEntry::operator new( size_t sz, size_t extra )
{
    return qalloc( sz + extra );        // 25.01.96 ig (by Denis Petroff, 2:5030/287.14)
}

void *TBufListEntry::operator new( size_t ) throw()
{
    return NULL;
}

void TBufListEntry::operator delete( void *b )
{
    qfree( b );
}

Boolean TBufListEntry::freeHead()
{
    if( bufList == 0 )
        return False;
    else
        {
        delete bufList;
        return True;
        }
}

void *TVMemMgr::safetyPool = 0;
size_t TVMemMgr::safetyPoolSize = 0;
int TVMemMgr::inited = 0;

//static TVMemMgr memMgr;

TVMemMgr::TVMemMgr()
{
    if( !inited )
        resizeSafetyPool();
}

void TVMemMgr::resizeSafetyPool( size_t sz )
{
    inited = 1;
    qfree( safetyPool );
    if( sz == 0 )
        safetyPool = 0;
    else
        safetyPool = qalloc( sz );
    safetyPoolSize = sz;
}

int TVMemMgr::safetyPoolExhausted()
{
    return inited && (safetyPool == 0);
}

void TVMemMgr::allocateDiscardable( void *&adr, size_t sz )
{
    if( safetyPoolExhausted() )
        adr = 0;
    else
        {
        TBufListEntry *newEntry = new( sz ) TBufListEntry( adr );
        if( newEntry == 0 )
            adr = 0;
        else
            adr = (char *)newEntry + sizeof(TBufListEntry);
        }
}

void TVMemMgr::freeDiscardable( void *block )
{
    delete (TBufListEntry *)((char *)block - sizeof(TBufListEntry)); //lint !e424 delete of modified data
}
