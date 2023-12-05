#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>

using namespace std;

struct hashFunction
{
  size_t operator()(const pair<u_int, u_int> &x) const
  {
    return x.first ^ x.second;
  }
};

vector<u_int> parseNumbers(const string &s, char delim)
{
  vector<u_int> result;
  stringstream ss(s);
  string item;

  while (getline(ss, item, delim))
  {
    result.push_back(stoul(item));
  }

  return result;
}

void showRanges(vector<pair<u_int, u_int>> ranges)
{
  for (int i = 0; i < ranges.size(); i++)
  {
    printf("[%u, %u] ", ranges[i].first, ranges[i].second);
  }
  printf("\n");
}

pair<vector<u_int>, vector<pair<u_int, u_int>>> processConversion(vector<u_int> prevNumbers, vector<pair<u_int, u_int>> ranges, ifstream &file)
{
  string line;
  vector<u_int> newNumbers;
  for (int i = 0; i < prevNumbers.size(); i++)
    newNumbers.push_back(prevNumbers[i]);
  unordered_set<pair<u_int, u_int>, hashFunction> notMoved;
  for (int i = 0; i < ranges.size(); i++)
    notMoved.insert(ranges[i]);

  vector<pair<u_int, u_int>> newRanges;

  getline(file, line);
  getline(file, line);

  while (file.good() && line.length() > 0)
  {
    vector<u_int> conversion = parseNumbers(line, ' ');
    u_int destinationStart = conversion[0];
    u_int sourceStart = conversion[1];
    u_int length = conversion[2] - 1;

    for (int i = 0; i < prevNumbers.size(); i++)
    {
      if (prevNumbers[i] >= sourceStart && prevNumbers[i] <= sourceStart + length)
      {
        u_int dist = prevNumbers[i] - sourceStart;
        newNumbers[i] = destinationStart + dist;
      }
    }
    for (int i = 0; i < ranges.size(); i++)
    {
      u_int rangeStart = ranges[i].first;
      u_int rangeEnd = ranges[i].second;
      // When seed range is including the modification range
      if (rangeStart <= sourceStart && sourceStart + length <= rangeEnd)
      {
        u_int newStart = destinationStart;
        u_int newEnd = destinationStart + length;
        if (sourceStart - rangeStart > 0)
        {
          notMoved.insert({rangeStart, sourceStart - 1});
        }
        if (rangeEnd - (sourceStart + length) > 0)
        {
          notMoved.insert({sourceStart + length + 1, rangeEnd});
        }
        newRanges.push_back({newStart, newEnd});
        notMoved.erase(ranges[i]);
      }
      // When seed range is included in the modification range
      else if (sourceStart <= rangeStart && rangeEnd <= sourceStart + length)
      {
        u_int newStart = destinationStart + (rangeStart - sourceStart);
        u_int newEnd = newStart + (rangeEnd - rangeStart);
        newRanges.push_back({newStart, newEnd});
        notMoved.erase(ranges[i]);
      }
      // When part of the seed range is on the left of the modification range
      else if (rangeStart <= sourceStart && rangeEnd <= sourceStart + length && rangeEnd >= sourceStart)
      {
        u_int newStart = destinationStart;
        u_int newEnd = destinationStart + (rangeEnd - sourceStart);
        if (sourceStart - rangeStart > 0)
        {
          notMoved.insert({rangeStart, sourceStart - 1});
        }
        newRanges.push_back({newStart, newEnd});
        notMoved.erase(ranges[i]);
      }
      // When part of the seed range is on the right of the modification range
      else if (rangeStart >= sourceStart && rangeStart <= sourceStart + length && sourceStart + length <= rangeEnd)
      {
        u_int newStart = destinationStart + (rangeStart - sourceStart);
        u_int newEnd = newStart + ((sourceStart + length) - rangeStart);
        if (rangeEnd - (sourceStart + length) > 0)
        {
          notMoved.insert({sourceStart + length + 1, rangeEnd});
        }
        newRanges.push_back({newStart, newEnd});
        notMoved.erase(ranges[i]);
      }
    }
    getline(file, line);
  }

  /*for (pair<u_int, u_int> range : notMoved)
  {
    newRanges.push_back(range);
  }*/
  return {newNumbers, newRanges};
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  getline(file, line, ' ');
  getline(file, line);

  vector<u_int> seeds = parseNumbers(line, ' ');

  vector<pair<u_int, u_int>> ranges;

  for (int i = 0; i < seeds.size(); i += 2)
  {
    pair<u_int, u_int> range;

    range.first = seeds[i];
    range.second = seeds[i] + seeds[i + 1] - 1;

    ranges.push_back(range);
  }

  getline(file, line);
  showRanges(ranges);

  for (int i = 0; i < 7; i++)
  {
    pair<vector<u_int>, vector<pair<u_int, u_int>>> out = processConversion(seeds, ranges, file);
    seeds = out.first;
    ranges = out.second;
    showRanges(ranges);
  }

  u_int lowestLocation = seeds[0];
  for (int i = 1; i < seeds.size(); i++)
  {
    if (lowestLocation > seeds[i])
    {
      lowestLocation = seeds[i];
    }
  }
  u_int lowestLocation_range = ranges[0].first;
  for (int i = 1; i < ranges.size(); i++)
  {
    if (lowestLocation_range > ranges[i].first)
    {
      lowestLocation_range = ranges[i].first;
      printf("[%u, %u] ", ranges[i].first, ranges[i].second);
    }
  }
  printf("Lowest location: %i\n", lowestLocation);
  printf("Lowest range location: %i\n", lowestLocation_range);
}
