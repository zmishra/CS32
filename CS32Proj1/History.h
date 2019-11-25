#ifndef HISTORY_H_INCLUDED
#define HISTORY_H_INCLUDED

#include "globals.h"

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_grid[MAXROWS][MAXCOLS];
	int m_rows;
	int m_cols;
};
#endif