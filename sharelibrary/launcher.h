#pragma once 
#include <istream>
#include "glad/glad.h"
#include <SDL.h>
#include "sharelibrary/sharetool/gui.h"
#include "sharelibrary/effect/trangle.h"
#include "sharelibrary/effect/quad.h"
#include "sharelibrary/configer.h"
#include "sharelibrary/effect/component_manager.h"
#include "sharelibrary/sharetool/time.h"


using namespace actor;
namespace Slotmatch {
    stari_posibile_pacanea current_state;

    float bgWidth = 0.0f;
    float bgHeight = 0.0f;
    GLint bgTID;
    //Texture2D* BG;

    // --- 物理参数 ---
    float rectPosX = 0.0f, rectPosY = 0.0f; // 当前位置
    float velX = 0.0f, velY = 0.0f;         // 当前速度
    float targetX = 0.0f, targetY = 0.0f;   // 目标位置（鼠标）
    float k = 40.0f;                        // 弹性系数 (Spring stiffness)
    float c = 5.0f;                         // 阻尼系数 (Damping)

    util::Timer timer;
    bool isDragging = false;

    component::Trangle trangle;
    GUIManager* guiManager = new GUIManager();

    inline GUIComponent* generateMonitor() {
        float startXOffset = 1.6;
        float gap = GetScreenViewWidth() / 3.0f;
        GUIComponent* wrapper = new GUIComponent(0 , 175, "monitor");
        MonitorColumn* column1 = new MonitorColumn(gap, 0, "monitor_column1", 0);
        wrapper->addSubComponent(column1);
        column1->init();
        MonitorColumn* column2 = new MonitorColumn(gap+100.f, 0, "monitor_column2", 0);
        wrapper->addSubComponent(column2);
        column2->init();
        MonitorColumn* column3 = new MonitorColumn(gap + 200.f, 0, "monitor_column3", 0);
        wrapper->addSubComponent(column3);
        column3->init();
        return wrapper;
    }

    vector<GUIComponent*>* generateMasks() {
        //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImVec4 clear_color = ImVec4(1.f, 0.f, 0.f, 1.00f);
        vector<GUIComponent*>* tmp = new vector<GUIComponent*>();
        GUIComponent* ctmp = new Rectang(0, 570, { 0,0 }, { 425,0 }, { 425,500 }, { 0,500 }, "upper_p", { clear_color.x,clear_color.y,clear_color.z });
        ctmp->width = GetScreenViewWidth();
        ctmp->heigh = 500;
        tmp->push_back(ctmp);
        clear_color = ImVec4(0.f, 1.f, 0.f, 1.00f);
        ctmp = new Rectang(0, 125, { 0,0 }, { 425,0 }, { 425,150 }, { 0,150 }, "lower_p", { clear_color.x,clear_color.y,clear_color.z });
        ctmp->width = GetScreenViewWidth();
        ctmp->heigh = 150;
        tmp->push_back(ctmp);
        return tmp;
    }

    inline void shiftElementsRight(MonitorColumn* mc) {
        deque<Symbol*>* v = mc->components;
        Symbol* symbol_to_be_deleted = v->at(v->size() - 1);
        for (int i = v->size() - 2; i >= 0; i--) {
            v->at(i + 1) = v->at(i);
            ((GUIComponent*)v->at(i)->current_used_guicomponent)->relative_y_position = 0;
        }
        ((GUIComponent*)symbol_to_be_deleted->current_used_guicomponent)->relative_y_position = 0;
        symbol_to_be_deleted->disposeInnerGUIComponent();
        mc->addSymbol(generateNewSymbol());
    }

    inline void paintSymbols(float speed) {
        for (int i = 0; i < guiManager->monitor->components->size(); i++) {
            MonitorColumn* mc = (MonitorColumn*)guiManager->monitor->components->at(i);
            int r = rand() % 4;
            if (speed == 0) {
                r = 0;
            }
            mc->shiftSymbols(speed + r);
            for (int i = 0; i < mc->components->size(); i++) {
                guiManager->drawSymbol(mc, i);
            }

            for (int i = 0; i < mc->components->size(); i++) {
                GUIComponent* comp = (GUIComponent*)mc->components->at(i)->current_used_guicomponent;
                if (comp == nullptr)
                    continue;
                float absPosY = comp->getAbsolutePositionY();

                if (absPosY > guiManager->disposalThreshold) {
                    shiftElementsRight(mc);
                }
            }
        }
    }
    void drawTest() {
       
       // float currentTime = glfwGetTime();
        float dt = timer.record_elapsed_seconds();;
        //lastTime = currentTime;

        // 2. 物理更新：阻尼弹簧公式 F = -k*x - c*v
        float forceX = -k * (rectPosX - targetX) - c * velX;
        float forceY = -k * (rectPosY - targetY) - c * velY;

        velX += forceX * dt;
        velY += forceY * dt;
        rectPosX += velX * dt;
        rectPosY += velY * dt;

        // 3. 渲染逻辑
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // 设置正交投影：左, 右, 下, 上, 近, 远
        glOrtho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(rectPosX, rectPosY, 0.0f); // 应用物理计算的位置

        // 画一个简单的方块
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.1f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glEnd();
    }
	int loop(SDL_Window* window) {
		bool quit = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        while (!quit)
        {
           // tests.Run();
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
             
                switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEMOTION:
       
                    // 映射到正交投影坐标 (假设窗口 800x600，范围 -10 到 10)
                    targetX = (event.motion.x / 800.0f) * 20.0f - 10.0f;
                    targetY = 10.0f - (event.motion.y / 600.0f) * 20.0f; // 注意 Y 轴反转
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE: // exit tests
                        quit = true;
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        // Tells the engine to reload window configuration (size and dpi)
                       // tests.SetWindow(tests.window);
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                       // tests.is_window_active = false;
                        break;
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                      /*  tests.is_window_active = true;
                        if (wi::shadercompiler::GetRegisteredShaderCount() > 0)
                        {
                            std::thread([] {
                                wi::backlog::post("[Shader check] Started checking " + std::to_string(wi::shadercompiler::GetRegisteredShaderCount()) + " registered shaders for changes...");
                                if (wi::shadercompiler::CheckRegisteredShadersOutdated())
                                {
                                    wi::backlog::post("[Shader check] Changes detected, initiating reload...");
                                    wi::eventhandler::Subscribe_Once(wi::eventhandler::EVENT_THREAD_SAFE_POINT, [](uint64_t userdata) {
                                        wi::renderer::ReloadShaders();
                                        });
                                }
                                else
                                {
                                    wi::backlog::post("[Shader check] All up to date");
                                }
                                }).detach();
                        }*/
                        break;
                    default:
                        break;
                    }
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        std::cout << "space key down" << std::endl;
                    }
                    break;
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        std::cout << "space key up" << std::endl;
                        if (current_state == IDLE) {
                            current_state = STARTING_TO_SHUFFLE;
                        }

                        if (current_state == SHUFFLING) {
                            current_state = STOPPING_FROM_SHUFFLE;
                        }
                    }
                    break;
                default:
                    break;
                }
                ShareTool::GUIProcessEvent(event);
                // wi::input::sdlinput::ProcessEvent(event);
            }
            ShareTool::TestGUI();
          //  ImGuiIO& io = ImGui::GetIO(); (void)io;
          //  glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
          //  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
          //  glClear(GL_COLOR_BUFFER_BIT);
          ////  paintSymbols(0.1);
          //  component::Canvas canvas{ GetScreenViewWidth(), GetScreenViewHeight(),0.0,0.0 };
          //  Color color = { 0.0,1.0,0.0,1.0 };
          //  component::QuadParam param{ bgWidth,bgHeight, 0.0, 0.0 ,canvas, 1.0,color ,bgTID };
          //  component::ImageDraw(param);
          //  float raito = 0.5;
          //  //const int SCREEN_WIDTH = 768;
          //  //onst int SCREEN_HEIGH = 768;

          //  component::UpdateViewPort(glm::vec4(0.0,io.DisplaySize.x , (int)io.DisplaySize.y -200, 0 ));
          //  glViewport(-100, 100, (int)io.DisplaySize.x , (int)io.DisplaySize.y  -200);
          //  if (current_state == STARTING_TO_SHUFFLE){
          //      current_state = SHUFFLING;
          //      paintSymbols(0);
          //  }
          //  else if (current_state == IDLE) {
          //      paintSymbols(0);
          //  }
          //  else if (current_state == STOPPING_FROM_SHUFFLE) {
          //      current_state = IDLE;
          //      paintSymbols(0.5);
          //  }
          //  if (current_state == SHUFFLING) {
          //      //guiManager->rotateComponent(guiManager->lever->components->at(2));
          //      paintSymbols(1);
          //  }
          //  
          //  //guiManager->drawComponentQuad();
          //  for (int i = 0; i < guiManager->masks->size(); i++) {
          //      guiManager->drawComponentQuad(guiManager->masks->at(i));
          //  }
          //  component::RestViewPort();
          //  /*
          //    component::Canvas canvas{ GetScreenViewWidth(), GetScreenViewHeight(),0.0,0.0 };
          //    component::QuadParam param{ component.width, component.heigh, component.getAbsolutePositionX(), component.getAbsolutePositionY() , canvas,component.scale, component.color,component.textureID };
          //    component::ImageDraw(param);
          //  */
            drawTest();
            SDL_GL_SwapWindow(window);
        }
		return 0;
	}
 
	int luanch_main() {
		// Setup SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
		{
			printf("Error: %s\n", SDL_GetError());
			return -1;
		}
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GetScreenViewWidth(), GetScreenViewHeight(), window_flags);
		if (window == nullptr)
		{
			printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
			return -1;
		}
        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        // Load OpenGL extensions
        // NOTE: GL procedures address loader is required to load extensions 
        if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0) {
            printf("GLAD: Cannot load OpenGL extensions!\n");
        }

        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync
        ShareTool::SetImgui(window, gl_context);
        component::Initialize({ GetScreenViewWidth(), GetScreenViewHeight() });
        trangle.Init();
        guiManager->monitor = generateMonitor();
        guiManager->masks = generateMasks();
        current_state = IDLE;

        Texture2D tx =  ResourceManager::LoadTexture("res/concreteTexture.png", false, "BG");
        bgTID = tx.ID;
        bgWidth = tx.Width;
        bgHeight = tx.Height;

        int ret = loop(window);
        ShareTool::Destory();
        component::Destroy();
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
		return 0;
	}
}