#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#define number long

using namespace std;

typedef enum
{
  RIGHT = 0,
  DOWN = 1,
  LEFT = 2,
  UP = 3,
} EDirection;

map<char, EDirection> charToDirection = {
    {'L', EDirection::LEFT},
    {'R', EDirection::RIGHT},
    {'U', EDirection::UP},
    {'D', EDirection::DOWN},
};

struct instruction
{
  EDirection direction;
  number size;
};

// Returns a pair containing the
vector<instruction> colorsToInstruction(vector<string> colors)
{
  vector<instruction> instructions;
  for (int i = 0; i < colors.size(); i++)
  {
    int direction = colors[i][colors[i].size() - 1] - '0';
    number size = stol(colors[i].substr(0, 5), 0, 16);
    instruction current;
    if (direction == EDirection::RIGHT)
    {
      current.direction = EDirection::RIGHT;
    }
    if (direction == EDirection::DOWN)
    {
      current.direction = EDirection::DOWN;
    }
    if (direction == EDirection::LEFT)
    {
      current.direction = EDirection::LEFT;
    }
    if (direction == EDirection::UP)
    {
      current.direction = EDirection::UP;
    }
    current.size = size;
    instructions.push_back(current);
  }
  return instructions;
}

pair<vector<pair<number, number>>, number> instructionsToPolygon(vector<instruction> instructions)
{
  vector<pair<number, number>> points;
  number posX = 0;
  number posY = 0;
  number perimeter = 0;

  for (instruction instr : instructions)
  {
    int size = instr.size;
    perimeter += size;

    if (instr.direction == EDirection::RIGHT)
    {
      posX += size;
    }
    if (instr.direction == EDirection::LEFT)
    {
      posX -= size;
    }
    if (instr.direction == EDirection::UP)
    {
      posY += size;
    }
    if (instr.direction == EDirection::DOWN)
    {
      posY -= size;
    }
    points.push_back({posX, posY});
  }
  return {points, perimeter};
}

// Calculate the area of a shape.
number polygonArea(const vector<pair<number, number>> &points)
{
  number area = 0.0;

  int j = points.size() - 1;
  for (int i = 0; i < points.size(); i++)
  {
    area += (points[j].first * points[i].second - points[i].first * points[j].second);
    j = i;
  }

  return abs(area) / 2.0;
}

number lagoonCapacity(vector<instruction> digPlan)
{
  pair<vector<pair<number, number>>, number> poly = instructionsToPolygon(digPlan);

  // The lagoon capacity is equal to the area of the polygon + the perimeter divided by two (as we need to account for the edges of the shape)
  return polygonArea(poly.first) + (poly.second / 2) + 1;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  vector<instruction> digPlan;
  vector<string> colors;

  while (file.good())
  {
    getline(file, line, ' ');
    EDirection direction = charToDirection.at(line[0]);
    getline(file, line, ' ');
    int size = stoi(line);
    getline(file, line);
    string color = line.substr(2, 6);
    instruction instr = {direction, size};
    digPlan.push_back(instr);
    colors.push_back(color);
  }

  printf("Area of the lagoon: %lu\n", lagoonCapacity(digPlan));
  printf("Area of the lagoon with the colors as instructions: %lu\n", lagoonCapacity(colorsToInstruction(colors)));
}