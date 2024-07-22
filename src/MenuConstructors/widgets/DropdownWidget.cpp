#include "include/DropdownWidget.hpp"

void dropdownWidget::setLabel(string _label) {
  label = _label;
}

void dropdownWidget::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

void dropdownWidget::setStyle(string _style) {
  style = _style;
}

void dropdownWidget::setText(string _text) {
  text = _text;
}

void dropdownWidget::setIsSearchable(bool _isSearchable) {
  isSearchable = _isSearchable;
}

void dropdownWidget::setChangeHandler(function<void (int)> _changeHandler) {
  changeHandler = _changeHandler;
}

int dropdownWidget::getValue() {
  return selected;
}

bool dropdownWidget::addIncomingValue(string InputValue, string) {
  try {
    selected = stoi(InputValue);
    Logger::debug("Dropdown selection updated: " + to_string(selected), LOG_AEREA_WIDGETS);
    if (changeHandler) changeHandler(selected);
  } catch (...) {
    Logger::urgent("Failed to convert the input string to an integer", LOG_AEREA_WIDGETS);
    return false;
  }
  return true;
}

bool dropdownWidget::addEntry(string widget, int position) {
  if (find(usedPositions.begin(), usedPositions.end(), position) != usedPositions.end()) {
    return false;
  }

  dropdownYAML[REPLY_MENU_WIDGET_DROPDOWN_CONTENT_SECTION_KEY_NAME][position] = widget;
  usedPositions.resize(usedPositions.size()+1);
  usedPositions.at(usedPositions.size()-1) = position;
  return true;
}

bool dropdownWidget::removeEntry(int position) {
  return dropdownYAML.remove(position);
}

YAML::Node dropdownWidget::getYAMLNode() {
  dropdownYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_WIDGET_DROPDOWN;
  dropdownYAML[REPLY_MENU_GENERAL_LABEL] = label;
  dropdownYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  dropdownYAML[REPLY_MENU_WIDGET_STYLE_KEY_NAME] = style;
  dropdownYAML[REPLY_MENU_GENERAL_CONTENT_SECTION_KEY_NAME] = text;
  dropdownYAML[REPLY_MENU_WIDGET_DEFAULT_SELECTED_KEY_NAME] = selected;
  dropdownYAML[REPLY_MENU_WIDGET_IS_SEARCHABLE_KEY_NAME] = isSearchable;
  return dropdownYAML;
}
