#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<long> parseNumbers(string s)
{
  stringstream ss(s);
  vector<long> numbers;
  while (ss.good())
  {
    string t;
    ss >> t;
    numbers.push_back(stol(t));
  }
  return numbers;
}

void showVector(vector<long> v)
{
  printf("<");
  for (int i = 0; i < v.size(); i++)
  {
    printf("%li", v[i]);
    if (i < v.size() - 1)
    {
      printf(", ");
    }
  }
  printf(">\n");
}

bool isAllZeros(vector<long> v)
{
  for (int i = 0; i < v.size(); i++)
  {
    if (v[i] != 0)
    {
      return false;
    }
  }
  return true;
}

long findNextValue(vector<long> v)
{
  if (isAllZeros(v))
  {
    return 0;
  }
  vector<long> diffVector;
  for (int i = 1; i < v.size(); i++)
  {
    diffVector.push_back(v[i] - v[i - 1]);
  }
  long lastValue = v[v.size() - 1];
  return lastValue + findNextValue(diffVector);
}

long findPrevValue(vector<long> v)
{
  if (isAllZeros(v))
  {
    return 0;
  }
  vector<long> diffVector;
  for (int i = 1; i < v.size(); i++)
  {
    diffVector.push_back(v[i] - v[i - 1]);
  }
  long firstValue = v[0];
  return firstValue - findPrevValue(diffVector);
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  long sumNextNumbers{0};
  long sumPrevNumbers{0};

  while (file.good())
  {
    getline(file, line);
    vector<long> init = parseNumbers(line);
    long predictionNext = findNextValue(init);
    long predictionPrev = findPrevValue(init);

    sumNextNumbers += predictionNext;
    sumPrevNumbers += predictionPrev;
  }
  printf("Sum of all the predictions for following numbers: %li.\n", sumNextNumbers);
  printf("Sum of all the predictions for preceding numbers: %li.\n", sumPrevNumbers);
}