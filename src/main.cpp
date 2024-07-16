#include <iostream>
#include <Widgets.hpp>
#include <MenuConstructor.hpp>
#include <GroupConstructor.hpp>
#include <SocketComms.hpp>
propertyWidget property;
    buttonWidget button;
    switchWidget switch_wid;
    textboxWidget textbox;
    expanderWidget expander;
    spinnerWidget spinner;
    dropdownWidget dropdown;
    groupConstructor group;
    menuConstructor menu;
using namespace std;

SocketComms comms;
fileState state = STATE_PAUSED;
float do_stuff_for_real_because_why_not_smileyface();
string file_loaded = "df";

void statusHandler(string message_id, SocketHandler* socket) {
  cout << "Got status request with network id: '" << message_id << "', assembling a status reply..." << endl;
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
  cout << "Assembled status reply:" << endl << status.getYAMLString() << endl;
  cout << "Sending status reply to client..." << endl;
  if (!socket->sendString(status.getYAMLString())) {
    cerr << "Error could not send status reply" << endl;
    return;
  }
  cout << "Successfully send status reply" << endl;
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
  cout << "Triggered general request handler with the message id: '" << message_id << endl;
  if (type == REQUEST_MENU) {
    cout << "Menu request got triggered: Constructing a menu reply..." << endl;
      menu.setMessageId(message_id);
      menu.setLabel(filename);
      menu.setIcon("battery-level-0-charging-symbolic");
      if (filename == "foo test") menu.setIcon("org.fedoraproject.AnacondaInstaller-symbolic");
      menu.setTooltip("In here are all the dining options...");
    cout << "Generated menu YAML: " << endl << menu.getYAMLString() << endl;
    cout << "Sending menu reply to client..." << endl;
    if (!socket->sendString(menu.getYAMLString())) {
      cerr << "Error could not send menu reply" << endl;
      return;
    }
    cout << "Successfully send menu reply" << endl;
  } else if (type == REQUEST_PLAY) {
    cout << "Received play request" << endl;
    state = STATE_PLAYING;
    if (file_loaded != filename) file_loaded = filename;
  } else if (type == REQUEST_PAUSE) {
    cout << "Received pause request" << endl;
    state = STATE_PAUSED;
  } else if (type == REQUEST_STOP) {
    cout << "Received stop request" << endl;
    file_loaded = "";
  } else {
    cerr << "Non-implemented action got triggered" << endl;
  }
}

void test_widget_constructor();

void dings(string messageId, string file, string path, string value, SocketHandler* socket) {
  cout << "received menu request: net-id=" << messageId << ", id=" << socket->getConnectionId() << ", file=" << file << ", path=" << path << ", value=" << value << endl;
  cout.flush();
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
      expander.setChangeHandler([](bool value) {
        cout << "Expander switch set to new value: " << value << endl;
      });

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
      dropdown.setChangeHandler([](int value) {
        cout << "Dropdown value changed: " << value << endl;
      });

      group.setLabel("What are you?");
      group.setTooltip("That is a 'question'");
      group.addWidget(&expander, 0);
      group.addWidget(&spinner, 1);
      group.addWidget(&dropdown, 2);

      menu.addGroup(&group, 0);





  if (!comms.begin(1200)) return -1;
  comms.setStatusRequestHandler(statusHandler);
  comms.setGeneralRequestHandler(generalRequestHandler);
  comms.setMenuChangeHandler(dings);
  while(true) usleep(1000000);
  return 0;
}






SocketAcceptor acceptor;

void recvHandler(string inputString, SocketHandler* socket) {
  cout << "Received string from socket with id " << socket->getConnectionId() << ":" << endl << inputString << endl;
}

void disconnHandler(SocketHandler* socket) {
  cout << "disconn handler triggered from id " << socket->getConnectionId() << endl;
}

void clientHandler(SocketHandler* socket) {
  cout << "Started client handler with id " << socket->getConnectionId() << endl;
  socket->setReceiveHandler(recvHandler);
  socket->setDisconnectHandler(disconnHandler);
  usleep(1000);
  while (socket->isOpen()) usleep(1000);
  cout << "shit" << endl;
}

int main() {
  return test_some_stuff_smileyface();
  /*if (!acceptor.begin(1200)) return 1;
  acceptor.setClientHandler(clientHandler);
  acceptor.startListening();
  while (true) usleep(1000000000);
  acceptor.closeSocket();*/
}

















void test_widget_constructor() {
  propertyWidget property;
  buttonWidget button;
  switchWidget switch_wid;
  textboxWidget textbox;
  expanderWidget expander;
  spinnerWidget spinner;
  dropdownWidget dropdown;
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

  group.setLabel("What are you?");
  group.setTooltip("That is a 'question'");
  group.addWidget(&expander, 0);
  group.addWidget(&spinner, 1);
  group.addWidget(&dropdown, 2);

  menu.setTooltip("In here are all the dining options...");
  menu.addGroup(&group, 0);
  cout << menu.getYAMLString() << endl;
  cout << menu.addIncomingValue("test", "group0widget0widget2");
}

