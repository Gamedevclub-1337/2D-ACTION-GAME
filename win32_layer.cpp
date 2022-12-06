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

float speed = 1.0f;
float jumping_velocity = 0.0008f;
float gravity = 0.0001f;

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

bool is_on_ground = 1;

void ProcessInput()
{
	if (KEY_DOWN(VK_LEFT))
	  speed = -0.5f;
    if (KEY_DOWN(VK_RIGHT))
	  speed = 0.5f;
	if (KEY_UP(VK_LEFT) && KEY_UP(VK_RIGHT))
	  speed = 0;
	if (KEY_DOWN(VK_SPACE) && is_on_ground)
	{
	  is_on_ground = 0;
	  jumping_velocity = 0.0008f;
	}
}

// The function pointers for the opengl functions we'll need here
typedef void   (*GL_GENBUFFERS) (GLsizei, GLuint*);
typedef GLuint (*GL_GETUNIFORMLOCATION) (GLuint, const char*);
typedef void   (*GL_DRAWELEMENT) (GLenum, GLsizei, GLenum, const void*);
typedef void   (*GL_SHADERSOURCE) (GLuint, GLsizei, const char* const*, const int*);
typedef void   (*GL_BUFFERDATA) (GLenum, ptrdiff_t, const void*, GLenum);
typedef void   (*GL_UNIFORM1F) (GLint, GLfloat);
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
    WNDCLASSEX 				dummy_window_class;
    WNDCLASSEX				real_window_class;
	MSG						msg = {};
    POINT					mouse_pos = {};

	dummy_window_class.cbSize = sizeof(WNDCLASSEX);
    dummy_window_class.style = CS_OWNDC;
    dummy_window_class.lpfnWndProc = DefWindowProc;
    dummy_window_class.cbClsExtra = 0;
    dummy_window_class.cbWndExtra = 0;
	dummy_window_class.hInstance = hinstance;
    dummy_window_class.hIcon = LoadIcon(0, IDI_APPLICATION);
    dummy_window_class.hCursor = LoadCursor(0, IDC_ARROW);
    dummy_window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    dummy_window_class.lpszMenuName = 0;
    dummy_window_class.lpszClassName = "CLASS1";
    dummy_window_class.hIconSm = LoadIcon(0, IDI_APPLICATION);

	RegisterClassEx(&dummy_window_class);

    real_window_class.cbSize = sizeof(WNDCLASSEX);
    real_window_class.style = CS_OWNDC;
    real_window_class.lpfnWndProc = WinProc;
    real_window_class.cbClsExtra = 0;
	real_window_class.cbWndExtra = 0;
	real_window_class.hInstance = hinstance;
	real_window_class.hIcon = LoadIcon(0, IDI_APPLICATION);
	real_window_class.hCursor = LoadCursor(0, IDC_ARROW);
	real_window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	real_window_class.lpszMenuName = 0;
	real_window_class.lpszClassName = "CLASS2";
	real_window_class.hIconSm = LoadIcon(0, IDI_APPLICATION);

	RegisterClassEx(&real_window_class);

	HWND hwindow = CreateWindowEx(0, "CLASS1", "Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, WIDTH, HEIGHT, 0, 0, hinstance, 0);
	HDC hdc = GetDC(hwindow);

	PIXELFORMATDESCRIPTOR pdf;
	pdf.nSize =	sizeof(PIXELFORMATDESCRIPTOR);
	pdf.nVersion = 1;
	pdf.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pdf.iPixelType = PFD_TYPE_RGBA;
	pdf.cColorBits = 32;
	pdf.cDepthBits = 24;
	pdf.cStencilBits = 8;
	pdf.cAlphaBits = 8;
	pdf.iLayerType = PFD_MAIN_PLANE;
	int l = ChoosePixelFormat(hdc, &pdf);
	SetPixelFormat(hdc, l, &pdf);
	HGLRC openglcontext = wglCreateContext(hdc);
	wglMakeCurrent(hdc, openglcontext);

	WGLCHOOSEPIXELFORMATARB *wglChoosePixelFormatARB = (WGLCHOOSEPIXELFORMATARB*)wglGetProcAddress("wglChoosePixelFormatARB");	
	WGLCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB = (WGLCREATECONTEXTATTRIBSARB)wglGetProcAddress("wglCreateContextAttribsARB");
	
	GL_CREATESHADER     glCreateShader = (GL_CREATESHADER)wglGetProcAddress("glCreateShader");
	GL_BINDBUFFER		glBindBuffer = (GL_BINDBUFFER)wglGetProcAddress("glBindBuffer");
	GL_USEPROGRAM		glUseProgram = (GL_USEPROGRAM)wglGetProcAddress("glUseProgram");
	GL_BUFFERDATA		glBufferData = (GL_BUFFERDATA)wglGetProcAddress("glBufferData");	
	GL_GENBUFFERS       glGenBuffers = (GL_GENBUFFERS)wglGetProcAddress("glGenBuffers");    
	GL_SHADERSOURCE		glShaderSource = (GL_SHADERSOURCE)wglGetProcAddress("glShaderSource");
	GL_COMPILESHADER	glCompileShader = (GL_COMPILESHADER)wglGetProcAddress("glCompileShader");
	GL_CREATEPROGRAM	glCreateProgram = (GL_CREATEPROGRAM)wglGetProcAddress("glCreateProgram");
	GL_ATTACHSHADER		glAttachShader = (GL_ATTACHSHADER)wglGetProcAddress("glAttachShader");
	GL_LINKPROGRAM		glLinkProgram = (GL_LINKPROGRAM)wglGetProcAddress("glLinkProgram");
	GL_DELETESHADER		glDeleteShader = (GL_DELETESHADER)wglGetProcAddress("glDeleteShader");
	GL_VERTEXATTRIBPOINTER glVertexAttribPointer = (GL_VERTEXATTRIBPOINTER)wglGetProcAddress("glVertexAttribPointer");
	GL_ENABLEVERTEXATTRIBARRAY glEnableVertexAttribArray = (GL_ENABLEVERTEXATTRIBARRAY)wglGetProcAddress("glEnableVertexAttribArray");
	GL_GENVERTEXARRAYS	glGenVertexArrays = (GL_GENVERTEXARRAYS)wglGetProcAddress("glGenVertexArrays");
	GL_BINDVERTEXARRAY	glBindVertexArray = (GL_BINDVERTEXARRAY)wglGetProcAddress("glBindVertexArray");
	GL_GETUNIFORMLOCATION glGetUniformLocation = (GL_GETUNIFORMLOCATION)wglGetProcAddress("glGetUniformLocation");
	GL_UNIFORM1F		glUniform1f = (GL_UNIFORM1F)wglGetProcAddress("glUniform1f");

	wglMakeCurrent(hdc, 0);
	wglDeleteContext(openglcontext);
	ReleaseDC(hwindow, hdc);
	DestroyWindow(hwindow);

	HWND hwindo = CreateWindowEx(0, "CLASS2", "Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, WIDTH, HEIGHT, 0, 0, hinstance, 0);
	hdc = GetDC(hwindo);
	openglcontext = 0;
	l = ChoosePixelFormat(hdc, &pdf);

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
	static const int attrib_list[] = {
	  WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
	  WGL_CONTEXT_MINOR_VERSION_ARB, 3,
	  WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	  WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
	  0};
    int pixel_format;
	UINT num_format;

	// Initialize opengl context
	wglChoosePixelFormatARB(hdc, attriblist, 0, 1, &pixel_format, &num_format);
	ZeroMemory(&pdf, sizeof(pdf));
	DescribePixelFormat(hdc, pixel_format, sizeof(pdf), &pdf);
	SetPixelFormat(hdc, pixel_format, &pdf);
	openglcontext = wglCreateContextAttribsARB(hdc, 0, attrib_list);
	wglMakeCurrent(hdc, openglcontext);
	
    const char *player_vertex_shader = "#version 330 core\n"
	  "layout (location = 0) in vec3 aPos;\n"
	  "void main()\n"
	  "{\n"
	  " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	  "}\0";
	const char *player_fragment_shader = "#version 330 core\n"
	  "out vec4 FragColor;\n"
	  "void main()\n"
	  "{\n"
	  "FragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);\n"
	  "}\0";

	const char *gun_vertex_shader = "#version 330 core\n"
	  "layout (location = 0) in vec3 aPos;\n"
	  "uniform float angle;\n"
	  "uniform float rotation_offset_x;\n" 
	  "uniform float rotation_offset_y;\n"
	  "void main()\n"
	  "{\n"
	  " float rotation_position_x = cos(angle) * (aPos.x - rotation_offset_x) - sin(angle) * (aPos.y - rotation_offset_y);\n"
	  " float rotation_position_y = sin(angle) * (aPos.x - rotation_offset_x) + cos(angle) * (aPos.y - rotation_offset_y);\n"
	  " rotation_position_x += rotation_offset_x;\n"
	  " rotation_position_y += rotation_offset_y;\n"
	  " gl_Position = vec4(rotation_position_x, rotation_position_y, aPos.z, 1.0);\n"
	  "}\0";	
	const char *gun_fragment_shader = "#version 330 core\n"
	  "out vec4 FragColor;\n"
	  "void main()\n"
	  "{\n"
	  "FragColor = vec4(0.4f, 0.4f, 0.4f, 0.0f);\n"
	  "}\0";
	const char *ground_fragment_shader = "#version 330 core\n"
	  "out vec4 FragColor;\n"
	  "void main()\n"
	  "{\n"
	  "FragColor = vec4(0.2f, 0.2f, 0.2f, 0.0f);\n"
	  "}\0";

	const char *ground_vertex_shader = "#version 330 core\n"
	  "layout (location = 0) in vec3 aPos;\n"
	  "uniform float angle;\n"
	  "uniform float rotation_offset_x;\n" 
	  "uniform float rotation_offset_y;\n"
	  "void main()\n"
	  "{\n"
	  " float rotation_position_x = cos(angle) * (aPos.x - rotation_offset_x) - sin(angle) * (aPos.y - rotation_offset_y);\n"
	  " float rotation_position_y = sin(angle) * (aPos.x - rotation_offset_x) + cos(angle) * (aPos.y - rotation_offset_y);\n"
	  " rotation_position_x += rotation_offset_x;\n"
	  " rotation_position_y += rotation_offset_y;\n"
	  " gl_Position = vec4(rotation_position_x, rotation_position_y, aPos.z, 1.0);\n"
	  "}\0";
	
	GLuint player_vertex_ID = glCreateShader(0x8B31);
	glShaderSource(player_vertex_ID, 1, &player_vertex_shader, 0);
	glCompileShader(player_vertex_ID);
	GLuint player_fragement_ID = glCreateShader(0x8B30);
	glShaderSource(player_fragement_ID, 1, &player_fragment_shader, 0);
	glCompileShader(player_fragement_ID);
	GLuint player_shader_program = glCreateProgram();
	glAttachShader(player_shader_program, player_vertex_ID);
	glAttachShader(player_shader_program, player_fragement_ID);
	glLinkProgram(player_shader_program);
	glDeleteShader(player_vertex_ID);
	glDeleteShader(player_fragement_ID);

	GLuint gun_vertex_ID = glCreateShader(0x8B31);
	glShaderSource(gun_vertex_ID, 1, &gun_vertex_shader, 0);
	glCompileShader(gun_vertex_ID);
	GLuint gun_fragement_ID = glCreateShader(0x8B30);
	glShaderSource(gun_fragement_ID, 1, &gun_fragment_shader, 0);
	glCompileShader(gun_fragement_ID);
	GLuint gun_shader_program = glCreateProgram();
	glAttachShader(gun_shader_program, gun_vertex_ID);
	glAttachShader(gun_shader_program, gun_fragement_ID);
	glLinkProgram(gun_shader_program);
	glDeleteShader(gun_vertex_ID);
	glDeleteShader(gun_fragement_ID);

	GLuint ground_vertex_ID = glCreateShader(0x8B31);
	glShaderSource(ground_vertex_ID, 1, &ground_vertex_shader, 0);
	glCompileShader(ground_vertex_ID);
	GLuint ground_fragement_ID = glCreateShader(0x8B30);
	glShaderSource(ground_fragement_ID, 1, &ground_fragment_shader, 0);
	glCompileShader(ground_fragement_ID);
	GLuint ground_shader_program = glCreateProgram();
	glAttachShader(ground_shader_program, ground_vertex_ID);
	glAttachShader(ground_shader_program, ground_fragement_ID);
	glLinkProgram(ground_shader_program);
	glDeleteShader(ground_vertex_ID);
	glDeleteShader(ground_fragement_ID);

	float	player_vertices[] =
	{
	  -0.05f, 0.1f, 0.0f,
 	   0.05f, 0.1f, 0.0f,
	   0.05f,-0.1f, 0.0f,
	  -0.05f,-0.1f, 0.0f
	};
	int    player_indecies[] =
	{
	  0, 1, 3,
	  1, 3, 2
	};
	float	gun_vertices[] =
	{
	   0.0f, 0.02f, 0.0f,
	   0.2f, 0.02f, 0.0f,
	   0.2f,-0.05f, 0.0f,
	   0.0f,-0.05f, 0.0f
	};
	int    gun_indecies[] =
	{
	  0, 1, 3,
	  1, 3, 2
	};
	float	ground_vertices[] =
	{
	  -1.0f, -0.1f, 0.0f,
	   1.0f, -0.1f, 0.0f,
	   1.0f,-0.2f, 0.0f,
	  -1.0f,-0.2f, 0.0f
	};
	int    ground_indecies[] =
	{
	  0, 1, 3,
	  1, 3, 2
	};
	
	GLuint player_vertex_buffer = 0;
	GLuint player_vertex_array = 0;
	GLuint player_element_buffer_object = 0;

	GLuint gun_vertex_buffer = 0;
	GLuint gun_vertex_array = 0;
	GLuint gun_element_buffer_object = 0;
	
	GLuint ground_vertex_buffer = 0;
	GLuint ground_vertex_array = 0;
	GLuint ground_element_buffer_object = 0;
	
	glGenVertexArrays(1, &player_vertex_array);
	glGenBuffers(1, &player_vertex_buffer);
	glGenBuffers(1, &player_element_buffer_object);

	glGenVertexArrays(1, &gun_vertex_array);
	glGenBuffers(1, &gun_vertex_buffer);
	glGenBuffers(1, &gun_element_buffer_object);

	glGenVertexArrays(1, &ground_vertex_array);
	glGenBuffers(1, &ground_vertex_buffer);
	glGenBuffers(1, &ground_element_buffer_object);
	
	glBindBuffer(0x8892, player_vertex_buffer);
	glBindVertexArray(player_vertex_array);
	glBufferData(0x8892, sizeof(player_vertices), player_vertices, 0x88E4);
	glBindBuffer(0x8893, player_element_buffer_object);
	glBufferData(0x8893, sizeof(player_indecies), player_indecies, 0x88E4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(0x8892, gun_vertex_buffer);
	glBindVertexArray(gun_vertex_array);
	glBufferData(0x8892, sizeof(gun_vertices), gun_vertices, 0x88E4);
	glBindBuffer(0x8893, gun_element_buffer_object);
	glBufferData(0x8893, sizeof(gun_indecies), gun_indecies, 0x88E4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(0x8892, ground_vertex_buffer);
	glBindVertexArray(ground_vertex_array);
	glBufferData(0x8892, sizeof(ground_vertices), ground_vertices, 0x88E4);
	glBindBuffer(0x8893, ground_element_buffer_object);
	glBufferData(0x8893, sizeof(ground_indecies), ground_indecies, 0x88E4);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    while (1)
	{
	  GetCursorPos(&mouse_pos);
	  float angle = atan2(mouse_pos.y - HEIGHT/2, mouse_pos.x - WIDTH/2);
	  player_vertices[0] += speed * 0.001f;
	  player_vertices[3] += speed * 0.001f;
	  player_vertices[6] += speed * 0.001f;
	  player_vertices[9] += speed * 0.001f;

	  gun_vertices[0] += speed * 0.001f;
	  gun_vertices[3] += speed * 0.001f;
	  gun_vertices[6] += speed * 0.001f;
	  gun_vertices[9] += speed * 0.001f;

	  glBindBuffer(0x8892, player_vertex_buffer);
	  glBindVertexArray(player_vertex_array);
	  glBufferData(0x8892, sizeof(player_vertices), player_vertices, 0x88E4);

	  if (!is_on_ground)
	  {
	    player_vertices[1] += jumping_velocity;
		player_vertices[4] += jumping_velocity;
		player_vertices[7] += jumping_velocity;
		player_vertices[10] += jumping_velocity;
		gun_vertices[1] += jumping_velocity;
		gun_vertices[4] += jumping_velocity;
		gun_vertices[7] += jumping_velocity;
		gun_vertices[10] += jumping_velocity;
		jumping_velocity -= gravity * 0.01f;
	  }
	  if (player_vertices[4] <= 0.1f)
		is_on_ground = 1;
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
	  glUseProgram(player_shader_program);	  
	  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	  glUseProgram(gun_shader_program);	  

	  glBindBuffer(0x8892, gun_vertex_buffer);
	  glBindVertexArray(gun_vertex_array);
	  glUniform1f(glGetUniformLocation(gun_shader_program, "angle"), -angle);	  
	  glUniform1f(glGetUniformLocation(gun_shader_program, "rotation_offset_x"), player_vertices[0] + 0.02f);	  
	  glUniform1f(glGetUniformLocation(gun_shader_program, "rotation_offset_y"), player_vertices[1] + 0.02f);
	  glBufferData(0x8892, sizeof(gun_vertices), gun_vertices, 0x88E4);  
	  glBindBuffer(0x8893, gun_element_buffer_object);      
	  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	  glUseProgram(ground_shader_program);
	  glBufferData(0x8892, sizeof(ground_vertices), ground_vertices, 0x88E4);  
	  glBindBuffer(0x8893, ground_element_buffer_object);      
	  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
