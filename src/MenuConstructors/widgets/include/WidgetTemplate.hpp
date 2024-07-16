#include <yaml-cpp/yaml.h>
#include <string>

using namespace std;

#ifndef WIDGET_TEMPLATE
#define WIDGET_TEMPLATE

class WidgetTemplate {
  public:
    virtual YAML::Node getYAMLNode() {
      YAML::Node node;
      return node;
    }

    virtual bool addIncomingValue(string value, string path = "") {
      cout << "path=" << path << ", value=" << value << endl;
      //if (value != "") delete &value;
      //if (path != "") delete &path;
      return true;
    }
};

#endif
