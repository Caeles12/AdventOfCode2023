#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#define number int

#define tileMap vector<vector<ETileType>>
#define vecMap vector<vector<vector<vec>>>

using namespace std;

typedef enum
{
  EmptySpace = 0,
  MirrorU = 1,
  MirrorD = 2,
  HorizontalSplitter = 3,
  VerticalSplitter = 4
} ETileType;

map<char, ETileType> charToTileType = {
    {'.', ETileType::EmptySpace},
    {'/', ETileType::MirrorU},
    {'\\', ETileType::MirrorD},
    {'-', ETileType::HorizontalSplitter},
    {'|', ETileType::VerticalSplitter},
};

map<ETileType, char> tileTypeToChar = {
    {ETileType::EmptySpace, '.'},
    {ETileType::MirrorU, '/'},
    {ETileType::MirrorD, '\\'},
    {ETileType::HorizontalSplitter, '-'},
    {ETileType::VerticalSplitter, '|'},
};

struct vec
{
  int x;
  int y;
};

vec addVecs(vec v1, vec v2)
{
  vec newVec;
  newVec.x = v1.x + v2.x;
  newVec.y = v1.y + v2.y;
  return newVec;
}

bool inMap(vec position, tileMap map)
{
  return position.x >= 0 && position.x < map[0].size() && position.y >= 0 && position.y < map.size();
}

void showMap(vec beamPosition, tileMap &map)
{
  for (int i = 0; i < map.size(); i++)
  {
    string s;
    for (int j = 0; j < map[i].size(); j++)
    {
      if (i == beamPosition.y && j == beamPosition.x)
      {
        s += '#';
      }
      else
      {
        s += tileTypeToChar.at(map[i][j]);
      }
    }
    printf("%s\n", s.c_str());
  }
}

bool directionExists(vec direction, vector<vec> tile)
{
  for (vec v : tile)
  {
    if (v.x == direction.x && v.y == direction.y)
    {
      return true;
    }
  }
  return false;
}

void showEnergyMap(vecMap &beams)
{
  for (int i = 0; i < beams.size(); i++)
  {
    string s;
    for (int j = 0; j < beams[i].size(); j++)
    {
      if (beams[i][j].size() > 0)
      {
        s += '#';
      }
      else
      {
        s += '.';
      }
    }
    printf("%s\n", s.c_str());
  }
}

number getTotalEnergy(vecMap &beams)
{
  number energy = 0;
  for (int i = 0; i < beams.size(); i++)
  {
    for (int j = 0; j < beams[i].size(); j++)
    {
      if (beams[i][j].size() > 0)
      {
        energy++;
      }
    }
  }
  return energy;
}

void forwardBeam(vec position, vec direction, tileMap &map, vecMap &beams)
{
  vec newPosition = addVecs(position, direction);
  if (inMap(newPosition, map) && !directionExists(direction, beams[newPosition.y][newPosition.x]))
  {
    beams[newPosition.y][newPosition.x].push_back(direction);

    ETileType newTile = map[newPosition.y][newPosition.x];
    if (newTile == ETileType::MirrorU)
    {
      vec newDirection;
      newDirection.x = -direction.y;
      newDirection.y = -direction.x;
      forwardBeam(newPosition, newDirection, map, beams);
    }
    else if (newTile == ETileType::MirrorD)
    {
      vec newDirection;
      newDirection.x = direction.y;
      newDirection.y = direction.x;
      forwardBeam(newPosition, newDirection, map, beams);
    }
    else if (newTile == ETileType::HorizontalSplitter)
    {
      if (direction.y == 0)
      {
        forwardBeam(newPosition, direction, map, beams);
      }
      else
      {
        vec newDirection1;
        vec newDirection2;

        newDirection1.x = direction.y;
        newDirection1.y = direction.x;

        newDirection2.x = -direction.y;
        newDirection2.y = -direction.x;

        forwardBeam(newPosition, newDirection1, map, beams);
        forwardBeam(newPosition, newDirection2, map, beams);
      }
    }
    else if (newTile == ETileType::VerticalSplitter)
    {
      if (direction.x == 0)
      {
        forwardBeam(newPosition, direction, map, beams);
      }
      else
      {
        vec newDirection1;
        vec newDirection2;

        newDirection1.x = direction.y;
        newDirection1.y = direction.x;

        newDirection2.x = -direction.y;
        newDirection2.y = -direction.x;

        forwardBeam(newPosition, newDirection1, map, beams);
        forwardBeam(newPosition, newDirection2, map, beams);
      }
    }
    else
    {
      forwardBeam(newPosition, direction, map, beams);
    }
  }
}

vecMap emptyBeamMap(int width, int height)
{
  vecMap beams;
  for (int i = 0; i < height; i++)
  {
    vector<vector<vec>> beamsLine;
    for (int j = 0; j < width; j++)
    {
      vector<vec> v;
      beamsLine.push_back(v);
    }
    beams.push_back(beamsLine);
  }
  return beams;
}

number maximizeEnergy(tileMap map)
{
  number maxEnergy = 0;
  vecMap beams;
  vec startDirection;
  vec startPosition;

  // The beam start on the left and right sides of the map
  for (int i = 0; i < map.size(); i++)
  {
    // Left side
    beams = emptyBeamMap(map[0].size(), map.size());

    startDirection.x = 1;
    startDirection.y = 0;

    startPosition.x = -1;
    startPosition.y = i;

    forwardBeam(startPosition, startDirection, map, beams);
    int leftSideEnergy = getTotalEnergy(beams);
    if (leftSideEnergy > maxEnergy)
    {
      maxEnergy = leftSideEnergy;
    }

    // Right side
    beams = emptyBeamMap(map[0].size(), map.size());

    startDirection.x = -1;
    startDirection.y = 0;

    startPosition.x = map[i].size();
    startPosition.y = i;

    forwardBeam(startPosition, startDirection, map, beams);
    int rightSideEnergy = getTotalEnergy(beams);
    if (rightSideEnergy > maxEnergy)
    {
      maxEnergy = rightSideEnergy;
    }
  }

  // The beam start on the top and bottom sides of the map
  for (int i = 0; i < map.size(); i++)
  {
    // Top side
    beams = emptyBeamMap(map[0].size(), map.size());

    startDirection.x = 0;
    startDirection.y = 1;

    startPosition.x = i;
    startPosition.y = -1;

    forwardBeam(startPosition, startDirection, map, beams);
    int topSideEnergy = getTotalEnergy(beams);
    if (topSideEnergy > maxEnergy)
    {
      maxEnergy = topSideEnergy;
    }

    // Bottom side
    beams = emptyBeamMap(map[0].size(), map.size());

    startDirection.x = 0;
    startDirection.y = -1;

    startPosition.x = i;
    startPosition.y = map.size();

    forwardBeam(startPosition, startDirection, map, beams);
    int bottomSideEnergy = getTotalEnergy(beams);
    if (bottomSideEnergy > maxEnergy)
    {
      maxEnergy = bottomSideEnergy;
    }
  }
  return maxEnergy;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  tileMap map;
  vecMap beams;

  while (file.good())
  {
    getline(file, line);
    printf("%s\n", line.c_str());
    vector<ETileType> tilesLine;
    for (char c : line)
    {
      if (charToTileType.count(c) > 0)
      {
        tilesLine.push_back(charToTileType.at(c));
      }
    }
    map.push_back(tilesLine);
  }

  // Part 1
  beams = emptyBeamMap(map[0].size(), map.size());
  vec startPosition;
  startPosition.x = -1;
  startPosition.y = 0;

  vec startDirection;
  startDirection.x = 1;
  startDirection.y = 0;
  forwardBeam(startPosition, startDirection, map, beams);
  // showEnergyMap(beams);
  printf("Total energy starting in the top left corner is: %i.\n", getTotalEnergy(beams));

  // Part 2
  number maxEnergy = maximizeEnergy(map);
  printf("The maximum energy we can achieve is %i.\n", maxEnergy);
}