#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <memory>

#define number long

using namespace std;

typedef enum
{
  HIGH = 1,
  LOW = 0,
  NONE = -1,
} EPulse;

map<EPulse, string> pulseToString = {
    {EPulse::HIGH, "high"},
    {EPulse::LOW, "low"},
    {EPulse::NONE, "none"},
};

// Base Module class, all modules implement this
class Module
{
protected:
  vector<Module *> outputs;
  vector<Module *> inputs;
  queue<pair<Module *, EPulse>> receptionQueue;

  int highPulseSent;
  int lowPulseSent;

  EPulse nextPulse;

  string name;

  virtual void attachInput(Module *m)
  {
    this->inputs.push_back(m);
  }

public:
  Module() : nextPulse(EPulse::NONE), name(""), highPulseSent(0), lowPulseSent(0){};
  Module(string name) : nextPulse(EPulse::NONE), name(name), highPulseSent(0), lowPulseSent(0)
  {
  }

  virtual void receive(EPulse pulse, Module *m) = 0;
  virtual bool isStartState() = 0;

  void emit(EPulse pulse)
  {
    this->nextPulse = pulse;
  }

  virtual void processEmission()
  {
    for (Module *m : outputs)
    {
      if (this->nextPulse != EPulse::NONE)
      {
        receptionQueue.push({m, this->nextPulse});
        if (this->nextPulse == EPulse::HIGH)
        {
          this->highPulseSent++;
        }
        else
        {
          this->lowPulseSent++;
        }
      }
    }
    this->nextPulse = EPulse::NONE;
  }

  void proccessReceptions()
  {
    while (!receptionQueue.empty())
    {
      pair<Module *, EPulse> signal = receptionQueue.front();
      signal.first->receive(signal.second, this);
      // printf("%s -%s-> %s\n", this->name.c_str(), pulseToString.at(signal.second).c_str(), signal.first->name.c_str());
      receptionQueue.pop();
    }
  }

  void attach(Module *m)
  {
    this->outputs.push_back(m);
    m->attachInput(this);
  }

  bool active()
  {
    return this->nextPulse != EPulse::NONE;
  }

  int getHighPulseSent()
  {
    return this->highPulseSent;
  }
  int getLowPulseSent()
  {
    return this->lowPulseSent;
  }
};

// Either On or Off, flips when it receive a low pulse.
// If it was off, it turns on and sends a high pulse. If it was on, it turns off and sends a low pulse
class FlipFlop : public Module
{
  using Module::Module;

private:
  bool activated;

public:
  FlipFlop(string name) : activated(false), Module(name)
  {
  }
  void receive(EPulse pulse, Module *m) override
  {
    if (pulse == EPulse::LOW)
    {
      if (activated)
      {
        activated = false;
        this->emit(EPulse::LOW);
      }
      else
      {
        activated = true;
        this->emit(EPulse::HIGH);
      }
    }
  }
  bool isStartState() override
  {
    return !activated;
  }
};

// Remembers pulse for all of its inputs. If it then remembers all HIGH, emit HIGH, else emit LOW.
class Conjunction : public Module
{
  using Module::Module;

private:
  int timesPassed = 0;
  map<Module *, EPulse> memory;

  void attachInput(Module *m) override
  {
    this->inputs.push_back(m);
    this->memory[m] = EPulse::LOW;
  }

public:
  void receive(EPulse pulse, Module *m) override
  {
    this->memory[m] = pulse;
    this->emit(EPulse::HIGH);
    timesPassed++;
  }

  void processEmission() override
  {
    if (this->nextPulse == EPulse::HIGH)
    {
      this->nextPulse = EPulse::LOW;

      for (Module *mod : inputs)
      {
        if (this->memory[mod] == EPulse::LOW)
        {
          this->nextPulse = EPulse::HIGH;
          break;
        }
      }
    }
    for (int i = 0; i < timesPassed; i++)
    {
      for (Module *m : outputs)
      {
        if (this->nextPulse != EPulse::NONE)
        {
          receptionQueue.push({m, this->nextPulse});
          if (this->nextPulse == EPulse::HIGH)
          {
            this->highPulseSent++;
          }
          else
          {
            this->lowPulseSent++;
          }
        }
      }
    }
    timesPassed = 0;
    this->nextPulse = EPulse::NONE;
  }

  bool isStartState() override
  {
    for (auto &m : memory)
    {
      if (m.second != EPulse::LOW)
      {
        return false;
      };
    }
    return true;
  }
};

// When it receives a pulse, it sends the same pulse to all of its destination modules
class Broadcast : public Module
{
  using Module::Module;

public:
  void receive(EPulse pulse, Module *m) override
  {
    this->emit(pulse);
  }

  bool isStartState() override
  {
    return true;
  }
};

// hen you push the button, a single low pulse is sent
class Button : public Module
{
  using Module::Module;

public:
  void receive(EPulse pulse, Module *m) override
  {
    return;
  }

  bool isStartState() override
  {
    return true;
  }
};

// hen you push the button, a single low pulse is sent
class UntypedModule : public Module
{
  using Module::Module;

public:
  void receive(EPulse pulse, Module *m) override
  {
    return;
  }

  bool isStartState() override
  {
    return true;
  }
};

int main(int argc, char *argv[])
{
  ifstream file(argv[1]);
  string line;
  map<string, unique_ptr<Module>> modules = {};
  vector<pair<string, string>> connexions;

  modules["button"] = make_unique<Button>("button");
  modules["exitModule"] = make_unique<UntypedModule>("exitModule");

  connexions.push_back({"button", "broadcaster"});

  while (file.good())
  {
    getline(file, line);
    string word;
    stringstream ss(line);

    string name;
    getline(ss, word, ' ');

    // Generate the module
    if (word.compare("broadcaster") == 0)
    {
      name = word;
      modules[name] = make_unique<Broadcast>(name);
    }
    else if (word[0] == '%')
    {
      name = word.erase(0, 1);

      FlipFlop ff = FlipFlop(name);
      modules[name] = make_unique<FlipFlop>(name);
    }
    else if (word[0] == '&')
    {
      name = word.erase(0, 1);
      Conjunction cj = Conjunction(name);
      modules[name] = make_unique<Conjunction>(name);
    }

    getline(ss, word, ' ');
    while (getline(ss, word, ','))
    {
      if (word[0] == ' ')
      {
        word.erase(0, 1);
      }
      connexions.push_back({name, word});
    }
  }

  for (pair<string, string> connexion : connexions)
  {
    if (modules.find(connexion.second) != modules.end())
    {
      modules.at(connexion.first)->attach(modules.at(connexion.second).get());
    }
    else
    {
      modules.at(connexion.first)->attach(modules.at("exitModule").get());
    }
  }
  bool loop = false;
  int loopCount = 0;
  while (!loop && loopCount < 1000)
  {
    loopCount++;
    // printf("Button press n°%i\n", loopCount);
    modules["button"]->emit(EPulse::LOW);
    bool finished = false;
    while (!finished)
    {
      {
        for (auto &m : modules)
        {
          m.second->processEmission();
        }
        for (auto &m : modules)
        {
          m.second->proccessReceptions();
        }

        finished = true;
        for (auto &m : modules)
        {
          if (m.second->active())
          {
            finished = false;
            break;
          };
        }
      }
    }
    loop = true;
    for (auto &m : modules)
    {
      if (!m.second->isStartState())
      {
        loop = false;
        break;
      }
    }
    int totalHighPulseSent = 0;
    int totalLowPulseSent = 0;
    for (auto &m : modules)
    {
      totalHighPulseSent += m.second->getHighPulseSent();
      totalLowPulseSent += m.second->getLowPulseSent();
    }
    // printf("%i\t%i\n", totalHighPulseSent, totalLowPulseSent);
  }
  int totalHighPulseSent = 0;
  int totalLowPulseSent = 0;
  for (auto &m : modules)
  {
    totalHighPulseSent += m.second->getHighPulseSent();
    totalLowPulseSent += m.second->getLowPulseSent();
  }

  int loops = 1000 / loopCount;
  totalHighPulseSent *= loops;
  totalLowPulseSent *= loops;
  printf("%i high pulses and %i low pulses in the loop\n", totalHighPulseSent, totalLowPulseSent);
  long total = totalHighPulseSent * totalLowPulseSent;
  printf("The total score is %lu.\n", total);
}