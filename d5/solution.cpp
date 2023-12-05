#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

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

vector<u_int> processConversionSingleSeed(vector<u_int> prevNumbers, ifstream &file)
{
  string line;
  vector<u_int> newNumbers;
  for (int i = 0; i < prevNumbers.size(); i++)
    newNumbers.push_back(prevNumbers[i]);

  getline(file, line);
  getline(file, line);

  while (file.good() && line.length() > 0)
  {
    vector<u_int> conversion = parseNumbers(line, ' ');
    for (int i = 0; i < prevNumbers.size(); i++)
    {
      if (prevNumbers[i] >= conversion[1] && prevNumbers[i] < conversion[1] + conversion[2])
      {
        int dist = prevNumbers[i] - conversion[1];
        newNumbers[i] = conversion[0] + dist;
      }
    }

    getline(file, line);
  }
  return newNumbers;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  getline(file, line, ' ');
  getline(file, line);

  vector<u_int> seeds = parseNumbers(line, ' ');

  getline(file, line);

  // Seeds to soil
  seeds = processConversionSingleSeed(seeds, file);

  // Soil to fertilizer
  seeds = processConversionSingleSeed(seeds, file);

  // Fertilizer to water
  seeds = processConversionSingleSeed(seeds, file);

  // Water to light
  seeds = processConversionSingleSeed(seeds, file);

  // Light to temperature
  seeds = processConversionSingleSeed(seeds, file);

  // Temperature to humidity
  seeds = processConversionSingleSeed(seeds, file);

  // Humidity to location
  seeds = processConversionSingleSeed(seeds, file);

  for (u_int seed : seeds)
  {
    printf("%u\n", seed);
  }
  u_int lowestLocation = seeds[0];
  for (int i = 1; i < seeds.size(); i++)
  {
    if (lowestLocation > seeds[i])
    {
      lowestLocation = seeds[i];
    }
  }
  printf("Lowest location: %i\n", lowestLocation);
}
