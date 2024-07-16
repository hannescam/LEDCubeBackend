#include "include/ButtonWidget.hpp"

void buttonWidget::setLabel(string _label) {
  label = _label;
}

void buttonWidget::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

void buttonWidget::setStyle(string _style) {
  style = _style;
}

void buttonWidget::setIcon(string _icon) {
  icon = _icon;
}

void buttonWidget::setUseRow(bool _useRow) {
  useRow = _useRow;
}

void buttonWidget::setClickHandler(function<void ()> _clickHandler) {
  clickHandler = _clickHandler;
}

bool buttonWidget::addIncomingValue(string, string) {
  if (clickHandler) clickHandler();
  return true;
}

YAML::Node buttonWidget::getYAMLNode() {
  YAML::Node buttonWidgetYAML;
  buttonWidgetYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_WIDGET_BUTTON;
  buttonWidgetYAML[REPLY_MENU_GENERAL_LABEL] = label;
  buttonWidgetYAML[REPLY_MENU_WIDGET_ICON_KEY_NAME] = icon;
  buttonWidgetYAML[REPLY_MENU_WIDGET_STYLE_KEY_NAME] = style;
  buttonWidgetYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  buttonWidgetYAML[REPLY_MENU_WIDGET_USE_ROW_KEY_NAME] = useRow;
  return buttonWidgetYAML;
}
