#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

vector<u_long> parseNumbers(string s)
{
  stringstream ss(s);
  vector<u_long> numbers;
  string concat;
  while (ss.good())
  {
    string t;
    ss >> t;
    concat += t;
    numbers.push_back(stoul(t));
  }
  cout << concat << endl;
  numbers.push_back(stoull(concat));
  return numbers;
}

u_long computeDistance(u_long time, u_long timePressed)
{
  return (time - timePressed) * timePressed;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;

  getline(file, line, ':');
  getline(file, line);
  vector<u_long> times = parseNumbers(line);

  getline(file, line, ':');
  getline(file, line);
  vector<u_long> distances = parseNumbers(line);

  u_long total = 1;
  for (int i = 0; i < times.size() - 1; i++)
  {
    u_long waysToBeatRecord = 0;
    for (int j = 0; j < times[i]; j++)
    {
      if (computeDistance(times[i], j) > distances[i])
        waysToBeatRecord++;
    }
    total *= waysToBeatRecord;
    printf("There are %u ways to beat the first race.\n", waysToBeatRecord);
  }
  printf("The multiplication of all the number of ways is equal to %u.\n", total);
  printf("The real race has a duration of %lu and a distance of %lu.\n", times[times.size() - 1], distances[distances.size() - 1]);
  u_long waysToBeatRecord = 0;
  for (int j = 0; j < times[times.size() - 1]; j++)
  {
    if (computeDistance(times[times.size() - 1], j) > distances[distances.size() - 1])
      waysToBeatRecord++;
  }
  printf("There are %lu ways to beat this race.\n", waysToBeatRecord);
}