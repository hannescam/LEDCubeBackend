#include <string>
#include <Logger.hpp>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <communicationDefinitions.hpp>
#include "WidgetTemplate.hpp"

using namespace std;

class propertyWidget: public WidgetTemplate {
  public:
    void setLabel(string _label);
    void setTooltip(string _tooltip);
    void setStyle(string _style);
    void setText(string _text);
    bool addIncomingValue(string value = "", string path = "");
    YAML::Node getYAMLNode();
  private:
    string label;
    string tooltip;
    string style;
    string text;
};
