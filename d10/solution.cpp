#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <bitset>

using namespace std;

typedef enum
{
  Undefined = 0,
  PipeLoop = 1,
  ZoneA = 2,
  ZoneB = 3,
} ETileType;

struct position
{
  int x;
  int y;
};

struct entity
{
  position currentPosition;
  position lastPosition;
  int distanceTraveled;
};

map<char, tuple<bool, bool, bool, bool>> CONNEXIONS = {
    {'|', {1, 1, 0, 0}},
    {'-', {0, 0, 1, 1}},
    {'L', {1, 0, 1, 0}},
    {'J', {1, 0, 0, 1}},
    {'7', {0, 1, 0, 1}},
    {'F', {0, 1, 1, 0}},
    {'.', {0, 0, 0, 0}},
};

char START_PIPE = 'S';

char findTileType(position p, vector<string> maze)
{

  bool connectedNorth = p.y > 0 && get<1>(CONNEXIONS.at(maze[p.y - 1][p.x]));
  bool connectedSouth = p.y < maze.size() && get<0>(CONNEXIONS.at(maze[p.y + 1][p.x]));
  bool connectedEast = p.x < maze[0].size() && get<3>(CONNEXIONS.at(maze[p.y][p.x + 1]));
  bool connectedWest = p.x > 0 && get<2>(CONNEXIONS.at(maze[p.y][p.x - 1]));

  map<char, tuple<bool, bool, bool, bool>>::iterator it = CONNEXIONS.begin();
  while (it != CONNEXIONS.end())
  {
    tuple<bool, bool, bool, bool> shape = it->second;
    bool sameNorth = connectedNorth == get<0>(shape);
    bool sameSouth = connectedSouth == get<1>(shape);
    bool sameEast = connectedEast == get<2>(shape);
    bool sameWest = connectedWest == get<3>(shape);

    if (sameNorth && sameSouth && sameEast && sameWest)
    {
      return it->first;
    }
    it++;
  }

  return '.';
}

void moveEntity(entity &e, vector<string> maze, vector<vector<ETileType>> &coloredMaze)
{
  tuple<bool, bool, bool, bool> possibleMoves = CONNEXIONS.at(maze[e.currentPosition.y][e.currentPosition.x]);
  // When the last position is equal to the current position, we do not move the last position
  if (e.currentPosition.x == e.lastPosition.x && e.currentPosition.y == e.lastPosition.y)
  {
    if (get<0>(possibleMoves))
    {
      e.currentPosition.y--;
    }
    else if (get<1>(possibleMoves))
    {
      e.currentPosition.y++;
    }
    else if (get<2>(possibleMoves))
    {
      e.currentPosition.x++;
    }
    else if (get<3>(possibleMoves))
    {
      e.currentPosition.x--;
    }
  }
  else
  {
    if (get<0>(possibleMoves) && e.lastPosition.y != e.currentPosition.y - 1)
    {
      e.lastPosition.y = e.currentPosition.y;
      e.lastPosition.x = e.currentPosition.x;
      e.currentPosition.y--;
    }
    else if (get<1>(possibleMoves) && e.lastPosition.y != e.currentPosition.y + 1)
    {
      e.lastPosition.y = e.currentPosition.y;
      e.lastPosition.x = e.currentPosition.x;
      e.currentPosition.y++;
    }
    else if (get<2>(possibleMoves) && e.lastPosition.x != e.currentPosition.x + 1)
    {
      e.lastPosition.y = e.currentPosition.y;
      e.lastPosition.x = e.currentPosition.x;
      e.currentPosition.x++;
    }
    else if (get<3>(possibleMoves) && e.lastPosition.x != e.currentPosition.x - 1)
    {
      e.lastPosition.y = e.currentPosition.y;
      e.lastPosition.x = e.currentPosition.x;
      e.currentPosition.x--;
    }
  }

  if (e.lastPosition.x > e.currentPosition.x)
  {
    if (e.currentPosition.y + 1 < coloredMaze.size())
    {
      if (coloredMaze[e.currentPosition.y + 1][e.currentPosition.x] == ETileType::Undefined)
      {
        coloredMaze[e.currentPosition.y + 1][e.currentPosition.x] = ETileType::ZoneA;
      }
      if (coloredMaze[e.lastPosition.y + 1][e.lastPosition.x] == ETileType::Undefined)
      {
        coloredMaze[e.lastPosition.y + 1][e.lastPosition.x] = ETileType::ZoneA;
      }
    }
    if (e.currentPosition.y > 0)
    {
      if (coloredMaze[e.currentPosition.y - 1][e.currentPosition.x] == ETileType::Undefined)
      {
        coloredMaze[e.currentPosition.y - 1][e.currentPosition.x] = ETileType::ZoneB;
      }
      if (coloredMaze[e.lastPosition.y - 1][e.lastPosition.x] == ETileType::Undefined)
      {
        coloredMaze[e.lastPosition.y - 1][e.lastPosition.x] = ETileType::ZoneB;
      }
    }
  }
  else if (e.lastPosition.x < e.currentPosition.x)
  {
    if (e.currentPosition.y + 1 < coloredMaze.size())
    {
      if (coloredMaze[e.currentPosition.y + 1][e.currentPosition.x] == ETileType::Undefined)
      {
        coloredMaze[e.currentPosition.y + 1][e.currentPosition.x] = ETileType::ZoneB;
      }
      if (coloredMaze[e.lastPosition.y + 1][e.lastPosition.x] == ETileType::Undefined)
      {
        coloredMaze[e.lastPosition.y + 1][e.lastPosition.x] = ETileType::ZoneB;
      }
    }
    if (e.currentPosition.y > 0)
    {
      if (coloredMaze[e.currentPosition.y - 1][e.currentPosition.x] == ETileType::Undefined)
      {
        coloredMaze[e.currentPosition.y - 1][e.currentPosition.x] = ETileType::ZoneA;
      }
      if (coloredMaze[e.lastPosition.y - 1][e.lastPosition.x] == ETileType::Undefined)
      {
        coloredMaze[e.lastPosition.y - 1][e.lastPosition.x] = ETileType::ZoneA;
      }
    }
  }
  else if (e.lastPosition.y > e.currentPosition.y)
  {
    if (e.currentPosition.x + 1 < coloredMaze[0].size())
    {
      if (coloredMaze[e.currentPosition.y][e.currentPosition.x + 1] == ETileType::Undefined)
      {
        coloredMaze[e.currentPosition.y][e.currentPosition.x + 1] = ETileType::ZoneB;
      }
      if (coloredMaze[e.lastPosition.y][e.lastPosition.x + 1] == ETileType::Undefined)
      {
        coloredMaze[e.lastPosition.y][e.lastPosition.x + 1] = ETileType::ZoneB;
      }
    }
    if (e.currentPosition.x > 0)
    {
      if (coloredMaze[e.currentPosition.y][e.currentPosition.x - 1] == ETileType::Undefined)
      {
        coloredMaze[e.currentPosition.y][e.currentPosition.x - 1] = ETileType::ZoneA;
      }
      if (coloredMaze[e.lastPosition.y][e.lastPosition.x - 1] == ETileType::Undefined)
      {
        coloredMaze[e.lastPosition.y][e.lastPosition.x - 1] = ETileType::ZoneA;
      }
    }
  }
  else if (e.lastPosition.y < e.currentPosition.y)
  {
    if (e.currentPosition.x + 1 < coloredMaze[0].size())
    {
      if (coloredMaze[e.currentPosition.y][e.currentPosition.x + 1] == ETileType::Undefined)
      {
        coloredMaze[e.currentPosition.y][e.currentPosition.x + 1] = ETileType::ZoneA;
      }
      if (coloredMaze[e.lastPosition.y][e.lastPosition.x + 1] == ETileType::Undefined)
      {
        coloredMaze[e.lastPosition.y][e.lastPosition.x + 1] = ETileType::ZoneA;
      }
    }
    if (e.currentPosition.x > 0)
    {
      if (coloredMaze[e.currentPosition.y][e.currentPosition.x - 1] == ETileType::Undefined)
      {
        coloredMaze[e.currentPosition.y][e.currentPosition.x - 1] = ETileType::ZoneB;
      }
      if (coloredMaze[e.lastPosition.y][e.lastPosition.x - 1] == ETileType::Undefined)
      {
        coloredMaze[e.lastPosition.y][e.lastPosition.x - 1] = ETileType::ZoneB;
      }
    }
  }
  e.distanceTraveled++;
  coloredMaze[e.currentPosition.y][e.currentPosition.x] = ETileType::PipeLoop;
  // printf("Bougé en (%i, %i) depuis (%i, %i)\n", e.currentPosition.x, e.currentPosition.y, e.lastPosition.x, e.lastPosition.y);
}

void showColoredMaze(vector<vector<ETileType>> maze)
{
  for (int i = 0; i < maze.size(); i++)
  {
    string s;
    for (int j = 0; j < maze[i].size(); j++)
    {
      if (maze[i][j] == ETileType::PipeLoop)
      {
        s += "#";
      }
      else if (maze[i][j] == ETileType::Undefined)
      {
        s += ".";
      }
      else if (maze[i][j] == ETileType::ZoneA)
      {
        s += "A";
      }
      else if (maze[i][j] == ETileType::ZoneB)
      {
        s += "B";
      }
    }
    printf("%s\n", s.c_str());
  }
}

vector<int> countColors(vector<vector<ETileType>> maze)
{
  vector<int> colors = {0, 0, 0, 0};
  for (int i = 0; i < maze.size(); i++)
  {
    for (int j = 0; j < maze[i].size(); j++)
    {
      colors[maze[i][j]]++;
    }
  }
  return colors;
}

void fillColorRec(int x, int y, vector<vector<ETileType>> &coloredMaze)
{
  ETileType currentColor = coloredMaze[y][x];
  if (x > 0 && coloredMaze[y][x - 1] == ETileType::Undefined)
  {
    coloredMaze[y][x - 1] = currentColor;
    fillColorRec(x - 1, y, coloredMaze);
  }
  if (x + 1 < coloredMaze[0].size() && coloredMaze[y][x + 1] == ETileType::Undefined)
  {
    coloredMaze[y][x + 1] = currentColor;
    fillColorRec(x + 1, y, coloredMaze);
  }
  if (y > 0 && coloredMaze[y - 1][x] == ETileType::Undefined)
  {
    coloredMaze[y - 1][x] = currentColor;
    fillColorRec(x, y - 1, coloredMaze);
  }
  if (y + 1 < coloredMaze.size() && coloredMaze[y + 1][x] == ETileType::Undefined)
  {
    coloredMaze[y + 1][x] = currentColor;
    fillColorRec(x, y + 1, coloredMaze);
  }
}

void fillColor(vector<vector<ETileType>> &coloredMaze)
{
  for (int i = 0; i < coloredMaze.size(); i++)
  {
    for (int j = 0; j < coloredMaze[i].size(); j++)
    {
      if (coloredMaze[i][j] == ETileType::ZoneA || coloredMaze[i][j] == ETileType::ZoneB)
      {
        fillColorRec(j, i, coloredMaze);
      }
    }
  }
}

int main(int argc, char *argv[])
{

  ifstream file(argv[1]);
  string line;
  vector<string> maze;
  vector<vector<ETileType>> coloredMaze;

  position startPosition;
  entity creature;

  // Parse the input file
  int i = 0;
  while (file.good())
  {
    getline(file, line);
    vector<ETileType> coloredLine;
    for (int j = 0; j < line.size(); j++)
    {
      coloredLine.push_back(ETileType::Undefined);
      if (line[j] == START_PIPE)
      {
        startPosition.x = j;
        startPosition.y = i;
      }
    }
    coloredMaze.push_back(coloredLine);
    maze.push_back(line);
    i++;
  }

  // Initialize the position of the entity.
  creature.currentPosition.x = startPosition.x;
  creature.currentPosition.y = startPosition.y;

  creature.lastPosition.x = startPosition.x;
  creature.lastPosition.y = startPosition.y;

  creature.distanceTraveled = 0;

  // Replace the S by the appropriate tile type
  maze[startPosition.y][startPosition.x] = findTileType(startPosition, maze);

  // Move the entity around the loop untill it is back to the starting point
  do
  {
    moveEntity(creature, maze, coloredMaze);
  } while (creature.currentPosition.x != startPosition.x || creature.currentPosition.y != startPosition.y);

  printf("Loop size: %i\n", creature.distanceTraveled);
  printf("Farthest point distance: %i\n", (creature.distanceTraveled / 2));
  fillColor(coloredMaze);
  showColoredMaze(coloredMaze);

  vector<int> colorsCount = countColors(coloredMaze);

  printf("Il y a %i cases non définies, %i cases avec un tuyau appartenant à la boucle, %i cases dans la zone A et %i cases dans la zone B.",
         colorsCount[ETileType::Undefined], colorsCount[ETileType::PipeLoop], colorsCount[ETileType::ZoneA], colorsCount[ETileType::ZoneB]);
}