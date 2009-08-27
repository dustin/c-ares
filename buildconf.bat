@echo off
REM
REM $Id: buildconf.bat,v 1.3 2009-06-08 14:27:36 yangtse Exp $
REM
REM This batch file must be used to set up a CVS tree to build on
REM systems where there is no autotools support (i.e. Microsoft).
REM
REM This file is not included nor needed for c-ares' release
REM archives, neither for c-ares' daily snapshot archives.

if exist CVS-INFO goto start_doing
ECHO ERROR: This file shall only be used with a c-ares CVS tree checkout.
goto end_all
:start_doing

if not exist ares_build.h.dist goto end_ares_build_h
copy /Y ares_build.h.dist ares_build.h
:end_ares_build_h

:end_all

