# define WIN32_LEAN_AND_MEAN
# define KEY_DOWN(key) ((GetAsyncKeyState(key) & 0x8000) ? 1 : 0)
# define KEY_UP(key) ((GetAsyncKeyState(key) & 0x8000) ? 0 : 1)
# define WIDTH 700
# define HEIGHT 700

#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <iostream>
#include <string>
#include <cmath>
#define ENABLE_INFO_LOG
#include "log.h"
#include "GL_functions_define.h"
#include "win32GL.h"
#include "tools.h"
#include "Entity_properties.h"
#include "glTools.h"

#include "win32GL.cpp"
#include "GL_functions_define.cpp"
#include "win32Tools.cpp"
#include "Entity_properties.cpp"
#include "log.cpp"
#include "glTools.cpp"


float speed = 1.0f;
float jumping_velocity = 0.001f;
float gravity = 0.0001f;

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


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

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prev_instance, LPSTR cmd, int cmd_show)
{
    
    WNDCLASSEX				real_window_class = {};
	
	real_window_class.cbSize = sizeof(WNDCLASSEX);
	real_window_class.style = CS_HREDRAW | CS_VREDRAW;
	real_window_class.lpfnWndProc =  WinProc;
	real_window_class.hInstance = hinstance;
	real_window_class.lpszClassName = "CLASS1";
	
	if (!RegisterClassEx(&real_window_class))
	{
		ERROR_LOG("RegisterClassEx() Failed in %s %d\n", __FILE__, __LINE__);;
		return (1);
	}
	
	HWND window_handle = CreateWindowEx(0, "CLASS1", "Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, WIDTH, HEIGHT, 0, 0, hinstance, 0);
	if (!window_handle)
	{
		ERROR_LOG("CreateWindowEx() Failed in %s %d\n", __FILE__, __LINE__);;
		return (1);
	}
	
	HDC	window_context = GetDC(window_handle);
	
	HGLRC glContext = win32InitOpenGL(window_handle);
	if (!glContext)
	{
		MessageBox(0, "win32InitOpenGL Failed\n", "ERROR", MB_OK);
		return (1);
	}
	wglMakeCurrent(window_context, glContext);
	
	INFO_LOG("%s\n", glGetString(GL_VENDOR));
	INFO_LOG("%s\n", glGetString(GL_RENDERER));
	INFO_LOG("%s\n", glGetString(GL_VERSION));
	
	Entity player(0.0f, 0.0f, 0.05f, 0.1f, "../src/shaders/player_vertex_shader.glsl", "../src/shaders/player_fragement_shader.glsl");
	Entity gun(0.1f, 0.0f, 0.1f, 0.03f, "../src/shaders/gun_vertex_shader.glsl", "../src/shaders/gun_fragement_shader.glsl");
	Entity grounds[3] =
	{
		Entity(0.4f, 0.0f, 0.2f, 0.06f, "../src/shaders/ground_vertex_shader.glsl", "../src/shaders/ground_fragment_shader.glsl"),
		Entity(-0.3f, 0.0f, 0.2f, 0.06f, "../src/shaders/ground_vertex_shader.glsl", "../src/shaders/ground_fragment_shader.glsl"),
		Entity(0.4f, -0.3f, 2.2f, 0.06f, "../src/shaders/ground_vertex_shader.glsl", "../src/shaders/ground_fragment_shader.glsl")
	};
	// To DO: Separate the gameplay logic from the interaction with the OS layer to abstract the game engine
	// To DO: make the collision detection
	
	MSG						msg = {};
	
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
		
		glClearColor(0.09f, 0.09f, 0.09f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		player.render_and_update_entity();
		gun.render_and_update_entity();
		grounds[0].render_and_update_entity();
		grounds[1].render_and_update_entity();
		grounds[2].render_and_update_entity();
		SwapBuffers(window_context);
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
		{
			DestroyWindow(hwnd);
		}
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
