#include <iostream>
#include <Widgets.hpp>
#include <MenuConstructor.hpp>
#include <GroupConstructor.hpp>
#include <SocketComms.hpp>
#include <Logger.hpp>
propertyWidget property;
    buttonWidget button;
    switchWidget switch_wid;
    textboxWidget textbox;
    expanderWidget expander;
    spinnerWidget spinner;
    dropdownWidget dropdown;
    sliderWidget slider;
    groupConstructor group;
    menuConstructor menu;
using namespace std;

SocketComms comms;
fileState state = STATE_PAUSED;
float do_stuff_for_real_because_why_not_smileyface();
string file_loaded = "df";

void statusHandler(string message_id, SocketHandler* socket) {
  Logger::debug("Got status request with network id: '" + message_id + "', assembling a status reply...", LOG_AEREA_MAIN);
  replyStatusMsg status;
  status.messageId = message_id;
  status.fileIsLoaded = (file_loaded != "");
  status.state = state;
  status.fileSelected = file_loaded;
  status.currentDraw = 9.11;
  status.voltage = do_stuff_for_real_because_why_not_smileyface();
  status.lidState = false;
  vector<animationWithIcon> anims;

  anims.resize(2);
  anims.at(0).animationName = "foo test";
  anims.at(0).animationGnomeIconName = "org.fedoraproject.AnacondaInstaller-symbolic";
  anims.at(1).animationName = "df";
  anims.at(1).animationGnomeIconName = "battery-level-0-charging-symbolic";

  status.availableAnimations = anims;
  Logger::debug("Assembled status reply:", LOG_AEREA_MAIN);
  Logger::debug(status.getYAMLString(), LOG_AEREA_MAIN);
  if (!socket->sendString(status.getYAMLString())) {
    Logger::warn("Could not send status reply", LOG_AEREA_MAIN);
    return;
  }
  Logger::debug("Successfully send status reply", LOG_AEREA_MAIN);
}

float val1 = 5.1;
float val2 = 5.2;
float val3 = 5.3;
int cnt;

int one = 1;

float do_stuff_for_real_because_why_not_smileyface() {
  cnt++;
  switch(cnt-one) {
    case 0:
      return val1;
      break;
    case 1:
      return val2;
      break;
    case 2:
      return val3;
      break;
    case 3:
      return val2;
      break;
    case 4:
      cnt = 0;
      return val1;
      break;
  }
  return -1;
}

void generalRequestHandler(string message_id, requestType type, string filename, SocketHandler* socket) {
  Logger::debug("Triggered general request handler with the message id: '" + message_id, LOG_AEREA_MAIN);
  if (type == REQUEST_MENU) {
    Logger::debug("Menu request got triggered: Constructing a menu reply...", LOG_AEREA_MAIN);
      menu.setMessageId(message_id);
      menu.setLabel(filename);
      menu.setIcon("battery-level-0-charging-symbolic");
      if (filename == "foo test") menu.setIcon("org.fedoraproject.AnacondaInstaller-symbolic");
      menu.setTooltip("In here are all the dining options...");
          Logger::debug("Generated menu YAML:", LOG_AEREA_MAIN);
          Logger::debug(menu.getYAMLString(), LOG_AEREA_MAIN);
          Logger::debug("Sending menu reply to client...", LOG_AEREA_MAIN);
    if (!socket->sendString(menu.getYAMLString())) {
      Logger::warn("Error could not send menu reply", LOG_AEREA_MAIN);
      return;
    }
    Logger::debug("Successfully send menu reply", LOG_AEREA_MAIN);
  } else if (type == REQUEST_PLAY) {
    Logger::debug("Received play request", LOG_AEREA_MAIN);
    state = STATE_PLAYING;
    if (file_loaded != filename) file_loaded = filename;
  } else if (type == REQUEST_PAUSE) {
    Logger::debug("Received pause request", LOG_AEREA_MAIN);
    state = STATE_PAUSED;
  } else if (type == REQUEST_STOP) {
    Logger::debug("Received stop request", LOG_AEREA_MAIN);
    file_loaded = "";
  } else {
    Logger::urgent("Non-implemented action got triggered", LOG_AEREA_MAIN);
  }
}

void test_widget_constructor();

void dings(string messageId, string file, string path, string value, SocketHandler* socket) {
  Logger::debug("received menu request: net-id=" + messageId + ", id=" + to_string(socket->getConnectionId()) + ", file=" + file + ", path=" + path + ", value=" + value, LOG_AEREA_MAIN);
  menu.addIncomingValue(value, path);
}


int test_some_stuff_smileyface() {//test_widget_constructor();



      property.setText("What is a Computer?");
      property.setLabel("Question");
      property.setTooltip("What should I ask? ");

      button.setIcon("MrHansimansi");
      button.setLabel("Hellorld!");
      button.setUseRow(true);
      button.setTooltip("A computer doesn't consume electricity!");
      button.setStyle("pill");

      switch_wid.setLabel("You can turn on the rocket with this switch");
      switch_wid.setStyle("totally valid style");
      switch_wid.setValue(false);
      switch_wid.setTooltip("Wuf");

      textbox.setLabel("What is a keyboard?");
      textbox.setText("PC XT/AT");
      textbox.setTooltip("Miau");
      textbox.setStyle("pill");
      textbox.setIsEditable(true);
      textbox.setHasAppyButton(true);

      expander.setLabel("What are you?");
      expander.setTooltip("That is a 'question'");
      expander.setIsToggleable(true);
      expander.addWidget(&property, 0);
      expander.addWidget(&button, 1);
      expander.addWidget(&switch_wid, 2);
      expander.addWidget(&textbox, 3);

      spinner.setTimeout(60*1000);
      spinner.setTooltip("Look at it and get hipnotised...");

      dropdown.setText("There is something that you don't expect in there...");
      dropdown.setLabel("DropieDownie");
      dropdown.setStyle("UpsieTroopsie");
      dropdown.setIsSearchable(true);
      dropdown.setTooltip("There was something a few centuries ago...\nIt was something mysterios...");
      dropdown.addEntry("Grour", 0);
      dropdown.addEntry("Meow", 1);
      dropdown.addEntry("Aaaaaaaaaaaa...", 2);

      slider.setLabel("What is the difference between 2 and 2*2 ?????");
      slider.setTooltip("What is this Hashmap<Hashmap<Hashmap<UUID, String>, String>, String>?");
      slider.setStyle("osd,background,destructive-action");
      slider.setMin(-500.0);
      slider.setValue(0.5);
      slider.setDigits(1);
      slider.setIsNumeric(true);
      slider.setIsSnap(true);
      slider.setIsWraparound(false);
      slider.setStep(0.1);
      slider.setClimbRate(4);
      slider.setMax(500.0);

      group.setLabel("What are you?");
      group.setTooltip("That is a 'question'");
      group.addWidget(&expander, 0);
      group.addWidget(&spinner, 1);
      group.addWidget(&dropdown, 2);
      group.addWidget(&slider, 3);

      menu.addGroup(&group, 0);


  if (!comms.begin(1200)) return -1;
  comms.setStatusRequestHandler(statusHandler);
  comms.setGeneralRequestHandler(generalRequestHandler);
  comms.setMenuChangeHandler(dings);
  while(true) usleep(1000000);
  return 0;
}


int main() {
  Logger::begin("LoggerConfig.yml");
  return test_some_stuff_smileyface();
}

















void test_widget_constructor() {
  propertyWidget property;
  buttonWidget button;
  switchWidget switch_wid;
  textboxWidget textbox;
  expanderWidget expander;
  spinnerWidget spinner;
  dropdownWidget dropdown;
  sliderWidget slider;
  groupConstructor group;
  menuConstructor menu;

  property.setText("What is a Computer?");
  property.setLabel("Question");
  property.setTooltip("What should I ask?");

  button.setIcon("MrHansimansi");
  button.setLabel("Hellorld!");
  button.setUseRow(true);
  button.setTooltip("A computer doesn't consume electricity!");
  button.setStyle("pill");

  switch_wid.setLabel("You can turn on the rocket with this switch");
  switch_wid.setStyle("totally valid style");
  switch_wid.setValue(false);
  switch_wid.setTooltip("Wuf");

  textbox.setLabel("What is a keyboard?");
  textbox.setText("PC XT/AT");
  textbox.setTooltip("Miau");
  textbox.setStyle("pill");
  textbox.setIsEditable(true);
  textbox.setHasAppyButton(true);

  expander.setLabel("What are you?");
  expander.setTooltip("That is a 'question'");
  expander.setIsToggleable(true);
  expander.addWidget(&property, 0);
  expander.addWidget(&button, 1);
  expander.addWidget(&switch_wid, 2);
  expander.addWidget(&textbox, 3);

  spinner.setTimeout(60*1000);
  spinner.setTooltip("Look at it and get hipnotised...");

  dropdown.setText("There is something that you don't expect in there...");
  dropdown.setLabel("DropieDownie");
  dropdown.setStyle("UpsieTroopsie");
  dropdown.setIsSearchable(true);
  dropdown.setTooltip("There was something a few centuries ago...\nIt was something mysterios...");
  dropdown.addEntry("Grour", 0);
  dropdown.addEntry("Meow", 1);
  dropdown.addEntry("Aaaaaaaaaaaa...", 2);

  slider.setLabel("What is the difference between 2 and 2*2 ?????");
  slider.setTooltip("What is this Hashmap<Hashmap<Hashmap<UUID, String>, String>, String>?");
  slider.setStyle("osd,background,destructive-action");
  slider.setMin(-500.0);
  slider.setValue(0.5);
  slider.setDigits(1);
  slider.setIsNumeric(true);
  slider.setIsSnap(true);
  slider.setIsWraparound(false);
  slider.setStep(0.1);
  slider.setClimbRate(4);
  slider.setMax(500.0);

  group.setLabel("What are you?");
  group.setTooltip("That is a 'question'");
  group.addWidget(&expander, 0);
  group.addWidget(&spinner, 1);
  group.addWidget(&dropdown, 2);
  group.addWidget(&slider, 3);

  menu.setTooltip("In here are all the dining options...");
  menu.addGroup(&group, 0);
  Logger::urgent(menu.getYAMLString(), LOG_AEREA_MAIN);
  Logger::urgent(to_string(menu.addIncomingValue("test", "group0widget0widget2")), LOG_AEREA_MAIN);
}

