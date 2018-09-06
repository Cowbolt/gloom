// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"


void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)
    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert",
      "../shaders/simple.frag");
    // float vertices [9*5] = {-0.9, -0.9, 0.f, -0.3, -0.9, 0.f, -0.6, -0.3, 0.f,
    //                       -0.6, -0.6, 0.f, -0.f, -0.6, 0.f, 0.f, 0.f, 0.f};
    //  int indices [9*5]    = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 };

    float vertices [9] = { 0.6, -0.8, -1.2, 0.f, 0.4, 0.f, -0.8, -0.2, 1.2 };
    int indices [9]    = { 0,1,2,3,4,5,6,7,8};
    int num_elems      = 9;
    int vao_id         = genTriangleVAO(vertices, indices, num_elems, num_elems);


    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        glBindVertexArray(vao_id);
        shader.activate();
        glDrawElements(GL_TRIANGLES, num_elems/3, GL_UNSIGNED_INT, 0);
        shader.deactivate();

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
    shader.destroy();
}


void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int genTriangleVAO(float* vertices, int* indices, int num_vertices, int num_indices)
{
   // Generate and bind VAO
   unsigned int vao_id = 0;
   glGenVertexArrays(1, &vao_id);
   glBindVertexArray(vao_id);

   // Setup vertex VBO
   unsigned int vbo_vertex_id = 0;
   glGenBuffers(1, &vbo_vertex_id);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex_id);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);

   // Takes index, size(num elems), enum type, bool normalised, stride, num bytes until start
   glVertexAttribPointer(0, 3, GL_FLOAT, false, 12, 0);
   glEnableVertexAttribArray(0);

   // Setup index VBO
   unsigned int vbo_index_id = 0;
   glGenBuffers(1, &vbo_index_id);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index_id);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, indices, GL_STATIC_DRAW);

   return vao_id;
}
