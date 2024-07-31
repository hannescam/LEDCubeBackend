#include "include/UserSettings.hpp"

string UserSettings::settingsPath;
YAML::Node UserSettings::settingsYAML;

bool UserSettings::begin(string _settingsPath) {
  settingsPath = _settingsPath;

  try {
    settingsYAML = YAML::LoadFile(settingsPath);
    return true;
  } catch (exception &error) {
    Logger::error("Standard error while loading user settings YAML: " + string(error.what()), LOG_AEREA_SETTINGS);
    return false;
  } catch (...) {
    Logger::error("Unknown error while loading user settings YAML: " + string(__cxxabiv1::__cxa_current_exception_type()->name()), LOG_AEREA_SETTINGS);
    return false;
  }
}

bool UserSettings::save() {
  try {
    ofstream outFile(settingsPath);
    outFile << settingsYAML;
    return true;
  } catch (exception &error) {
    Logger::warn("Standard error while saving user settings YAML: " + string(error.what()), LOG_AEREA_SETTINGS);
    return false;
  } catch (...) {
    Logger::warn("Unknown error while saving user settings YAML: " + string(__cxxabiv1::__cxa_current_exception_type()->name()), LOG_AEREA_SETTINGS);
    return false;
  }
}

int UserSettings::getMaxBrightness() {
  if (YAML::Node maxBrightnessYAML = settingsYAML[MAX_BRIGHTNESS_KEY_NAME]) {
    try {
      return maxBrightnessYAML.as<uint8_t>();
    } catch (...) {
      goto print_error;
    }
  }
  print_error:
    Logger::warn("Failed to parse max brightness value in user settings", LOG_AEREA_SETTINGS);
  return -1;
}

void UserSettings::setMaxBrightness(int maxBrightness) {
  settingsYAML[MAX_BRIGHTNESS_KEY_NAME] = maxBrightness;
}

bool UserSettings::getRestorePrevState() {
  if (YAML::Node restorePrevStateYAML = settingsYAML[RESTORE_PREVIOUS_STATE_KEY_NAME]) {
    try {
      return restorePrevStateYAML.as<bool>();
    } catch (...) {
      goto print_error;
    }
  }
  print_error:
    Logger::warn("Failed to parse restore previous state value in user settings", LOG_AEREA_SETTINGS);
  return false;
}

void UserSettings::setRestorePrevState(bool restorePrevState) {
  settingsYAML[RESTORE_PREVIOUS_STATE_KEY_NAME] = restorePrevState;
}

string UserSettings::getPrevAnimation() {
  if (YAML::Node prevAnimationYAML = settingsYAML[RESTORE_PREVIOUS_STATE_KEY_NAME]) {
    try {
      return prevAnimationYAML.as<string>();
    } catch (...) {
      goto print_error;
    }
  }
  print_error:
    Logger::warn("Failed to parse previous animation name in user settings", LOG_AEREA_SETTINGS);
  return "";
}

void UserSettings::setPrevAnimation(string prevAnimation) {
  settingsYAML[PREVIOUS_ANIMATION_KEY_NAME] = prevAnimation;
}

fileState UserSettings::getPrevFileState() {
  if (YAML::Node prevAnimationYAML = settingsYAML[RESTORE_PREVIOUS_STATE_KEY_NAME]) {
    try {
      string fileStateStr = prevAnimationYAML.as<string>();
      if (fileStateStr == PREVIOUS_FILE_STATE_NOT_LOADED) {
        return STATE_NOT_LOADED;
      } else if (fileStateStr == PREVIOUS_FILE_STATE_PLAYING) {
        return STATE_PLAYING;
      } else if (fileStateStr == PREVIOUS_FILE_STATE_PAUSED) {
        return STATE_PAUSED;
      } else {
        goto print_error;
      }
    } catch (...) {
      goto print_error;
    }
  }
  print_error:
    Logger::warn("Failed to parse previous state value in user settings", LOG_AEREA_SETTINGS);
  return STATE_NOT_LOADED;
}

void UserSettings::setPrevFileState(fileState state) {
  string fileStateStr;
  switch (state) {
    case STATE_NOT_LOADED:
      fileStateStr = PREVIOUS_FILE_STATE_NOT_LOADED;
      break;

    case STATE_PLAYING:
      fileStateStr = PREVIOUS_FILE_STATE_PLAYING;
      break;

    case STATE_PAUSED:
      fileStateStr = PREVIOUS_FILE_STATE_PAUSED;
      break;
  }

  settingsYAML[PREVIOUS_FILE_STATE_KEY_NAME] = fileStateStr;
}

bool UserSettings::getUseInternalScreen() {
  if (YAML::Node useInternalScreenYAML = settingsYAML[USE_INTERNAL_SCREEN_KEY_NAME]) {
    try {
      return useInternalScreenYAML.as<bool>();
    } catch (...) {
      goto print_error;
    }
  }
  print_error:
    Logger::warn("Failed to parse use internal screen value in user settings", LOG_AEREA_SETTINGS);
  return false;
}

void UserSettings::setUseInternalScreen(bool useInternalScreen) {
  settingsYAML[USE_INTERNAL_SCREEN_KEY_NAME] = useInternalScreen;
}

bool UserSettings::getUseRemoteManagement() {
  if (YAML::Node useRemoteManagementYAML = settingsYAML[USE_REMOTE_MANAGEMENT_KEY_NAME]) {
    try {
      return useRemoteManagementYAML.as<bool>();
    } catch (...) {
      goto print_error;
    }
  }
  print_error:
    Logger::warn("Failed to parse use remote management value in user settings", LOG_AEREA_SETTINGS);
  return false;
}

void UserSettings::setUseRemoteManagement(bool useRemoteManagement) {
  settingsYAML[USE_REMOTE_MANAGEMENT_KEY_NAME] = useRemoteManagement;
}

bool UserSettings::getSendErrorsToClient() {
  if (YAML::Node sendErrorsToClientYAML = settingsYAML[SEND_ERRORS_TO_CLIENT_KEY_NAME]) {
    try {
      return sendErrorsToClientYAML.as<bool>();
    } catch (...) {
      goto print_error;
    }
  }
  print_error:
    Logger::warn("Failed to parse send errors to client value in user settings", LOG_AEREA_SETTINGS);
  return false;
}

void UserSettings::setSendErrorsToClient(bool sendErrorsToCLient) {
  settingsYAML[SEND_ERRORS_TO_CLIENT_KEY_NAME] = sendErrorsToCLient;
}
