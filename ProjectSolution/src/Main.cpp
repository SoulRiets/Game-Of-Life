#include <GLFW\glfw3.h>

constexpr int widthLimit{ 1000 };

double frameSpeed       { 0.1 };

float gridColor     { 0 };
float deadCellColor { 0 };
float aliveCellColor{ 0 };

int boardSize{ 50 };  //равнозначно количеству колон и рядов
int minSize{ 2 };
int maxSize {100 };

int ScreenWidth{ 850 };
int ScreenHeight{ 850 };

double xPos{ 0 };
double yPos{ 0 };

bool** board;
bool** nBoard;

bool isIniting{ false };
bool isDeleting{ false };
bool isPaused{ true };
bool isDarkTheme{ false };
bool isGriding{ true };
bool leftClick{ false };
bool rightClick{ false };

void SetupLightTheme();
void SetupDarkTheme();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void BoardInit(int sz);
void BoardDraw();
void BoardReset();
void SetCell(int rwInd, int clInd, bool val);
void CalculateNextFrame();
int CellsAround(int rw, int cl);
bool GetCellState(int rw, int cl);
void DrawGrid();
void MouseClickPosition(int x, int y, bool mode);
void DecreaseFrameSpeed();
void IncreaseFrameSpeed();
void IncreaseSize();
void DecreaseSize();
void DeleteBoards();


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
    
    BoardInit(boardSize);
    
    SetCell(10, 8, true);
    SetCell(10, 9, true);
    SetCell(10, 10, true);
    SetCell(11, 10, true);
    SetCell(12, 9, true);

    SetupLightTheme();


    while (!glfwWindowShouldClose(window))
    {
        glfwGetCursorPos(window, &xPos, &yPos);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(deadCellColor, deadCellColor, deadCellColor, 1.0f); //цвет мертвых клеток

        if (!isDeleting && !isIniting)
        {
            BoardDraw();
        }
        if (isGriding) DrawGrid();
        if (leftClick && !isDeleting)
        {
            MouseClickPosition(xPos, yPos, true);
        }
        if (rightClick && !isDeleting)
        {
            MouseClickPosition(xPos, yPos, false);
        }
        

        if (glfwGetTime() > frameSpeed && !isPaused)
        {
            CalculateNextFrame();
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
        IncreaseFrameSpeed();
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    {
        DecreaseFrameSpeed();
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS &&
        !isDeleting && !isIniting)
    {
        IncreaseSize();
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS &&
        !isDeleting && !isIniting)
    {
        DecreaseSize();
    }


    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        if (isDarkTheme)
        {
            isDarkTheme = false;
            SetupLightTheme();
        }
        else
        {
            isDarkTheme = true;
            SetupDarkTheme();
        }
    }

    if (key == GLFW_KEY_G && action == GLFW_PRESS)
        isGriding = !isGriding;
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        BoardReset();
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

void SetupLightTheme() // setup for light theme
{
    gridColor = 0.12f;
    deadCellColor = 0.89f;
    aliveCellColor = 0.12f;
}

void SetupDarkTheme() // setup for light theme
{
    gridColor = 0.88f;
    deadCellColor = 0.11f;
    aliveCellColor = 0.88f;
}

void BoardInit(int sz)
{


    board = new bool*[boardSize];
    for (int row{ 0 }; row < boardSize; row++)
        board[row] = new bool[boardSize];
    nBoard = new bool* [boardSize];
    for (int row{ 0 }; row < boardSize; row++)
        nBoard[row] = new bool[boardSize];


    BoardReset();
}

void SetCell(int rwInd, int clInd, bool val)
{
    if (rwInd < boardSize && rwInd >= 0 && clInd < boardSize && clInd >= 0)
        board[rwInd][clInd] = val;
}

void BoardDraw()
{
    glLoadIdentity();
    glOrtho(0, boardSize, 0, boardSize, 0, 1);
    glColor3f(aliveCellColor, aliveCellColor, aliveCellColor);
    glBegin(GL_QUADS);
    for (int rw{ 0 }; rw < boardSize; ++rw)
    {
        for (int cl{ 0 }; cl < boardSize; ++cl)
        {
            if (board[rw][cl])
            {
                glVertex2f(cl, rw);
                glVertex2f(cl + 1, rw);
                glVertex2f(cl + 1, rw + 1);
                glVertex2f(cl, rw + 1);
            }
        }
    }
    glEnd();
}

void BoardReset()
{
        
        for (int rw{ 0 }; rw < boardSize; ++rw)
        {
            for (int cl{ 0 }; cl < boardSize; ++cl)
            {
                board[rw][cl] = false;
                nBoard[rw][cl] = false;
            }
        }
    
}

void DrawGrid(){
    //Initialize openGl for drawing lines
    glLoadIdentity();
    glOrtho(0, boardSize, 0, boardSize, 0, 1);
    glLineWidth(0.1); //Set the width of the line to 1
    glColor3f(gridColor, gridColor, gridColor); //Set the color of the line to grey
    glBegin(GL_LINES);

    //Draw the vertical lines
    for (int cl = 0; cl <= boardSize; cl++)
    {
        float xPos = static_cast<float>((cl / boardSize) * 2 - 1);
        glVertex2f(cl, 0);
        glVertex2f(cl, ScreenHeight);
    }

    //Draw the horizontal lines
    for (int rw = 0; rw <= boardSize; rw++)
    {
        float yPos = static_cast<float>((rw / boardSize) * 2 - 1);
        glVertex2f(0, rw);
        glVertex2f(ScreenWidth, rw);
    }
    glEnd();
}

void CalculateNextFrame()
{

    for (int rw{ 0 }; rw < boardSize; ++rw)
    {
        for (int cl{ 0 }; cl < boardSize; ++cl)
        {
            int cellsAround{ CellsAround(rw, cl) };

            if (board[rw][cl])
            {
                if (cellsAround < 2 || cellsAround > 3)
                    nBoard[rw][cl] = false;
                else
                    nBoard[rw][cl] = true;
            }
            if (!board[rw][cl])
            {
                if (cellsAround == 3)
                    nBoard[rw][cl] = true;
                else
                    nBoard[rw][cl] = false;
            }
        }
    }

    bool** tmp{ board };
    board = nBoard;
    nBoard = tmp;
    glViewport(0, 0, ScreenWidth, ScreenHeight);
}

int CellsAround(int rw, int cl)
{
    int cellsAlive{ 0 };
    
    
    // проверяем верхние клетки
    for (int i{ -1 }; i < 2; ++i)
    {
        if (GetCellState(rw + 1, cl + i))
            ++cellsAlive;
    }
    // проверяем нижние клетки
    for (int i{ -1 }; i < 2; ++i)
    {
        if (GetCellState(rw - 1, cl + i))
            ++cellsAlive;
    }
    // проверяем боковые клетки
    if (GetCellState(rw, cl - 1))
        ++cellsAlive;
    if (GetCellState(rw, cl + 1))
        ++cellsAlive;
    return cellsAlive;
}

bool GetCellState(int rw, int cl)
{
    if (rw < 0)
        rw = boardSize - 1;
    else if (rw > boardSize - 1)
        rw = 0;
    if (cl < 0)
        cl = boardSize - 1;
    else if (cl > boardSize - 1)
        cl = 0;

    return board[rw][cl];
}

void MouseClickPosition(int x, int y, bool mode)
{   //x и y позиции мышки
    int xBoardPos{ 0 };
    int yBoardPos{ 0 };
    xBoardPos =  x / (ScreenWidth / boardSize);
    yBoardPos = boardSize - (y / (ScreenHeight / boardSize)) - 1;
    SetCell(yBoardPos, xBoardPos, mode);
}

void IncreaseFrameSpeed()
{
    if (frameSpeed > 0.01 || frameSpeed < 2.0)
        frameSpeed += 0.01;
}
void DecreaseFrameSpeed()
{
    if (frameSpeed > 0.01 || frameSpeed < 2.0)
        frameSpeed -= 0.01;
}

void IncreaseSize()
{
    if (boardSize > (minSize - 1) && boardSize < maxSize)
    {
        DeleteBoards();
        ++boardSize;
        isIniting = true;
        BoardInit(boardSize);
        isIniting = false;
    }
}

void DecreaseSize()
{
    if (boardSize > minSize && boardSize < (maxSize + 1))
    {
        DeleteBoards();
        --boardSize;
        isIniting = true;
        BoardInit(boardSize);
        isIniting = false;
    }
}

void DeleteBoards()
{
    isDeleting = true;
    for (int count{ 0 }; count < boardSize; ++count)
        delete[] board[count];
    delete[] board;
    for (int count{0}; count < boardSize; ++count)
        delete[] nBoard[count];
    delete[] nBoard;
    isDeleting = false;
}
