#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int hashString(string s)
{
  int value = 0;
  for (char c : s)
  {
    value += c;
    value *= 17;
    value = value % 256;
  }
  return value;
}

void removeLens(string label, vector<vector<string>> &boxes)
{
  int boxID = hashString(label);
  vector<string> &box = boxes[boxID];
  vector<string>::iterator it = find(box.begin(), box.end(), label);
  if (it != box.end())
  {
    box.erase(it);
    // printf("Removed lens %s from box n°%i.\n", label.c_str(), boxID);
  }
}

void addLens(string label, int focal, vector<vector<string>> &boxes, map<string, int> &lenses)
{
  int boxID = hashString(label);
  vector<string> &box = boxes[boxID];
  lenses[label] = focal;

  vector<string>::iterator it = find(box.begin(), box.end(), label);
  if (it == box.end())
  {
    box.push_back(label);
  }
  // printf("Added lens %s with focal %i to box n°%i.\n", label.c_str(), focal, boxID);
}

int focusingPower(vector<vector<string>> &boxes, map<string, int> &lenses)
{
  int sum = 0;
  for (int i = 0; i < boxes.size(); i++)
  {
    for (int j = 0; j < boxes[i].size(); j++)
    {
      sum += (i + 1) * (j + 1) * lenses.at(boxes[i][j]);
    }
  }
  return sum;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  getline(file, line);

  int sum = 0;
  vector<vector<string>> boxes;
  map<string, int> lenses;

  for (int i = 0; i < 256; i++)
  {
    vector<string> box;
    boxes.push_back(box);
  }

  stringstream ss(line);
  while (ss.good())
  {
    string step;
    getline(ss, step, ',');
    if (step[step.size() - 1] == '-')
    {
      step.pop_back();
      removeLens(step, boxes);
    }
    else
    {
      char c = step[step.size() - 1];
      int focal = c - '0';
      step.pop_back();
      step.pop_back();
      addLens(step, focal, boxes, lenses);
    }
    sum += hashString(step);
  }

  printf("The hashed value of the input string is: %i.\n", sum);
  printf("The total focusing power is %i.\n", focusingPower(boxes, lenses));
}