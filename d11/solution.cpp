#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

char GALAXY_SYMBOL = '#';
long EXPANSION_SIZE = 1000000;

struct position
{
  long x;
  long y;
};

vector<position> expand(vector<position> galaxies, vector<bool> rowsWithGalaxies, vector<bool> columnsWithGalaxies)
{
  vector<position> newGalaxiesPosition;
  for (position p : galaxies)
  {
    long offsetY = 0;
    long offsetX = 0;
    for (int i = 0; i < p.y; i++)
    {
      offsetY += (1 - rowsWithGalaxies[i]) * (EXPANSION_SIZE - 1);
    }
    for (int i = 0; i < p.x; i++)
    {
      offsetX += (1 - columnsWithGalaxies[i]) * (EXPANSION_SIZE - 1);
    }
    position newPosition;
    newPosition.x = p.x + offsetX;
    newPosition.y = p.y + offsetY;
    newGalaxiesPosition.push_back(newPosition);
  }
  return newGalaxiesPosition;
}

void showGalaxiesPositions(vector<position> galaxies)
{
  for (position p : galaxies)
  {
    printf("(%i, %i)\n", p.x, p.y);
  }
}

long getShortestPath(position p1, position p2)
{
  long d = 0;

  if (p1.x > p2.x)
    d += p1.x - p2.x;
  else
    d += p2.x - p1.x;

  if (p1.y > p2.y)
    d += p1.y - p2.y;
  else
    d += p2.y - p1.y;

  return d;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);

  string line;
  vector<position> galaxies;
  vector<bool> rowsWithGalaxies;
  vector<bool> columnsWithGalaxies;

  int i = 0;
  while (file.good())
  {
    bool lineHasGalaxies = false;
    getline(file, line);
    for (int j = 0; j < line.size(); j++)
    {
      if (j >= columnsWithGalaxies.size())
      {
        columnsWithGalaxies.push_back(false);
      }
      if (line[j] == GALAXY_SYMBOL)
      {
        position galaxyPosition;
        galaxyPosition.x = j;
        galaxyPosition.y = i;

        galaxies.push_back(galaxyPosition);
        lineHasGalaxies = true;
        columnsWithGalaxies[j] = true;
      }
    }
    rowsWithGalaxies.push_back(lineHasGalaxies);
    i++;
  }
  galaxies = expand(galaxies, rowsWithGalaxies, columnsWithGalaxies);

  long shortestPathDistances = 0;
  for (int i = 0; i < galaxies.size() - 1; i++)
  {
    for (int j = i + 1; j < galaxies.size(); j++)
    {
      shortestPathDistances += getShortestPath(galaxies[i], galaxies[j]);
    }
  }
  printf("Sum of the shortest path between all the galaxies: %lu\n", shortestPathDistances);
}