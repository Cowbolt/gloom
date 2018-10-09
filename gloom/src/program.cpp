// Local headers
#include "program.hpp"
#include "OBJLoader.hpp"
#include "toolbox.hpp"
#include "sceneGraph.hpp"
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


    // MinecraftCharacter steve = loadMinecraftCharacterModel("../gloom/res/steve.obj");
    Mesh chessboard = generateChessboard(7, 5, 20, float4(0.0,0.0,0.0,1.0), float4(1.0,1.0,1.0,1.0));

    // No more DRY meat
    int vao_leftLeg = genVAOFromMesh(steve.leftLeg); // Need int/uint consistency
    SceneNode* leftLegNode = createSceneNode();
    leftLegNode->vertexArrayObjectID = vao_leftLeg;


    int vao_rightLeg = genVAOFromMesh(steve.rightLeg);
    SceneNode* rightLegNode = createSceneNode();
    rightLegNode->vertexArrayObjectID = vao_rightLeg;


    int vao_leftArm = genVAOFromMesh(steve.leftArm);
    SceneNode* leftArmNode = createSceneNode();
    leftArmNode->vertexArrayObjectID = vao_leftArm;


    int vao_rightArm = genVAOFromMesh(steve.rightArm);
    SceneNode* rightArmNode = createSceneNode();
    rightArmNode->vertexArrayObjectID = vao_rightArm;

    int vao_head = genVAOFromMesh(steve.head);
    SceneNode* headNode = createSceneNode();
    headNode->vertexArrayObjectID = vao_head;

    int vao_torso = genVAOFromMesh(steve.torso);
    SceneNode* torsoNode = createSceneNode();
    torsoNode->vertexArrayObjectID = vao_torso;
    addChild(torsoNode, leftLegNode);
    addChild(torsoNode, rightLegNode);
    addChild(torsoNode, leftArmNode);
    addChild(torsoNode, rightArmNode);
    addChild(torsoNode, headNode);


    int vao_chessboard = genVAOFromMesh(chessboard);
    SceneNode* chessboardNode = createSceneNode();
    chessboardNode->vertexArrayObjectID = vao_chessboard;

    SceneNode* rootNode = createSceneNode();
    addChild(rootNode, torsoNode);
    addChild(rootNode, chessboardNode);


    glm::mat4 matrix;

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        shader.activate();

        // Perform transformations
        // matrix = glm::perspective(90.00f, 1.0f, 1.0f, 1000.0f) * glm::translate(glm::vec3(-30.0, -30.0, -100.0)) * glm::rotate(0.2f, glm::vec3(1.0, 0.0, 0.0));
        matrix = glm::perspective(90.f, 1.f, 1.f, 1000.f);
        matrix = matrix*glm::translate(transVec);
        matrix = matrix*glm::rotate(rotX, glm::vec3(0.f,1.f,0.f));
        matrix = matrix*glm::rotate(rotY, glm::vec3(1.f,0.f,0.f));

        glUniformMatrix4fv(2, 1, false, (float*)(&matrix));

        glBindVertexArray(vao_chessboard);
        glDrawElements(GL_TRIANGLES, chessboard.faceCount(), GL_UNSIGNED_INT, 0);
        
        // glBindVertexArray(vao_leftLeg);
        // glDrawElements(GL_TRIANGLES, steve.leftLeg.faceCount(), GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vao_rightLeg);
        // glDrawElements(GL_TRIANGLES, steve.rightLeg.faceCount(), GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vao_leftArm);
        // glDrawElements(GL_TRIANGLES, steve.leftArm.faceCount(), GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vao_rightArm);
        // glDrawElements(GL_TRIANGLES, steve.rightArm.faceCount(), GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vao_torso);
        // glDrawElements(GL_TRIANGLES, steve.torso.faceCount(), GL_UNSIGNED_INT, 0);

        // glBindVertexArray(vao_head);
        // glDrawElements(GL_TRIANGLES, steve.head.faceCount(), GL_UNSIGNED_INT, 0);

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
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Translations
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transVec[2] += 0.3;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transVec[2] -= 0.3;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transVec[0] += 0.1;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transVec[0] -= 0.1;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        transVec[1] += 0.1;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        transVec[1] -= 0.1;

    //Rotations
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotX -= 0.01;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotX += 0.01;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        rotY -= 0.01;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        rotY += 0.01;
}

int genVAO(float4* vertices, float4* colors, unsigned int* indices,  int num_vertices, int num_colors, int num_indices)
{

   // No. of elems of each (to calculate stride, offset)
   int size_verts = 4;
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

int genVAOFromMesh(Mesh mesh)
{
    return genVAO(mesh.vertices.data(), mesh.colors.data(), mesh.indices.data(), mesh.vertices.size(), mesh.colors.size(), mesh.indices.size());
}
