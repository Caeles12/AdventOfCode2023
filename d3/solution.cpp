#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

char GEAR_SYMBOL = '*';
int GEAR_PART_QUANTITY = 2;

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  vector<vector<int>> numbersTab;
  vector<string> textTab;

  // Parse the file for the numbers
  while (file.good())
  {
    file >> line;
    textTab.push_back(line);
    vector<int> lineNumbers;
    for (int i = 0; i < line.size(); i++)
    {
      if (line[i] >= '0' && line[i] <= '9')
      {
        int start = i;
        string numberString;

        while (line[i] >= '0' && line[i] <= '9' && i < line.size())
        {
          numberString += line[i];
          i++;
        }
        i--;
        int number = stoi(numberString);
        for (int k = 0; k < numberString.size(); k++)
        {
          lineNumbers.push_back(number);
        }
      }
      else
      {
        lineNumbers.push_back(0);
      }
    }
    numbersTab.push_back(lineNumbers);
  }

  int sum = 0;
  int gearRatio = 0;

  // Look for numbers next to symbols
  for (int i = 0; i < textTab.size(); i++)
  {
    for (int j = 0; j < textTab[i].size(); j++)
    {
      char c = textTab[i][j];
      bool isGear = c == GEAR_SYMBOL;
      int partQuantity = 0;
      int partsRatio = 1;

      // If the character is a symbol
      if ((c < '0' || c > '9') && c != '.')
      {
        // Check for number on the left
        if (j > 0 && textTab[i][j - 1] >= '0' && textTab[i][j - 1] <= '9')
        {
          sum += numbersTab[i][j - 1];
          partsRatio *= numbersTab[i][j - 1];
          partQuantity += 1;
        }
        // Check for numbers on the right
        if (j < textTab[i].size() && textTab[i][j + 1] >= '0' && textTab[i][j + 1] <= '9')
        {
          sum += numbersTab[i][j + 1];
          partsRatio *= numbersTab[i][j + 1];
          partQuantity += 1;
        }
        // Check for numbers directly on top
        if (i > 0 && textTab[i - 1][j] >= '0' && textTab[i - 1][j] <= '9')
        {

          sum += numbersTab[i - 1][j];
          partsRatio *= numbersTab[i - 1][j];
          partQuantity += 1;
        }
        // Or numbers in diagonally on top
        else
        {
          if (i > 0 && j > 0 && textTab[i - 1][j - 1] >= '0' && textTab[i - 1][j - 1] <= '9')
          {
            sum += numbersTab[i - 1][j - 1];
            partsRatio *= numbersTab[i - 1][j - 1];
            partQuantity += 1;
          }
          if (i > 0 && j < textTab[i].size() && textTab[i - 1][j + 1] >= '0' && textTab[i - 1][j + 1] <= '9')
          {
            sum += numbersTab[i - 1][j + 1];
            partsRatio *= numbersTab[i - 1][j + 1];
            partQuantity += 1;
          }
        }
        // Check for numbers directly below
        if (i < textTab.size() && textTab[i + 1][j] >= '0' && textTab[i + 1][j] <= '9')
        {

          sum += numbersTab[i + 1][j];
          partsRatio *= numbersTab[i + 1][j];
          partQuantity += 1;
        }
        // Or numbers diagonally below
        else
        {
          if (i < textTab.size() && j > 0 && textTab[i + 1][j - 1] >= '0' && textTab[i + 1][j - 1] <= '9')
          {
            sum += numbersTab[i + 1][j - 1];
            partsRatio *= numbersTab[i + 1][j - 1];
            partQuantity += 1;
          }
          if (i < textTab.size() && j < textTab[i].size() && textTab[i + 1][j + 1] >= '0' && textTab[i + 1][j + 1] <= '9')
          {
            sum += numbersTab[i + 1][j + 1];
            partsRatio *= numbersTab[i + 1][j + 1];
            partQuantity += 1;
          }
        }
      }
      if (isGear && partQuantity == GEAR_PART_QUANTITY)
      {
        gearRatio += partsRatio;
      }
    }

    // std::cout << textTab[i] << " | " << sum << " | " << i << std::endl;
  }
  std::cout << "Sum of all the parts numbers: " << sum << std::endl;
  std::cout << "Total gear ratio: " << gearRatio << std::endl;
}