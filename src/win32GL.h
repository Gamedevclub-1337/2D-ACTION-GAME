#ifndef WIN32_G_L_H
#define WIN32_G_L_H
#include <windows.h>
#include "GL_functions_define.h"
// NOTE(Yassine): this is probably temporary until we do the linux version.


WGLCHOOSEPIXELFORMATARB *wglChoosePixelFormatARB;
WGLCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB;

HGLRC	win32InitOpenGL(HWND window_handle);

#endif //WIN32_G_L_H
