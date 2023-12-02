#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

const int MAX_RED = 12;
const int MAX_GREEN = 13;
const int MAX_BLUE = 14;

std::vector<std::pair<std::string, int>> COLORS = {
    {"red", MAX_RED},
    {"green", MAX_GREEN},
    {"blue", MAX_BLUE},
};

int main(int argc, char *argv[])
{
  std::ifstream file(argv[1]);
  std::string firstWord;
  std::string secondWord;

  int amounts[3] = {0};
  int maxs[3] = {0};

  int line = 1;
  bool validLine = true;

  int sum = 0;
  int totalPower = 0;

  while (file.good())
  {
    // Parse the line
    file >> firstWord;
    // Skip the first words of the line
    if (firstWord == "Game")
    {
      file >> firstWord >> firstWord;
    }

    int quantity = std::stoi(firstWord);
    file >> secondWord;
    char delimitor = secondWord.back();
    if (delimitor == ';' || delimitor == ',')
    {
      secondWord.pop_back();
    }

    for (int i = 0; i < COLORS.size(); i++)
    {
      if (secondWord == COLORS[i].first)
      {
        if (COLORS[i].second < quantity)
        {
          validLine = false;
        }
        if (maxs[i] < quantity)
        {
          maxs[i] = quantity;
        }
        break;
      }
    }
    if (delimitor != ',')
    {
      for (int i = 0; i < COLORS.size(); i++)
      {
        amounts[i] = 0;
      }
    }

    if (delimitor != ';' && delimitor != ',')
    {
      // Add to the sum of IDs if game is valid
      if (validLine)
      {
        sum += line;
      }

      // Total power of the game
      int mul = 1;
      for (int i = 0; i < COLORS.size(); i++)
      {
        mul *= maxs[i];
      }
      totalPower += mul;

      for (int i = 0; i < COLORS.size(); i++)
      {
        maxs[i] = 0;
      }
      validLine = true;
      line++;
    }
  }
  std::cout << "Sum of valid games IDs: " << sum << std::endl;
  std::cout << "Total power of the games: " << totalPower << std::endl;
}