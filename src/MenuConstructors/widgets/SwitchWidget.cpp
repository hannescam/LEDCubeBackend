#include "include/SwitchWidget.hpp"

void switchWidget::setLabel(string _label) {
  label = _label;
}

void switchWidget::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

void switchWidget::setStyle(string _style) {
  style = _style;
}

void switchWidget::setValue(bool _value) {
  value = _value;
}

void switchWidget::setSwitchHandler(function<void(bool)> _switchHandler) {
  switchHandler = _switchHandler;
}

bool switchWidget::getValue() {
  return value;
}

bool switchWidget::addIncomingValue(string InputValue, string) {
  if (InputValue == "true") {
    value = true;
  } else if (InputValue == "false") {
    value = false;
  } else {
    Logger::urgent("Switch inputValue doesn't contain 'true' or 'false' when trying to parse it as bool", LOG_AEREA_WIDGETS);
    return false;
  }
  Logger::debug("Switch position updated: " + to_string(value), LOG_AEREA_WIDGETS);
  if (switchHandler) switchHandler(value);
  return true;
}

YAML::Node switchWidget::getYAMLNode() {
  YAML::Node switchWidgetYAML;
  switchWidgetYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_WIDGET_SWITCH;
  switchWidgetYAML[REPLY_MENU_GENERAL_LABEL] = label;
  switchWidgetYAML[REPLY_MENU_WIDGET_VALUE_KEY_NAME] = value;
  switchWidgetYAML[REPLY_MENU_WIDGET_STYLE_KEY_NAME] = style;
  switchWidgetYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  return switchWidgetYAML;
}
