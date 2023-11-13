#ifndef ___extrn_h_
#define ___extrn_h__

//=============================================================================

#define ida_local
#define qfree           free
#define qisspace        isspace
#define qisalpha        isalpha
#define qisalnum        isalnum
#define qtoupper        toupper
#define qalloc          malloc
#define qrealloc        realloc
#define qgetenv         getenv
#define qfopen          fopen
#define qfseek          fseek
#define qftell          ftell
#define qfclose         fclose
#define qrename         rename
#define qunlink		_unlink
#define qstrdup		_strdup
#define qfread(f,b,s)	fread(b,s,1,f)
#define qfwrite(f,b,s)	fwrite(b,s,1,f)

#define OPERATOR_NEW(c,s) new c[s]

#define FA_DIREC	_A_SUBDIR
#define FA_RDONLY	_A_RDONLY
#define FA_ARCH		_A_ARCH

#ifndef MAXSTR
#define MAXSTR	  1024
#endif

#ifndef MAXPATH
#define MAXPATH   _MAX_PATH
#define MAXDIR    _MAX_DIR
#define MAXFILE   _MAX_FNAME
#define MAXEXT    _MAX_EXT
#define MAXDRIVE  _MAX_DRIVE
#endif

#define NT_CDECL	__cdecl

typedef unsigned 	int uint;

#include <string>
#include <vector>
typedef std::vector<std::string> qstrvec_t;
typedef std::string     qstring;

//-----------------------------------------------------------------------------

#ifdef Uses_TProgram
#include <windows.h>
extern bool     doswin32;
extern HANDLE   request_ready;
void store_functor_request(void);
#endif

//-----------------------------------------------------------------------------

struct qffblk_t                       // Win32 - use Borland's ffblk
{
    long           ff_reserved;
    long           ff_fsize;
    unsigned long  ff_attrib;
    unsigned short ff_ftime;
    unsigned short ff_fdate;
    char           ff_name[MAXPATH];
};

// Find first file that matches the pattern
//      pattern - file name pattern, usually with * and ? wildcards
//      blk     - structure that will hold the answer
//                blk->ff_name will hold the file name, for example
//      attr    - the desired file types (FA_DIREC or 0)
// Returns: 0 if found a file, other values mean error

int qfindfirst64(const char *pattern, struct qffblk64_t *blk, int attr);


// Find next file that matches the pattern
//      blk     - structure that holds the current state
//                blk->ff_name will hold the next file name upon return
// Returns: 0 if found the next file, other values mean error

int qfindnext64(struct qffblk64_t *blk);

// Stop the file enumeration and free internal structures
//      blk     - file enumeration structure

void qfindclose64(struct qffblk64_t *blk);

struct qffblk64_t
{
  int ff_attrib;
  char ff_name[MAXPATH];
  uint64 ff_fsize;
  uint16 ff_fdate;
  uint16 ff_ftime;
  // private field
  struct qffblk_t base;
  qffblk64_t(void) {}
  ~qffblk64_t(void) { qfindclose64(this); }
};

//=============================================================================

#include <stdlib.h>     /* size_t, NULL, memory */
#include <stdarg.h>
#include <stddef.h>
#include <assert.h>
#include <ctype.h>
#if defined(__BORLANDC__)
#  include <io.h>
#  include <dir.h>
#  ifdef __NT__
#    include <alloc.h>
#  endif
#  include <new.h>
#define WIN32_LEAN_AND_MEAN
#else
#if defined(__WATCOMC__) || defined(_MSC_VER)
#ifndef UNDER_CE
#  include <io.h>
#  include <direct.h>
#endif
#  include <limits.h>
#else
#  include <unistd.h>
#  include <sys/stat.h>
#endif
#endif
#ifdef UNDER_CE         // Windows CE does not have many files...
#define getenv(x) NULL  // no getenv under Windows CE
int rename(const char *ofile, const char *nfile);
int unlink(const char *file);
#else
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
#ifdef __X64__
#include <time.h>
#endif

#if defined(__GNUC__)
#define NORETURN  __attribute__((noreturn))
#define AS_PRINTF(format_idx, varg_idx) __attribute__((format(printf, format_idx, varg_idx)))
#define AS_SCANF(format_idx, varg_idx)  __attribute__((format(scanf, format_idx, varg_idx)))
#else
#define NORETURN  __declspec(noreturn)
#define AS_PRINTF(format_idx, varg_idx)
#define AS_SCANF(format_idx, varg_idx)
#endif

//-----------------------------------------------------------------------
typedef          char    int8;
typedef signed   char    sint8;
typedef unsigned char    uint8;
typedef          __int16 int16;
typedef unsigned __int16 uint16;
typedef          __int32 int32;
typedef unsigned __int32 uint32;
typedef          __int64 int64;
typedef unsigned __int64 uint64;

#ifdef __X64__
typedef int64 ssize_t;
#else
typedef int32 ssize_t;
#endif

#define qmin(a,b)   ((a) < (b)? (a): (b))
#define qmax(a,b)   ((a) > (b)? (a): (b))
#define qnumber(a)  (sizeof(a)/sizeof((a)[0]))

//------------------------------------------------------------------------
char *qstrncpy(char *dst, const char *src, size_t dstsize);
char *qstrncat(char *dst, const char *src, size_t dstsize);
int  qsnprintf(char *buffer, size_t n, const char *format, ...) AS_PRINTF(3, 4);
int  qvsnprintf(char *buffer, size_t n, const char *format, va_list va) AS_PRINTF(3, 0);

//-------------------------------------------------------------------------
#if defined(__MSDOS__) || defined(__OS2__) || defined(__NT__)
#define __FAT__
#define SDIRCHAR "\\"
#define DIRCHAR '\\'
#define DRVCHAR ':'
#else
#define SDIRCHAR "/"
#define DIRCHAR '/'
#endif

#define EXTCHAR '.'

#if defined(__X64__) && defined(__VC__)

#define MAXDRIVE  _MAX_DRIVE
#define MAXDIR    _MAX_DIR
#define MAXFILE   _MAX_FNAME
#define MAXEXT    _MAX_EXT
#define MAXPATH   _MAX_PATH

struct qffblk_t : public __finddata64_t
{
  intptr_t handle;
};

#define FA_DIREC    _A_SUBDIR
#define FA_RDONLY   _A_RDONLY
#define FA_ARCH     _A_ARCH

#define qfindfirst(file,blk,attr) (((blk)->handle=_findfirst64(file,blk))==intptr_t(-1L))
#define qfindnext(blk)            (_findnext64((blk)->handle,blk)!=0)
#define qfindclose(blk)           _findclose((blk)->handle)

#define ff_attrib   attrib
#define ff_name     name
#define ff_ftime    time_write
#define ff_fsize    size

#define localtime   _localtime32
#define time_t      __time32_t

#endif // __X64__ && __VC__

#if defined(_MSC_VER) && _MSC_VER > 1200
#pragma warning(disable : 4200) // zero-sized array in structure (non accept from cmdline)
#endif

#endif // ___extrn_h__
