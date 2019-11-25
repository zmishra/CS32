#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			m_grid[r-1][c-1] = 0;
}

bool History::record(int r, int c)
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
		return false;
	else
	{
		m_grid[r - 1][c - 1]++;
		return true;
	}
}

void History::display() const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

	for (r = 1; r <= m_rows; r++)
		for (c = 1; c <= m_cols; c++)
		{
			if (m_grid[r - 1][c - 1] == 0)
				displayGrid[r - 1][c - 1] = '.';
			else if (m_grid[r - 1][c - 1] >= 26)
				displayGrid[r - 1][c - 1] = 'Z';
			else
				displayGrid[r - 1][c - 1] = ('A' + m_grid[r - 1][c - 1] - 1);
		}
	
	clearScreen();
	for (r = 1; r <= m_rows; r++)
	{
		for (c = 1; c <= m_cols; c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;
}