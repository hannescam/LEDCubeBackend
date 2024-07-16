/* LumaC++Wrapper.cpp
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

#include <LumaC++Wrapper.hpp>

using namespace std;
namespace py = pybind11;
using namespace py::literals;

// Function to update the screen based on what is in the buffer
// INPUT: N/A
// OUTPUT: N/A

void LumaCPPWrapper::display() {
  device.attr("display")(draw); // Push the canvas to the screen
}

// Function for filling the enire screen with a single color (default specified in the header: black (0, 0, 0))
// INPUT: color *the color that the screen should be filled with*
// OUTPUT: N/A

void LumaCPPWrapper::fillScreen(Color color) {
  drawCanvas.attr("rectangle")(xywh2shape(0, 0, displayWidth-1, displayHeight-1), "fill"_a=Color2PILColor(color));
}

// Function for converting a integer coordinate, a width, and a height to a PIL compatible "shape"
// INPUT: x *x coordinate*, y *y coordinate* w *width*, h *height*
// OUTPUT: py::object *containing the point like in python [x, y, w, h]*

py::object LumaCPPWrapper::xywh2shape(int x0, int y0, int w, int h){
  int x1 = x0 + w; // Convert width/height to x1/y1
  int y1 = y0 + h;
  py::object shape = py::dict("x0"_a=x0, "y0"_a=y0, "x1"_a=x1, "y1"_a=y1); // Create a py::object for saving the X, Y, W, and H integer to
  py::exec(R"(shape = [x0, y0, x1, y1])", py::globals(), shape); // Run the conversion using the python interpreter
  return shape["shape"]; // Return the shape
}

// Function for converting integer coordinates to python compatible coordinates
// INPUT: x *x coordinate*, y *y coordinate*
// OUTPUT: py::object *containing the point like in python (x, y)*

py::object LumaCPPWrapper::xy2point(int x, int y){
  py::object point = py::dict("x"_a=x, "y"_a=y); // Create a py::object for saving the X and Y integer to
  py::exec(R"(point = (x, y))", py::globals(), point); // Run the conversion using the python interpreter
  return point["point"]; // Return the point
}

// Function for converting the Color class to python/PIL compatible color
// INPUT: color *the color*
// OUTPUT: py::object *containing the color like in python (r, g, b)*

py::object LumaCPPWrapper::Color2PILColor(Color color) {
  py::object PILColor = py::dict("r"_a=color.r, "g"_a=color.g, "b"_a=color.b); // Create a py::object for saving the r, g and b color values to a py::object
  py::exec(R"(PILColor = (r, g, b))", py::globals(), PILColor); // Run the conversion using the python interpreter
  return PILColor["PILColor"]; // Return the PILColor
}

// Function for initializing Luma and PIL (through pybind11) for accessing a SPI/I2C/other screen
// INPUT: LumaConfigFile *the Luma configuration file path*
// OUTPUT: N/A

void LumaCPPWrapper::init(string LumaConfigFile, string fontFilename, int fontSize, bool shouldBeAntialiased) {
  py::module_ PIL = py::module_::import("PIL"); // Import (python module) PIL for handling the image operations
  py::module_ LumaCmdline = py::module_::import("luma.core.cmdline");

  py::object argsInput = LumaCmdline.attr("create_parser")("description"_a="LumaMenuC++").attr("parse_args")(LumaCmdline.attr("load_config")(LumaConfigFile));
  device = LumaCmdline.attr("create_device")(argsInput);

  ImageFont = PIL.attr("ImageFont");
  draw = PIL.attr("Image").attr("new")(device.attr("mode"), device.attr("size")); // Create a new PIL Image
  drawCanvas = PIL.attr("ImageDraw").attr("Draw")(draw); // Create a new PIL ImageDraw for drawing text/shapes

  displayWidth = device.attr("width").cast<int>();
  displayHeight = device.attr("height").cast<int>();

  font = ImageFont.attr("truetype")(fontFilename, fontSize);

  if (!shouldBeAntialiased) {
    font.attr("fontmode") = "1";
    drawCanvas.attr("fontmode") = "1";
  }
}

// Function to set a truetype font that will be used to draw text to the screen
// INPUT: filename *the filename/filepath of the font*, fontSize *height of the font in pixels*, shouldBeAntialiased *if true the text will be antialiased*
// OUTPUT: N/A

void LumaCPPWrapper::setTrueTypeFont(string filename, int fontSize, bool shouldBeAntialiased) {
  font = ImageFont.attr("truetype")(filename, fontSize);
  if (!shouldBeAntialiased) {
    font.attr("fontmode") = "1";
  }
}

// Function to draw a rectangle to the screen using height and width
// INPUT: x *the x coordinate of the top left point in the rectangle*, y *the y coordinate of the top left point in the rectangle*, w *width of the rectangle in pixel*, h *height of the rectangle in pixel*, color *the color of the outline or filling of the rectangle*, filled *set to true to fill the rectangle and set to false if you only want an outline*
// OUTPUT: N/A

void LumaCPPWrapper::drawRectangle(int x, int y, int w, int h, Color color, bool filled) {
  if (w > 0 && h > 0) {
    if (filled) {
      drawCanvas.attr("rectangle")(xywh2shape(x, y, w-1, h-1), "fill"_a=Color2PILColor(color));
    } else {
      drawCanvas.attr("rectangle")(xywh2shape(x, y, w-1, h-1), "outline"_a=Color2PILColor(color));
    }
  }
}

// Function to draw a rectangle to the screen using height and width
// INPUT: x *the x coordinate of the top left point in the rectangle*, y *the y coordinate of the top left point in the rectangle*, w *width of the rectangle in pixel*, h *height of the rectangle in pixel*, color *the color of the outline or filling of the rectangle*, filled *set to true to fill the rectangle and set to false if you only want an outline*, radius *the radius of the rounding*
// OUTPUT: N/A

void LumaCPPWrapper::drawRoundRectangle(int x, int y, int w, int h, Color color, bool filled, int radius) {
  if (w > 0 && h > 0) {
    if (filled) {
      drawCanvas.attr("rounded_rectangle")(xywh2shape(x, y, w-1, h-1), "fill"_a=Color2PILColor(color), "radius"_a=radius);
    } else {
      drawCanvas.attr("rounded_rectangle")(xywh2shape(x, y, w-1, h-1), "outline"_a=Color2PILColor(color), "radius"_a=radius);
    }
  }
}

// Function for printing text to the screen using PIL/Luma through pybind11
// INPUT: text *the actual text*, x *the top left x coordinate*, y *the top left y coordinate*
// OUTPUT: N/A

void LumaCPPWrapper::drawText(string text, int x, int y, Color color, string anchor) {
  drawCanvas.attr("text")(xy2point(x, y), text, "fill"_a=Color2PILColor(color), "anchor"_a=anchor, "font"_a=font); // Actually place the text including setting the color and the position
}

// Function to get the width of the inputted string in pixels with the current font
// INPUT: text *the text that is used to determine length*
// OUTPUT: int *the width of the text*

int LumaCPPWrapper::getTextWidth(string text) {
  py::tuple textSizePyTuple = drawCanvas.attr("textbbox")("text"_a=text,"xy"_a=xy2point(0, 0), "font"_a=font);
  return textSizePyTuple[2].cast<int>();
}

// Function to get the height of the inputted string in pixels with the current font
// INPUT: text *the text that is used to determine height*
// OUTPUT: int *the height of the text*

int LumaCPPWrapper::getTextHeight(string text) {
  py::tuple textSizePyTuple = drawCanvas.attr("textbbox")("text"_a=text,"xy"_a=xy2point(0, 0), "font"_a=font);
  return textSizePyTuple[3].cast<int>();
}
