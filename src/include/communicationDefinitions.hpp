// Timeouts
#define KEEPALIVE_TIMEOUT 1000
#define KEEPALIVE_GRACE_TIME_AFTER_START 5000
// Aux stuff
#define STANDART_DELIMITOR "."
// Key names
#define PACKET_TYPE_NAME "packet_type"
#define INTERNAL_NETWOTK_ID_NAME "internal_network_event_id"
// Packet types
#define PACKET_ERROR_NAME "error"
#define PACKET_REPLY_NAME "reply"
#define PACKET_REQUEST_NAME "request" // For later use
// Key names
#define ERROR_SOURCE_NAME "error_source"
#define ERROR_CODE_NAME "error_code"
#define ERROR_NAME_NAME "error_name"
#define ERROR_SEVERITY_NAME "error_severity"
// Error sources
#define ERROR_SOURCE_POWER "power"
#define ERROR_SOURCE_INVALID_FILE "invalid_file"
#define ERROR_SOURCE_INVALID_REQUEST "invalid_request"
#define ERROR_SOURCE_OTHER "other"
// Key name
#define REPLY_TYPE_NAME "reply_type"
// Reply types
#define REPLY_TYPE_MENU "menu"
#define REPLY_TYPE_STATUS "status"
// Key names
#define REPLY_STATUS_FILE_LOADED "file_is_loaded"
#define REPLY_STATUS_FILE_STATE "file_state"
#define REPLY_STATUS_FILE_SELECTED "file_selected"
#define REPLY_STATUS_CURRENT_DRAW "current_draw"
#define REPLY_STATUS_VOLTAGE "voltage"
#define REPLY_STATUS_LID_STATE "lid_state"
#define REPLY_STATUS_AVAILIBLE_ANIMATIONS "available_animations"
// File states
#define REPLY_STATUS_FILE_STATE_PAUSED "paused"
#define REPLY_STATUS_FILE_STATE_PLAYING "playing"
// Key names
#define REQUEST_TYPE "request_type"
#define REQUEST_FILE "request_file"
#define REQUEST_FILE_BYTE_COUNT REQUEST_MENU_OBJECT_VALUE // Some shit because of the frontend
#define REQUEST_MENU_OBJECT_PATH "object_path"
#define REQUEST_MENU_OBJECT_VALUE "object_new_value"
// Request types
#define REQUEST_TYPE_STATUS "status"
#define REQUEST_TYPE_PLAY "play"
#define REQUEST_TYPE_PAUSE "pause"
#define REQUEST_TYPE_STOP "stop"
#define REQUEST_TYPE_MENU "menu"
#define REQUEST_TYPE_MENU_CHANGE "menu_change"
#define REQUEST_TYPE_FILE_UPLOAD "file_upload"
#define REQUEST_TYPE_KEEPALIVE "keepalive"

// Menu related stuff
#define REPLY_MENU_WIDGET_PREFIX "widget"
// Key names
#define REPLY_MENU_WIDGET_NAME_KEY_NAME "widget"
#define REPLY_MENU_WIDGET_STYLE_KEY_NAME "style"
#define REPLY_MENU_WIDGET_ICON_KEY_NAME "icon"
#define REPLY_MENU_WIDGET_USE_ROW_KEY_NAME "row"
#define REPLY_MENU_WIDGET_VALUE_KEY_NAME "value"
#define REPLY_MENU_WIDGET_HAS_APPLY_BUTTON_KEY_NAME "apply_button"
#define REPLY_MENU_WIDGET_IS_EDITABLE_KEY_NAME "editable"
#define REPLY_MENU_WIDGET_HAS_SWITCH_KEY_NAME "toggleable"
#define REPLY_MENU_WIDGET_TIMOUT_KEY_NAME "time"
#define REPLY_MENU_WIDGET_DEFAULT_SELECTED_KEY_NAME "selected"
#define REPLY_MENU_WIDGET_IS_SEARCHABLE_KEY_NAME "searchable"
#define REPLY_MENU_WIDGET_DROPDOWN_CONTENT_SECTION_KEY_NAME "dropdown"
// Widgets
#define REPLY_MENU_WIDGET_PROPERTY "property"
#define REPLY_MENU_WIDGET_BUTTON "button"
#define REPLY_MENU_WIDGET_SWITCH "switch"
#define REPLY_MENU_WIDGET_NUMBER_INPUT_FIELD "slider"
#define REPLY_MENU_WIDGET_TEXT_BOX "entry"
#define REPLY_MENU_WIDGET_EXPANDER "expander"
#define REPLY_MENU_WIDGET_DROPDOWN "dropdown"
#define REPLY_MENU_WIDGET_SPINNER "spinner"
// Stuff for groups
#define REPLY_MENU_GROUP_NAME "group"
#define REPLY_MENU_GROUP_PREFIX "group"
#define REPLY_MENU_GROUP_WIDGET_SECTION "widget"
// Global stuff for menus (used in groups, menus and widgets)
#define REPLY_MENU_GENERAL_CONTENT_SECTION_KEY_NAME "content"
#define REPLY_MENU_GENERAL_LABEL "label"
#define REPLY_MENU_GENERAL_TOOLTIP "tooltip"


