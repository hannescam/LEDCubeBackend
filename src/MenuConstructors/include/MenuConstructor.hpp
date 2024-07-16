#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <communicationDefinitions.hpp>
#include "GroupConstructor.hpp"

#define TOOLTIP "tooltip"

using namespace std;

class menuConstructor {
  public:
    void setTooltip(string _tooltip);
    void setMessageId(string _messageId);
    void setLabel(string _label);
    void setIcon(string _icon);
    bool addGroup(groupConstructor* group, unsigned long position);
    bool removeGroup(unsigned long position);
    bool addIncomingValue(string value, string path);
    string getYAMLString();
  private:
    bool path2Position(string& path, unsigned long& position);
    groupConstructor* emptyGroup;
    YAML::Node menuYAML;
    string tooltip;
    string messageId;
    string label;
    string icon;
    vector<groupConstructor*> groupConstructors;
};
