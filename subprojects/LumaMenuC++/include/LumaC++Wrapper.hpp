/* LumaC++Wrapper.hpp
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
#include <pybind11/embed.h> // everything needed for embedding python

using namespace std;
namespace py = pybind11;

class Color { // Color class to define a color with one variable
    public:
        uint8_t r = 0; // Red color value
        uint8_t g = 0; // Green color value
        uint8_t b = 0; // Blue color value
};

class LumaCPPWrapper {
  public:
    void display();
    void drawText(string text, int x, int y, Color color, string anchor = "lt");
    void drawRectangle(int x, int y, int w, int h, Color color, bool filled = false);
    void drawRoundRectangle(int x, int y, int w, int h, Color color, bool filled = false, int radius = 8);
    void fillScreen(Color color = {0, 0, 0});
    void init(string LumaConfigFile, string fontFilename, int fontSize = 11, bool shouldBeAntialiased = false);
    void setTrueTypeFont(string filename, int fontSize = 11, bool shouldBeAntialiased = false);
    int getTextWidth(string text);
    int getTextHeight(string text);
    int displayWidth = 0;
    int displayHeight = 0;
  private:
    py::scoped_interpreter guard{}; // Init interpreter here so it doesn't shutdown when another function starts
    py::object device;
    py::object draw;
    py::object drawCanvas;
    py::object font;
    py::object ImageFont;

    py::object xy2point(int x, int y);
    py::object Color2PILColor(Color color);
    py::object xywh2shape(int x, int y, int w, int h);
};
