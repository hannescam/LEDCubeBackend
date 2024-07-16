#include "include/TextboxWidget.hpp"

void textboxWidget::setLabel(string _label) {
  label = _label;
}

void textboxWidget::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

void textboxWidget::setStyle(string _style) {
  style = _style;
}

void textboxWidget::setText(string _text) {
  text = _text;
}

void textboxWidget::setIsEditable(bool _isEditable) {
  isEditable = _isEditable;
}

void textboxWidget::setHasAppyButton(bool _hasApplyButton) {
  hasApplyButton = _hasApplyButton;
}

void textboxWidget::setChangeHandler(function<void (string)> _changeHandler) {
  changeHandler = _changeHandler;
}

string textboxWidget::getValue() {
  return text;
}

bool textboxWidget::addIncomingValue(string value, string) {
  if (isEditable) {
    text = value;
    return true;
  }
  return false;
}

YAML::Node textboxWidget::getYAMLNode() {
  YAML::Node textboxWidgetYAML;
  textboxWidgetYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_WIDGET_TEXT_BOX;
  textboxWidgetYAML[REPLY_MENU_GENERAL_LABEL] = label;
  textboxWidgetYAML[REPLY_MENU_GENERAL_CONTENT_SECTION_KEY_NAME] = text;
  textboxWidgetYAML[REPLY_MENU_WIDGET_IS_EDITABLE_KEY_NAME] = isEditable;
  textboxWidgetYAML[REPLY_MENU_WIDGET_HAS_APPLY_BUTTON_KEY_NAME] = hasApplyButton;
  textboxWidgetYAML[REPLY_MENU_WIDGET_STYLE_KEY_NAME] = style;
  textboxWidgetYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  return textboxWidgetYAML;
}
