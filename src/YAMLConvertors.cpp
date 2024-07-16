#include "include/SocketComms.hpp"

string errorMsg::getYAMLString() {
  YAML::Node errorMsgYAML; // Initialize the YAML node
  errorMsgYAML[INTERNAL_NETWOTK_ID_NAME] = messageId; // Save the required constants/variables into the YAML node
  errorMsgYAML[PACKET_TYPE_NAME] = PACKET_ERROR_NAME;
  errorMsgYAML[ERROR_NAME_NAME] = name;
  errorMsgYAML[ERROR_CODE_NAME] = code;
  errorMsgYAML[ERROR_SEVERITY_NAME] = (int) severity; // Cast to convert enum that contains an integer into a raw integer

  string sourceStr;
  switch (source) { // Switch to convert the errorSource enum to a string error source
    case SOURCE_POWER:
      sourceStr = ERROR_SOURCE_POWER;
      break;

    case SOURCE_INVALID_FILE:
      sourceStr = ERROR_SOURCE_INVALID_FILE;
      break;

    case SOURCE_INVALID_REQUEST:
      sourceStr = ERROR_SOURCE_INVALID_REQUEST;
      break;

    case SOURCE_OTHER:
      sourceStr = ERROR_SOURCE_OTHER;
      break;
  }

  errorMsgYAML[ERROR_SOURCE_NAME] = sourceStr; // Put the string error source into the YAML node

  return YAML::Dump(errorMsgYAML); // Convert the YAML node to a string using the Dump function and return it as the output of the function
}

string replyStatusMsg::getYAMLString() {
  YAML::Node replyStatusMsgYAML; // Initialize the YAML node
  replyStatusMsgYAML[INTERNAL_NETWOTK_ID_NAME] = messageId; // Save the required constants/variables into the YAML node
  replyStatusMsgYAML[PACKET_TYPE_NAME] = PACKET_REPLY_NAME;
  replyStatusMsgYAML[REPLY_TYPE_NAME] = REPLY_TYPE_STATUS;
  replyStatusMsgYAML[REPLY_STATUS_FILE_LOADED] = fileIsLoaded;

  string fileStateStr;
  switch (state) { // Switch to convert the fileState enum to a string file state
    case STATE_PLAYING:
      fileStateStr = REPLY_STATUS_FILE_STATE_PLAYING;
      break;

    case STATE_PAUSED:
      fileStateStr = REPLY_STATUS_FILE_STATE_PAUSED;
      break;
  }

  replyStatusMsgYAML[REPLY_STATUS_FILE_STATE] = fileStateStr; // Put the string error source into the YAML node
  replyStatusMsgYAML[REPLY_STATUS_FILE_SELECTED] = fileSelected; // Save the required variables into the YAML node
  replyStatusMsgYAML[REPLY_STATUS_CURRENT_DRAW] = currentDraw;
  replyStatusMsgYAML[REPLY_STATUS_VOLTAGE] = voltage;
  replyStatusMsgYAML[REPLY_STATUS_LID_STATE] = lidState;

  unsigned int cnt = 0;
  while (cnt < availableAnimations.size()) { // Loop through all the contents of the availableAnimations vector
    animationWithIcon selectedAnimation = availableAnimations.at(cnt); // Extract the animation information
    replyStatusMsgYAML[REPLY_STATUS_AVAILIBLE_ANIMATIONS][selectedAnimation.animationName] = selectedAnimation.animationGnomeIconName; // Insert all the information for the animation entry into a YAML sub-node
    cnt++; // You know the deal with this thing
  }

  return YAML::Dump(replyStatusMsgYAML); // Convert the YAML node to a string using the Dump function and return it as the output of the function
}
