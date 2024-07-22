#include "include/SpinnerWidget.hpp"

void spinnerWidget::setTimeout(int _timeout) {
  timeout = _timeout;
}

void spinnerWidget::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

bool spinnerWidget::addIncomingValue(string, string) {
  Logger::urgent("Tried to add a value to a spinner widget", LOG_AEREA_WIDGETS);
  return false;
}

YAML::Node spinnerWidget::getYAMLNode() {
  YAML::Node buttonWidgetYAML;
  buttonWidgetYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_WIDGET_SPINNER;
  buttonWidgetYAML[REPLY_MENU_WIDGET_TIMOUT_KEY_NAME] = timeout;
  buttonWidgetYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  return buttonWidgetYAML;
}
