#include <GameBoard.h>

extern double frameSpeed;

void GameBoard::BoardInit(int sz)
{


    board = new bool* [boardSize];
    for (int row{ 0 }; row < boardSize; row++)
        board[row] = new bool[boardSize];
    nBoard = new bool* [boardSize];
    for (int row{ 0 }; row < boardSize; row++)
        nBoard[row] = new bool[boardSize];


    BoardReset();
}

GameBoard::GameBoard(int width, int size)
{
    boardSize = size;
    minSize = 2;
    maxSize = 100;

    screenWidth = width;
    screenHeight = width;

    isDarkTheme = false;
    isGriding = true;
    isIniting = false;
    isDeleting = false;

    BoardInit(boardSize);
}

void GameBoard::MouseClickPosition(int x, int y, bool mode)
{   //x и y позиции мышки
    int xBoardPos{ 0 };
    int yBoardPos{ 0 };
    xBoardPos = x / (screenWidth / boardSize);
    yBoardPos = boardSize - (y / (screenHeight / boardSize)) - 1;
    SetCell(yBoardPos, xBoardPos, mode);
}

int GameBoard::CellsAround(int rw, int cl)
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

bool GameBoard::GetCellState(int rw, int cl)
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

void GameBoard::SetCell(int rwInd, int clInd, bool val)
{
    if (rwInd < boardSize && rwInd >= 0 && clInd < boardSize && clInd >= 0)
        board[rwInd][clInd] = val;
}

void GameBoard::CalculateNextFrame()
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
    glViewport(0, 0, screenWidth, screenHeight);
}

void GameBoard::BoardReset()
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

void GameBoard::BoardDraw()
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

void GameBoard::DrawGrid()
{
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
        glVertex2f(cl, screenHeight);
    }

    //Draw the horizontal lines
    for (int rw = 0; rw <= boardSize; rw++)
    {
        float yPos = static_cast<float>((rw / boardSize) * 2 - 1);
        glVertex2f(0, rw);
        glVertex2f(screenWidth, rw);
    }
    glEnd();
}

void GameBoard::GridSwitch()
{
    isGriding = !isGriding;
}

void GameBoard::ThemeSwitch()
{
     isDarkTheme = !isDarkTheme;
     
}

void GameBoard::SetupLightTheme() // setup for light theme
{
    isDarkTheme = false;
    gridColor = 0.12f;
    deadCellColor = 0.89f;
    aliveCellColor = 0.12f;
}

void GameBoard::SetupDarkTheme() // setup for light theme
{
    isDarkTheme = true;
    gridColor = 0.88f;
    deadCellColor = 0.11f;
    aliveCellColor = 0.88f;
}

void GameBoard::IncreaseFrameSpeed()
{
    if (frameSpeed > 0.01 || frameSpeed < 2.0)
        frameSpeed += 0.01;
}

void GameBoard::DecreaseFrameSpeed()
{
    if (frameSpeed > 0.01 || frameSpeed < 2.0)
        frameSpeed -= 0.01;
}

void GameBoard::IncreaseSize()
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

void GameBoard::DecreaseSize()
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

void GameBoard::DeleteBoards()
{
    isDeleting = true;
    for (int count{ 0 }; count < boardSize; ++count)
        delete[] board[count];
    delete[] board;
    for (int count{ 0 }; count < boardSize; ++count)
        delete[] nBoard[count];
    delete[] nBoard;
    isDeleting = false;
}