@echo off

set LIBUSB_INCLUDE_DIR=C:\Users\bwalle\devel\libusb-pbatard\libusb
set LIBUSB_LIBDIR=C:\Users\bwalle\devel\libusb-pbatard\Win32\Release\dll
set GENERATOR="NMake Makefiles"
rem set GENERATOR="Visual Studio 9 2008"

cd ..\build
cmake -G %GENERATOR% -DLIBUSB_ADDITIONAL_INCLUDEDIR=%LIBUSB_INCLUDE_DIR% -DLIBUSB_ADDITIONAL_LIBDIR=%LIBUSB_LIBDIR% ..
cd ..\win32
