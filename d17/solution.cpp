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

vector<pair<int, int>> DIRECTIONS{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

struct coord
{
  int x;
  int y;
};

bool inBounds(int x, int y, int rows, int columns)
{
  return (y >= 0 && y < rows && x >= 0 && x < columns);
}

string intVectorToString(vector<int> v)
{
  string s;
  for (int i : v)
  {
    s += to_string(i) + ';';
  }
  return s;
}

int dijkstra(coord start, coord end, int minStep, int maxStep, vector<vector<int>> &grid)
{
  priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> minheap;
  minheap.push({0, start.x, start.y, 0, 0}); // {heat, i, j, dirX, dirY}
  unordered_set<string> seen;

  int height = grid.size();
  int width = grid[0].size();

  while (!minheap.empty())
  {
    vector<int> current = minheap.top();
    minheap.pop();

    int heat = current[0];
    int x = current[1];
    int y = current[2];
    int directionX = current[3];
    int directionY = current[4];
    if (x == end.x && y == end.y)
    {
      return heat;
    }

    string key = intVectorToString({x, y, directionX, directionY});

    if (seen.find(key) == seen.end())
    {
      seen.insert(key);
      for (pair<int, int> direction : DIRECTIONS)
      {
        if (!((direction.first == directionX && direction.second == directionY) || (direction.first == -directionX && direction.second == -directionY)))
        {
          int newX = x;
          int newY = y;
          int newHeat = heat;
          for (int i = 1; i <= maxStep; i++)
          {
            newX += direction.first;
            newY += direction.second;
            if (inBounds(newX, newY, height, width))
            {
              newHeat += grid[newY][newX];
              if (i >= minStep)
              {
                minheap.push({newHeat, newX, newY, direction.first, direction.second});
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

    vector<int> lineNumbers;
    for (char c : line)
    {
      lineNumbers.push_back(c - '0');
    }
    map.push_back(lineNumbers);
  }

  int mapWidth = map[0].size();
  int mapHeight = map.size();

  int part1 = dijkstra({0, 0}, {mapWidth - 1, mapHeight - 1}, 1, 3, map);
  printf("Heat loss for a maximum of 3 blocks in a single direction: %i.\n", part1);
  int part2 = dijkstra({0, 0}, {mapWidth - 1, mapHeight - 1}, 4, 10, map);
  printf("Heat loss for a maximum of 10 blocks and a minimum of 4 blocks in a single direction: %i.\n", part2);
}