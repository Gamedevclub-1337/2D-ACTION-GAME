#include "win32GL.h"

HGLRC	win32InitOpenGL(HWND window_handle)
{
	HGLRC	result = NULL;
	WNDCLASSEX	dummy_window_class = {};
	
	dummy_window_class.cbSize = sizeof(WNDCLASSEX);
    dummy_window_class.style = CS_HREDRAW | CS_VREDRAW;
    dummy_window_class.lpfnWndProc = DefWindowProc;
	dummy_window_class.hInstance = GetModuleHandle(0);
    dummy_window_class.lpszClassName = "CLASS2";
	
	if (!RegisterClassEx(&dummy_window_class))
	{
		MessageBox(0, "RegisterClassEx(): Dummy Window Failed\n", "ERROR", MB_OK);
		return (NULL);
	}
	
	HWND	dummy_window_handle = CreateWindowEx(0, "CLASS2", "DUMMYWINDOW", 0,
											  CW_USEDEFAULT,
											  CW_USEDEFAULT,
											  CW_USEDEFAULT,
											  CW_USEDEFAULT,
											  0, 0, dummy_window_class.hInstance, 0);
	if (!dummy_window_handle)
	{
		MessageBox(0, "CreateWindowEx(): Dummy Window Failed\n", "ERROR", MB_OK);
		return (NULL);
	}
	
	PIXELFORMATDESCRIPTOR pdf = {};
	
	pdf.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pdf.nVersion = 1;
	pdf.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pdf.iPixelType = PFD_TYPE_RGBA;
	pdf.cColorBits = 32;
	pdf.cDepthBits = 24;
	pdf.cStencilBits = 8;
	pdf.cAlphaBits = 8;
	pdf.iLayerType = PFD_MAIN_PLANE;
	
	HDC	dummy_window_context = GetDC(dummy_window_handle);
	
	int pf = ChoosePixelFormat(dummy_window_context, &pdf);
	if (pf == 0)
	{
		MessageBox(0, "ChoosePixelFormat() Failed\n", "ERROR", MB_OK);
		return (NULL);
	}
	
	SetPixelFormat(dummy_window_context, pf, &pdf);
	
	HGLRC dummy_GL_context = wglCreateContext(dummy_window_context);
	if (dummy_GL_context == NULL)
	{
		MessageBox(0, "wglCreateContext(): Dummy GLContext Failed\n", "ERROR", MB_OK);
		return (NULL);
	}
	
	wglMakeCurrent(dummy_window_context, dummy_GL_context);
	
	wglChoosePixelFormatARB = (WGLCHOOSEPIXELFORMATARB*)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (WGLCREATECONTEXTATTRIBSARB)wglGetProcAddress("wglCreateContextAttribsARB");
	
	gl_GetFunctions();
	
	wglMakeCurrent(dummy_window_context, 0);
	wglDeleteContext(dummy_GL_context);
	ReleaseDC(dummy_window_handle, dummy_window_context);
	DestroyWindow(dummy_window_handle);
	
	
	HDC window_context = GetDC(window_handle);
	
	pf= ChoosePixelFormat(window_context, &pdf);
	if (pf == 0)
	{
		MessageBox(0, "ChoosePixelFormat(): Real Window Context Failed\n", "ERROR", MB_OK);
		return (NULL);
	}
	
    const   int attriblist[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};
	static const int attrib_list[] =
    {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};
    
	int pixel_format;
	UINT num_format;
	
	// Initialize opengl context
	wglChoosePixelFormatARB(window_context, attriblist, 0, 1, &pixel_format, &num_format);
	if (num_format == 0)
	{
		MessageBox(0, "wglChoosePixelFormatARB() Failed\n", "ERROR", MB_OK);
		return (NULL);
	}
	
	ZeroMemory(&pdf, sizeof(pdf));
	
	DescribePixelFormat(window_context, pixel_format, sizeof(pdf), &pdf);
	if (!SetPixelFormat(window_context, pixel_format, &pdf))
	{
		MessageBox(0, "SetPixelFormat() Failed\n", "ERROR", MB_OK);
		return (NULL);
	}
	result = wglCreateContextAttribsARB(window_context, 0, attrib_list);
	
	return (result);
}