@echo off
IF NOT EXIST build mkdir build
pushd build
echo building...
cl /Zi /EHsc ..\src\win32_layer.cpp /link kernel32.lib gdi32.lib user32.lib ddraw.lib dxguid.lib ole32.lib opengl32.lib /out:game.exe
popd
