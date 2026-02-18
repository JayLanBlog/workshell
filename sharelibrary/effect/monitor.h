#pragma once
#include "actor.h"
namespace actor {
    class Symbol {
    public:
        GUIComponentTypes type;
        int column;
        Actor* current_used_guicomponent = nullptr;
        bool markedForDeletion = false;

        Symbol(GUIComponentTypes type, int column) {
            this->type = type;
            this->column = column;
        }

        void disposeInnerGUIComponent() {
            this->current_used_guicomponent->isAvailable = true;
            ((GUIComponent*)this->current_used_guicomponent)->parent = nullptr;
            ((GUIComponent*)this->current_used_guicomponent)->relative_y_position = 0;
            ((GUIComponent*)this->current_used_guicomponent)->position_y = 1300;
            this->current_used_guicomponent = nullptr;
            this->markedForDeletion = true;
        }
    };

    inline GUIComponentTypes generateNewSymbol() {
        //random generator to implement
        int random_index = rand() % 3;
        GUIComponentTypes types[3] = { GUIComponentTypes::SQUARE, GUIComponentTypes::SQUARE, GUIComponentTypes::SQUARE };
        return types[random_index];
    }

    class MonitorColumn : public GUIComponent {
    public:
        deque<Symbol*>* components;
        vector<Actor*>* actors;
        float distance_between_symbols = 100;
        float relative_speed = 0.1;
        float height = 400;
        float width = 150;
        float column = 0;
        MonitorColumn(float posx, float posy, string label, int column) :GUIComponent(posx, posy, label) {
            components = new deque<Symbol*>();
            actors = new vector<Actor*>();
            this->column = column;
        }

        void init() {
            this->actors = generateActorSetFor();
            this->components->push_front(nullptr);
            this->addSymbol(generateNewSymbol());

            this->components->push_front(nullptr);
            this->addSymbol(generateNewSymbol());

            this->components->push_front(nullptr);
            this->addSymbol(generateNewSymbol());

            this->components->push_front(nullptr);
            this->addSymbol(generateNewSymbol());
        }

        void addSymbol(GUIComponentTypes type) {
            int free_actor_index = -1;
            for (int i = 0; i < actors->size(); i++) {
                Actor* ca = actors->at(i);

                if (ca->isAvailable && ((GUIComponent*)ca)->type == type) {
                    free_actor_index = i;
                    ca->isAvailable = false;
                    break;
                }
            }
            if (free_actor_index != -1) {
                Symbol* symbol = new Symbol(type, column);
                symbol->current_used_guicomponent = actors->at(free_actor_index);
                ((GUIComponent*)symbol->current_used_guicomponent)->position_x = 0;
                ((GUIComponent*)symbol->current_used_guicomponent)->position_y = 0;
                ((GUIComponent*)symbol->current_used_guicomponent)->parent = this;
                components->at(0) = symbol;
            }
        }


        vector<Actor*>* generateActorSetFor() {
            MonitorColumn* mc = this;
            float absX = mc->getAbsolutePositionX();
            float hidePosition = 1300;
            vector<Actor*>* tmp = new vector<Actor*>();

    /*        GUIComponent* actor_tmp = new Rectang(absX, hidePosition, {   }, {   }, {   }, {   }, "actor_4", {1.0,0.0,0.0,1.0});
            tmp->push_back((Actor*)actor_tmp);

            actor_tmp = new Rectang(absX, hidePosition, { }, {  }, {   }, {  }, "actor_5", { 1.0,1.0,0.0,1.0 });
            tmp->push_back((Actor*)actor_tmp);

            actor_tmp = new Rectang(absX, hidePosition, {   }, {   }, {   }, {  }, "actor_6", { 0.0,1.0,0.0,1.0 });
            tmp->push_back((Actor*)actor_tmp);

            actor_tmp = new Rectang(absX, hidePosition, {   }, {   }, {   }, {   }, "actor_7", { 0.0,0.0,1.0,1.0 });
            tmp->push_back((Actor*)actor_tmp);*/

            GUIComponent* actor_tmp = new RectangImage(absX, hidePosition,"res/1.jpg", "actor_4", { 1.0,0.0,0.0,1.0 });
            tmp->push_back((Actor*)actor_tmp);

            actor_tmp = new RectangImage(absX, hidePosition, "res/2.jpg", "actor_4", { 1.0,1.0,0.0,1.0 });
            tmp->push_back((Actor*)actor_tmp);

            actor_tmp = new RectangImage(absX, hidePosition, "res/3.jpg", "actor_4", { 0.0,1.0,0.0,1.0 });
            tmp->push_back((Actor*)actor_tmp);
            
            actor_tmp = new RectangImage(absX, hidePosition, "res/4.jpg", "actor_4", { 1.0,0.0,1.0,1.0 });
            tmp->push_back((Actor*)actor_tmp);
            
           // actor_tmp = new RectangImage(absX, hidePosition, "res/", "actor_4", { 1.0,0.0,0.0,1.0 });

            return tmp;
        }


        void shiftSymbols(float new_rspeed) {
            this->relative_speed = new_rspeed;
        }

        float computeYVector(int s_index) {
            return this->relative_speed;
        }

        float computeAbsY(int s_index) {

            return getParentAbsolutePosition()[1] + s_index * distance_between_symbols;
        }

        float computeRelativeYPos(int s_index) {
            GUIComponent* current_comp = (GUIComponent*)components->at(s_index)->current_used_guicomponent;
            return current_comp->position_y / height;
        }

        float computeCurrentScale(int s_index) {
            float relative_y_pos = computeRelativeYPos(s_index) * 100;
            if (relative_y_pos < 40 || relative_y_pos > 60)
                return 1.0; //no maginification
            return 1 + (-0.008 * (relative_y_pos - 50) * (relative_y_pos - 50) + 0.75); //-0.008*(x-50)^2 + 0.75
        }

    };



}