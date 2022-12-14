@echo off
IF NOT EXIST build mkdir build

set LIBS=kernel32.lib user32.lib opengl32.lib gdi32.lib
set DEFINES=-DDEBUG
set DEBUG_FLAGS=/Zi
set FLAGS=/EHsc
set OUTPUT=/Fe:game

pushd build
echo building...

cl ..\src\win32_layer.cpp %FLAGS% %DEBUG_FLAGS% %DEFINES% %LIBS% %OUTPUT%

popd
