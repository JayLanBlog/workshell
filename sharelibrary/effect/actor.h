#pragma once
#include "sharelibrary/configer.h"
#include "color.h"
#include <deque>
#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <random>
#include "quad.h"

#include "sharelibrary/sharetool/resource_manager.h"

using namespace component;
using namespace std;
namespace actor {

    enum stari_posibile_pacanea { IDLE, STARTING_TO_SHUFFLE, SHUFFLING, STOPPING_FROM_SHUFFLE };
    enum class tipuri_evenimente { LEVER_PULLED, STAKE_INC, STAKE_DEC };
    enum class GUIComponentTypes { COMPLEX, TRIANGLE, SQUARE, CIRCLE };
    
    struct Point {
        float x, y;
    };

	class Actor {
	public:
		bool isAvailable = true;
	};

    class GUIComponent :public Actor {
    public:
        vector<GUIComponent*>* components;//subcomponents
        vector<Point>* puncte_componenta;
        float position_x;
        float position_y;
        float relative_y_position;
        float relative_rotation = 0;
        Color color;
        string label;

        float width;
        float heigh;
        float scale = 1.0;  
        GLint textureID = 0;

        GUIComponentTypes type;
        GUIComponent* parent;
        GUIComponent(float posx, float posy, string label) {
            this->components = new vector<GUIComponent*>();
            this->puncte_componenta = new vector<Point>();
            this->position_x = posx;
            this->position_y = posy;
            this->label = label;
            this->type = type;
            this->color = { 1,0,0 };
            this->type = GUIComponentTypes::COMPLEX; //by default componenta e una complexa (nu e o componenta nativa)
            this->parent = nullptr;
            this->width = 125;
            this->heigh = 125;
        }

        void changeColor(Color clr) {
            this->color = clr;
        }

        void addPoint(Point p) {
            puncte_componenta->push_back(p);

        }
        void addSubComponent(GUIComponent* cmp) {
            components->push_back(cmp);
            cmp->parent = this;
        }

        float getAbsolutePositionX() {
            float x = this->position_x;

            GUIComponent* tmp = this->parent;
            while (tmp != nullptr) {
                x += tmp->position_x;
                tmp = tmp->parent;
            }
            return x;
        }

        float getAbsolutePositionY() {
            float y = this->position_y;

            GUIComponent* tmp = this->parent;
            while (tmp != nullptr) {
                y += tmp->position_y;
                tmp = tmp->parent;
            }
            return y;
        }

        float* getParentAbsolutePosition() {
            float x = 0;
            float y = 0;

            GUIComponent* tmp = this->parent;
            while (tmp != nullptr) {
                x += tmp->position_x;
                y += tmp->position_y;
                tmp = tmp->parent;
            }
            return new float[] {x, y};
        }
    };

    class Rectang : public GUIComponent {
    public:
        Rectang(float posx, float posy, Point p1, Point p2, Point p3, Point p4, string label, Color clr) :GUIComponent(posx, posy, label) {
            this->color = clr;
            this->puncte_componenta->push_back(p1);
            this->puncte_componenta->push_back(p2);
            this->puncte_componenta->push_back(p3);
            this->puncte_componenta->push_back(p4);
            this->type = GUIComponentTypes::SQUARE;
            this->width = 100;
            this->heigh = 100;
        }
    };


    class RectangImage : public GUIComponent {
    public:
        RectangImage(float posx, float posy, std::string img, string label, Color clr) :GUIComponent(posx, posy, label) {
            this->color = clr;
            textureID = ResourceManager::loadGLTexture(img.c_str());
            this->type = GUIComponentTypes::SQUARE;
            this->width = 100;
            this->heigh = 100;
        }
    };


    inline void DrawComponent(GUIComponent &component) {
        component::Canvas canvas{ GetScreenViewWidth(), GetScreenViewHeight(),0.0,0.0 };
        component::QuadParam param{ component.width, component.heigh, component.position_x, component.position_y , canvas,component.scale, component.color };
        component::QuadDraw(param);
    }

    inline void DrawComponentAbs(GUIComponent& component) {
        component::Canvas canvas{ GetScreenViewWidth(), GetScreenViewHeight(),0.0,0.0 };
        component::QuadParam param{ component.width, component.heigh, component.getAbsolutePositionX(), component.getAbsolutePositionY() , canvas,component.scale, component.color};
        component::QuadDraw(param);
    }

    inline void DrawComponentImgAbs(GUIComponent& component) {
        component::Canvas canvas{ GetScreenViewWidth(), GetScreenViewHeight(),0.0,0.0 };
        component::QuadParam param{ component.width, component.heigh, component.getAbsolutePositionX(), component.getAbsolutePositionY() , canvas,component.scale, component.color,component.textureID };
        component::ImageDraw(param);
    }
}