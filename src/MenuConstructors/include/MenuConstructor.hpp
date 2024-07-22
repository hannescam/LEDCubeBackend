#include <string>
#include <vector>
#include <Logger.hpp>
#include <iostream>
#include <sstream>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <communicationDefinitions.hpp>
#include "GroupConstructor.hpp"

#define TOOLTIP "tooltip"

using namespace std;

#ifndef MENU_CONSTRUCTOR
#define MENU_CONSTRUCTOR

class menuConstructor {
  public:
    void setTooltip(string _tooltip);
    void setLabel(string _label);
    void setIcon(string _icon);
    bool addGroup(groupConstructor* group, unsigned long position);
    bool removeGroup(unsigned long position);
    bool addIncomingValue(string value, string path);
    string getYAMLString(string messageId);
  private:
    bool path2Position(string& path, unsigned long& position);
    groupConstructor* emptyGroup;
    YAML::Node menuYAML;
    string tooltip;
    string label;
    string icon;
    vector<groupConstructor*> groupConstructors;
};
#endif
