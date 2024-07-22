#include "include/MenuConstructor.hpp"

void menuConstructor::setTooltip(string _tooltip) {
  tooltip = _tooltip;
}

void menuConstructor::setLabel(string _label) {
  label = _label;
}

void menuConstructor::setIcon(string _icon) {
  icon = _icon;
}

bool menuConstructor::path2Position(string& path, unsigned long& position) {
  if (path.find(REPLY_MENU_GROUP_NAME) >= path.size()) return false;
  path.erase(path.find(REPLY_MENU_GROUP_NAME), string(REPLY_MENU_GROUP_NAME).size());
  position = stoi(path);
  path.erase(path.find(to_string(position)), to_string(position).size());
  if (path.find(STANDART_DELIMITOR) < path.size()) path.erase(path.find(STANDART_DELIMITOR), string(STANDART_DELIMITOR).size());
  return true;
}

bool menuConstructor::addIncomingValue(string value, string path) {
  unsigned long position;
  string oldPath = path;
  bool success = path2Position(path, position);

  if (position < groupConstructors.size() && success) {
    Logger::debug("Got value and path for a group: value: " + value + ", path: " + oldPath, LOG_AEREA_WIDGET_MENU_CONSTRUCTOR);
    return groupConstructors.at(position)->addIncomingValue(value, path);
  }
  Logger::urgent("Invalid path given: " + oldPath, LOG_AEREA_WIDGET_MENU_CONSTRUCTOR);
  return false;
}

bool menuConstructor::addGroup(groupConstructor* group, unsigned long position) {
  bool success = false;
  if (position >= groupConstructors.size()) {
    groupConstructors.push_back(group);
    success = true;
  } else if (groupConstructors.at(position) == emptyGroup) {
    groupConstructors.at(position) = group;
    success = true;
  }

  if (success) menuYAML[REPLY_MENU_GROUP_PREFIX + to_string(position)] = group->getYAMLNode();

  return success;
}

bool menuConstructor::removeGroup(unsigned long position) {
  if (position < groupConstructors.size()) {
    groupConstructors.at(position) = emptyGroup;
    menuYAML.remove(position);
    return true;
  }
  return false;
}

string menuConstructor::getYAMLString(string messageId) {
  menuYAML[PACKET_TYPE_NAME] = PACKET_REPLY_NAME;
  menuYAML[REPLY_TYPE_NAME] = REPLY_TYPE_MENU;
  menuYAML[TOOLTIP] = tooltip;
  menuYAML[REPLY_MENU_GENERAL_LABEL] = label;
  menuYAML[REPLY_MENU_WIDGET_ICON_KEY_NAME] = icon;
  menuYAML[INTERNAL_NETWOTK_ID_NAME] = messageId;
  return YAML::Dump(menuYAML);
}
