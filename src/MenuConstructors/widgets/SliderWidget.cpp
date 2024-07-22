#include "include/SliderWidget.hpp"

void sliderWidget::setLabel(string _label) {
  label = _label;
}

void sliderWidget::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

void sliderWidget::setStyle(string _style) {
  style = _style;
}

void sliderWidget::setMin(double _min) {
  min = _min;
}

void sliderWidget::setMax(double _max){
  max = _max;
}

void sliderWidget::setStep(double _step){
  step = _step;
}

void sliderWidget::setClimbRate(double _climbRate){
  climbRate = _climbRate;
}

void sliderWidget::setDigits(int _digits){
  digits = _digits;
}

void sliderWidget::setIsNumeric(bool _numeric){
  numeric = _numeric;
}

void sliderWidget::setIsSnap(bool _snap){
  snap = _snap;
}

void sliderWidget::setIsWraparound(bool _wraparound){
  wraparound = _wraparound;
}

void sliderWidget::setValue(double _value) {
  value = _value;
}

void sliderWidget::setChangeHandler(function<void (double)> _changeHandler) {
  changeHandler = _changeHandler;
}

double sliderWidget::getValue() {
  return value;
}

bool sliderWidget::addIncomingValue(string _value, string) {
 try {
    value = stod(_value);
    Logger::debug("Slider value updated: " + to_string(value), LOG_AEREA_WIDGETS);
    if (changeHandler) changeHandler(value);
  } catch (...) {
    Logger::urgent("Failed to convert the input string to an integer", LOG_AEREA_WIDGETS);
    return false;
  }
  return true;
}

YAML::Node sliderWidget::getYAMLNode() {
  YAML::Node sliderWidgetYAML;
  sliderWidgetYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_WIDGET_NUMBER_INPUT_FIELD;
  sliderWidgetYAML[REPLY_MENU_GENERAL_LABEL] = label;
  sliderWidgetYAML[REPLY_MENU_WIDGET_MINIMUM_KEY_NAME] = min;
  sliderWidgetYAML[REPLY_MENU_WIDGET_MAXIMUM_KEY_NAME] = max;
  sliderWidgetYAML[REPLY_MENU_WIDGET_STEP_KEY_NAME] = step;
  sliderWidgetYAML[REPLY_MENU_WIDGET_CLIMB_RATE_KEY_NAME] = climbRate;
  sliderWidgetYAML[REPLY_MENU_WIDGET_DIGITS_KEY_NAME] = digits;
  sliderWidgetYAML[REPLY_MENU_WIDGET_NUMERIC_KEY_NAME] = numeric;
  sliderWidgetYAML[REPLY_MENU_WIDGET_SNAP_KEY_NAME] = snap;
  sliderWidgetYAML[REPLY_MENU_WIDGET_WRAPAROUND_KEY_NAME] = wraparound;
  sliderWidgetYAML[REPLY_MENU_WIDGET_VALUE_KEY_NAME] = value;
  sliderWidgetYAML[REPLY_MENU_WIDGET_STYLE_KEY_NAME] = style;
  sliderWidgetYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  return sliderWidgetYAML;
}
