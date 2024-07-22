#include <string>
#include <Logger.hpp>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <functional>
#include <communicationDefinitions.hpp>
#include "WidgetTemplate.hpp"

using namespace std;

class dropdownWidget: public WidgetTemplate {
  public:
    void setLabel(string _label);
    void setTooltip(string _tooltip);
    void setStyle(string _style);
    void setText(string _text);
    void setDefaultSelected(int _selected);
    void setIsSearchable(bool _isSearchable);
    bool addEntry(string entry, int position);
    bool removeEntry(int position);
    void setChangeHandler(function<void(int)> _changeHandler);
    int getValue();
    bool addIncomingValue(string InputValue, string path = "");
    YAML::Node getYAMLNode();
    void setValue(string value);
  private:
    function<void(int)> changeHandler;

    YAML::Node dropdownYAML;
    vector<int> usedPositions;
    bool isSearchable = false;
    int selected = -1;
    string label;
    string tooltip;
    string style;
    string text;
};
