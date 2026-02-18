 #pragma once
#include "monitor.h"

namespace actor {
    class GUIManager {
    public:
        GUIComponent* monitor;
        vector<GUIComponent*>* masks;
        float disposalThreshold = 575;
        void drawComponent(GUIComponent* gcomp, string ignored_component = "") {
            if (gcomp->label.compare(ignored_component) == 0) {
                return;
            }
           // std::cout << "Label : " << gcomp->label << " , r : " << gcomp->color.r << " , g:" << gcomp->color.g << " , b : " << gcomp->color.b << std::endl;
            DrawComponentImgAbs(*gcomp);
        }

        void drawComponentQuad(GUIComponent* gcomp, string ignored_component = "") {
            if (gcomp->label.compare(ignored_component) == 0) {
                return;
            }
            // std::cout << "Label : " << gcomp->label << " , r : " << gcomp->color.r << " , g:" << gcomp->color.g << " , b : " << gcomp->color.b << std::endl;
            DrawComponentAbs(*gcomp);
        }

        void moveComponent(GUIComponent* gcomp, float x, float y, float scale) {
            gcomp->position_x = x;
            gcomp->position_y = y;
          /*  float pX = gcomp->getAbsolutePositionX();
            float pY = gcomp->getAbsolutePositionY();*/
            drawComponent(gcomp);
        }

        void drawSymbol(MonitorColumn* m, int symbol_index) {
            Symbol* s = m->components->at(symbol_index);
            GUIComponent* physical_component = (GUIComponent*)s->current_used_guicomponent;
            if (physical_component == nullptr)
                return;
            float x = physical_component->type == GUIComponentTypes::CIRCLE ? 20 : 0;
            float y = m->distance_between_symbols * symbol_index + physical_component->relative_y_position + m->computeYVector(symbol_index);
            physical_component->relative_y_position += m->computeYVector(symbol_index);
            float scale = m->computeCurrentScale(symbol_index);
            this->moveComponent(physical_component, x, y, scale);
        }

        void rotateComponent(GUIComponent* gcmp) {
          
        }

        void translateComponent(GUIComponent* gcmp, float x, float y) {
           
        }

    };

}