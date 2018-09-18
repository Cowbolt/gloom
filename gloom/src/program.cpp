// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

// Globals
// Transformation variables
glm::vec3 transVec = glm::vec3(0.0);
glm::vec3 rotVec = glm::vec3(0.0);
float rotX = 0.f;
float rotY = 0.f;

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

    float vertices [3*3*5] = {-0.6, -0.6, -10.f, // 0
                               0.6, -0.6, -10.f,
                               0.f,  0.6, -10.f,};
                              // 0.f,  0.0, -1.4};
/*
                               0.9, -0.9,  0.2, // 3
                               0.9,  0.9,  0.2,
                              -0.6,  0.f,  0.2,

                               0.9,  0.9,  0.1, // 6
                              -0.9,  0.9,  0.1,
                               0.f, -0.6,  0.1,

                              -0.9, -0.9,  0.0, // 9
                               0.9, -0.9,  0.0,
                               0.f,  0.6,  0.0};
*/

    float colors [4*3*5]   = {
                               1.f,  0.f,  1.f,  1.f, // 0
                               //1.f,  1.f,  1.f,  1.f,
                               1.f,  1.f,  0.f,  1.f,
                               0.f,  1.f,  1.f,  1.f};
/*
                               1.f,  0.f,  0.f,  0.5, // 3
                               1.f,  0.f,  0.f,  0.5,
                               1.f,  0.f,  0.f,  0.5,

                               0.f,  0.f,  1.f,  0.5, // 6
                               0.f,  0.f,  1.f,  0.5,
                               0.f,  0.f,  1.f,  0.5,

                               1.f,  1.f,  0.f,  0.5, // 9
                               1.f,  1.f,  0.f,  0.5,
                               1.f,  1.f,  0.f,  0.5};
*/


    int indices [3*5]    = { 0,  1,  2};
                             //2,  3,  0,
                             //3,  1,  0, 
                             //1,  3,  2};

    int num_triangles = 5;
    int num_verts     = num_triangles*3;
    int num_coords    = num_verts*3;
    int num_colors    = num_verts*4;

    int vao_id        = genVAO(vertices, colors, indices, num_coords, num_colors, num_verts);

    glm::mat4 matrix;


    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        shader.activate();

        // Perform transformations
        // matrix = glm::mat4(1);
        matrix = glm::perspective(90.f, 1.f, 1.f, 100.f);
        matrix = matrix*glm::translate(transVec);
        matrix = matrix*glm::rotate(rotX, glm::vec3(1.f,0.f,0.f));
        matrix = matrix*glm::rotate(rotY, glm::vec3(0.f,1.f,0.f));


        glUniformMatrix4fv(2, 1, false, (float*)(&matrix));
        glDrawElements(GL_TRIANGLES, num_verts, GL_UNSIGNED_INT, 0);
        shader.deactivate();

        // counter += 0.01;
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

    // Translations
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        transVec[2] -= 0.3;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        transVec[2] += 0.3;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        transVec[0] += 0.1;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        transVec[0] -= 0.1;
    }

    //Rotations
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rotX -= 0.01;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rotX += 0.01;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        rotY -= 0.01;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        rotY += 0.01;
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
   glVertexAttribPointer(0, size_verts, GL_FLOAT, false, size_verts*4, 0);
   glEnableVertexAttribArray(0);

   // Setup color VBO
   unsigned int vbo_color_id = 0;
   glGenBuffers(1, &vbo_color_id);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_color_id);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_colors, colors, GL_STATIC_DRAW);

   // Takes index, size(num elems), enum type, bool normalised, stride, num bytes until start
   glVertexAttribPointer(1, size_colors, GL_FLOAT, false, size_colors*4, 0);
   glEnableVertexAttribArray(1);

   // Setup index VBO
   unsigned int vbo_index_id = 0;
   glGenBuffers(1, &vbo_index_id);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index_id);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_indices, indices, GL_STATIC_DRAW);

   return vao_id;
}
