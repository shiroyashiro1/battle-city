#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLfloat points[] =
{
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
};

GLfloat colors[] =
{
    0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f
};

const char* vertex_shader =
"#version 460\n"
"layuot(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main() {"
"    gl_Position = vec4(vertex_position, 1.0);"
"    color = vertex_color;"
"}";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 fragment_color;"
"void main() {"
"    fragment_color = vec4(color, 1.0);"
"}";

int window_width = 480;
int window_height = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    window_width = width;
    window_height = height;
    glViewport(0, 0, window_width, window_height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{
    GLFWwindow* pWindow;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    pWindow = glfwCreateWindow(window_width, window_height, "BattleCity", nullptr, nullptr);
    if (!pWindow)
    {
        std::cout << "\nError pWindow!\n";
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        std::cout << "\nCannot load GLAD\n";
        return -1;
    }

    std::cout << "\nGLAD is loading!\n";
    std::cout << "\nRender: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\nOpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 1, 0, 1);

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    /* CREATE OUR SHADERS */

    GLuint vertsh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertsh, 1, &vertex_shader, nullptr);
    glCompileShader(vertsh);

    GLuint fragsh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragsh, 1, &fragment_shader, nullptr);
    glCompileShader(fragsh);

    /* LINKING OUR BOTH SHADERS */

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertsh);
    glAttachShader(shader_program, fragsh);
    glLinkProgram(shader_program);

    /* DELETE SHADERS */

    glDeleteShader(vertsh);
    glDeleteShader(fragsh);

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
    GLuint colors_vbo = 1; // - id
    glGenBuffers(1, &colors_vbo); // - generating
    /* Binding the generated array */
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    /* Content our buffer for current (colors_vbo)*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    /* THAT WHAT HAVE TO DO VIDEO-CARD
       WITH SHADERS INFO IN ITSELF MEMORY
       (VAO) */

    /* Generating id */
    GLuint vao = 2; // - id
    glGenVertexArrays(1, &vao); // - generating
    /* Binding the generated array */
    glBindVertexArray(vao);

    /* Swith on position points and colors */

    glEnableVertexAttribArray(0);
    glBindBuffer(1, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(1, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /*  SHADERS DRAWING  */

        /* Swith on the shader program what we want */
        glUseProgram(shader_program);
        /* Connect our vertex array object */
        glBindVertexArray(vao);
        /* The command what draw a current binded vao */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    std::cout << "\nPress any ...\n";
    std::cin.get();

    return 0;
}