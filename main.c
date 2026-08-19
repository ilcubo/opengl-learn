#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "shader.h"
#include "stb_image.h"
#include "texture.h"

float points[] = {
    0.5f,  0.5f,  0.0f, // First point
    0.5f,  -0.5f, 0.0f, // Second point
    -0.5f, -0.5f, 1.0f, // Third point
};

float colours[] = {
    1.0f, 0.0f, 0.0f, // Point #1
    0.0f, 1.0f, 0.0f, // Point #2
    0.0f, 0.0f, 1.0f, // Point #3
};

float texture_coords[] = {
    1.0, 1.0, // Point #1
    1.0, 0.0, // Point #2
    0.0, 0.0, // Point #3
};

void ErrorCallback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
}

void InitializeBuffer(GLuint *vbo, const float *source, int size) {
  glGenBuffers(1, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), source, GL_STATIC_DRAW);
}

void InitializePoint(GLuint *vao, GLuint points_vbo, GLuint colours_vbo, GLuint texcoord_vbo) {
  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);
  glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, texcoord_vbo);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

int main() {
  glfwSetErrorCallback(ErrorCallback);
  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);

  bool full_screen = false;
  GLFWmonitor *monitor = NULL;
  int win_w = 800;
  int win_h = 600;

  if (full_screen) {
    monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    win_w = mode->width;
    win_h = mode->height;
  }

  GLFWwindow *window =
      glfwCreateWindow(win_w, win_h, "OpenGL Experiments", monitor, NULL);
  if (!window) {
    return -1;
  }

  glfwMakeContextCurrent(window);

  int version_glad = gladLoadGL(glfwGetProcAddress);
  if (version_glad == 0) {
    fprintf(stderr, "Error: Failed to initialize OpenGL Context.\n");
  }

  // Loading texture
  Texture texture_0 = load_texture_rgb("wall.jpg", GL_TEXTURE0);
  Texture texture_1 = load_texture_rgba("awesomeface.png", GL_TEXTURE1);

  // Loading shader
  char *vert_path = "shaders/test.vert";
  char *frag_path = "shaders/test.frag";
  GLuint shader_program;

  loadShaderFromFile(&shader_program, vert_path, frag_path);

  // Prepare vertex buffer
  GLuint points_vbo;
  GLuint colours_vbo;
  GLuint texcoord_vbo;
  GLuint vao;

  InitializeBuffer(&points_vbo, points, 9);
  InitializeBuffer(&colours_vbo, colours, 9);
  InitializeBuffer(&texcoord_vbo, texture_coords, 6);
  InitializePoint(&vao, points_vbo, colours_vbo, texcoord_vbo);

  // GLint time_loc = glGetUniformLocation(shader_program, "time");
  // assert(time_loc > -1);

  double prev_s = glfwGetTime();
  double title_countdown_s = 0.1;

  glfwSwapInterval(1);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glUniform1i(glGetUniformLocation(shader_program, "texture_0"), 0);
  glUniform1i(glGetUniformLocation(shader_program, "texture_1"), 1);

  while (!glfwWindowShouldClose(window)) {
    double curr_s = glfwGetTime();
    double elapsed_s = curr_s - prev_s;
    prev_s = curr_s;
    title_countdown_s -= elapsed_s;

    if (title_countdown_s <= 0.0 && elapsed_s > 0.0) {
      char tmp[256];
      double fps = 1.0 / elapsed_s;
      sprintf(tmp, "FPS: %.2lf", fps);
      glfwSetWindowTitle(window, tmp);
      title_countdown_s = 0.1;
    }

    glfwPollEvents();

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }

    glfwGetWindowSize(window, &win_w, &win_h);
    glViewport(0, 0, win_w, win_h);

    glClearColor(0.175, 0.175, 0.175, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader_program);
    // glUniform1f(time_loc, (float)curr_s);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
  }

  glfwPollEvents();
  glfwDestroyWindow(window);
  glfwTerminate();
  free(texture_0);
  free(texture_1);
  return 0;
}
