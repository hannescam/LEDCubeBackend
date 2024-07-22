#include "include/GroupConstructor.hpp"

void groupConstructor::setLabel(string _label) {
  label = _label;
}

void groupConstructor::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

bool groupConstructor::path2Position(string& path, unsigned long& position) {
  if (path.find(REPLY_MENU_WIDGET_PREFIX) >= path.size()) return false;
  path.erase(path.find(REPLY_MENU_WIDGET_PREFIX), string(REPLY_MENU_WIDGET_PREFIX).size());
  position = stoi(path);
  path.erase(path.find(to_string(position)), to_string(position).size());
  if (path.find(STANDART_DELIMITOR) < path.size()) path.erase(path.find(STANDART_DELIMITOR), string(STANDART_DELIMITOR).size());
  return true;
}

bool groupConstructor::addIncomingValue(string value, string path) {
  unsigned long position;
  string oldPath = path;
  bool success = path2Position(path, position);

  if (position < widgets.size() && success) {
    Logger::debug("Got value and path for a widget: value: " + value + ", path: " + oldPath, LOG_AEREA_WIDGET_MENU_CONSTRUCTOR);
    return widgets.at(position)->addIncomingValue(value, path);
  }
  Logger::urgent("Invalid path given: " + oldPath, LOG_AEREA_WIDGET_MENU_CONSTRUCTOR);
  return false;
}

bool groupConstructor::addWidget(WidgetTemplate* widget, unsigned long position) {
  bool success = false;
  if (position >= widgets.size()) {
    widgets.push_back(widget);
    success = true;
  } else if (widgets.at(position) == emptyWidget) {
    widgets.at(position) = widget;
    success = true;
  }

  if (success) groupYAML[REPLY_MENU_GENERAL_CONTENT_SECTION_KEY_NAME][REPLY_MENU_WIDGET_PREFIX + to_string(position)] = widget->getYAMLNode();

  return success;
}

bool groupConstructor::removeWidget(unsigned long position) {
  if (position < widgets.size()) {
    widgets.at(position) = emptyWidget;
    groupYAML.remove(position);
    return true;
  }
  return false;
}

YAML::Node groupConstructor::getYAMLNode() {
  groupYAML[REPLY_MENU_GENERAL_LABEL] = label;
  groupYAML[REPLY_MENU_GENERAL_TOOLTIP] = tooltip;
  groupYAML[REPLY_MENU_WIDGET_NAME_KEY_NAME] = REPLY_MENU_GROUP_NAME;
  return groupYAML;
}
