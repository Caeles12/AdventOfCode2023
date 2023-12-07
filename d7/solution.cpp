#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<char> CARD_ORDER{'A', 'K', 'Q', 'T', '9', '8', '7', '6', '5', '4', '3', '2', 'J'};

typedef enum
{
  FiveOfAKind = 6,
  FourOfAKind = 5,
  FullHouse = 4,
  ThreeOfAKind = 3,
  TwoPair = 2,
  OnePair = 1,
  HighCard = 0,
} EHandType;

int indexOf(char c, vector<char> v)
{
  for (int i = 0; i < v.size(); i++)
  {
    if (v[i] == c)
    {
      return i;
    }
  }
  return -1;
}

EHandType getType(string hand)
{
  int counts[CARD_ORDER.size()]{0};
  int jokers{0};

  for (char c : hand)
  {
    if (c == 'J')
    {
      jokers++;
    }
    else
    {
      counts[indexOf(c, CARD_ORDER)]++;
    }
  }

  int associations[6]{0};
  for (int i = 0; i < CARD_ORDER.size(); i++)
  {
    associations[counts[i]]++;
  }

  if (jokers > 0)
  {
    for (int i = 5 - jokers; i >= 0; i--)
    {
      if (associations[i] > 0)
      {
        associations[i + jokers]++;
        associations[i]--;
        break;
      }
    }
  }

  if (associations[5] == 1)
  {
    return EHandType::FiveOfAKind;
  }
  if (associations[4] == 1 && associations[1] == 1)
  {
    return EHandType::FourOfAKind;
  }
  if (associations[3] == 1 && associations[2] == 1)
  {
    return EHandType::FullHouse;
  }
  if (associations[3] == 1 && associations[1] == 2)
  {
    return EHandType::ThreeOfAKind;
  }
  if (associations[2] == 2 && associations[1] == 1)
  {
    return EHandType::TwoPair;
  }
  if (associations[2] == 1 && associations[1] == 3)
  {
    return EHandType::OnePair;
  }
  return EHandType::HighCard;
}

bool compareHands(tuple<string, u_int, u_int> hand1, tuple<string, u_int, u_int> hand2)
{
  bool compareOnRank = (get<2>(hand1) < get<2>(hand2));
  bool compareOnCards;
  for (int i = 0; i < get<0>(hand1).size(); i++)
  {
    if (get<0>(hand1)[i] != get<0>(hand2)[i])
    {
      compareOnCards = indexOf(get<0>(hand1)[i], CARD_ORDER) > indexOf(get<0>(hand2)[i], CARD_ORDER);
      break;
    }
  }
  return compareOnRank || ((get<2>(hand1) == get<2>(hand2)) && compareOnCards);
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string hand;
  string bid;

  vector<tuple<string, u_int, u_int>> hands;

  while (file.good())
  {
    file >> hand;
    file >> bid;

    hands.push_back({hand, stoul(bid), getType(hand)});
  }
  sort(hands.begin(), hands.end(), compareHands);
  u_long total = 0;
  for (int i = 0; i < hands.size(); i++)
  {
    // printf("%s -> %i (%i)\n", get<0>(hands[i]).c_str(), get<1>(hands[i]), get<2>(hands[i]));
    total += (i + 1) * get<1>(hands[i]);
  }
  printf("Total winnings: %lu\n", total);
}