#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <climits>
#include <utility>
#include <unordered_set>

#define number int

using namespace std;

struct coord
{
  int x;
  int y;
};

bool inBounds(int x, int y, int rows, int columns)
{
  return (y >= 0 && y < rows && x >= 0 && x < columns);
}

int dijkstra(coord start, coord end, int minStep, int maxStep, vector<vector<int>> &grid)
{
  vector<pair<int, int>> dirs{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> minheap;
  minheap.push({0, start.x, start.y, 0, 0}); // {dist, i, j, dirX, dirY}
  vector<pair<coord, coord>> seen;
  while (!minheap.empty())
  {
    auto curr = minheap.top();
    minheap.pop();

    int heat = curr[0];
    int x = curr[1];
    int y = curr[2];
    int px = curr[3];
    int py = curr[4];
    if (x == end.x && y == end.y)
    {
      return heat;
    }
    bool alreadySeen = false;
    for (pair<coord, coord> p : seen)
    {
      if (p.first.x == x && p.first.y == y && p.second.x == px && p.second.y == py)
      {
        alreadySeen = true;
        break;
      }
    }
    if (!alreadySeen)
    {
      seen.push_back({{x, y}, {px, py}});
      for (pair<int, int> d : dirs)
      {
        if (!((d.first == px && d.second == py) || (d.first == -px && d.second == -py)))
        {
          int newX = x;
          int newY = y;
          int newHeat = heat;
          for (int i = 1; i <= maxStep; i++)
          {
            newX += d.first;
            newY += d.second;
            if (inBounds(newX, newY, grid.size(), grid[0].size()))
            {
              newHeat += grid[newY][newX];
              if (i >= minStep)
              {
                minheap.push({newHeat, newX, newY, d.first, d.second});
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  vector<vector<int>> map;

  while (file.good())
  {
    getline(file, line);
    // printf("%s\n", line.c_str());

    vector<int> lineNumbers;
    for (char c : line)
    {
      lineNumbers.push_back(c - '0');
    }
    map.push_back(lineNumbers);
  }

  int mapWidth = map[0].size();
  int mapHeight = map.size();

  int res = dijkstra({0, 0}, {mapWidth - 1, mapHeight - 1}, 4, 10, map);
  printf("%i\n", res);
}