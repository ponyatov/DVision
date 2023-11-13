#
#   Borland C++ - (C) Copyright 1991, 1992 by Borland International
#   Makefile for building Turbo Vision library
#

#
#	Please note that default .CFG files define NO_TV_STREAMS for
#	TVision w/o streams.
#

#
#	__IDA__ changes behaviour of TVision in some places:
#		- color selector allows to select bright backgrounds
#		- color display shows "black on black" as is (was: blinking on red)
#		- use snprintf() function (see msgbox.cpp)
#

# MAC_EXPORT_ALL=1 # export all symbols under MAC OS X
_CFLAGS =-I../include -D__NOROW__ -D__RUS__ -D__IDA__ -DNO_TV_STREAMS -D__IDP__
				# see tscreen.cpp for the use of __NOROW__
				# __RUS__ changes some characters to russian
#Warning 1506 Call to virtual function '' within a constructor or destructor
#Warning 1524 new in constructor for class '' which has no explicit destructor
#Info 1732 new in constructor for class '' which has no assignment operator
#Info 1733 new in constructor for class '' which has no copy constructor
#Info 1747 binary operator '' returning a reference
#Info 1749 base class '' of class '' need not be virtual
LINTFLAGS="-e1506 -e1524 -e1732 -e1733 -e1747 -e1749 -esym(1754,operator+=)"

ifdef __LINUX__
  ALLOW_UNDEFS=1
endif

include ../../allmake.mak

OBJS=   $(F)colorsel$(O) $(F)drivers$(O)  $(F)edits2$(O)   $(F)editstat$(O) \
        $(F)frmline2$(O) $(F)grp$(O)      $(F)help$(O)     $(F)helpbase$(O) \
        $(F)histlist$(O) $(F)linux$(O)    $(F)mapcolor$(O) $(F)menu$(O)     \
        $(F)misc$(O)     $(F)msgbox$(O)   $(F)names$(O)    $(F)new$(O)      \
        $(F)newstr$(O)   $(F)os2handl$(O) $(F)palette$(O)  $(F)sall$(O)     \
        $(F)stddlg$(O)   $(F)swapst2$(O)  $(F)syserr$(O)   $(F)tapplica$(O) \
        $(F)tbkgrnd$(O)  $(F)tbutton$(O)  $(F)tchdrdlg$(O) $(F)tcheckbo$(O) \
        $(F)tcluster$(O) $(F)tcmdset$(O)  $(F)tcollect$(O) $(F)tdesktop$(O) \
        $(F)tdialog$(O)  $(F)tdircoll$(O) $(F)tdirlist$(O) $(F)teditor1$(O) \
        $(F)teditor2$(O) $(F)teditwnd$(O) $(F)tevent$(O)   $(F)textview$(O) \
        $(F)tfildlg$(O)  $(F)tfilecol$(O) $(F)tfiledtr$(O) $(F)tfillist$(O) \
        $(F)tframe$(O)   $(F)tgrmv2$(O)   $(F)tgroup$(O)   $(F)thistory$(O) \
        $(F)thistwin$(O) $(F)thstview$(O) $(F)tindictr$(O) $(F)tinputli$(O) \
        $(F)tlabel$(O)   $(F)tlistbox$(O) $(F)tlstview$(O) $(F)tmemo$(O)    \
        $(F)tmenubar$(O) $(F)tmenubox$(O) $(F)tmnuview$(O) $(F)tmouse$(O)   \
        $(F)tobject$(O)  $(F)tobjstrm$(O) $(F)tparamte$(O) $(F)tpoint$(O)   \
        $(F)tprogram$(O) $(F)tradiobu$(O) $(F)trescoll$(O) $(F)tresfile$(O) \
        $(F)tscreen$(O)  $(F)tscrlbar$(O) $(F)tscrolle$(O) $(F)tsortcol$(O) \
        $(F)tstatict$(O) $(F)tstatusl$(O) $(F)tstrcoll$(O) $(F)tstrlist$(O) \
        $(F)ttprvln2$(O) $(F)tvcursr2$(O) $(F)tvexpsd2$(O) $(F)tview$(O)    \
        $(F)tvtext1$(O)  $(F)tvtext2$(O)  $(F)tvwrite2$(O) $(F)twindow$(O)  \
        $(F)ustrings$(O)

ifdef __NT__
  MAINGOAL=$(L)tv$(A)
else
  ifndef __BSD__
    DL=-ldl
  endif
  ifdef __MAC__
    INSTALL_NAME=-Wl,-install_name,@executable_path/libtvision$(DLLSUFF)
    UNDEFS=-undefined dynamic_lookup
  else
    EXPORTS_DEF=tvision.script
    UNDEFS=-Wl,--allow-shlib-undefined
  endif
  MAINGOAL=objdir $(R)libtvision$(DLLSUFF)
endif

all: objdir $(MAINGOAL)

ifdef __NT__
hello:	./hello$(B) $(L)tv$(A)
./hello$(B): $(F)tv.at $(F)hello$(O)
else
./$(F)hello$(B): $(L)tv$(A) $(F)hello$(O)
endif
	$(CCL) $(OUTSW)$@ $(F)hello$(O) $(OBJS) $(L)pro$(A) $(DL)

$(R)libtvision$(DLLSUFF): $(OBJS) $(EXPORTS_DEF)
	$(CCL) $(OUTDLL) $(OUTSW)$@ $(EXPORTS_DEF) $(OBJS) $(DL) $(UNDEFS) $(INSTALL_NAME)

$(L)tv$(A): $(OBJS)
ifdef __NT__
	lib /out:$(L)tv$(A) $(OBJS)
else
	ar ruv $(L)tv$(A) $(OBJS)
endif

include $(IDA)objdir.mak
#-----------------------------------------------------------------------
$(F)hello$(O)   : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/helpbase.h                \
	          ../include/menus.h ../include/msgbox.h                    \
	          ../include/objects.h ../include/resource.h                \
	          ../include/stddlg.h ../include/system.h                   \
	          ../include/textview.h ../include/tkeys.h                  \
	          ../include/tobjstrm.h ../include/ttypes.h                 \
	          ../include/tv.h ../include/tvhelp.h ../include/tvobjs.h   \
	          ../include/views.h hello.cpp

$(F)colorsel$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h colorsel.cpp
$(F)drivers$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h drivers.cpp
$(F)edits2$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h edits2.cpp
$(F)editstat$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h editstat.cpp
$(F)frmline2$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h frmline2.cpp
$(F)grp$(O)     : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h grp.cpp
$(F)help$(O)    : $(I)llong.hpp $(I)pro.h $(I)prodir.h ../include/app.h     \
	          ../include/buffers.h ../include/cm_codes.h                \
	          ../include/colorsel.h ../include/dialogs.h                \
	          ../include/drawbuf.h ../include/editors.h                 \
	          ../include/helpbase.h ../include/menus.h                  \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvhelp.h   \
	          ../include/tvobjs.h ../include/views.h help.cpp
$(F)helpbase$(O): $(I)help.h $(I)prodir.h ../include/app.h                  \
	          ../include/buffers.h ../include/cm_codes.h                \
	          ../include/colorsel.h ../include/dialogs.h                \
	          ../include/drawbuf.h ../include/editors.h                 \
	          ../include/helpbase.h ../include/menus.h                  \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h helpbase.cpp
$(F)histlist$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h histlist.cpp
$(F)linux$(O)   : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h linux.cpp
$(F)mapcolor$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h mapcolor.cpp
$(F)menu$(O)    : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h menu.cpp
$(F)misc$(O)    : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h misc.cpp
$(F)msgbox$(O)  : $(I)llong.hpp $(I)pro.h $(I)prodir.h ../include/app.h     \
	          ../include/buffers.h ../include/cm_codes.h                \
	          ../include/colorsel.h ../include/dialogs.h                \
	          ../include/drawbuf.h ../include/editors.h                 \
	          ../include/menus.h ../include/msgbox.h                    \
	          ../include/objects.h ../include/resource.h                \
	          ../include/stddlg.h ../include/system.h                   \
	          ../include/textview.h ../include/tkeys.h                  \
	          ../include/tobjstrm.h ../include/ttypes.h                 \
	          ../include/tv.h ../include/tvobjs.h ../include/views.h    \
	          msgbox.cpp
$(F)names$(O)   : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h names.cpp
$(F)new$(O)     : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h new.cpp
$(F)newstr$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h newstr.cpp
$(F)os2handl$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h os2handl.cpp
$(F)palette$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h palette.cpp
$(F)sall$(O)    : sall.cpp
$(F)stddlg$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h stddlg.cpp
$(F)swapst2$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h swapst2.cpp
$(F)syserr$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h syserr.cpp
$(F)tapplica$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tapplica.cpp
$(F)tbkgrnd$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tbkgrnd.cpp
$(F)tbutton$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tbutton.cpp
$(F)tchdrdlg$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tchdrdlg.cpp
$(F)tcheckbo$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tcheckbo.cpp
$(F)tcluster$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tcluster.cpp
$(F)tcmdset$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tcmdset.cpp
$(F)tcollect$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tcollect.cpp
$(F)tdesktop$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tdesktop.cpp
$(F)tdialog$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tdialog.cpp
$(F)tdircoll$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tdircoll.cpp
$(F)tdirlist$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tdirlist.cpp
$(F)teditor1$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h teditor1.cpp
$(F)teditor2$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h teditor2.cpp
$(F)teditwnd$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h teditwnd.cpp
$(F)tevent$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tevent.cpp
$(F)textview$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h textview.cpp
$(F)tfildlg$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tfildlg.cpp
$(F)tfilecol$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tfilecol.cpp
$(F)tfiledtr$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tfiledtr.cpp
$(F)tfillist$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tfillist.cpp
$(F)tframe$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tframe.cpp
$(F)tgrmv2$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tgrmv2.cpp
$(F)tgroup$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tgroup.cpp
$(F)thistory$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h thistory.cpp
$(F)thistwin$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h thistwin.cpp
$(F)thstview$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h thstview.cpp
$(F)tindictr$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tindictr.cpp
$(F)tinputli$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tinputli.cpp
$(F)tlabel$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tlabel.cpp
$(F)tlistbox$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tlistbox.cpp
$(F)tlstview$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tlstview.cpp
$(F)tmemo$(O)   : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tmemo.cpp
$(F)tmenubar$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tmenubar.cpp
$(F)tmenubox$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tmenubox.cpp
$(F)tmnuview$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tmnuview.cpp
$(F)tmouse$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tmouse.cpp
$(F)tobject$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tobject.cpp
$(F)tobjstrm$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tobjstrm.cpp
$(F)tparamte$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tparamte.cpp
$(F)tpoint$(O)  : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tpoint.cpp
$(F)tprogram$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/helpbase.h                \
	          ../include/menus.h ../include/msgbox.h                    \
	          ../include/objects.h ../include/resource.h                \
	          ../include/stddlg.h ../include/system.h                   \
	          ../include/textview.h ../include/tkeys.h                  \
	          ../include/tobjstrm.h ../include/ttypes.h                 \
	          ../include/tv.h ../include/tvhelp.h ../include/tvobjs.h   \
	          ../include/views.h tprogram.cpp
$(F)tradiobu$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tradiobu.cpp
$(F)trescoll$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h trescoll.cpp
$(F)tresfile$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tresfile.cpp
$(F)tscreen$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tscreen.cpp
$(F)tscrlbar$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tscrlbar.cpp
$(F)tscrolle$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tscrolle.cpp
$(F)tsortcol$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tsortcol.cpp
$(F)tstatict$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tstatict.cpp
$(F)tstatusl$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tstatusl.cpp
$(F)tstrcoll$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tstrcoll.cpp
$(F)tstrlist$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tstrlist.cpp
$(F)ttprvln2$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h ttprvln2.cpp
$(F)tvcursr2$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tvcursr2.cpp
$(F)tvexpsd2$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tvexpsd2.cpp
$(F)tview$(O)   : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tview.cpp
$(F)tvtext1$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tvtext1.cpp
$(F)tvtext2$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tvtext2.cpp
$(F)tvwrite2$(O): $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h tvwrite2.cpp
$(F)twindow$(O) : $(I)prodir.h ../include/app.h ../include/buffers.h        \
	          ../include/cm_codes.h ../include/colorsel.h               \
	          ../include/dialogs.h ../include/drawbuf.h                 \
	          ../include/editors.h ../include/menus.h                   \
	          ../include/msgbox.h ../include/objects.h                  \
	          ../include/resource.h ../include/stddlg.h                 \
	          ../include/system.h ../include/textview.h                 \
	          ../include/tkeys.h ../include/tobjstrm.h                  \
	          ../include/ttypes.h ../include/tv.h ../include/tvobjs.h   \
	          ../include/views.h twindow.cpp
$(F)ustrings$(O): $(I)llong.hpp $(I)pro.h $(I)prodir.h ../include/_extrn.h  \
                  ../include/app.h ../include/buffers.h                     \
                  ../include/cm_codes.h ../include/colorsel.h 		    \
		  ../include/dialogs.h ../include/drawbuf.h 		    \
		  ../include/editors.h ../include/menus.h 		    \
                  ../include/msgbox.h ../include/objects.h 		    \
                  ../include/resource.h ../include/stddlg.h 		    \
		  ../include/system.h ../include/tcombo.h 		    \
		  ../include/textview.h ../include/tkeys.h 		    \
		  ../include/tobjstrm.h ../include/ttypes.h ../include/tv.h \
		  ../include/tvobjs.h ../include/ustrings.h 		    \
		  ../include/util.h ../include/views.h ustrings.cpp
# MAKEDEP dependency list ------------------
