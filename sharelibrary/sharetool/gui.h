#pragma once
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL/SDL_opengl.h>

namespace ShareTool{
     //Our state
     void SetImgui(SDL_Window* window, SDL_GLContext gl_context);
     
     void GUIProcessEvent(SDL_Event event);
     
      void TestGUI();
     
     void RenderGUI(ImVec4 clear_color);

     void Destory();
}