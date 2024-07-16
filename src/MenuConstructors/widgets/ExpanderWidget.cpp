#include "include/ExpanderWidget.hpp"

void expanderWidget::setLabel(string _label) {
  label = _label;
}

void expanderWidget::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

void expanderWidget::setIsToggleable(bool _isToggleable) {
  isToggleable = _isToggleable;
}

void expanderWidget::setValue(bool _value) {
  value = _value;
}

void expanderWidget::setChangeHandler(function<void (bool)> _changeHandler) {
  changeHandler = _changeHandler;
}

bool expanderWidget::getValue() {
  return value;
}

bool expanderWidget::path2Position(string& path, unsigned long& position) {
  if (path.find(REPLY_MENU_WIDGET_PREFIX) >= path.size()) return false;
  path.erase(path.find(REPLY_MENU_WIDGET_PREFIX), string(REPLY_MENU_WIDGET_PREFIX).size());
  position = stoi(path);
  path.erase(path.find(to_string(position)), to_string(position).size());
  if (path.find(STANDART_DELIMITOR) < path.size()) path.erase(path.find(STANDART_DELIMITOR), string(STANDART_DELIMITOR).size());
  return true;
}

bool expanderWidget::addIncomingValue(string InputValue, string path) {
  if (path == "") {
    if (InputValue == "true") {
      value = true;
    } else if (InputValue == "false") {
      value = false;
    } else {
      cerr << "String doesn't contain 'true' or 'false' when trying to parse it as bool" << endl;
      return false;
    }
    if (changeHandler) changeHandler(value);
  }

  unsigned long position;
  string newPath;
  bool success = path2Position(path, position);

  if (position < widgets.size() && success) {
    return widgets.at(position)->addIncomingValue(InputValue, path);
  }

  return false;
}

bool expanderWidget::addWidget(WidgetTemplate* widget, unsigned long position) {
  bool success = false;
  if (position >= widgets.size()) {
    widgets.push_back(widget);
    success = true;
  } else if (widgets.at(position) == emptyWidget) {
    widgets.at(position) = widget;
    success = true;
  }

  if (success) expanderWidgetYAML[REPLY_MENU_GENERAL_CONTENT_SECTION_KEY_NAME][REPLY_MENU_WIDGET_PREFIX + to_string(position)] = widget->getYAMLNode();

  return success;
}

bool expanderWidget::removeWidget(unsigned long position) {
  if (position < widgets.size()) {
    widgets.at(position) = emptyWidget;
    expanderWidgetYAML.remove(position);
    return true;
  }
  return false;
}

YAML::Node expanderWidget::getYAMLNode() {
  expanderWidgetYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_WIDGET_EXPANDER;
  expanderWidgetYAML[REPLY_MENU_GENERAL_LABEL] = label;
  expanderWidgetYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  expanderWidgetYAML[REPLY_MENU_WIDGET_HAS_SWITCH_KEY_NAME] = isToggleable;
  expanderWidgetYAML[REPLY_MENU_WIDGET_VALUE_KEY_NAME] = value;
  return expanderWidgetYAML;
}
