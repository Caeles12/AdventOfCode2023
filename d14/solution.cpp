#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#define number int

long CYCLES = 1000000000;

using namespace std;

typedef enum
{
  EmptySpace = 0,
  CubicRock = 1,
  RoundedRock = 2,
} ETileType;

map<char, ETileType> charToTile = {
    {'.', ETileType::EmptySpace},
    {'#', ETileType::CubicRock},
    {'O', ETileType::RoundedRock},
};

map<ETileType, char> tileToChar = {
    {ETileType::EmptySpace, '.'},
    {ETileType::CubicRock, '#'},
    {ETileType::RoundedRock, 'O'},
};

void showPlatform(vector<vector<ETileType>> &platform)
{
  for (int i = 0; i < platform.size(); i++)
  {
    string s;
    for (int j = 0; j < platform[i].size(); j++)
    {
      s += tileToChar.at(platform[i][j]);
    }
    printf("%s\n", s.c_str());
  }
}

string platformHash(vector<vector<ETileType>> &platform)
{
  string hash;
  for (int i = 0; i < platform.size(); i++)
  {
    for (int j = 0; j < platform[i].size(); j++)
    {
      if (platform[i][j] == ETileType::RoundedRock)
      {
        hash += to_string(i) + '.' + to_string(j) + ';';
      }
    }
  }
  return hash;
}

void tiltNorth(vector<vector<ETileType>> &platform)
{
  for (int i = 1; i < platform.size(); i++)
  {
    for (int j = 0; j < platform[i].size(); j++)
    {
      if (platform[i][j] == ETileType::RoundedRock)
      {
        int newPosition = i;
        while (newPosition - 1 >= 0 && platform[newPosition - 1][j] == ETileType::EmptySpace)
        {
          newPosition--;
        }
        platform[i][j] = ETileType::EmptySpace;
        platform[newPosition][j] = ETileType::RoundedRock;
      }
    }
  }
}

void tiltSouth(vector<vector<ETileType>> &platform)
{
  for (int i = platform.size() - 2; i >= 0; i--)
  {
    for (int j = 0; j < platform[i].size(); j++)
    {
      if (platform[i][j] == ETileType::RoundedRock)
      {
        int newPosition = i;
        while (newPosition + 1 < platform.size() && platform[newPosition + 1][j] == ETileType::EmptySpace)
        {
          newPosition++;
        }
        platform[i][j] = ETileType::EmptySpace;
        platform[newPosition][j] = ETileType::RoundedRock;
      }
    }
  }
}

void tiltWest(vector<vector<ETileType>> &platform)
{
  for (int i = 0; i < platform.size(); i++)
  {
    for (int j = 1; j < platform[i].size(); j++)
    {
      if (platform[i][j] == ETileType::RoundedRock)
      {
        int newPosition = j;
        while (newPosition - 1 >= 0 && platform[i][newPosition - 1] == ETileType::EmptySpace)
        {
          newPosition--;
        }
        platform[i][j] = ETileType::EmptySpace;
        platform[i][newPosition] = ETileType::RoundedRock;
      }
    }
  }
}

void tiltEast(vector<vector<ETileType>> &platform)
{
  for (int i = 0; i < platform.size(); i++)
  {
    for (int j = platform[i].size() - 2; j >= 0; j--)
    {
      if (platform[i][j] == ETileType::RoundedRock)
      {
        int newPosition = j;
        while (newPosition + 1 < platform[i].size() && platform[i][newPosition + 1] == ETileType::EmptySpace)
        {
          newPosition++;
        }
        platform[i][j] = ETileType::EmptySpace;
        platform[i][newPosition] = ETileType::RoundedRock;
      }
    }
  }
}

void spin(vector<vector<ETileType>> &platform)
{
  tiltNorth(platform);
  tiltWest(platform);
  tiltSouth(platform);
  tiltEast(platform);
}

number getLoad(vector<vector<ETileType>> &platform)
{
  number load = 0;
  for (int i = 0; i < platform.size(); i++)
  {
    for (int j = 0; j < platform[i].size(); j++)
    {
      if (platform[i][j] == ETileType::RoundedRock)
      {
        number rockLoad = platform.size() - i;
        load += rockLoad;
      }
    }
  }
  return load;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;

  vector<vector<ETileType>> platform;
  map<string, number> platformMovements;
  vector<number> loads;

  while (file.good())
  {
    getline(file, line);
    vector<ETileType> row;
    for (char c : line)
    {
      row.push_back(charToTile.at(c));
    }
    platform.push_back(row);
  }
  // PART 1
  tiltNorth(platform);
  printf("The load of the platform tilted to the north is %i.\n", getLoad(platform));

  // PART 2
  number loopStart = 0;
  number loopEnd = 0;
  for (long i = 0; i < CYCLES; i++)
  {
    string key = platformHash(platform);
    if (platformMovements.find(key) != platformMovements.end())
    {
      loopStart = platformMovements[key];
      loopEnd = i;
      break;
    }
    loads.push_back(getLoad(platform));
    spin(platform);
    platformMovements[key] = i;
  }

  printf("Found a loop starting at spin %lu and ending at spin %lu.\n", loopStart, loopEnd);

  long remaining = (CYCLES - loopStart) % (loopEnd - loopStart);
  printf("The load after %lu cycles is %i.\n", CYCLES, loads[loopStart + remaining]);
}