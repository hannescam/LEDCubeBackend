#pragma once
#include <yaml-cpp/node/emit.h>
#include <yaml-cpp/yaml.h>
#include <Logger.hpp>
#include <string>
#include <fstream>
#include <GlobalEnums.hpp>

#define MAX_BRIGHTNESS_KEY_NAME "max_brightness"
#define RESTORE_PREVIOUS_STATE_KEY_NAME "restore_previous_state"
#define PREVIOUS_ANIMATION_KEY_NAME "previous_animation"
#define PREVIOUS_FILE_STATE_KEY_NAME "previous_file_state"
#define USE_INTERNAL_SCREEN_KEY_NAME "use_internal_screen"
#define USE_REMOTE_MANAGEMENT_KEY_NAME "use_remote_management"
#define SEND_ERRORS_TO_CLIENT_KEY_NAME "send_errors_to_client"

#define PREVIOUS_FILE_STATE_NOT_LOADED "not_loaded"
#define PREVIOUS_FILE_STATE_PLAYING "playing"
#define PREVIOUS_FILE_STATE_PAUSED "paused"

using namespace std;

class UserSettings {
  public:
    static bool begin(string _settingsPath);
    static bool save();

    static int getMaxBrightness();
    static void setMaxBrightness(int maxBrightness);

    static bool getRestorePrevState();
    static void setRestorePrevState(bool restorePrevState);

    static string getPrevAnimation();
    static void setPrevAnimation(string prevAnimation);

    static fileState getPrevFileState();
    static void setPrevFileState(fileState state);

    static bool getUseInternalScreen();
    static void setUseInternalScreen(bool useInternalScreen);

    static bool getUseRemoteManagement();
    static void setUseRemoteManagement(bool useRemoteManagement);

    static bool getSendErrorsToClient();
    static void setSendErrorsToClient(bool sendErrorsToCLient);
  private:
    static string settingsPath;
    static YAML::Node settingsYAML;
};
