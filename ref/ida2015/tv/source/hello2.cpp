/*---------------------------------------------------------*/
/*                                                         */
/*   Turbo Vision 1.0                                      */
/*   Copyright (c) 1991 by Borland International           */
/*                                                         */
/*   Turbo Vision Hello World Demo Source File             */
/*---------------------------------------------------------*/

#define Uses_TKeys
#define Uses_TApplication
#define Uses_TEvent
#define Uses_TRect
#define Uses_TDialog
#define Uses_TStaticText
#define Uses_TButton
#define Uses_TMenuBar
#define Uses_TSubMenu
#define Uses_TMenuItem
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TDeskTop
#define Uses_TFileDialog
#define Uses_TScreen
#define Uses_MsgBox
#define Uses_TMemo
#define Uses_TLabel
#define Uses_TInputLine
#define Uses_TIndicator
#define Uses_TSItem
#define Uses_TCheckBoxes
#define Uses_TFindDialogRec
#define Uses_TReplaceDialogRec

#define Uses_TTerminal
#define Uses_otstream

#include <tv.h>
#include <tvhelp.h>
#include <stdio.h>
#include <stdarg.h>

const int GreetThemCmd = 100;
const int FileBoxCmd = 101;
const int CodeCmd = 102;
const int TestCmd = 103;

ushort findDialog(TFindDialogRec *frd)
{
    TRect r(10,5,70,13);
    TDialog *d = new TDialog(r, "Find pattern" );

    TInputLine *il = new TInputLine( TRect( 2, 3, 58, 4 ), 80 );
    d->insert( il );
    d->insert( new TLabel( TRect( 2, 2, 22, 3 ), "~F~ind pattern", il ) );
    d->insert( new TButton( TRect(  6, 5, 12, 7 ), "Ok", cmOK, bfDefault ) );
    d->insert( new TButton( TRect( 16, 5, 28, 7 ), "Cancel", cmCancel, bfNormal ) );
    TCheckBoxes *cb = new TCheckBoxes( TRect( 32, 5, 52, 6 ), new TSItem("~C~ase sensitive", 0) );
    d->insert( cb );
    d->selectNext(False);

    ushort ret = TProgram::deskTop->execView( d );
    if (ret == cmOK) {
        il->getData(frd->find, maxFindStrLen);
        if (frd->find[0] == '\0')
            ret = cmCancel;
        frd->options &= ~efCaseSensitive;
        if (cb->mark(0)) frd->options |= efCaseSensitive;
    }
    TProgram::destroy(d);
    return ret;
}

ushort replaceDialog(TReplaceDialogRec *frd)
{
    TRect r(10,5,70,15);
    TDialog *d = new TDialog(r, "Find pattern" );

    TInputLine *il = new TInputLine( TRect( 2, 3, 58, 4 ), 80 );
    d->insert( il );
    d->insert( new TLabel( TRect( 2, 2, 22, 3 ), "~F~ind pattern", il ) );
    TInputLine *ir = new TInputLine( TRect( 2, 5, 58, 6 ), 80 );
    d->insert( ir );
    d->insert( new TLabel( TRect( 2, 4, 22, 5 ), "~R~eplace with", ir ) );
    d->insert( new TButton( TRect(  6, 7, 12, 9 ), "Ok", cmOK, bfDefault ) );
    d->insert( new TButton( TRect( 16, 7, 28, 9 ), "Cancel", cmCancel, bfNormal ) );
    TCheckBoxes *cb = new TCheckBoxes( TRect( 32, 7, 52, 8 ), new TSItem("~C~ase sensitive", 0) );
    d->insert( cb );
    d->selectNext(False);

    ushort ret = TProgram::deskTop->execView( d );
    if (ret == cmOK) {
        il->getData(frd->find, maxFindStrLen);
        if (frd->find[0] == '\0')
            ret = cmCancel;
        else
            ir->getData(frd->replace, maxReplaceStrLen);
        frd->options &= ~efCaseSensitive;
        if (cb->mark(0)) frd->options |= efCaseSensitive;
    }
    TProgram::destroy(d);
    return ret;
}

ushort testEditorDialog( int dialog, ... )
{
    va_list ap;
    va_start(ap, dialog);

    switch (dialog) {
    case edSearchFailed:
        messageBox("Search failed", mfInformation|mfOKButton);
        return cmCancel;
    case edFind:
        {   TFindDialogRec *frd = va_arg(ap, TFindDialogRec *);
            return findDialog(frd);
        }
    case edReplace:
        {   TReplaceDialogRec *frd = va_arg(ap, TReplaceDialogRec *);
            return replaceDialog(frd);
        }
    case edReplacePrompt:
        return cmYes;
    }
    return cmCancel;
}

class THelloApp : public TApplication
{

public:

    THelloApp();

    virtual void handleEvent( TEvent& event );
    static TMenuBar *initMenuBar( TRect );
    static TStatusLine *initStatusLine( TRect );
    void greetingBox();

private:

    void fileBox();
};

THelloApp::THelloApp() :
    TProgInit( THelloApp::initStatusLine,
               THelloApp::initMenuBar,
               THelloApp::initDeskTop
             )
{
    TEditor::editorDialog = testEditorDialog;
}

void THelloApp::greetingBox()
{
    TRect r(25,5,55,16);
//    r.move(-30,-7);
    TDialog *d = new TDialog(r, "Hello, World!" );

    d->insert( new TStaticText( TRect( 3, 5, 15, 6 ), "How are you?" ) );
    d->insert( new TButton( TRect( 16, 2, 28, 4 ), "Terrific", cmCancel, bfNormal ) );
    d->insert( new TButton( TRect( 16, 4, 28, 6 ), "Ok", cmCancel, bfNormal ) );
    d->insert( new TButton( TRect( 16, 6, 28, 8 ), "Lousy", cmCancel, bfNormal ) );
    d->insert( new TButton( TRect( 16, 8, 28, 10 ), "Cancel", cmCancel, bfNormal ) );

    deskTop->execView( d );
    destroy(d);
}

void msgbox(const char *str)
{
    TRect r(25,5,55,16);
    TDialog *d = new TDialog(r, "Message" );

    d->insert( new TStaticText( TRect( 3, 2, 28, 10 ), str ) );
    d->insert( new TButton( TRect( 10, 8, 20, 10 ), "Ok", cmOK, bfNormal ) );

    TProgram::deskTop->execView( d );
    TProgram::destroy(d);
}

void memoFile(char *Fname)
{
    FILE    *f = fopen(Fname, "rb");
    if (f == NULL)
        return;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    TMemoData *md;
    uchar *pp = (uchar *) qalloc(sizeof(md->length) + len + 1);
    fread(&pp[sizeof(md->length)], len, 1, f);
    fclose(f);
    pp[sizeof(md->length)+len] = '\0';
    TRect r(5, 1, 75, 21);
    TDialog *d = new TDialog(r, "Memo Test" );
    TRect rr(1, 1, 69, 19);
    TScrollBar *hs = new TScrollBar(TRect(15, 19, 69, 20));
    TScrollBar *vs = new TScrollBar(TRect(69, 1, 70, 19));
    TIndicator *Ind = new TIndicator(TRect(1, 19, 15, 20));
// TMemo( const TRect&, TScrollBar *, TScrollBar *, TIndicator *, size_t );
    TMemo *m = new TMemo(rr, hs, vs, Ind, 64*1024);
    md = (TMemoData *) pp;
    md->length = len;
    m->setData(pp);
    m->normalizeBuffer();
    d->insert(m);
    d->insert(hs);
    d->insert(vs);
    d->insert(Ind);
    TProgram::deskTop->execView( d );
    TProgram::destroy(d);
}

void THelloApp::fileBox() {
  TFileDialog *dialog = new TFileDialog("*.cpp","File","Select file",
                                fdOKButton|fdHelpButton,
                                5);
  ushort c = TProgram::deskTop->execView(dialog);
  char buf[MAXPATH];
  dialog->getFileName(buf, sizeof(buf));
  if ( c != cmCancel )
    memoFile(buf);
//    messageBox(buf, mfInformation|mfOKButton);
// printf("File: '%s'\n",buf);
  TObject::destroy( dialog );
}

void inputBox(void)
{
    char s[60];
    ushort c = inputBox("Тест ввода", "Метка", s, sizeof(s));
//    ushort c = inputBox("Тест ввода", "Label", s, sizeof(s));
    if (c != cmCancel)
      messageBox(s, mfInformation|mfOKButton);
}

void THelloApp::handleEvent( TEvent& event )
{
    TApplication::handleEvent( event );
    if( event.what == evCommand )
        {
        switch( event.message.command )
            {
            case GreetThemCmd:
                greetingBox();
                clearEvent( event );
                break;
            case FileBoxCmd:
                fileBox();
                clearEvent( event );
                break;
            case CodeCmd:
                msgbox("Code command has been invoked");
                break;
            case TestCmd:
                inputBox();
                clearEvent( event );
                break;
            default:
                break;
            }
        }
}

TMenuBar *THelloApp::initMenuBar( TRect r )
{

    r.b.y = r.a.y+1;

    return new TMenuBar( r,
//     ( *new TSubMenu( "~�~", kbAltSpace ) +
     ( *new TSubMenu( "~#~", kbAltSpace ) +
        *new TMenuItem( "~G~reeting...", GreetThemCmd, kbAltR )) +
     ( *new TSubMenu( "~H~ello", kbAltH ) +
        *new TMenuItem( "~G~reeting...", GreetThemCmd, kbAltR ) +
        *new TMenuItem( "~F~ilebox...", FileBoxCmd, kbAltB ) +
        *new TMenuItem( "~C~ode...", CodeCmd, 'C' ) +
        *new TMenuItem( "~T~est...", TestCmd, kbF11 ) +
         newLine() +
        *new TMenuItem( "E~x~it", cmQuit, cmQuit, hcNoContext, "Alt-X" ))
        );

}

TStatusLine *THelloApp::initStatusLine( TRect r )
{
    r.a.y = r.b.y-1;
    return new TStatusLine( r,
        *new TStatusDef( 0, 0xFFFF ) +
            *new TStatusItem( "~Alt-F~ Filebox", kbAltF, FileBoxCmd ) +
            *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmQuit ) +
            *new TStatusItem( 0, kbF10, cmMenu )
            );
}


//----------------------------------------------------------------------
int main(int argc,char *argv[])
{
#ifndef __MSDOS__
    int mode = 35 + (100<<8);
    if ( argc > 2 ) mode = atoi(argv[1]) + (atoi(argv[2]) << 8);
                        //  row                 col
    TScreen::setVideoMode(ushort(mode));
#endif

// init_kernel(CALLUI, argc, argv);

 THelloApp *app = new THelloApp;

#if 0
 TRect c(2,2,60,20);
// c.grow(-1,-1);

 TTerminal *tt = new TTerminal(c, NULL, NULL, 2048);
 TProgram::deskTop->insert(tt);
 otstream *tty = new otstream(tt);

 *tty << "test string";
 *tty << '\n';
 *tty << "test2\n";
 char const *p = "qq";
 *tty << p;
#endif

#if 0
  TProgram::application->suspend();
  TProgram::application->resume();
  TProgram::application->redraw();
#endif
  app->run();
//  doom("delete app\n");
  delete app;
//    TScreen::setVideoMode(35+(100<<8));
//    THelloApp app;
//    app.run();
//  doom("return 0\n");
  return 0;
}

#ifdef __IDA__
idaman void ActionKey(const char *, char *buf, size_t size ) { buf[0] = '\0'; }
idaman char *ida_export ivalue1(int) { return ""; }
idaman void ida_export verror(const char *message, va_list va)
{
  vfprintf(stderr, message, va);
  exit(-1);
}
#endif
