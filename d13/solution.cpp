#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#define number long

using namespace std;

const int SMUDGES = 1;

int getHorizontalDiff(vector<string> s, int l1, int l2)
{
  int diffs = 0;
  for (int i = 0; i < s[l1].size(); i++)
  {
    if (s[l1][i] != s[l2][i])
    {
      diffs++;
    }
  }
  return diffs;
}

int getVerticalDiff(vector<string> s, int c1, int c2)
{
  int diffs = 0;
  for (int i = 0; i < s.size(); i++)
  {
    if (s[i][c1] != s[i][c2])
    {
      diffs++;
    }
  }
  return diffs;
}

int getHorizontalReflexion(vector<string> s)
{
  for (int i = 0; i < s.size() - 1; i++)
  {
    if (getHorizontalDiff(s, i, i + 1) <= SMUDGES)
    {
      int diffs = 0;
      bool isSymmetry = true;
      for (int j = 0; i + j + 1 < s.size() && i - j >= 0; j++)
      {
        diffs += getHorizontalDiff(s, i + j + 1, i - j);
        if (diffs > SMUDGES)
        {
          isSymmetry = false;
          break;
        }
      }
      if (isSymmetry && diffs == SMUDGES)
      {
        return i + 1;
      }
    }
  }
  return 0;
}

int getVerticalReflexion(vector<string> s)
{
  for (int i = 0; i < s[0].size() - 1; i++)
  {
    if (getVerticalDiff(s, i, i + 1) <= SMUDGES)
    {
      int diffs = 0;
      bool isSymmetry = true;
      for (int j = 0; i + j + 1 < s[0].size() && i - j >= 0; j++)
      {
        diffs += getVerticalDiff(s, i + j + 1, i - j);
        if (diffs > SMUDGES)
        {
          isSymmetry = false;
          break;
        }
      }
      if (isSymmetry && diffs == SMUDGES)
      {
        return i + 1;
      }
    }
  }
  return 0;
}

int summarize(vector<string> s)
{
  int vertical = getVerticalReflexion(s);
  int horizontal = getHorizontalReflexion(s);
  return vertical + 100 * horizontal;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;

  vector<string> currentPattern;

  number sum = 0;
  while (file.good())
  {
    getline(file, line);
    if (line.size() > 0)
    {
      currentPattern.push_back(line);
    }
    else
    {
      for (int i = 0; i < currentPattern.size(); i++)
      {
        printf("%i %s\n", i + 1, currentPattern[i].c_str());
      }
      number summary = summarize(currentPattern);
      printf("Score: %lu\n", summary);
      sum += summary;

      currentPattern = {};
    }
  }
  for (int i = 0; i < currentPattern.size(); i++)
  {
    printf("%i %s\n", i + 1, currentPattern[i].c_str());
  }
  number summary = summarize(currentPattern);
  printf("Score: %lu\n", summary);
  sum += summary;
  printf("Summary of all the notes: %lu\n", sum);
}