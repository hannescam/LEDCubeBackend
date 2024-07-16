#include <string>
#include <vector>
#include <iostream>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <communicationDefinitions.hpp>
#include <WidgetTemplate.hpp>

using namespace std;

#ifndef GROUP_CONSTRUCTOR
#define GROUP_CONSTRUCTOR

class groupConstructor {
  public:
    void setLabel(string _label);
    void setTooltip(string _tooltip);
    bool addWidget(WidgetTemplate* widget, unsigned long position);
    bool removeWidget(unsigned long position);
    bool addIncomingValue(string value, string path);
    YAML::Node getYAMLNode();
  private:
    bool path2Position(string& path, unsigned long& position);
    WidgetTemplate* emptyWidget;
    YAML::Node groupYAML;
    string label;
    string tooltip;
    vector<int> usedPositions;
    vector<WidgetTemplate*> widgets;
};

#endif
