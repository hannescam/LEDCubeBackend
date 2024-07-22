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
