#include "Resources/ResourceManager.h"
#include "Render/ShaderProgram.h"
#include "Render/Texture2D.h"
#include "Render/Sprite.h"

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/* Coordinates & colors for triangle */

GLfloat points[] =
{
    0.0f,  50.f, 0.0f,
   -50.f, -50.f, 0.0f,
    50.f, -50.f, 0.0f
};

GLfloat colors[] = 
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

/* Coordinates for texture to triangle */

GLfloat texCoord[] =
{
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

/*
const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"    color = vertex_color;"
"    gl_Position = vec4(vertex_position, 1.0);"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 fragment_color;"
"void main() {"
"    fragment_color = vec4(color, 1.0);"
"}";
*/

int window_width = 480;
int window_height = 480;

glm::ivec2 g_windowSize(window_width, window_height);

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv)
{
    GLFWwindow* pWindow;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "\nFILE main.cpp | Error Init! : glfwInit failed!\n";
        std::cin.get();
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "BattleCity", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "\nFILE main.cpp | Error pWindow! : glfwCreateWindow failed!\n";
        glfwTerminate();
        std::cin.get();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        std::cout << "\nFILE main.cpp | Cannot load GLAD\n";
        std::cin.get();
        return -1;
    }

    std::cout << "\nGLAD is loading!\n";
    std::cout << "\nRender: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\nOpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 1, 0, 1);

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    /*

    // Now it in class ShaderProgram

    // CREATE OUR SHADERS 

    //GLuint vertsh = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertsh, 1, &vertex_shader, nullptr);
    //glCompileShader(vertsh);

    //GLuint fragsh = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragsh, 1, &fragment_shader, nullptr);
    //glCompileShader(fragsh);

    // LINKING OUR BOTH SHADERS 

    //GLuint shader_program = glCreateProgram();
    //glAttachShader(shader_program, vertsh);
    //glAttachShader(shader_program, fragsh);
    //glLinkProgram(shader_program);

    // DELETE SHADERS 

    //glDeleteShader(vertsh);
    //glDeleteShader(fragsh);*/

    /* scope - area of visibility for Resource Manager
        to destroy the RecourceManager befor context OpenGL */
    {
        /* By class ResourceManager */

        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShader("DefaultShader",
            "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "\nFILE main.cpp | Can't create shader program\n";
            std::cin.get();
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShader("SpriteShader",
            "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "\nFILE main.cpp | Can't create sprite shader program\n";
            std::cin.get();
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");
        if (!tex)
        {
            std::cerr << "\nFILE main.cpp | Can't create pTexture\n";
            std::cin.get();
            return -1;
        }

        auto pSprite = resourceManager.loadSprite(
            "NewSprite", "DefaultTexture", "SpriteShader", 100, 100);

        pSprite->setPostion(glm::vec2(300, 100));

        /* SEND TO VIDEO-CARD MEMORY SHADERS INFO ABOUT
           POSITIONS AND COLORS OUR VERTEX
           (VBO) */

        /* Generating id */
        GLuint points_vbo = 0; // - id
        glGenBuffers(1, &points_vbo); // - generating
        /* Binding the generated array */
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        /* Content our buffer for current (points_vbo)*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        /* Same for vertex colors */

        /* Generating id */
        GLuint colors_vbo = 0; // - id
        glGenBuffers(1, &colors_vbo); // - generating
        /* Binding the generated array */
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        /* Content our buffer for current (colors_vbo)*/
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        /* For texture */

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        /* THAT WHAT HAVE TO DO VIDEO-CARD
           WITH SHADERS INFO IN ITSELF MEMORY
           (VAO) */

        /* Generating id */
        GLuint vao = 0; // - id
        glGenVertexArrays(1, &vao); // - generating
        /* Binding the generated array */
        glBindVertexArray(vao);

        /* Swith on position points and colors (buffers) */

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        
        /* Buffer for texutre */

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        /* Activate shader program */
        pDefaultShaderProgram->use();
        /* Set uniform for texture */
        pDefaultShaderProgram->setInt("tex", 0);

        /* WORKING WITH TRANSFORMATION MATRIX 
           (GLM) */

        // MODEL MATRIX

        // Creating a identity model matrix
        glm::mat4 modelMatrix = glm::mat4(1.f);
        // Translating our triangle
        modelMatrix = glm::translate(modelMatrix, glm::vec3(240.f, 50.f, 0.0f));
        // Now we in "World Space"

        // PROJECTION MATRIX

        // Creating our frustum
        glm::mat4 projMatrix = glm::ortho(0.0f, static_cast<float>(g_windowSize.x), 
                                          0.0f, static_cast<float>(g_windowSize.y),
                                         -10.f, 50.f);
        // Now we in "Clip Space"

        /* TRANSFER OF MATRIX TO SHADER */

        pDefaultShaderProgram->setMatrix4("projMatrix", projMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projMatrix", projMatrix);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /*  SHADERS DRAWING  */

            /* Swith on the shader program what we want */
            pDefaultShaderProgram->use();
            /* Connect our vertex array object */
            glBindVertexArray(vao);

            /* Activate our texture */
            tex->bind();

            // Transfering our triangle
            pDefaultShaderProgram->setMatrix4("modelMatrix", modelMatrix);
            /* The command what draw a current binded vao */
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
        /*std::cout << "\nExit for block while\n";
        std::cin.get();*/
    }

    /*std::cout << "\nTerminate";
    std::cin.get();*/

    glfwTerminate();

    std::cout << "\nPress any keyboard ...\n";
    std::cin.get();

    return 0;
}