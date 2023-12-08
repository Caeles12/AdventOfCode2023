#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;
string START_NODE = "AAA";
string END_NODE = "ZZZ";

long long gcd(long long int a, long long int b)
{
  if (b == 0)
    return a;
  return gcd(b, a % b);
}

long long lcm(long long a, long long b)
{
  if (a > b)
    return (a / gcd(a, b)) * b;
  else
    return (b / gcd(a, b)) * a;
}

bool allDefined(vector<int> loopSize)
{
  for (int size : loopSize)
  {
    if (size == -1)
    {
      return false;
    }
  }
  return true;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string pattern;

  string input;
  map<string, pair<string, string>> network;
  string currentNode = START_NODE;
  vector<string> ghostNodes;
  vector<int> loopSize;

  int steps{0};
  int ghostSteps{0};

  file >> pattern;

  getline(file, input);

  while (file.good())
  {
    string node;
    string leftConnexion;
    string rightConnexion;

    file >> node;

    file >> input;

    file >> leftConnexion;
    leftConnexion = leftConnexion.substr(1, 3);
    file >> rightConnexion;
    rightConnexion = rightConnexion.substr(0, 3);
    if (node[2] == 'A')
    {
      ghostNodes.push_back(node);
      loopSize.push_back(-1);
    }
    network.insert({node, {leftConnexion, rightConnexion}});
  }

  // Part 1
  while (currentNode != END_NODE)
  {
    if (pattern[steps % pattern.size()] == 'L')
    {
      currentNode = network.find(currentNode)->second.first;
    }
    else
    {
      currentNode = network.find(currentNode)->second.second;
    }
    steps++;
  }

  // Part 2
  while (!allDefined(loopSize))
  {
    for (int i = 0; i < ghostNodes.size(); i++)
    {
      if (ghostNodes[i][2] == 'Z' && loopSize[i] == -1)
      {
        loopSize[i] = ghostSteps;
      }
      if (pattern[ghostSteps % pattern.size()] == 'L')
      {
        ghostNodes[i] = network.find(ghostNodes[i])->second.first;
      }
      else
      {
        ghostNodes[i] = network.find(ghostNodes[i])->second.second;
      }
    }
    ghostSteps++;
  }

  u_long total = lcm(loopSize[0], loopSize[1]);
  for (int i = 1; i < ghostNodes.size(); i++)
  {
    total = lcm(total, loopSize[i]);
  }
  printf("Found %s in %i steps!\n", END_NODE.c_str(), steps);
  printf("For the ghosts, it took %lu steps!\n", total);
}