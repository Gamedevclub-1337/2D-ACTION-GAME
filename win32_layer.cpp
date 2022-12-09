# define WIN32_LEAN_AND_MEAN
# define KEY_DOWN(key) ((GetAsyncKeyState(key) & 0x8000) ? 1 : 0)
# define KEY_UP(key) ((GetAsyncKeyState(key) & 0x8000) ? 0 : 1)
# define WIDTH 700
# define HEIGHT 700

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

float speed = 1.0f;
float jumping_velocity = 0.001f;
float gravity = 0.0001f;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); // Window Procedure Handler

#include "GL_functions_define.cpp"
#include "Entity_properties.cpp"

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
	  gravity = 0.0001f;
	  is_on_ground = 0;
	  jumping_velocity = 0.001f;
	}
}

bool Axis_aligned_bounding_box_collision_x(Entity entity1, Entity entity2)
{
  return ((entity1.x + speed * 0.001f + entity1.width < entity2.x - entity2.width) ||
		  (entity1.x + speed * 0.001f - entity1.width > entity2.x + entity2.width) ||
		  (entity1.y + jumping_velocity > entity2.y + entity2.height) ||
		  (entity1.y + jumping_velocity + entity1.height < entity2.y));
}

bool Axis_aligned_bounding_box_collision_y(Entity entity1, Entity entity2)
{
  return !(entity1.y > entity2.y + entity2.height + entity1.height)
	&& entity1.x + speed * 0.001f > entity2.x - entity2.width - entity1.width
	&& entity1.x + speed * 0.001f < entity2.x + entity2.width + entity1.width;
}

WGLCHOOSEPIXELFORMATARB *wglChoosePixelFormatARB;
WGLCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB;

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

	wglChoosePixelFormatARB = (WGLCHOOSEPIXELFORMATARB*)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (WGLCREATECONTEXTATTRIBSARB)wglGetProcAddress("wglCreateContextAttribsARB");

	Get_functions();

	// Replace this with glew in the future
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
	wglChoosePixelFormatARB(hdc, attriblist, 0, 1, &pixel_format, &num_format);
	ZeroMemory(&pdf, sizeof(pdf));
	DescribePixelFormat(hdc, pixel_format, sizeof(pdf), &pdf);
	SetPixelFormat(hdc, pixel_format, &pdf);
	openglcontext = wglCreateContextAttribsARB(hdc, 0, attrib_list);
	wglMakeCurrent(hdc, openglcontext);
	wglMakeCurrent(hdc, openglcontext);
	
	Entity player(0.0f, 0.0f, 0.05f, 0.1f, "player_vertex_shader.glsl", "player_fragement_shader.glsl");
	Entity gun(0.1f, 0.0f, 0.1f, 0.03f, "gun_vertex_shader.glsl", "gun_fragement_shader.glsl");
	Entity grounds[3] =
	{
	  Entity(0.4f, 0.0f, 0.2f, 0.06f, "ground_vertex_shader.glsl", "ground_fragment_shader.glsl"),
	  Entity(-0.3f, 0.0f, 0.2f, 0.06f, "ground_vertex_shader.glsl", "ground_fragment_shader.glsl"),
	  Entity(0.4f, -0.3f, 2.2f, 0.06f, "ground_vertex_shader.glsl", "ground_fragment_shader.glsl")
	};
	// To DO: Separate the gameplay logic from the interaction with the OS layer to abstract the game engine
	// To DO: make the collision detection
	while (1)
	{
	  if (Axis_aligned_bounding_box_collision_x(player, grounds[0]) && Axis_aligned_bounding_box_collision_x(player, grounds[1]))
	  {
		player.x += speed * 0.001f;
		gun.x += speed * 0.001f;
	  }
	  if (!is_on_ground)
	  {
		player.y += jumping_velocity;
		gun.y += jumping_velocity;
		jumping_velocity -= gravity * 0.01f;
		gravity += 0.0000002f;
	  }
	  is_on_ground = Axis_aligned_bounding_box_collision_y(player, grounds[0]) 
		|| Axis_aligned_bounding_box_collision_y(player, grounds[1]) || Axis_aligned_bounding_box_collision_y(player, grounds[2]);
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
	  player.render_and_update_entity();
	  gun.render_and_update_entity();
	  grounds[0].render_and_update_entity();
	  grounds[1].render_and_update_entity();
	  grounds[2].render_and_update_entity();
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
