#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string card;
  string winningNumbersString;
  string currentNumbersString;

  bitset<100> winningNumbers;

  vector<u_int> cardCopies;
  cardCopies.push_back(1);

  u_int totalScore = 0;
  u_int totalCards = 0;

  u_int lineNumber = 0;
  while (file.good())
  {
    //  Parse the lines
    getline(file, card, ':');
    getline(file, winningNumbersString, '|');
    getline(file, currentNumbersString);

    // Set winning numbers
    winningNumbers.reset();
    for (int i = 0; i < winningNumbersString.size(); i++)
    {
      string buf = "";
      if (winningNumbersString[i] >= '0' && winningNumbersString[i] <= '9')
      {
        while (winningNumbersString[i] >= '0' && winningNumbersString[i] <= '9' && i < winningNumbersString.size())
        {
          buf += winningNumbersString[i];
          i++;
        }
        i--;
        winningNumbers[stoi(buf)] = 1;
      }
    }

    // Compute the number of points
    u_int winningCards = 0;
    u_int score = 0;
    for (int i = 0; i < currentNumbersString.size(); i++)
    {
      string buf = "";
      if (currentNumbersString[i] >= '0' && currentNumbersString[i] <= '9')
      {
        while (currentNumbersString[i] >= '0' && currentNumbersString[i] <= '9' && i < currentNumbersString.size())
        {
          buf += currentNumbersString[i];
          i++;
        }
        i--;
        bool isWinning = winningNumbers[stoi(buf)];
        winningCards += isWinning;
        if (isWinning)
        {
          if (score == 0)
          {
            score = 1;
          }
          else
          {
            score *= 2;
          }
        }
      }
    }

    // Compute the number of cards
    if (lineNumber >= cardCopies.size())
    {
      cardCopies.push_back(1);
    }
    for (int i = 1; i <= winningCards; i++)
    {
      if (lineNumber + i >= cardCopies.size())
      {
        cardCopies.push_back(1);
      }
      cardCopies[lineNumber + i] += cardCopies[lineNumber];
    }

    totalScore += score;
    totalCards += cardCopies[lineNumber];

    lineNumber++;
  }
  printf("Total score: %i\nTotal cards: %i\n", totalScore, totalCards);
}