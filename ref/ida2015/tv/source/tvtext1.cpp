/*------------------------------------------------------------*/
/* filename -       tvtext1.cpp                               */
/*                                                            */
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

#define Uses_TScreen
#define Uses_TDeskTop
#define Uses_TRadioButtons
#define Uses_TMenuBox
#define Uses_TCombo
#define Uses_TFrame
#define Uses_TKeys
#define Uses_TIndicator
#define Uses_THistory
#define Uses_TColorSelector
#define Uses_TMonoSelector
#define Uses_TColorDialog
#define Uses_TInputLine
#define Uses_TStatusLine
#define Uses_TCheckBoxes
#define Uses_TScrollBar
#define Uses_TButton
#define Uses_TDirListBox
#define Uses_TFileEditor
#include <tv.h>

static unsigned getCodePage()
{
    return 437;
}

void TDisplay::updateIntlChars()
{
    if(getCodePage() != 437 )
        TFrame::frameChars[30] = 'Í';
}

#ifdef __UTF8__
void updatePseudoGraphics(void)
{
    static const TSymbol wspecialChars[] = {
        _UTF8_3('<'),    _UTF8_3('>'),
        _UTF8_3(0x2192), _UTF8_3(0x2190),
        _UTF8_3(' '),    _UTF8_3(' ')
    };
    static const TSymbol menuChars[] = {
        ' ', _UTF8_3(0x250C), _UTF8_3(0x2500), _UTF8_3(0x2510), ' ',
        ' ', _UTF8_3(0x2514), _UTF8_3(0x2500), _UTF8_3(0x2518), ' ',
        ' ', _UTF8_3(0x2502),             ' ', _UTF8_3(0x2502), ' ',
        ' ', _UTF8_3(0x251C), _UTF8_3(0x2500), _UTF8_3(0x2524), ' '
    };
    static const TSymbol wframeChars[] = {
                    ' ',             ' ',             ' ', _UTF8_3(0x2514),
                    ' ', _UTF8_3(0x2502), _UTF8_3(0x250C), _UTF8_3(0x251C),
                    ' ', _UTF8_3(0x2518), _UTF8_3(0x2500), _UTF8_3(0x2534),
        _UTF8_3(0x2510), _UTF8_3(0x2524), _UTF8_3(0x252C), _UTF8_3(0x253C),
                    ' ',             ' ',             ' ', _UTF8_3(0x255A),
                    ' ', _UTF8_3(0x2551), _UTF8_3(0x2554), _UTF8_3(0x255F),
                    ' ', _UTF8_3(0x255D), _UTF8_3(0x2550), _UTF8_3(0x2567),
        _UTF8_3(0x2557), _UTF8_3(0x2562), _UTF8_3(0x2564),             ' '
    };
    static const TSymbol scrollVchars[] = {
        _UTF8_3(0x25B2), _UTF8_3(0x25BC), _UTF8_3(0x2592), _UTF8_3(0x25AE), _UTF8_3(0x2593)
    };
    static const TSymbol scrollHchars[] = {
        _UTF8_3(0x25C4), _UTF8_3(0x25BA), _UTF8_3(0x2592), _UTF8_3(0x25AE), _UTF8_3(0x2593)
    };
    static const TSymbol inputLineRightArrow  = _UTF8_3(0x25BA);
    static const TSymbol inputLineLeftArrow   = _UTF8_3(0x25C4);
    static const TSymbol buttonShadows[] = {
        _UTF8_3(0x2584), _UTF8_3(0x2588), _UTF8_3(0x2580)
    };
    static const char *frameCloseIcon  = "[~\xE2\x80\xA2~]";
    static const char *frameZoomIcon   = "[~\xE2\x86\x91~]";
    static const char *frameUnZoomIcon = "[~\xE2\x86\x95~]";
    static const char *frameDragIcon   = "~\xE2\x94\x80\xE2\x94\x98~";
    static const char *historyIcon     = "\xE2\x96\x90~\xE2\x86\x93~\xE2\x96\x8C";
    static const char *hintSeparator   = "\xE2\x94\x82 ";

    uCreateCnvLocale();

    specialChars = wspecialChars;
    TMenuBox::frameChars = menuChars;
    memcpy(TFrame::frameChars, wframeChars, sizeof(wframeChars));
    TDeskTop::defaultBkgrnd = _UTF8_3(0x2591);
    memcpy(TScrollBar::vChars, scrollVchars, sizeof(scrollVchars));
    memcpy(TScrollBar::hChars, scrollHchars, sizeof(scrollHchars));
    TIndicator::dragFrame   = _UTF8_3(0x2550);
    TIndicator::normalFrame = _UTF8_3(0x2500);
    TColorSelector::icon    = _UTF8_3(0x2588);
    TInputLine::rightArrow  = &inputLineRightArrow;
    TInputLine::leftArrow   = &inputLineLeftArrow;
    TButton::shadows        = buttonShadows;
    TFrame::closeIcon  = frameCloseIcon;
    TFrame::zoomIcon   = frameZoomIcon;
    TFrame::unZoomIcon = frameUnZoomIcon;
    TFrame::dragIcon   = frameDragIcon;
    THistory::icon     = historyIcon;
    TCombo::icon       = historyIcon;
    TStatusLine::hintSeparator = hintSeparator;
    TRadioButtons::buttonDot = _UTF8_3(0x2022);
}
#endif

#ifdef __UTF8__
#define _UCH_   uchar
#else
#define _UCH_
#endif

static const TSymbol __specialChars[] =
{
#ifdef __RUS__
    '<', '>', 26, 27, ' ', ' '
#else
    175, 174, 26, 27, ' ', ' '
#endif
};
const TSymbol *specialChars = __specialChars;

const char *TRadioButtons::button = " ( ) ";
TSymbol TRadioButtons::buttonDot = '\7';

static const TSymbol _menuFrameChars[] = {
    ' ', _UCH_('\332'), _UCH_('\304'), _UCH_('\277'), ' ',
    ' ', _UCH_('\300'), _UCH_('\304'), _UCH_('\331'), ' ',
    ' ', _UCH_('\263'),           ' ', _UCH_('\263'), ' ',
    ' ', _UCH_('\303'), _UCH_('\304'), _UCH_('\264'), ' '
};
const TSymbol *TMenuBox::frameChars = _menuFrameChars;

const char TFrame::initFrame[19] =
  "\x06\x0A\x0C\x05\x00\x05\x03\x0A\x09\x16\x1A\x1C\x15\x00\x15\x13\x1A\x19";

TSymbol TFrame::frameChars[33] = {      // for UnitedStates code page
    _UCH_(' '), _UCH_(' '), _UCH_(' '), _UCH_('À'),
    _UCH_(' '), _UCH_('³'), _UCH_('Ú'), _UCH_('Ã'),
    _UCH_(' '), _UCH_('Ù'), _UCH_('Ä'), _UCH_('Á'),
    _UCH_('¿'), _UCH_('´'), _UCH_('Â'), _UCH_('Å'),
    _UCH_(' '), _UCH_(' '), _UCH_(' '), _UCH_('È'),
    _UCH_(' '), _UCH_('º'), _UCH_('É'), _UCH_('Ç'),
    _UCH_(' '), _UCH_('¼'), _UCH_('Í'), _UCH_('Ï'),
    _UCH_('»'), _UCH_('¶'), _UCH_('Ñ'), _UCH_(' '), 0
};

const char *TFrame::closeIcon = "[~\xFE~]";
const char *TFrame::zoomIcon = "[~\x18~]";
const char *TFrame::unZoomIcon = "[~\x12~]";
const char *TFrame::dragIcon = "~ÄÙ~";

TSymbol TIndicator::dragFrame = '\xCD';
TSymbol TIndicator::normalFrame = '\xC4';

const char *THistory::icon = "\xDE~\x19~\xDD";

TSymbol TColorSelector::icon = '\xDB';

const char *TMonoSelector::button = " ( ) ";
const char *TMonoSelector::normal = "Normal";
const char *TMonoSelector::highlight = "Highlight";
const char *TMonoSelector::underline = "Underline";
const char *TMonoSelector::inverse = "Inverse";

const char *TColorDialog::colors = "Colors";
const char *TColorDialog::groupText = "~G~roup";
const char *TColorDialog::itemText = "~I~tem";
const char *TColorDialog::forText = "~F~oreground";
const char *TColorDialog::bakText = "~B~ackground";
const char *TColorDialog::textText = "Text ";
const char *TColorDialog::colorText = "Color";
const char *TColorDialog::okText = "O~K~";
const char *TColorDialog::cancelText = "Cancel";

static const TSymbol _inputLineRightArrow = 0x10;
static const TSymbol _inputLineLeftArrow  = 0x11;
const TSymbol * TInputLine::rightArrow = &_inputLineRightArrow;
const TSymbol * TInputLine::leftArrow  = &_inputLineLeftArrow;

const char *TStatusLine::hintSeparator = "\xB3 ";

const char *TCheckBoxes::button = " [ ] ";

TScrollChars TScrollBar::vChars = { 30, 31, _UCH_('\xB1'), _UCH_('\xFE'), _UCH_('\xB2') };
TScrollChars TScrollBar::hChars = { 17, 16, _UCH_('\xB1'), _UCH_('\xFE'), _UCH_('\xB2') };

static const TSymbol _buttonShadows[] = { _UCH_('\xDC'), _UCH_('\xDB'), _UCH_('\xDF') };
const TSymbol *TButton::shadows = _buttonShadows;
const char *TButton::markers = "[]";

const char *TDirListBox::pathDir   = "\xC0\xC4\xC2";    // ÀÄÂ
const char *TDirListBox::firstDir  =   "\xC0\xC2\xC4";  //   ÀÂÄ
const char *TDirListBox::middleDir =   " \xC3\xC4";     //    ÃÄ
const char *TDirListBox::lastDir   =   " \xC0\xC4";     //    ÀÄ
const char *TDirListBox::drives = "Drives";
const char *TDirListBox::graphics = "\xC0\xC3\xC4";

TSymbol TDeskTop::defaultBkgrnd = '\xB0';

const char *TFileEditor::backupExt = ".BAK";

#ifndef __UTF8__
TSymbol TCombo::showChar = 25;              // a thin arrow
#else
TSymbol TCombo::showChar = _UTF8_3(0x2193); // a thin arrow
#endif
const char *TCombo::icon = "\xDE~\x19~\xDD";
ushort  TCombo::activateCode = kbDown;  // a down arrow
