/*------------------------------------------------------------*/
/* filename -       editstat.cpp                              */
/*                                                            */
/* defines the static members of class TEditor                */
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

#define Uses_TEditor
#include <tv.h>

ushort defEditorDialog( int, ... )
{
    return cmCancel;
}

TEditorDialog TEditor::editorDialog = defEditorDialog;
ushort TEditor::editorFlags = efBackupFiles | efPromptOnReplace;
char TEditor::findStr[maxFindStrLen] = "";
char TEditor::replaceStr[maxReplaceStrLen] = "";
TEditor *TEditor::clipboard = 0;

