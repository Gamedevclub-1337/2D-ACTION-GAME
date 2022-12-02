# define WIN32_LEAN_AND_MEAN
# define RGBIT(a, r, g, b) (a << 24 | r << 16 | g << 8 | b)
# define DDRAW_INIT(dsurface) {memset(&dsurface, 0, sizeof(dsurface));}
# define KEY_DOWN(key) ((GetAsyncKeyState(key) & 0x8000) ? 1 : 0)
# define KEY_UP(key) ((GetAsyncKeyState(key) & 0x8000) ? 0 : 1)
# define WIDTH 700
# define HEIGHT 700
# define RAD2DEG(ang) {return (ang * 180.0/M_PI);}

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <io.h>
#include <ddraw.h>
#include <iostream>
#include <string>
#include <d2d1.h>
#include <cmath>
#include <GL/GL.h>
#include <wingdi.h>
#include "wglext.h"
//#include "glcorearb.h"

int x = 0;
int gun_dir = 1;
int speed = 20;
float rot_angle = 0;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); // Window Procedure Handler

struct bullet
{
  float x;
  float y;
  float velocity;
  float shoot_angle;
};

struct player
{
  float x;
  float y;
};

void ProcessInput()
{
	if (KEY_DOWN(VK_LEFT))
    {
	  x-=speed;
	  gun_dir = 300;
	}
    if (KEY_DOWN(VK_RIGHT))
	{
	  x+=speed;
	  gun_dir = 0;
	}
}

// The function pointers for the opengl functions we'll need here
typedef void   (*GL_GENBUFFERS) (GLsizei, GLuint*);
typedef void   (*GL_DRAWELEMENT) (GLenum, GLsizei, GLenum, const void*);
typedef void   (*GL_DRAWARRAYS) (GLenum, GLint, GLsizei);
typedef void   (*GL_SHADERSOURCE) (GLuint, GLsizei, const char* const*, const int*);
typedef void   (*GL_BUFFERDATA) (GLenum, ptrdiff_t, const void*, GLenum);
typedef void   (*GL_BINDVERTEXARRAY) (GLuint);
typedef void   (*GL_BINDBUFFER) (GLenum, GLuint);
typedef void   (*GL_LINKPROGRAM) (GLuint);
typedef void   (*GL_BINDVERTEXARRAY) (GLuint);
typedef void   (*GL_GENVERTEXARRAYS) (GLsizei, GLuint*);
typedef void   (*GL_VERTEXATTRIBPOINTER) (GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
typedef void   (*GL_ENABLEVERTEXATTRIBARRAY) (GLuint);
typedef void   (*GL_COMPILESHADER) (GLuint);
typedef void   (*GL_DELETESHADER) (GLuint);
typedef void   (*GL_USEPROGRAM) (GLuint);
typedef GLuint (*GL_CREATESHADER) (GLenum);
typedef GLuint (*GL_CREATEPROGRAM) ();
typedef void   (*GL_ATTACHSHADER) (GLuint, GLuint);
typedef BOOL WINAPI WGLCHOOSEPIXELFORMATARB (HDC, const int *, const FLOAT *, UINT, int * , UINT *);
typedef HGLRC (*WGLCREATECONTEXTATTRIBSARB) (HDC, HGLRC, const int*);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prev_instance, LPSTR cmd, int cmd_show)
{
	WNDCLASSEX 				winclass;
	WNDCLASSEX				realclass;
	MSG						msg = {};
    POINT					mouse_pos;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_OWNDC;
	winclass.lpfnWndProc = DefWindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(0, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(0, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = 0;
	winclass.lpszClassName = "CLASS1";
	winclass.hIconSm = LoadIcon(0, IDI_APPLICATION);

	RegisterClassEx(&winclass);

    realclass.cbSize = sizeof(WNDCLASSEX);
    realclass.style = CS_OWNDC;
    realclass.lpfnWndProc = WinProc;
    realclass.cbClsExtra = 0;
	realclass.cbWndExtra = 0;
	realclass.hInstance = hinstance;
	realclass.hIcon = LoadIcon(0, IDI_APPLICATION);
	realclass.hCursor = LoadCursor(0, IDC_ARROW);
	realclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	realclass.lpszMenuName = 0;
	realclass.lpszClassName = "CLASS2";
	realclass.hIconSm = LoadIcon(0, IDI_APPLICATION);

	RegisterClassEx(&realclass);

	HWND hwindow = CreateWindowEx(0, "CLASS1", "Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, WIDTH, HEIGHT, 0, 0, hinstance, 0);
	HDC hdc = GetDC(hwindow);

	PIXELFORMATDESCRIPTOR pdf =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	  };
	int l = ChoosePixelFormat(hdc, &pdf);
	SetPixelFormat(hdc, l, &pdf);
	HGLRC openglcontext = wglCreateContext(hdc);
	wglMakeCurrent(hdc, openglcontext);

	GL_CREATESHADER glCreateShader = (GL_CREATESHADER)wglGetProcAddress("glCreateShader");
	GL_GENBUFFERS glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");
    WGLCHOOSEPIXELFORMATARB *wglChoosePixelFormatARB = (WGLCHOOSEPIXELFORMATARB*)wglGetProcAddress("wglChoosePixelFormatARB");	
	WGLCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB = (WGLCREATECONTEXTATTRIBSARB)wglGetProcAddress("wglCreateContextAttribsARB");
	GL_SHADERSOURCE		glShaderSource = (GL_SHADERSOURCE)wglGetProcAddress("glShaderSource");
	GL_COMPILESHADER	glCompileShader = (GL_COMPILESHADER)wglGetProcAddress("glCompileShader");
	GL_CREATEPROGRAM	glCreateProgram = (GL_CREATEPROGRAM)wglGetProcAddress("glCreateProgram");
	GL_ATTACHSHADER		glAttachShader = (GL_ATTACHSHADER)wglGetProcAddress("glAttachShader");
	GL_LINKPROGRAM		glLinkProgram = (GL_LINKPROGRAM)wglGetProcAddress("glLinkProgram");
	GL_DELETESHADER		glDeleteShader = (GL_DELETESHADER)wglGetProcAddress("glDeleteShader");
	GL_VERTEXATTRIBPOINTER glVertexAttribPointer = (GL_VERTEXATTRIBPOINTER)wglGetProcAddress("glVertexAttribPointer");
	GL_ENABLEVERTEXATTRIBARRAY glEnableVertexAttribArray = (GL_ENABLEVERTEXATTRIBARRAY)wglGetProcAddress("glEnableVertexAttribArray");
	GL_BINDBUFFER		glBindBuffer = (GL_BINDBUFFER)wglGetProcAddress("glBindBuffer");
	GL_USEPROGRAM		glUseProgram = (GL_USEPROGRAM)wglGetProcAddress("glUseProgram");
	GL_BUFFERDATA		glBufferData = (GL_BUFFERDATA)wglGetProcAddress("glBufferData");
	GL_GENVERTEXARRAYS	glGenVertexArrays = (GL_GENVERTEXARRAYS)wglGetProcAddress("glGenVertexArrays");
	GL_BINDVERTEXARRAY	glBindVertexArray = (GL_BINDVERTEXARRAY)wglGetProcAddress("glBindVertexArray");
	GL_DRAWARRAYS		glDrawArrays = (GL_DRAWARRAYS)wglGetProcAddress("glDrawArrays");

	DestroyWindow(hwindow);

	HWND hwindo = CreateWindowEx(0, "CLASS2", "Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, WIDTH, HEIGHT, 0, 0, hinstance, 0);
	hdc = GetDC(hwindo);
	openglcontext = 0;

	hdc = GetDC(hwindo);
	l = ChoosePixelFormat(hdc, &pdf);

    const   int attriblist[] =
	{
	  WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
	  WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
	  WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
      WGL_COLOR_BITS_ARB, 32,
	  WGL_DEPTH_BITS_ARB, 24,
	  WGL_STENCIL_BITS_ARB, 8,
	  0
	};
	static const int attrib_list[] = {
	  WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	  WGL_CONTEXT_MINOR_VERSION_ARB, 2,
	  WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	  0, 0 };
    int pixel_format;
	UINT num_format;

	// Initialize opengl context
	wglChoosePixelFormatARB(hdc, attriblist, 0, 1, &pixel_format, &num_format);	
	SetPixelFormat(hdc, pixel_format, &pdf);
	openglcontext = wglCreateContextAttribsARB(hdc, 0, attrib_list);
	wglMakeCurrent(hdc, openglcontext);

	const char *vertex_src = "#version 330 core\n"
	  "layout (location = 0) in vec3 aPos;\n"
	  "void main()\n"
	  "{\n"
	  " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	  "}\0";
	const char *frag_src = "#version 330 core\n"
	  "out vec4 FragColor;\n"
	  "void main()\n"
	  "{\n"
	  "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	  "}\0";

	GLuint v_ID = glCreateShader(0x8B31);
	glShaderSource(v_ID, 1, &vertex_src, 0);
	glCompileShader(v_ID);
	GLuint f_ID = glCreateShader(0x8B30);
	glShaderSource(f_ID, 1, &frag_src, 0);
	glCompileShader(f_ID);
	GLuint gun_p = glCreateProgram();
	glAttachShader(gun_p, v_ID);
	glAttachShader(gun_p, f_ID);
	glLinkProgram(gun_p);
	glDeleteShader(v_ID);
	glDeleteShader(f_ID);

	const float	player_vert[6] =
	{
	  0.0f, 0.5f,
	  0.5f, 0.5f,
	  0.5f, -0.5f
	};

	GLuint vertex_buffer = 0;

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(0x8892, vertex_buffer);
	glBufferData(0x8892, sizeof(player_vert), player_vert, 0x88E4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GLuint vertex_arr = 0;

	glGenVertexArrays(1, &vertex_arr);
	glBindVertexArray(vertex_arr);
	glBindBuffer(0x8892, vertex_buffer);
	glBufferData(0x8892, sizeof(player_vert), player_vert, 0x88E4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    while (1)
	{ 
      if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	  { 
		  if (msg.message == WM_QUIT)
			break;
		  TranslateMessage(&msg);
		  DispatchMessage(&msg);
	  }
	  ProcessInput();
	  glClearColor(0.09f, 0.09f, 0.09f, 1.0f);
	  glClear(GL_COLOR_BUFFER_BIT);	    
	  glUseProgram(gun_p);
	  glDrawArrays(GL_TRIANGLES, 0, 3);
	  SwapBuffers(hdc);
	}
	return 0;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	PAINTSTRUCT          ps;
	HDC                  hdc = 0;
	RECT                 rect;
	HDC 				 windowdc = 0;	
	HGLRC 				 openglcontext = 0;

	switch (msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			GetClientRect(hwnd, &rect);
			EndPaint(hwnd, &ps);
			return 0;
		}
		case WM_QUIT:
		case WM_DESTROY:
		{
		  	wglMakeCurrent(windowdc, 0);
			wglDeleteContext(openglcontext);
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
