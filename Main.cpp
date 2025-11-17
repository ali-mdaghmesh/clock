#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//////////
//to be clear ai help with:
//shaders that begin from line 19 :)
//with texture:)

using namespace std;

const char* basicVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoord;

void main() {
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    TexCoord = tex;
}
)";


const char* brownFrag = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(0.40, 0.22, 0.08, 1.0);
}
)";

const char* headFrag = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D texClock;
uniform bool front;

void main() {
    if(front)
        FragColor = texture(texClock, TexCoord);
    else
        FragColor = vec4(0.0,0.0,0.0,1.0);
}
)";


const char* yellowFrag = R"(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0,1.0,0.0,1.0);
}
)";



int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Clock (simplified code)", NULL, NULL);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);


    float bodyVerts[] = {
        -0.5f,-1.0f, 0.5f,
         0.5f,-1.0f, 0.5f,
         0.5f, 1.0f, 0.5f,
        -0.5f, 1.0f, 0.5f,

        -0.5f,-1.0f,-0.5f,
         0.5f,-1.0f,-0.5f,
         0.5f, 1.0f,-0.5f,
        -0.5f, 1.0f,-0.5f
    };

    unsigned int bodyIdx[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,4,7, 7,3,0,
        1,5,6, 6,2,1,
        3,2,6, 6,7,3,
        0,1,5, 5,4,0
    };


    float headVerts[] = {
        -0.5f,-0.5f,0.5f, 0,0,
         0.5f,-0.5f,0.5f, 1,0,
         0.5f, 0.5f,0.5f, 1,1,
        -0.5f, 0.5f,0.5f, 0,1,

        -0.5f,-0.5f,-0.5f,0,0,
         0.5f,-0.5f,-0.5f,0,0,
         0.5f,0.5f,-0.5f,0,0,
        -0.5f,0.5f,-0.5f,0,0
    };

    unsigned int headIdx[] = {
        0,1,2,2,3,0,   
        4,5,6,6,7,4,    
        4,0,3,3,7,4,    
        1,5,6,6,2,1,   
        3,2,6,6,7,3,    
        4,5,1,1,0,4    
    };


    float stickVerts[] = {
        -0.02f,-0.5f,0.52f,
         0.02f,-0.5f,0.52f,
         0.02f, 0.5f,0.52f,
        -0.02f, 0.5f,0.52f,

        -0.02f,-0.5f,0.48f,
         0.02f,-0.5f,0.48f,
         0.02f, 0.5f,0.48f,
        -0.02f, 0.5f,0.48f
    };

    unsigned int stickIdx[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,4,7, 7,3,0,
        1,5,6, 6,2,1,
        3,2,6, 6,7,3,
        0,1,5, 5,4,0
    };



    unsigned int vaoBody, vboBody, eboBody;
    glGenVertexArrays(1, &vaoBody);
    glGenBuffers(1, &vboBody);
    glGenBuffers(1, &eboBody);

    glBindVertexArray(vaoBody);
    glBindBuffer(GL_ARRAY_BUFFER, vboBody);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bodyVerts), bodyVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboBody);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bodyIdx), bodyIdx, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vaoHead, vboHead, eboHead;
    glGenVertexArrays(1, &vaoHead);
    glGenBuffers(1, &vboHead);
    glGenBuffers(1, &eboHead);

    glBindVertexArray(vaoHead);
    glBindBuffer(GL_ARRAY_BUFFER, vboHead);
    glBufferData(GL_ARRAY_BUFFER, sizeof(headVerts), headVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHead);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(headIdx), headIdx, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int vaoStick, vboStick, eboStick;
    glGenVertexArrays(1, &vaoStick);
    glGenBuffers(1, &vboStick);
    glGenBuffers(1, &eboStick);

    glBindVertexArray(vaoStick);
    glBindBuffer(GL_ARRAY_BUFFER, vboStick);
    glBufferData(GL_ARRAY_BUFFER, sizeof(stickVerts), stickVerts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboStick);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(stickIdx), stickIdx, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    auto makeShader = [&](const char* fragSrc) {
        unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert, 1, &basicVertexShader, nullptr);
        glCompileShader(vert);

        unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &fragSrc, nullptr);
        glCompileShader(frag);

        unsigned int prog = glCreateProgram();
        glAttachShader(prog, vert);
        glAttachShader(prog, frag);
        glLinkProgram(prog);

        glDeleteShader(vert);
        glDeleteShader(frag);

        return prog;
    };

    unsigned int shBody = makeShader(brownFrag);
    unsigned int shHead = makeShader(headFrag);
    unsigned int shStick = makeShader(yellowFrag);

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, hiegh, chanelNumber;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* img = stbi_load("../clock.jpg", &width, &hiegh, &chanelNumber, 0);
    if (img) {
        if (chanelNumber == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, hiegh, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        }
        else if (chanelNumber == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, hiegh, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
        }
       glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "failed loading image";
    }
    stbi_image_free(img);

    glUseProgram(shHead);
    glUniform1i(glGetUniformLocation(shHead, "texClock"), 0);



    float rotate = 0;
    double lastX = 400;
    while (!glfwWindowShouldClose(window)) {

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        rotate += (mx - lastX) * 0.005f;
        lastX = mx;

        glClearColor(0.2f, 0.3f, 0.3f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::translate(glm::mat4(1), glm::vec3(0, 0, -6));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);

        glm::mat4 base = glm::rotate(glm::mat4(1), rotate, glm::vec3(0, 1, 0));


        glUseProgram(shBody);
        glm::mat4 mB = glm::scale(base, glm::vec3(0.7f, 1.0f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shBody, "view"), 1, 0, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shBody, "proj"), 1, 0, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(shBody, "model"), 1, 0, glm::value_ptr(mB));
        glBindVertexArray(vaoBody);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        glUseProgram(shHead);
        glm::mat4 mH = glm::translate(base, glm::vec3(0, 1.25f, 0));
        mH = glm::scale(mH, glm::vec3(0.8f));

        glUniformMatrix4fv(glGetUniformLocation(shHead, "view"), 1, 0, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shHead, "proj"), 1, 0, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(shHead, "model"), 1, 0, glm::value_ptr(mH));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        glBindVertexArray(vaoHead);

        glUniform1i(glGetUniformLocation(shHead, "front"), 1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniform1i(glGetUniformLocation(shHead, "front"), 0);
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned)));

        glUseProgram(shStick);

        float swing = glm::radians(15.f) * sin(glfwGetTime() * 2);
        glm::mat4 mS = base;
        mS = glm::translate(mS, glm::vec3(0, 0, 0));
        mS = glm::translate(mS, glm::vec3(0, 0.9f, 0));
        mS = glm::rotate(mS, swing, glm::vec3(0, 0, 1));
        mS = glm::translate(mS, glm::vec3(0, -0.9f, 0));

        glUniformMatrix4fv(glGetUniformLocation(shStick, "view"), 1, 0, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shStick, "proj"), 1, 0, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(shStick, "model"), 1, 0, glm::value_ptr(mS));

        glBindVertexArray(vaoStick);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
