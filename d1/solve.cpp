#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
  std::string numbers[9] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
  std::ifstream file(argv[1]);
  std::string line;
  u_int sum = 0;

  while (file.good())
  {
    char firstDigit = -1;
    char lastDigit = -1;

    file >> line;

    for (int i = 0; i < line.size(); i++)
    {
      // Check for spelled out digits
      for (int n = 0; n < 9; n++)
      {
        std::string number = numbers[n];
        bool isNumber = true;
        for (int j = 0; j < number.size(); j++)
        {
          if (i + j >= line.size() || number[j] != line[i + j])
          {
            isNumber = false;
            break;
          }
        }

        if (isNumber)
        {
          char c = '1' + n;
          if (firstDigit == -1)
          {
            firstDigit = c;
          }
          lastDigit = c;
        }
      }

      // Check for 'normal' digits
      if (line[i] >= '0' && line[i] <= '9')
      {
        if (firstDigit == -1)
        {
          firstDigit = line[i];
        }
        lastDigit = line[i];
      }
    }
    int val = (firstDigit - '0') * 10 + (lastDigit - '0');
    sum = sum + val;
    std::cout << line << " : " << firstDigit << lastDigit << std::endl;
  }
  std::cout << sum << std::endl;
}