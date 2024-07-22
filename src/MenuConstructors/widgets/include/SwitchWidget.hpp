#include <string>
#include <Logger.hpp>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <functional>
#include <communicationDefinitions.hpp>
#include "WidgetTemplate.hpp"

using namespace std;

class switchWidget: public WidgetTemplate {
  public:
    void setLabel(string _label);
    void setTooltip(string _tooltip);
    void setStyle(string _style);
    void setValue(bool _value);
    bool getValue();
    void setSwitchHandler(function<void(bool)> _switchHandler);
    bool addIncomingValue(string InputValue, string path = "");
    YAML::Node getYAMLNode();
  private:
    function<void(bool)> switchHandler;
    string label;
    string tooltip;
    string style;
    bool value;
};
