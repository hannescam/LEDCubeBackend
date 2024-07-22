#include <string>
#include <vector>
#include <Logger.hpp>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <functional>
#include <communicationDefinitions.hpp>
#include "WidgetTemplate.hpp"
#include "WidgetTemplate.hpp"

using namespace std;

class expanderWidget: public WidgetTemplate {
  public:
    void setLabel(string _label);
    void setTooltip(string _tooltip);
    void setIsToggleable(bool _isToggleable);
    void setValue(bool _value);
    bool addWidget(WidgetTemplate* widget, unsigned long position);
    bool removeWidget(unsigned long position);
    void setChangeHandler(function<void(bool)> _changeHandler);
    bool getValue();
    bool addIncomingValue(string InputValue, string path = "");
    YAML::Node getYAMLNode();
  private:
    function<void(bool)> changeHandler;
    bool path2Position(string& path, unsigned long& position);
    WidgetTemplate* emptyWidget;
    YAML::Node expanderWidgetYAML;
    string label;
    string tooltip;
    bool isToggleable = false;
    bool value = false;
    vector<int> usedPositions;
    vector<WidgetTemplate*> widgets;
};
