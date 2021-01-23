#include <GLFW\glfw3.h>
#include <GameBoard.h> 

double frameSpeed{ 0.1 };

constexpr int widthLimit{ 1000 };
int ScreenWidth{ 850 };
int ScreenHeight{ 850 };

double xPos{ 0 };
double yPos{ 0 };

bool isPaused{ true };

bool leftClick{ false };
bool rightClick{ false };


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GameBoard board(widthLimit, 50);

int main()
{
    GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "Game of Life", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowSizeLimits(window, GLFW_DONT_CARE, GLFW_DONT_CARE, widthLimit, widthLimit);
    glViewport(0, 0, ScreenWidth, ScreenHeight);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetWindowAspectRatio(window, 1, 1);
  
    

    board.SetupLightTheme();


    while (!glfwWindowShouldClose(window))
    {
        glfwGetCursorPos(window, &xPos, &yPos);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(board.deadCellColor, board.deadCellColor, board.deadCellColor, 1.0f); //цвет мертвых клеток

        if (!board.isDeleting && !board.isIniting)
        {
            board.BoardDraw();
        }
        if (board.isGriding) board.DrawGrid();
        if (leftClick && !board.isDeleting)
        {
            board.MouseClickPosition(xPos, yPos, true);
        }
        if (rightClick && !board.isDeleting)
        {
            board.MouseClickPosition(xPos, yPos, false);
        }
        

        if (glfwGetTime() > frameSpeed && !isPaused)
        {
            board.CalculateNextFrame();
            glfwSetTime(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    

    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (GLFW_PRESS == action)
            leftClick = true;
        else if (GLFW_RELEASE == action)
            leftClick = false;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (GLFW_PRESS == action)
            rightClick = true;
        else if (GLFW_RELEASE == action)
            rightClick = false;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    {
        board.IncreaseFrameSpeed();
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        board.DecreaseFrameSpeed();
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS &&
        !board.isDeleting && !board.isIniting)
    {
        board.IncreaseSize();
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS &&
        !board.isDeleting && !board.isIniting)
    {
        board.DecreaseSize();
    }


    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        if (board.isDarkTheme)
        {
            board.SetupLightTheme();
        }
        else
        {
            board.SetupDarkTheme();
        }
    }

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
        board.GridSwitch();
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        board.BoardReset();
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        isPaused = !isPaused;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int nWidth, int nHeight)
{
    glfwGetFramebufferSize(window, &nWidth, &nHeight);
    if (nWidth > nHeight)
    {
        ScreenWidth = nWidth;
        ScreenHeight = nWidth;
    }
    else
    {
        ScreenHeight = nHeight;
        ScreenWidth = nHeight;
    }
    
    glViewport(0, 0, ScreenWidth, ScreenHeight);
}
