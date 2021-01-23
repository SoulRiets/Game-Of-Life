#pragma once
#include <GLFW/glfw3.h>

class GameBoard
{
public:
	GameBoard(int width, int size);

	

	float gridColor{};
	float deadCellColor{};
	float aliveCellColor{};

	bool isIniting{};
	bool isDeleting{};
	bool isGriding{};
	bool isDarkTheme{};

	void BoardDraw();
	void BoardReset();
	void CalculateNextFrame();
	void DrawGrid();
	void GridSwitch();
	void ThemeSwitch();
	void SetupLightTheme();
	void SetupDarkTheme();
	void DecreaseFrameSpeed();
	void IncreaseFrameSpeed();
	void IncreaseSize();
	void DecreaseSize();
	void MouseClickPosition(int x, int y, bool mode);

private:

	bool** board;
	bool** nBoard;

	int boardSize{};  //равнозначно количеству колон и рядов
	int minSize{};
	int maxSize{};

	int screenWidth{};
	int screenHeight{};

	void DeleteBoards();
	int CellsAround(int rw, int cl);
	bool GetCellState(int rw, int cl);
	void SetCell(int rwInd, int clInd, bool val);
	void BoardInit(int sz);
};