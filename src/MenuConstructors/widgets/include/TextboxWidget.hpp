#include <string>
#include <Logger.hpp>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <functional>
#include <communicationDefinitions.hpp>
#include "WidgetTemplate.hpp"

using namespace std;

class textboxWidget: public WidgetTemplate {
  public:
    void setLabel(string _label);
    void setTooltip(string _tooltip);
    void setStyle(string _style);
    void setText(string _text);
    void setIsEditable(bool _isEditable);
    void setHasAppyButton(bool _hasApplyButton);
    void setChangeHandler(function<void(string)> _changeHandler);
    string getValue();
    bool addIncomingValue(string value, string path = "");
    YAML::Node getYAMLNode();
  private:
    function<void(string)> changeHandler;
    string label;
    string tooltip;
    string style;
    string text;
    bool isEditable = true;
    bool hasApplyButton = true;
};
