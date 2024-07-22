#include <string>
#include <Logger.hpp>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <functional>
#include <communicationDefinitions.hpp>
#include "WidgetTemplate.hpp"

using namespace std;

class buttonWidget: public WidgetTemplate {
  public:
    void setLabel(string _label);
    void setTooltip(string _tooltip);
    void setStyle(string _style);
    void setIcon(string _icon);
    void setUseRow(bool _useRow);
    void setClickHandler(function<void()> _clickHandler);
    bool addIncomingValue(string value = "", string path = "");
    YAML::Node getYAMLNode();
  private:
    function<void()> clickHandler;
    string label;
    string tooltip;
    string style;
    string icon;
    bool useRow = false;
};
