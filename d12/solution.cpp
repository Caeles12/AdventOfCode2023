#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#define number long

using namespace std;

const int FOLDS = 5;

typedef enum
{
  Operational = 0,
  Damaged = 1,
  Unknown = 2,
} ESpringStatus;

map<char, ESpringStatus> charToStatus = {
    {'.', ESpringStatus::Operational},
    {'#', ESpringStatus::Damaged},
    {'?', ESpringStatus::Unknown},
};

map<ESpringStatus, char> statusToChar = {
    {ESpringStatus::Operational, '.'},
    {ESpringStatus::Damaged, '#'},
    {ESpringStatus::Unknown, '?'},
};

map<string, number> cache;

struct springGroup
{
  int size;
  ESpringStatus status;
};

vector<int> parseNumbers(const string &s, char delim)
{
  vector<int> result;
  stringstream ss(s);
  string item;

  while (getline(ss, item, delim))
  {
    result.push_back(stoul(item));
  }

  return result;
}

string springStatusToString(vector<ESpringStatus> status)
{
  string s;
  for (int i = 0; i < status.size(); i++)
  {
    s += statusToChar[status[i]];
  }
  return s;
}

string generateKey(vector<ESpringStatus> status, vector<int> sizes)
{
  string s;
  s += springStatusToString(status);
  s += ":";
  for (int i = 0; i < sizes.size(); i++)
  {
    s += to_string(sizes[i]) + ",";
  }
  return s;
}

number countCachedPossibilities(vector<ESpringStatus> status, vector<int> sizeOfGroups);

number countPossibilities(vector<ESpringStatus> status, vector<int> sizeOfGroups)
{

  if (status.size() == 0)
  {
    if (sizeOfGroups.size() == 0)
    {
      return 1;
    }
    return 0;
  }

  if (status[0] == ESpringStatus::Operational)
  {
    status.erase(status.begin());
    number possibilities = countCachedPossibilities(status, sizeOfGroups);
    return possibilities;
  }
  else if (status[0] == ESpringStatus::Unknown)
  {
    number count = 0;
    status[0] = ESpringStatus::Damaged;
    count += countCachedPossibilities(status, sizeOfGroups);
    status[0] = ESpringStatus::Operational;
    count += countCachedPossibilities(status, sizeOfGroups);
    return count;
  }
  else
  {
    if (sizeOfGroups.size() == 0 || sizeOfGroups[0] > status.size())
    {
      return 0;
    }
    for (int i = 0; i < sizeOfGroups[0]; i++)
    {
      if (status[i] == ESpringStatus::Operational)
      {
        return 0;
      }
    }
    if (sizeOfGroups[0] == status.size() && sizeOfGroups.size() == 1)
    {
      return 1;
    }
    if (status[sizeOfGroups[0]] == ESpringStatus::Damaged)
    {
      return 0;
    }

    if (status[sizeOfGroups[0]] == ESpringStatus::Unknown)
    {
      status[sizeOfGroups[0]] = ESpringStatus::Operational;
    }

    status.erase(status.begin(), status.begin() + sizeOfGroups[0]);
    sizeOfGroups.erase(sizeOfGroups.begin());
    number possibilities = countCachedPossibilities(status, sizeOfGroups);
    return possibilities;
  }
}

number countCachedPossibilities(vector<ESpringStatus> status, vector<int> sizeOfGroups)
{
  string key = generateKey(status, sizeOfGroups);
  if (cache.find(key) != cache.end())
  {
    return cache[key];
  }
  number possibilities = countPossibilities(status, sizeOfGroups);
  cache[key] = possibilities;
  return possibilities;
}

vector<ESpringStatus> unfoldSpringsMap(vector<ESpringStatus> s, int f)
{
  vector<ESpringStatus> unfolded;
  for (int i = 0; i < f; i++)
  {
    for (int j = 0; j < s.size(); j++)
    {
      unfolded.push_back(s[j]);
    }
    if (i < f - 1)
    {
      unfolded.push_back(ESpringStatus::Unknown);
    }
  }
  return unfolded;
}

vector<int> unfoldSizes(vector<int> s, int f)
{
  vector<int> unfolded;
  for (int i = 0; i < f; i++)
  {
    for (int j = 0; j < s.size(); j++)
    {
      unfolded.push_back(s[j]);
    }
  }
  return unfolded;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  number sumPart1 = 0;
  number sumPart2 = 0;
  while (file.good())
  {
    string springs;
    string sizesString;

    vector<ESpringStatus> status;
    vector<int> sizeOfGroups;

    getline(file, springs, ' ');
    for (int i = 0; i < springs.size(); i++)
    {
      char currentChar = springs[i];
      status.push_back(charToStatus.at(currentChar));
    }

    getline(file, sizesString);
    sizeOfGroups = parseNumbers(sizesString, ',');

    number possibilitiesFolded = countCachedPossibilities(status, sizeOfGroups);
    sumPart1 += possibilitiesFolded;

    status = unfoldSpringsMap(status, FOLDS);
    sizeOfGroups = unfoldSizes(sizeOfGroups, FOLDS);

    number possibilitiesUnfolded = countCachedPossibilities(status, sizeOfGroups);
    sumPart2 += possibilitiesUnfolded;
  }
  printf("Sum of the possible arrangement counts for the map: %lu\n", sumPart1);
  printf("Sum of the possible arrangement counts for the unfolded map: %lu\n", sumPart2);
}