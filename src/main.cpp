#include <iostream>
#include <UserSettings.hpp>
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

void test_widget_constructor();

void testPlayHandler(string, string filename, SocketHandler*) {
  comms.setSelectedFile(STATE_PLAYING, filename);
}

void testPauseHandler(string, string filename, SocketHandler*) {
  comms.setSelectedFile(STATE_PAUSED, filename);
}

void testStopHandler(string, string filename, SocketHandler*) {
  comms.setSelectedFile(STATE_NOT_LOADED, filename);
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

      menu.setLabel("foo test");
      menu.setIcon("org.fedoraproject.AnacondaInstaller-symbolic");
      menu.addGroup(&group, 0);


  if (!comms.begin(1200)) return -1;
  comms.updateVoltage(5.1682);
  comms.updateCurrentDraw(0.5849);
  comms.setSelectedFile(STATE_PLAYING, "foo test");
  comms.addAnimationEntry("foo test", "org.fedoraproject.AnacondaInstaller-symbolic", &menu);
  comms.setPlayHandler(testPlayHandler);
  comms.setPauseHandler(testPauseHandler);
  comms.setStopHandler(testStopHandler);
  while(true) usleep(1000000);
  return 0;
}

int main() {
  Logger::begin("LoggerConfig.yml");
  UserSettings::begin("/home/hannes/Projekte/LED Cube/Software/Cube code/LEDCubeBackend/data/UserSettings.yml");
  UserSettings::setPrevFileState(STATE_NOT_LOADED);
  UserSettings::save();
  //return test_some_stuff_smileyface();
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
  Logger::log(menu.getYAMLString("stupidity"), LOG_AEREA_MAIN, 500);
  Logger::log(to_string(menu.addIncomingValue("test", "group0widget0widget2")), LOG_AEREA_MAIN, 500);
}

