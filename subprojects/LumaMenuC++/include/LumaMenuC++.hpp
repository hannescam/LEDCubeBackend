/* LumaMenuC++.hpp
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

#include <string.h>
#include <vector>
#include <iostream>
#include <functional>
#include <cstdlib>
#include <filesystem>
#include <gpiod.hpp>
#include "LumaC++Wrapper.hpp"

#define BOOLEAN 0
#define INTEGER 1
#define DECIMAL 2

class  menuEntry { // menuEntry class saves all relevant information about a single menu entry on the screen
  public:
    string name = ""; // The visible name that will be displayed on the screen
    bool hasValue = false; // true if there is a value to the left of the 'name'
    bool isConfigurable = false; // true if the user can interact/change the value
    int valueType = BOOLEAN; // Stores what the value is
    float value = 0; // Stores the value
    float minValue = 0; // Minimum of the value
    float maxValue = 100; // Maximum of the value
    float steps = 1; // Increment/decrement steps
    bool wrapArround = true; // true if it should wrap back around when the value has gone over the max
    string humanReadableUnit = ""; // The 'unit' this string gets displayed (without spaces) after the value
};

class LumaMenuCPP {
  public:
    void init(string LumaConfigFilename, string fontFilename); // Initializer for the menu
    void setOptionVector(vector<menuEntry> options); // Put in a new string of selectable options
    void TMP_setSelectedOption(int option); // Temporary. yup!
    void setClickHandler(function<void(int, vector<menuEntry>)> _clickHandler); // Set a click handler that will be called when the virtual or the real encoder button has been pressed there is no output expected
    void stdinInput(string incStr = "w", string decStr = "s", string btnStr = "b", string quitStr = "q"); // Use stdin as an input if you need to test this code on a PC
    void buttonInput(int upPin, int downPin, int okPin, string GPIOChipName = "/dev/gpiochip0");
    void endInputHandler(); // End all/any input handler that are running
    void setBackgroundColor(Color _backgroundColor);
    void setForegroundColor(Color _foregroundColor);
    LumaCPPWrapper displayInstance; // The internally used LumaCPPWrapper instance for using the luma python module
  private:
    #define DEBOUNCE_TIME 5 // Debouncing time in milliseconds of the button/logic input
    #define OPTIONS_OFFSET_X 5 // Distance of the text to the left and right edge of the screen/window
    #define SPACING 18 // Distance between the options (top of the first rectangle to top of the next rectangle)
    #define ROUNDING 8 // How rounded the rounded rectangles are
    #define MAX_ENTRIES_ON_SCREEN 7 // How many entries fit on the screen at once
    #define TEXT_V_OFFSET 4 // The distance from the rectangle to the top of the text
    #define PIXEL_OFFSET -15 // Offset when scrolling (down only for now)
    
    bool runInputHandler = true; // If true the input handler loop runs if its false it will stop (NOTE: Input handlers need to set it to true before running)
    
    bool doesNotRequireFullRefresh = false; // Same here, maybe i need to look into my code NOTE: look into the code
    int highlightedEntryOld = -1; 
    Color foregroundColor = {255, 255, 255};
    Color backgroundColor = {0, 0, 0};
    int optionCount;
    bool optionSelectedForEditOld = false;
    bool optionSelectedForEdit = false;
    bool optionValueChanged = false;
    int highlightedEntry;
    int previoisFinalTextLength = 0;
    int previoisFinalTextHeight = 0; // If the font gets changed midway through somehow
    vector<menuEntry> optionVector; // Internal options vector
    
    string fontPath;

    void changeValueHandler(bool increment);  // Increment/decrement handler: does all things that should be done when the value gets changed
    void buttonHandler(); // Button handler: does all things that should be done when the button gets pressed
    
    void displayOption(); // Display the new options from the internal vector
    void displayValue(int x, int y, menuEntry menuEntry, Color backgroundColor, Color textColor);
    function<void(int, vector<menuEntry>)> clickHandler; // The click handler
};

