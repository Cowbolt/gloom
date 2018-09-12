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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set default color after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)
    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert",
      "../gloom/shaders/simple.frag");
    // float vertices [9*5] = {-0.9, -0.9, 0.f, -0.3, -0.9, 0.f, -0.6, -0.3, 0.f,
    //                       -0.6, -0.6, 0.f, -0.f, -0.6, 0.f, 0.f, 0.f, 0.f};
    //  int indices [9*5]    = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 };

    float vertices [9] = { 0.6, -0.8, -1.2, 0.f, 0.4, 0.f, -0.8, -0.2, 1.2 };
    int indices [9]    = { 0,1,2,3,4,5,6,7,8};
    int num_elems      = 9;
    // int vao_id        = genVAO(vertices, indices, num_elems, num_elems);


    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        // glBindVertexArray(vao_id);
        shader.activate();
        // glDrawElements(GL_TRIANGLES, num_elems/3, GL_UNSIGNED_INT, 0);
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

int genVAO(float* vertices, float* colors, int* indices,  int num_vertices, int num_colors, int num_indices)
{

   // No. of elems of each (to calculate stride, offset)
   int size_verts = 3;
   int size_colors = 4;

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
   glVertexAttribPointer(0, size_verts, GL_FLOAT, false, (size_verts+size_colors)*4, 0);
   glEnableVertexAttribArray(0);

   // Setup color VBO
   unsigned int vbo_color_id = 0;
   glGenBuffers(1, &vbo_color_id);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_color_id);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_colors, colors, GL_STATIC_DRAW);

   // Takes index, size(num elems), enum type, bool normalised, stride, num bytes until start
   int offset = size_verts*4; // Reference operator requires a memregion to reference
   glVertexAttribPointer(1, size_colors, GL_FLOAT, false, (size_verts+size_colors)*4, &offset);
   glEnableVertexAttribArray(1);

   // Setup index VBO
   unsigned int vbo_index_id = 0;
   glGenBuffers(1, &vbo_index_id);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index_id);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, indices, GL_STATIC_DRAW);

   return vao_id;
}
