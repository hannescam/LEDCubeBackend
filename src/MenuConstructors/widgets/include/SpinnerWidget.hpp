#include <string>
#include <Logger.hpp>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <communicationDefinitions.hpp>
#include "WidgetTemplate.hpp"

using namespace std;

class spinnerWidget: public WidgetTemplate {
  public:
    void setTimeout(int _timeout);
    void setTooltip(string _tooltip);
    bool addIncomingValue(string value = "", string path = "");
    YAML::Node getYAMLNode();
  private:
    int timeout = -1;
    string tooltip;
};
