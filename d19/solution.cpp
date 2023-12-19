#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#define number long

using namespace std;

const string ACCEPTED_WORKFLOW = "A";
const string REJECTED_WORKFLOW = "R";
const string START_WORKFLOW = "in";

typedef enum
{
  INF = 0,
  SUP = 1,
} EComparison;

typedef enum
{
  EXTREMELY_COOL_LOOKING = 0,
  MUSICAL = 1,
  AERODYNAMIC = 2,
  SHINY = 3,
} ECategory;

vector<ECategory> categories = {ECategory::EXTREMELY_COOL_LOOKING, ECategory::AERODYNAMIC, ECategory::MUSICAL, ECategory::SHINY};

map<char, ECategory> charToCategory = {
    {'x', ECategory::EXTREMELY_COOL_LOOKING},
    {'m', ECategory::MUSICAL},
    {'a', ECategory::AERODYNAMIC},
    {'s', ECategory::SHINY}};

map<ECategory, char> categoryToChar = {
    {ECategory::EXTREMELY_COOL_LOOKING, 'x'},
    {ECategory::MUSICAL, 'm'},
    {ECategory::AERODYNAMIC, 'a'},
    {ECategory::SHINY, 's'}};

map<char, EComparison> charToComparison = {
    {'>', EComparison::SUP},
    {'<', EComparison::INF}};

struct rule
{
  ECategory category;
  EComparison comparison;
  number value;
  string workflow;
};

struct workflow
{
  string name;
  vector<rule> rules;
  string nextWorkflow;
};

void showRanges(map<ECategory, pair<number, number>> ranges)
{
  printf("{");
  for (int i = 0; i < categories.size(); i++)
  {
    number start = ranges[categories[i]].first;
    number end = ranges[categories[i]].second;
    printf("%c: (%i, %i); ", categoryToChar.at(categories[i]), start, end);
  }
  printf("}\n");
}

class Node
{
public:
  string name;
  map<ECategory, pair<number, number>> ranges;
  vector<Node *> children;

  Node(string name, map<ECategory, pair<number, number>> ranges)
  {
    // showRanges(ranges);
    this->name = name;
    this->ranges = ranges;
  }
};

workflow parseWorkflow(string workflowString)
{
  string name;
  vector<rule> rules;
  string nextWorkflow;

  workflow wf;

  workflowString.pop_back();

  stringstream ss(workflowString);

  getline(ss, name, '{');
  ;

  string r;
  vector<string> rulesString;

  while (getline(ss, r, ','))
  {
    rulesString.push_back(r);
  }
  nextWorkflow = rulesString[rulesString.size() - 1];

  for (int i = 0; i < rulesString.size() - 1; i++)
  {
    rule newRule;
    stringstream ruleStream(rulesString[i]);
    string part;
    getline(ruleStream, part, ':');

    newRule.category = charToCategory.at(part[0]);
    newRule.comparison = charToComparison.at(part[1]);

    part.erase(0, 2);

    newRule.value = stoi(part);

    getline(ruleStream, part);

    newRule.workflow = part;
    rules.push_back(newRule);
  }
  wf.name = name;
  wf.nextWorkflow = nextWorkflow;
  wf.rules = rules;
  return wf;
}

string runWorkflow(map<ECategory, number> values, workflow wf)
{
  for (rule r : wf.rules)
  {
    number currentValue = values.at(r.category);
    if (r.comparison == EComparison::INF && currentValue < r.value)
    {
      return r.workflow;
    }
    else if (r.comparison == EComparison::SUP && currentValue > r.value)
    {
      return r.workflow;
    }
  }
  return wf.nextWorkflow;
}

map<ECategory, number> parsePart(string partsStrings)
{
  map<ECategory, number> part;
  partsStrings.erase(0, 1);

  partsStrings.pop_back();

  stringstream ss(partsStrings);
  string current;
  while (getline(ss, current, ','))
  {
    ECategory cat = charToCategory.at(current[0]);
    current.erase(0, 2);
    int n = stoi(current);
    part[cat] = n;
  }
  return part;
}

string partResult(map<ECategory, number> part, map<string, workflow> workflows)
{
  string currentWorkflow = START_WORKFLOW;
  while (currentWorkflow != ACCEPTED_WORKFLOW && currentWorkflow != REJECTED_WORKFLOW)
  {
    currentWorkflow = runWorkflow(part, workflows.at(currentWorkflow));
  }
  return currentWorkflow;
}

number calcPossibilities(map<ECategory, pair<number, number>> ranges)
{
  number possibilities = 1;
  for (ECategory cat : categories)
  {
    number range = (ranges.at(cat).second - ranges.at(cat).first) + 1;
    possibilities *= range;
  }
  return possibilities;
}

number buildTree(Node &root, map<string, workflow> workflows)
{
  if (root.name == ACCEPTED_WORKFLOW || root.name == REJECTED_WORKFLOW)
  {
    if (root.name == ACCEPTED_WORKFLOW)
    {
      number possibilities = calcPossibilities(root.ranges);
      return possibilities;
    }
    return 0;
  }
  workflow wf = workflows.at(root.name);
  map<ECategory, pair<number, number>> ranges = root.ranges;

  number sum = 0;

  for (rule r : wf.rules)
  {
    map<ECategory, pair<number, number>> newRanges = ranges;
    if (r.comparison == EComparison::INF)
    {

      newRanges.at(r.category).second = r.value - 1;
      ranges.at(r.category).first = r.value;
    }
    if (r.comparison == EComparison::SUP)
    {
      newRanges.at(r.category).first = r.value + 1;
      ranges.at(r.category).second = r.value;
    }
    Node newNode(r.workflow, newRanges);
    root.children.push_back(&newNode);
    sum += buildTree(newNode, workflows);
  }
  Node newNode(wf.nextWorkflow, ranges);
  root.children.push_back(&newNode);
  sum += buildTree(newNode, workflows);
  return sum;
}

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;

  map<string, workflow> workflows;

  getline(file, line);
  while (line.size() != 0)
  {
    workflow wf = parseWorkflow(line);
    workflows[wf.name] = wf;
    getline(file, line);
  }
  int sum = 0;
  while (file.good())
  {
    getline(file, line);
    map<ECategory, number> part = parsePart(line);
    string result = partResult(part, workflows);
    if (result == ACCEPTED_WORKFLOW)
    {
      int value = part.at(ECategory::EXTREMELY_COOL_LOOKING) + part.at(ECategory::AERODYNAMIC) + part.at(ECategory::MUSICAL) + part.at(ECategory::SHINY);
      sum += value;
    }
  }
  printf("Sum of the rating number of all the valid parts: %i.\n", sum);

  map<ECategory, pair<number, number>> initialRanges;
  initialRanges[ECategory::EXTREMELY_COOL_LOOKING] = {1, 4000};
  initialRanges[ECategory::AERODYNAMIC] = {1, 4000};
  initialRanges[ECategory::MUSICAL] = {1, 4000};
  initialRanges[ECategory::SHINY] = {1, 4000};

  Node startNode("in", initialRanges);
  number total = buildTree(startNode, workflows);
  printf("Total distinct combinations of ratings that will be accepted: %lu.\n", total);
}