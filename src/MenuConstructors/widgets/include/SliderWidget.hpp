#include <string>
#include <iostream>
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <communicationDefinitions.hpp>
#include <functional>
#include "WidgetTemplate.hpp"

using namespace std;

class sliderWidget: public WidgetTemplate {
  public:
    void setLabel(string _label);
    void setTooltip(string _tooltip);
    void setStyle(string _style);
    void setValue(double _value);
    void setMin(double _min);
    void setMax(double _max);
    void setStep(double _step);
    void setClimbRate(double _climbRate);
    void setDigits(int _digits);
    void setIsNumeric(bool _numeric);
    void setIsSnap(bool _snap);
    void setIsWraparound(bool _wraparound);
    double getValue();
    void setChangeHandler(function<void(double)> _changeHandler);
    bool addIncomingValue(string value, string path = "");
    YAML::Node getYAMLNode();
  private:
    string label;
    string tooltip;
    string style;
    double value = 0;
    double min = 0;
    double max = 100;
    double step = 1;
    double climbRate = 2;
    int digits = 0;
    bool numeric = true;
    bool snap = true;
    bool wraparound = false;
    function<void(double)> changeHandler;
};
