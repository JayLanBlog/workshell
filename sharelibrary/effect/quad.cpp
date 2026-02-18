#include "quad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
namespace component {
    void Quad::Generate() {
        // configure VAO/VBO
        unsigned int VBO;
        float vertices[] = {
            // pos      // tex
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f
        };
        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindVertexArray(this->quadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Quad::Draw() {
        glBindVertexArray(this->quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

	Shader* qudaShader;
    Quad* mQuad;
    glm::vec2 mViewSize;
    glm::mat4 projection;
    Shader* imgShader;
    
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection; \n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * model *vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "uniform vec4 customColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = customColor;\n"
        "}\n\0";

    const char* imageVertex = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 uv;\n"
        "out vec2 aUV;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection; \n"
        "void main()\n"
        "{\n"
        "   aUV = uv; \n"
        "   gl_Position = projection * model *vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char* imageFragment = "#version 330 core\n"
        "uniform vec4 customColor;\n"
        "in vec2 aUV;\n"
        "uniform sampler2D texture1;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "  FragColor = texture(texture1, aUV);\n"
        "}\n\0";

    
    void Initialize(glm::vec2 viewSize) {
        qudaShader = new Shader(vertexShaderSource, fragmentShaderSource);
        imgShader = new Shader( imageVertex , imageFragment);
        mQuad = new Quad();
        mQuad->Generate();
        mViewSize = viewSize;
        projection = glm::ortho(0.0f, viewSize.x, viewSize.y, 0.0f, -1.0f, 1.0f);
    }

    void Update(QuadParam param) {
        qudaShader->use();
        glm::vec2 position(param.x, param.y);
        glm::vec2 size(param.width * param.scale, param.height * param.scale);
        //glm::mat4 projection = glm::ortho(0.0f, 800.0f, 800.0f, 0.0f, -1.0f, 1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        //model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));
        qudaShader->setVec4("customColor", glm::vec4(param.color.r, param.color.g, param.color.b,param.color.a));
        qudaShader->setMat4("model", model);
        qudaShader->setMat4("projection", projection);
    }

    void QuadDraw(QuadParam param) {
        glViewport(param.canvas.x,param.canvas.y,param.canvas.width,param.canvas.height);
        Update(param);
        qudaShader->use();
        mQuad->Draw();
    }
    
    void UpdateImage(QuadParam param) {
        imgShader->use();
        glm::vec2 position(param.x, param.y);
        glm::vec2 size(param.width * param.scale, param.height * param.scale);
        //glm::mat4 projection = glm::ortho(0.0f, 800.0f, 800.0f, 0.0f, -1.0f, 1.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        //model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));
        //imgShader->setVec4("customColor", glm::vec4(param.color.r, param.color.g, param.color.b, param.color.a));
        imgShader->setMat4("model", model);
        imgShader->setMat4("projection", projection);
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, param.textureId);
    }

    void ImageDraw(QuadParam param) {
        glViewport(param.canvas.x, param.canvas.y, param.canvas.width, param.canvas.height);
        UpdateImage(param);
        imgShader->use();
        mQuad->Draw();
    }

    void Destroy() {
        if (qudaShader) {
            delete qudaShader;
        }
        if (mQuad) {
            delete mQuad;
        }
    }
}
