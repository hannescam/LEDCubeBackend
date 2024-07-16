/* LumaMenuC++.cpp
 *
 * Copyright 2024 Hannes Campidell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <LumaMenuC++.hpp>

void LumaMenuCPP::init(string LumaConfigFilename, string fontFilename) {
  displayInstance.init(LumaConfigFilename, fontFilename); // Initialize the display
  fontPath = fontFilename;
}

void LumaMenuCPP::setOptionVector(vector<menuEntry> options) {
  optionVector = options; // Import the options parameter into the internal vector
  highlightedEntry = 0; // Reset the highlightedEntry
  displayOption(); // Redraw the menu on the screen
}

void LumaMenuCPP::setClickHandler(function<void(int, vector<menuEntry>)> _clickHandler) {
  clickHandler = _clickHandler; // Import the clock handler function into the internal one
}

void LumaMenuCPP::TMP_setSelectedOption(int option) {
  highlightedEntry = option;
  displayOption();
}

void LumaMenuCPP::changeValueHandler(bool increment) {
  if (optionSelectedForEdit && optionVector.at(highlightedEntry).isConfigurable) { // Increment/decrement the value
    if (increment && optionVector.at(highlightedEntry).wrapArround) { // Increment with wrap-around
      optionVector.at(highlightedEntry).value++; // Increment the value in the respective location
      if (optionVector.at(highlightedEntry).value > optionVector.at(highlightedEntry).maxValue) optionVector.at(highlightedEntry).value = optionVector.at(highlightedEntry).minValue; // Reset it if the value is outside the specified range
    } else if (optionVector.at(highlightedEntry).wrapArround) { // Decrement with wrap-around
      optionVector.at(highlightedEntry).value--; // Decrement the value in the respective location
      if (optionVector.at(highlightedEntry).value < optionVector.at(highlightedEntry).minValue) optionVector.at(highlightedEntry).value = optionVector.at(highlightedEntry).maxValue; // Reset it if the value is outside the specified range
    } else if (increment && !optionVector.at(highlightedEntry).wrapArround) { // Increment without wrap-around
      if (optionVector.at(highlightedEntry).value < optionVector.at(highlightedEntry).maxValue) optionVector.at(highlightedEntry).value++; // Increment the value in the respective location if the value in within the specified range
    } else if (!optionVector.at(highlightedEntry).wrapArround) { // Decrement without wrap-around
      if (optionVector.at(highlightedEntry).value > optionVector.at(highlightedEntry).minValue) optionVector.at(highlightedEntry).value--; // Decrement the value in the respective location if the value in within the specified range
    }
    optionValueChanged = true; // Tell the menu updater that a option value was changed
  } else if (!optionSelectedForEdit) { // Scroll in the menu
    if (increment) { // Increment
      highlightedEntry++; // Increment the highlightedEntry variable so that the next option is selected
      if (highlightedEntry >= optionVector.size()) highlightedEntry = 0; // Reset it if the selected option is outside the vector
    } else { // Decrement
      highlightedEntry--; // Decrement the highlightedEntry variable so that the previous option is selected
      if (highlightedEntry < 0) highlightedEntry = optionVector.size() - 1; // Reset it if the selected option is outside the vector
    }
  }

  displayOption(); // Apply the changes
}

void LumaMenuCPP::buttonHandler() {
  if (optionVector.at(highlightedEntry).isConfigurable == true) { // Check if the selected option is configurable
    optionSelectedForEdit = !optionSelectedForEdit;
  }
  displayOption();
  clickHandler(highlightedEntry, optionVector); // Run the click handler
}

void LumaMenuCPP::endInputHandler() {
  runInputHandler = false;
}

void LumaMenuCPP::stdinInput(string incStr, string decStr, string btnStr, string quitStr) {
  string inputString; // Initialize in input string for the stdin to be saved into

  runInputHandler = true;

  while (runInputHandler) {
    inputString = ""; // Clear the input string
    cin >> inputString; // Read the cin into the input string

    if (inputString.find(incStr) != string::npos) { // Check if the input string contains the 'incStr' string
      changeValueHandler(true); // If yes run the increment handler
    }

    if (inputString.find(decStr) != string::npos) { // Check if the input string contains the 'decStr' string
      changeValueHandler(false); // If yes run the decrement handler
    }

    if (inputString.find(btnStr) != string::npos) { // Check if the input string contains the 'btnStr' string
      buttonHandler(); // If yes run the button handler
    }

     if (inputString.find(quitStr) != string::npos) { // Check if the input string contains the 'quitStr' string
      return; // If yes just quit
    }
  }
}



void LumaMenuCPP::buttonInput(int upPin, int downPin, int okPin, string GPIOChipName) {
  gpiod::chip chip((filesystem::path) GPIOChipName); // Initialize gpiochip with path to the character device

  gpiod::line_request upLine = chip.prepare_request().set_consumer("upLine").add_line_settings(upPin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT).set_bias(gpiod::line::bias::PULL_UP)).do_request(); // Initialize all the GPIO lines with PULL-UP and as inputs
  gpiod::line_request downLine = chip.prepare_request().set_consumer("downLine").add_line_settings(downPin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT).set_bias(gpiod::line::bias::PULL_UP)).do_request();
  gpiod::line_request okLine = chip.prepare_request().set_consumer("okLine").add_line_settings(okPin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT).set_bias(gpiod::line::bias::PULL_UP)).do_request();

  runInputHandler = true;

  while (runInputHandler) {

    if (upLine.get_value(upPin) == gpiod::line::value::INACTIVE) { // Check if the 'up button' has been pressed
      while (upLine.get_value(upPin) == gpiod::line::value::INACTIVE) usleep(10000);
      changeValueHandler(true); // If yes run the increment handler
    }

    if (downLine.get_value(downPin) == gpiod::line::value::INACTIVE) { // Check if the 'down button' has been pressed
      while (downLine.get_value(downPin) == gpiod::line::value::INACTIVE) usleep(10000);
      changeValueHandler(false); // If yes run the decrement handler
    }

    if (okLine.get_value(okPin) == gpiod::line::value::INACTIVE) { // Check if the 'ok button' has been pressed
      while (okLine.get_value(okPin) == gpiod::line::value::INACTIVE) usleep(10000);
      buttonHandler(); // If yes run the button handler
    }


  }
}

void LumaMenuCPP::displayValue(int x, int y, menuEntry option, Color backgroundColor, Color textColor) { // Draw a menu entry at a given coordinate
  if (option.hasValue) { // Only draw it if a value exists
    string finalText = "Error"; // Default to displaying "Error" SHOULD NEVER HAPPEN
    if (previoisFinalTextLength != 0) displayInstance.drawRectangle(x - previoisFinalTextLength, y, previoisFinalTextLength, previoisFinalTextHeight, backgroundColor, true /*fill*/); // Draw a rectangle over the region where the text was
    if (option.valueType == BOOLEAN) { // Check if the value is a boolean if it is it will be displayed as On/Off
      if (option.value == 1) { // Check if the value is true
        finalText = "On"+option.humanReadableUnit;
      } else if (option.value == 0) { // Check if the value is false
        finalText = "Off"+option.humanReadableUnit;
      }
    } else if (option.valueType == INTEGER) { // If the value is an integer the float will be converted to a integer
      finalText = to_string((int)option.value)+option.humanReadableUnit; // Combine all the values into one string for displaying after converting the float to int
    } else if (option.valueType == DECIMAL) { // Otherwise the value will be used raw
      finalText = to_string(option.value)+option.humanReadableUnit; // Combine all the values into one string for displaying
    }
    displayInstance.drawText(finalText, x, y, textColor, "rt" /*set the anchor to top right*/); // If the value is numeric it will be just printed as is with the human readable unit for things like "°C", "W" or "%"
    previoisFinalTextLength = displayInstance.getTextWidth(finalText); // Get the width of the text for deleting it later
    previoisFinalTextHeight = displayInstance.getTextHeight(finalText); // Get the height of the text for deleting it later
  }
}

void LumaMenuCPP::setBackgroundColor(Color _backgroundColor) {
  backgroundColor = _backgroundColor;
  displayOption();
}

void LumaMenuCPP::setForegroundColor(Color _foregroundColor) {
  foregroundColor = _foregroundColor;
  displayOption();
}

void LumaMenuCPP::displayOption() {
  if (highlightedEntry != highlightedEntryOld | optionSelectedForEdit != optionSelectedForEditOld | optionValueChanged) { // Checks if the content on the display matches the current "needs"
    displayInstance.setTrueTypeFont(fontPath);
    int cnt = 0; // Value resetting
    int offset = 0; // IDKWTD
    int pixelOffset = 0;
    int entryCount = optionVector.size();
    Color textColor;

    if (highlightedEntry >= MAX_ENTRIES_ON_SCREEN /*&& !optionValueChanged*/) { // Checks if it needs to scroll the screen up
      offset = highlightedEntry - MAX_ENTRIES_ON_SCREEN; // At which entry it needs to start drawing
      cnt = offset; // Sets the counter to the offset value so it doesn't start from 0
      pixelOffset = PIXEL_OFFSET; // Sets the Individual pixel offset to -8 to move the menu a bit down so the last option (selected most of the times) can be seen fully
      if (!optionValueChanged) doesNotRequireFullRefresh = false; // Requires a full redraw of the display
      if (!optionValueChanged) displayInstance.drawRectangle(0, displayInstance.displayHeight - SPACING, displayInstance.displayWidth, SPACING, backgroundColor, true /*fill*/); // Clears a bit of the lower section of the screen to avoid remains staying visible (if it would clear the whole screen it would flicker a lot more)
    }

    if (highlightedEntry < MAX_ENTRIES_ON_SCREEN && highlightedEntryOld == entryCount-1 && entryCount > MAX_ENTRIES_ON_SCREEN && !optionValueChanged) {  // Marks the operation as needing a full refresh when the cursor wraps around from the end to the start of the menu and there are more entries than entries that fit on a single page
      doesNotRequireFullRefresh = false; // Requires full refresh and not just "patchwork"
      displayInstance.fillScreen(backgroundColor); // Erases the screen completely
    }

    if (highlightedEntry < MAX_ENTRIES_ON_SCREEN && highlightedEntryOld >= MAX_ENTRIES_ON_SCREEN && !optionValueChanged) { // Refreshes (clearing and redrawing) the screen when the cursor moves to the first page
      doesNotRequireFullRefresh = false; // Requires full refresh and not just "patchwork"
      displayInstance.fillScreen(backgroundColor); // Erases the screen completely
    }

    while (cnt < entryCount) { // Counts through the entries (from the start of the visible page)
      int yBegin = (SPACING * (cnt - offset)) + pixelOffset; // Calculates the starting point of the current option (could end up above the screen)
      if (cnt == highlightedEntry) { // If the counter matches the selected entry

        if (optionSelectedForEdit && !optionValueChanged) { // Make a rounded rectangle around the option and fill it in if the current option is selected for editing but only on the first call
          displayInstance.drawRoundRectangle(0, yBegin, displayInstance.displayWidth, SPACING - 1, foregroundColor, true /*fill*/, ROUNDING); // Make a filled rounded rectangle around it with the given foregroundColor
          textColor = backgroundColor; // Set the text color to the given background color when the option is selected (subject to change)
        } else if (optionSelectedForEditOld && !optionSelectedForEdit) { // Clear the whole option and make a rounded rectangle around the option and not fill it in
          displayInstance.drawRectangle(0, yBegin, displayInstance.displayWidth, SPACING, backgroundColor, true /*filled*/); // Clear the whole previous text (and filled round rectangle) that was here
          displayInstance.drawRoundRectangle(0, yBegin, displayInstance.displayWidth, SPACING - 1, foregroundColor, false /*don't fill*/, ROUNDING); // Make a rounded rectangle around it with the given foregroundColor
          textColor = foregroundColor; // Set the text color to the given foreground color when the option is selected (subject to change)
        } else if (!optionValueChanged) { // Normal behavior
          displayInstance.drawRoundRectangle(0, yBegin, displayInstance.displayWidth, SPACING - 1, foregroundColor, false /*don't fill*/, ROUNDING); // Make a rounded rectangle around it with the given foregroundColor
          textColor = foregroundColor; // Set the text color to the given foreground color when the option is selected (subject to change)
        }

        if (!optionValueChanged) displayInstance.drawText(optionVector.at(cnt).name, OPTIONS_OFFSET_X, yBegin + TEXT_V_OFFSET, textColor); // Draw the text of the option over the rounded rectangle if the value of the selected option isn't tweaked (optimization)
        if (optionSelectedForEdit) { // Check if the value is getting edited
          displayValue(displayInstance.displayWidth - OPTIONS_OFFSET_X, yBegin + TEXT_V_OFFSET, optionVector.at(cnt), foregroundColor, backgroundColor); // Draw the right bound value that can be modified by the user if allowed using the foreground as the background for editing mode
        } else {
          displayValue(displayInstance.displayWidth - OPTIONS_OFFSET_X, yBegin + TEXT_V_OFFSET, optionVector.at(cnt), backgroundColor, foregroundColor); // Draw the right bound value that can be modified by the user if allowed using the background as the background for non-editing mode
        }

      } else if (highlightedEntryOld == cnt | !doesNotRequireFullRefresh) { // Executes when the counter hits the previously selected option
        displayInstance.drawRectangle(0, yBegin, displayInstance.displayWidth, SPACING, backgroundColor, true /*fill*/); // Overwriting the field of the previous option with a black square
        textColor = foregroundColor; // Set the text color to the set color for visibility
        displayInstance.drawText(optionVector.at(cnt).name, OPTIONS_OFFSET_X, yBegin + TEXT_V_OFFSET, textColor); // Draw the text
        displayValue(displayInstance.displayWidth - OPTIONS_OFFSET_X, yBegin + TEXT_V_OFFSET, optionVector.at(cnt), backgroundColor, foregroundColor);
      }

      cnt++; // What do I see here?!?
    }
    optionValueChanged = false;
    optionSelectedForEditOld = optionSelectedForEdit; // Update the "optionSelectedForEditOld" variable
    doesNotRequireFullRefresh = true; // Defaults to not refreshing the screen
    highlightedEntryOld = highlightedEntry; // Transfer the highlightedEntry variable to the "shighlightedEntryOld" variable for later use
    displayInstance.display(); // Apply
  }
}
