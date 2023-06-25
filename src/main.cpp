#include <glad/glad.h>
#include <GLFW/glfw3.h> // GLFW gives us the bare necessities required for rendering goodies to the screen. It allows us to create an OpenGL context, define window parameters, and handle user input, which is plenty enough for our purposes.
#include "shader.h"
#include "raytracing.h"
#include "Scene.h" 
#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);

float vertices[] = {
    // (x, y, z) 
     0.5f,  0.5f, 0.0f,  // top right 
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
// float vertices[] = {
//      0.5f,  0.5f, 0.0f, //  1.0f, 0.0f, 0.0f, // top right
//      0.5f, -0.5f, 0.0f, //  0.0f, 1.0f, 0.0f, // bottom right
//     -0.5f, -0.5f, 0.0f, //  0.0f, 0.0f, 1.0f, // bottom left
//     -0.5f,  0.5f, 0.0f, //  0.0f, 0.0f, 0.0f// top left 
// };

float colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,    
    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f
};
// float vertices_w[] = {
//     0.5f,  0.5f, 0.0f,// 750.0f,  250.0f, 0.0f, // top right
//     0.5f, -0.5f, 0.0f,// 750.0f, 750.0f, 0.0f, // bottom right
//     -0.5f, -0.5f, 0.0f,// 250.0f, 750.0f, 0.0f,  // bottom left
//     -0.5f,  0.5f, 0.0f // 250.0f,  250.0f, 0.0f, // top left 
// };



// unsigned int indices[] = {  // note that we start from 0!
//     0, 1, 3,   // first triangle
//     1, 2, 3    // second triangle
// };  


int main() {
    // std::cout << v << std::endl;
    std::cout << "hello" << std::endl;
    static Scene scene;
    ray_trace(scene);
    // for (int i=0; i<300; ++i) {
    //     for (int j=0; j<300; ++j) {
    //         std::cout << scene.vertices[i][j][0] << ' ' << scene.vertices[i][j][1] << ' ' << scene.vertices[i][j][2] << "   ";
    //     }
    //     std::cout << std::endl;
    // }
    std::cout << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(g_length, g_length, "this is NOT a window!", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
 
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    // glad: load all openGL funtion pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // glViewport(0, 0, 1000, 1000);

    // build and compile and create shader program object, and link the shaders and delete them once linked to the shaderProgram object.
    Shader shader("..\\shader\\vertex_shader", "..\\shader\\fragment_shader");
    // generate VAO(vertex array object first).
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  // n개의 size인 Vertex Array Object를 만든다(Array의 형태로 만듦). 그리고 그 array의 첫번째 주소를 VAO에 set 
    // bind vertex array object first.
    glBindVertexArray(VAO); // VAO array의 첫번째 VAO를 바인딩 해서, 앞으로 glBindBuffer, glVertexAttribPointer, glEnableVertexAttribArray 함수들 호출하면 VAO에 저장이 된다..

    // 0. create VBO for OpenGL to use. copy vertices in it.
    unsigned int VBO[2];
    glGenBuffers(2, VBO); // crate buffer object, 
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // from now on, glVertexAttribPointer() will be bound to this Array_buffer
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_w), vertices_w, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(scene.vertices), scene.vertices, GL_STATIC_DRAW);

    // 1. then set the vertex attributes pointers(tell OpenGL how to link the vertex data to the vertex shader's verter attributes)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(scene.colors), scene.colors, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);  // enable the vertex attribute. 0 is the attr number.
    glEnableVertexAttribArray(1);  // enable the vertex attribute. 0 is the attr number.

    // create EBO
    // unsigned int EBO;
    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        
        process_input(window);        
        glfwPollEvents(); // checks if keybord/mouse/etc events are triggered and calls the corresponding functions
 
        // rendering commands here

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function
        glClear(GL_COLOR_BUFFER_BIT); // state using function. should be loacated before actual rendering 

        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue) / 2) + 0.5;

        shader.use();
        // shader.setFloat("ourColor", greenValue);

        // glUseProgram(shaderProgram);
        // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // takes vertices from VBO and it's indices are taken from EBO. 6: take 6 indices from EBO
        glDrawArrays(GL_POINTS, 0, g_length*g_length);
 
        glfwSwapBuffers(window); // double buffer
    }
 
     // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, VBO);
    // glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}