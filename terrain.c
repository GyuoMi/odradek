#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <noise/noise.h>

#define WIDTH 100
#define HEIGHT 100
#define SCALE 0.1
#define HILL_HEIGHT 5.0

float terrain[WIDTH][HEIGHT];

float cameraPosX = WIDTH / 2;
float cameraPosY = SCALE * 10;
float cameraPosZ = HEIGHT / 2;

float cameraYaw = 0.0f;
float cameraPitch = 0.0f;

float cameraSpeed = 0.005f;
float cameraRotationSpeed = 0.005f;
float baseSpeed = 1.0f;

void generateTerrain() {
    noise::module::Perlin perlin;
    perlin.SetOctaveCount(6);

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            double x = i * 0.1;
            double y = j * 0.1;
            double value = perlin.GetValue(x, y, 0.0) * SCALE;
            value += HILL_HEIGHT * perlin.GetValue(x * 0.5, y * 0.5, 0.0);
            terrain[i][j] = static_cast<float>(value);
        }
    }
}

void drawTerrain() {
    for (int i = 0; i < WIDTH - 1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < HEIGHT; j++) {
            // Adjust color based on height for grassy shades
            float color = terrain[i][j] / HILL_HEIGHT;
            glColor3f(0.0, 0.5 + color * 0.5, 0.0); // Shades of green
            glVertex3f(i, terrain[i][j], j);
            glColor3f(0.0, 0.5 + color * 0.5, 0.0);
            glVertex3f(i + 1, terrain[i + 1][j], j);
        }
        glEnd();
    }
}

void updateCamera() {
    glLoadIdentity();
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ,
              cameraPosX + cos(cameraYaw) * cos(cameraPitch),
              cameraPosY + sin(cameraPitch),
              cameraPosZ + sin(cameraYaw) * cos(cameraPitch),
              0.0f, 1.0f, 0.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    generateTerrain();
    drawTerrain();

    glFlush();
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosX += baseSpeed * cos(cameraYaw);
        cameraPosZ += baseSpeed * sin(cameraYaw);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosX -= baseSpeed * cos(cameraYaw);
        cameraPosZ -= baseSpeed * sin(cameraYaw);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosX += baseSpeed * sin(cameraYaw);
        cameraPosZ -= baseSpeed * cos(cameraYaw);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosX -= baseSpeed * sin(cameraYaw);
        cameraPosZ += baseSpeed * cos(cameraYaw);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPosY += baseSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraPosY -= baseSpeed;
    }

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    glfwSetCursorPos(window, 800 / 2, 600 / 2);

    cameraYaw -= cameraRotationSpeed * (800 / 2 - mouseX);
    cameraPitch += cameraRotationSpeed * (600 / 2 - mouseY);

    if (cameraPitch > 1.5f) {
        cameraPitch = 1.5f;
    }
    if (cameraPitch < -1.5f) {
        cameraPitch = -1.5f;
    }
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Terrain Visualization", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        display();
        updateCamera();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}