# Make file for Scintilla on Windows
# Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
# The License.txt file describes the conditions under which this software may be distributed.
# This makefile assumes the mingw32 version of GCC 2.95.3 is used and changes will
# be needed to use other compilers.
# The -fvtable-thunks option is required for older (2.9*) versions of gcc but can be removed
# safely if using a newer version which may display warning messages.

.SUFFIXES: .cxx
CC = g++
DLLWRAP = dllwrap
DEL = del /q

COMPONENT = ../bin/Scintilla.dll
LEXCOMPONENT = ../bin/SciLexer.dll
LEXLIB = Lexers.a

ifndef NOTHUNKS
gversion = $(word 1,$(subst ., ,$(shell g++ --version)))
ifeq ($(gversion),2)
THUNKFLAGS=-fvtable-thunks
endif
endif

vpath %.h ../src ../include
vpath %.cxx ../src

LDFLAGS=-mwindows -lstdc++ -limm32 -lole32 -luuid -mno-cygwin
# Add -MMD to get dependencies
#CXXFLAGS = -g -pg -pedantic -Os -fno-exceptions -fvtable-thunks -fno-rtti
INCLUDEDIRS=-I ../include -I ../src
CXXBASEFLAGS=-Wall -Wno-missing-braces -Wno-char-subscripts -pedantic $(INCLUDEDIRS) $(THUNKFLAGS) -fno-rtti -mno-cygwin

ifdef DEBUG
CXXFLAGS=-DDEBUG -g $(CXXBASEFLAGS)
else
CXXFLAGS=-DNDEBUG -Os $(CXXBASEFLAGS)
STRIPFLAG=-s
endif

.cxx.o:
	$(CC) $(CXXFLAGS) -c $<

ALL:	$(COMPONENT) $(LEXCOMPONENT) $(LEXLIB) ScintillaWinS.o WindowAccessor.o

clean:
	$(DEL) *.exe *.o *.obj *.dll *.res *.map

deps:
	$(CC) -MM $(CXXFLAGS) *.cxx ../src/*.cxx >deps.mak

#++Autogenerated -- run src/LexGen.py to regenerate
#**LEXOBJS=\\\n\(\*.o \)
LEXOBJS=\
LexAbaqus.o LexAda.o LexAPDL.o LexAsm.o LexAsn1.o LexASY.o LexAU3.o LexAVE.o \
LexBaan.o LexBash.o LexBasic.o LexBullant.o LexCaml.o LexCLW.o LexCmake.o \
LexCOBOL.o LexConf.o LexCPP.o LexCrontab.o LexCsound.o LexCSS.o LexD.o \
LexEiffel.o LexErlang.o LexEScript.o LexFlagship.o LexForth.o LexFortran.o \
LexGAP.o LexGui4Cli.o LexHaskell.o LexHTML.o LexInno.o LexKix.o LexLisp.o \
LexLout.o LexLua.o LexMagik.o LexMarkdown.o LexMatlab.o LexMetapost.o \
LexMMIXAL.o LexMPT.o LexMSSQL.o LexMySQL.o LexNimrod.o LexNsis.o LexOpal.o \
LexOthers.o LexPascal.o LexPB.o LexPerl.o LexPLM.o LexPOV.o LexPowerPro.o \
LexPowerShell.o LexProgress.o LexPS.o LexPython.o LexR.o LexRebol.o LexRuby.o \
LexScriptol.o LexSmalltalk.o LexSML.o LexSorcus.o LexSpecman.o LexSpice.o \
LexSQL.o LexTACL.o LexTADS3.o LexTAL.o LexTCL.o LexTeX.o LexVB.o LexVerilog.o \
LexVHDL.o LexYAML.o
#--Autogenerated -- end of automatically generated section

SOBJS	= ScintillaWin.o ScintillaBase.o Editor.o CharClassify.o Decoration.o \
	Document.o ContractionState.o CellBuffer.o CallTip.o \
	ScintRes.o PlatWin.o PositionCache.o KeyMap.o Indicator.o LineMarker.o RESearch.o RunStyles.o \
	Selection.o Style.o ViewStyle.o AutoComplete.o UniConversion.o PropSet.o XPM.o PerLine.o
$(COMPONENT): $(SOBJS) Scintilla.def
	$(DLLWRAP) --add-stdcall-alias --target i386-mingw32 -o $@ $(SOBJS) $(LDFLAGS) $(STRIPFLAG) --relocatable

LOBJS	= ScintillaWinL.o ScintillaBaseL.o Editor.o CharClassify.o Decoration.o \
	Document.o ContractionState.o CellBuffer.o CallTip.o \
	ScintRes.o PlatWin.o PositionCache.o KeyMap.o Indicator.o LineMarker.o RESearch.o RunStyles.o \
	Selection.o Style.o ViewStyle.o AutoComplete.o UniConversion.o KeyWords.o \
	DocumentAccessor.o PropSet.o ExternalLexer.o StyleContext.o XPM.o PerLine.o $(LEXOBJS)
$(LEXCOMPONENT): $(LOBJS) Scintilla.def
	$(DLLWRAP) --add-stdcall-alias --target i386-mingw32 -o $@ $(LOBJS) $(LDFLAGS) $(STRIPFLAG) --relocatable

$(LEXLIB): $(LEXOBJS)
	$(AR) rc $@ $^
	ranlib $@

# Automatically generate dependencies for most files with "make deps"
include deps.mak

# These dependencies are maintained by hand as they do not use the default output name
ScintillaBaseL.o: ScintillaBase.cxx Platform.h Scintilla.h SciLexer.h \
 ContractionState.h CellBuffer.h CallTip.h KeyMap.h Indicator.h \
 LineMarker.h Style.h AutoComplete.h ViewStyle.h Document.h Editor.h \
 ScintillaBase.h PropSet.h Accessor.h DocumentAccessor.h \
 KeyWords.h ExternalLexer.h PerLine.h
ScintillaWinL.o: ScintillaWin.cxx Platform.h Scintilla.h SciLexer.h \
 ContractionState.h CellBuffer.h CallTip.h KeyMap.h Indicator.h \
 LineMarker.h Style.h AutoComplete.h ViewStyle.h Document.h Editor.h \
 ScintillaBase.h PropSet.h Accessor.h KeyWords.h \
 ExternalLexer.h UniConversion.h PerLine.h
ScintillaWinS.o: ScintillaWin.cxx Platform.h Scintilla.h \
 ContractionState.h CellBuffer.h CallTip.h KeyMap.h Indicator.h \
 LineMarker.h Style.h AutoComplete.h ViewStyle.h Document.h Editor.h \
 ScintillaBase.h UniConversion.h PerLine.h

ScintillaBaseL.o:
	$(CC) $(CXXFLAGS) -D SCI_LEXER -c $< -o $@

ScintillaWinS.o:
	$(CC) $(CXXFLAGS) -D STATIC_BUILD -c $< -o $@

ScintillaWinL.o:
	$(CC) $(CXXFLAGS) -D SCI_LEXER -c $< -o $@

ScintRes.o:	ScintRes.rc PlatformRes.h
	windres ScintRes.rc $@
