/*------------------------------------------------------------*/
/* filename -       tfildlg.cpp                               */
/*                                                            */
/* function(s)                                                */
/*                  TFileDialog member functions              */
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

#define Uses_TFileDialog
#define Uses_MsgBox
#define Uses_TRect
#define Uses_TFileInputLine
#define Uses_TButton
#define Uses_TLabel
#define Uses_TFileList
#define Uses_THistory
#define Uses_TScrollBar
#define Uses_TEvent
#define Uses_TFileInfoPane
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>

TFileDialog::TFileDialog( const char *aWildCard,
                          const char *aTitle,
                          const char *inputName,
                          ushort aOptions,
                          historyId_t histId
                        ) :
    TWindowInit(TFileDialog::initFrame),
    TDialog(TRect(15, 1, 64, 21), aTitle),
    directory( NULL ),
    dlg_opts(aOptions),
    input_name(inputName),
    hist_id(histId)
{    
  ustrncpy(wildCard, aWildCard, sizeof(wildCard));
  fileName = NULL;
  fileList = NULL;
  if ( (fdNoCreateControls & aOptions) == 0 )
    fill();
}

void TFileDialog::fill()
{
  options |= ofCentered;

  createInput(2, 2, wildCard);
  createFileList(2, 5, 12);
  createButtons(35, 3);
  insert(new TFileInfoPane(TRect(1, 16, 48, 19)));
  selectNext( False );
  if( (dlg_opts & fdNoLoadDir) == 0 )
    readDirectory();
}

// There is no memory leak because fileList was passed to TLabel
//lint -esym(423, TFileDialog::fileList) Creation of memory leak in assignment to ''
TFileDialog::~TFileDialog()
{
  delStr( directory );
  fileList = NULL;
  fileName = NULL;
  input_name = NULL;
}

void TFileDialog::createInput(int x, int y, const char *text)
{
  fileName = new TFileInputLine(TRect( x+1, y+1, x+29, y+2 ), MAXPATH);
  fileName->str = text;
  insert(fileName);
  insert(new TLabel(TRect(x, y, 3+ucstrlen(input_name), y+1),
                        input_name,
                        fileName
                      ));
  insert(new THistory(TRect( x+29, y+1, x+32, y+2), fileName, hist_id));

}

void TFileDialog::createFileList(int x, int y, int h)
{  
  TScrollBar *sb = new TScrollBar(TRect(x+1, y+h-2, x+32, y+h-1));
  insert(sb);
  insert(fileList = new TFileList(TRect(x+1, y+1, x+32, y+h-2), sb));
  insert(new TLabel(TRect(x, y, x+6, y+1), filesText, fileList));
}

void TFileDialog::createButtons(int x, int y)
{
  ushort opt = bfDefault;
  TRect r(x, y, x+11, y+2);

  if ( (dlg_opts & fdOpenButton) != 0 )
  {
    insert(new TButton(r, openText, cmFileOpen, opt));
    opt = bfNormal;
    r.a.y += 3;
    r.b.y += 3;
  }

  if ( (dlg_opts & fdOKButton) != 0 )
  {
    insert(new TButton(r, okText, cmFileOpen, opt));
    opt = bfNormal;
    r.a.y += 3;
    r.b.y += 3;
  }

  if ( (dlg_opts & fdReplaceButton) != 0 )
  {
    insert(new TButton(r, replaceText, cmFileReplace, opt));
    opt = bfNormal;
    r.a.y += 3;
    r.b.y += 3;
  }

  if ( (dlg_opts & fdClearButton) != 0 )
  {
    insert(new TButton(r, clearText, cmFileClear, opt));
    r.a.y += 3;
    r.b.y += 3;
  }

  insert(new TButton( r, cancelText, cmCancel, bfNormal));
  r.a.y += 3;
  r.b.y += 3;

  if ( (dlg_opts & fdHelpButton) != 0 )
  {
    insert(new TButton(r, helpText, cmHelp, bfNormal));
    r.a.y += 3;
    r.b.y += 3;
  }
}

void TFileDialog::shutDown()
{
    fileName = NULL;
    fileList = NULL;
    TDialog::shutDown();
}

static Boolean relativePath( const char *path )
{
    if( path[0] == DIRCHAR )
        return False;
#ifdef __FAT__
    if( path[0] != EOS && path[1] == ':' )
        return False;
#endif
    return True;
}

#ifdef __FAT__
static void noWildChars( char *dest, const char *src )
{
    while( *src != EOS )
        {
        if( *src != '?' && *src != '*' )
            *dest++ = *src;
        src++;
        }
    *dest = EOS;
}
#endif

#ifdef __MSDOS__
static void trim( char *dest, const char *src, size_t destsize )
{
  if ( ssize_t(destsize) > 0 )
  {
    while( *src != EOS && qisspace(*src) )
        src++;
    char *end = dest + destsize;
    while( *src != EOS && !qisspace(*src) && dest < end )
        *dest++ = *src++;
    if ( dest >= end )
      dest--;
    *dest = EOS;
  }
}
#else
#define trim ustrncpy
#endif

void TFileDialog::getFileName( char *str, size_t strsize ) const
{
char buf[2*MAXPATH];

#ifdef __FAT__
char drive[MAXDRIVE];
char path[MAXDIR];
char name[MAXFILE];
char ext[MAXEXT];
char TName[MAXFILE];
char TExt[MAXEXT];

    if ( fileName->str[0] == '.'                                 // +++ yjh
      && (fileName->str[1] == DIRCHAR || fileName->str[1] == '\0') ) // +++ yjh
    {                                                            // +++ yjh
      getcwd(buf, sizeof(buf));                                  // +++ yjh
      if ( fileName->str[2] )                                    // +++ yjh
        trim( buf + strlen(buf), &fileName->str[1], sizeof(buf)-strlen(buf) ); // +++ yjh
    }                                                            // +++ yjh
    else                                                         // +++ yjh
    {
      trim( buf, fileName->str.c_str(), sizeof(buf) );
    }
    if( relativePath( buf ) == True )
        {
        qstrncpy( buf, directory, sizeof(buf) );
        trim( buf + strlen(buf), fileName->str.c_str(), sizeof(buf)-strlen(buf) );
        }
    fexpand( buf, sizeof(buf) );
    fnsplit( buf, drive, path, name, ext );
//    printf("split %s\n drive=%s,path=%s,name=%s,ext=%s\n",buf,drive,path,name,ext);
    if( (name[0] == EOS || ext[0] == EOS) && !isDir( buf ) )
        {
        fnsplit( wildCard, 0, 0, TName, TExt );
        if( name[0] == EOS && ext[0] == EOS )
            fnmerge( buf, drive, path, TName, TExt );
        else if( name[0] == EOS )
            fnmerge( buf, drive, path, TName, ext );
        else if( ext[0] == EOS )
            {
            if( isWild( name ) )
                fnmerge( buf, drive, path, name, TExt );
            else
                {
                fnmerge( buf, drive, path, name, 0 );
                noWildChars( buf + strlen(buf), TExt );
                }
            }
        }
#else
  ustrncpy( buf, fileName->str.c_str(), sizeof(buf) );
  if( relativePath( buf ) == True )
  {
    ustrncpy( buf, directory, sizeof(buf) );
    ustrncpy( buf + strlen(buf), fileName->str.c_str(), sizeof(buf)-strlen(buf) );
  }
  fexpand( buf, sizeof(buf) );
#endif
  ustrncpy( str, buf, strsize );
}

void TFileDialog::handleEvent(TEvent& event)
{
  TDialog::handleEvent(event);
  if( event.what == evCommand )
    switch( event.message.command )
    {
      case cmFileOpen:
      case cmFileReplace:
      case cmFileClear:
        endModal(event.message.command);
        clearEvent(event);
        break;
      default:
        break;
    }
}

void TFileDialog::readDirectory()
{
    char curDir[MAXPATH];
#ifdef __FAT__
    if ( relativePath(wildCard) )
    {
      getCurDir( curDir, sizeof(curDir) );
    }
    else
    {
      char drive[MAXDRIVE], dir[MAXDIR], name[MAXFILE], ext[MAXEXT];
      fnsplit( wildCard, drive, dir, name, ext );
      qstrncpy(curDir, drive, sizeof(curDir));
      qstrncat(curDir, dir, sizeof(curDir));
      qstrncpy(wildCard, name, sizeof(wildCard));
      qstrncat(wildCard, ext, sizeof(wildCard));
    }
#else
    getCurDir( curDir, sizeof(curDir) );
#endif
    if ( directory != NULL )
      delStr( directory );
    directory = newStr( curDir );
    fileList->readDirectory( directory, wildCard );    
}

void TFileDialog::setData( void *rec )
{
  TDialog::setData( rec );
  if( *(char *)rec != EOS && isWild( (char *)rec ) )
  {
      valid( cmFileInit );
      fileName->select();
  }
}

void TFileDialog::getData( void *rec, size_t recsize )
{
    getFileName( (char *)rec, recsize );
}

Boolean TFileDialog::checkDirectory( const char *str )
{
  if( pathValid( str ) )
    return True;

  messageBox( invalidDriveText, mfError | mfOKButton );
  fileName->select();
  return False;
}

Boolean TFileDialog::valid(ushort command)
{
  if ( !TDialog::valid( command ) )
    return False;
  if ( command == cmValid || command == cmCancel || command == cmFileClear )
       return True;

  char fName[MAXPATH], name[MAXFILE];
#ifdef __FAT__
  char dir[MAXDIR];
#endif

  getFileName(fName, sizeof(fName));
  if ( isWild(fName) )
  {
    char path[MAXPATH];
#ifdef __FAT__
    char drive[MAXDRIVE];
    char ext[MAXEXT];
    fnsplit( fName, drive, dir, name, ext );
    // Form path
    qstrncpy( path, drive, sizeof(path) );
    qstrncat( path, dir, sizeof(path) );
    // Form file name (with its extension)
    qstrncat( name, ext, sizeof(name) );
#else
    expandPath(fName, path, sizeof(path), name, sizeof(name));
#endif
    if ( checkDirectory( path ) )
    {
      delStr( directory );
      directory = newStr( path );
      ustrncpy( wildCard, name, sizeof(wildCard) );
      if( command != cmFileInit ) fileList->select();
      fileList->readDirectory( directory, wildCard );
    }
    return False;
  }
  if ( isDir(fName) )
  {
    if ( checkDirectory(fName) )
    {
      delStr( directory );
      if ( fName[strlen(fName)-1] != DIRCHAR )
        qstrncat( fName, SDIRCHAR, sizeof(fName) );
      directory = newStr( fName );
      if ( command != cmFileInit )
        fileList->select();
      fileList->readDirectory( directory, wildCard );
    }
    return False;
  }
  if ( validFileName(fName) ) return True;

  messageBox( invalidFileText, mfError | mfOKButton );
  return False;
}

#ifndef NO_TV_STREAMS
void TFileDialog::write( opstream& os )
{
    TDialog::write( os );
    os.writeString( wildCard );
    os << fileName << fileList;
}

void *TFileDialog::read( ipstream& is )
{
    TDialog::read( is );
    is.readString( wildCard, sizeof(wildCard) );
    is >> fileName >> fileList;
    readDirectory();
    return this;
}

TStreamable *TFileDialog::build()
{
    return new TFileDialog( streamableInit );
}
#endif  // ifndef NO_TV_STREAMS
