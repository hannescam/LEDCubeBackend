#include "include/PropertyWidget.hpp"

void propertyWidget::setLabel(string _label) {
  label = _label;
}

void propertyWidget::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

void propertyWidget::setStyle(string _style) {
  style = _style;
}

void propertyWidget::setText(string _text) {
  text = _text;
}

bool propertyWidget::addIncomingValue(string, string) {
  cerr << "Tried to add a value to a property widget (non-editable textbox)" << endl;
  return false;
}

YAML::Node propertyWidget::getYAMLNode() {
  YAML::Node propertyWidgetYAML;
  propertyWidgetYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_WIDGET_PROPERTY;
  propertyWidgetYAML[REPLY_MENU_GENERAL_LABEL] = label;
  propertyWidgetYAML[REPLY_MENU_GENERAL_CONTENT_SECTION_KEY_NAME] = text;
  propertyWidgetYAML[REPLY_MENU_WIDGET_STYLE_KEY_NAME] = style;
  propertyWidgetYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  return propertyWidgetYAML;
}
