#include <queue>
#include <iostream>
using namespace std;

class Coord;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	maze[sr][sc] = 'o';

	while (!coordQueue.empty())
	{
		Coord curr = coordQueue.front();
		coordQueue.pop();
		int r = curr.r();
		int c = curr.c();

		if (r == er && c == ec)
			return true;
		else
		{
			if (maze[r - 1][c] == '.')
			{
				coordQueue.push(Coord(r - 1, c));
				maze[r - 1][c] = 'o';
			}

			if (maze[r][c + 1] == '.')
			{
				coordQueue.push(Coord(r, c + 1));
				maze[r][c + 1] = 'o';
			}

			if (maze[r + 1][c] == '.')
			{
				coordQueue.push(Coord(r + 1, c));
				maze[r + 1][c] = 'o';
			}

			if (maze[r][c - 1] == '.')
			{
				coordQueue.push(Coord(r, c - 1));
				maze[r][c - 1] = 'o';
			}
		}
	}
	return false;
}