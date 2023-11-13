/*------------------------------------------------------------*/
/* filename -       newstr.cpp                                */
/*                                                            */
/* function(s)                                                */
/*                  newStr member function                    */
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


#include <tv.h>

#ifdef __IDA__

// use IDA's memory management
#include <pro.h>
char *newStr( const char *s )
{
    return qstrdup(s);
}

void delStr( const char *s )
{
    qfree((void*)s);
}

#else  // __IDA__

char *newStr( const char *s )
{
    if( s == 0 )
        return 0;
    size_t size = strlen(s) + 1;
    char *temp = new char[ size ];
    memcpy( temp, s, size );
    return temp;
}

void delStr( const char *s )
{
    delete [] (char*) s;
}

#endif // __IDA__
