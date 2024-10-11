# GUIFile

The `GUIFile` class is designed to manage the loading and saving of a GUI environment using XML files. It supports the staging of GUI elements (lines, boxes, points) using setter methods, which can then be written to a file. Additionally, it provides methods for reading an XML file and storing the parsed GUI elements, which can later be accessed through getter methods.

## Class Overview

The `GUIFile` class uses internal containers to store lines, boxes, and points, which can be easily managed and cleared between file operations. These containers are used for both staging elements before writing to a file and for storing elements parsed from an XML file.

## Public Methods

### addLine
- **Description**: Stages a line element to be stored in the class.
- **Parameters**:
  - `start` (std::array<float, 2>): Starting coordinates of the line.
  - `end` (std::array<float, 2>): Ending coordinates of the line.
  - `color` (std::array<float, 3>): Color of the line in RGB format.
- **Behavior**: Adds the specified line to the internal lines container for future writing to an XML file.

### addBox
- **Description**: Stages a box element to be stored in the class.
- **Parameters**:
  - `min` (std::array<float, 2>): Minimum bounds of the box.
  - `max` (std::array<float, 2>): Maximum bounds of the box.
  - `color` (std::array<float, 3>): Color of the box in RGB format.
- **Behavior**: Adds the specified box to the internal boxes container for future writing to an XML file.

### addPoint
- **Description**: Stages a point element to be stored in the class.
- **Parameters**:
  - `position` (std::array<float, 2>): Position of the point.
  - `color` (std::array<float, 3>): Color of the point in RGB format.
- **Behavior**: Adds the specified point to the internal points container for future writing to an XML file.

### readFile
- **Description**: Reads an XML file and parses the contents into the internal containers.
- **Parameters**:
  - `filename` (std::string): The name of the file to read.
- **Behavior**: Opens the specified XML file, reads its contents, and parses the GUI elements (lines, boxes, points) into the appropriate containers. Returns `true` if the file was successfully read, otherwise `false`.

### getLines
- **Description**: Retrieves the stored line elements.
- **Return Type**: `const std::vector<Line>&`
- **Behavior**: Returns a constant reference to the vector containing all the parsed line elements.

### getBoxes
- **Description**: Retrieves the stored box elements.
- **Return Type**: `const std::vector<Box>&`
- **Behavior**: Returns a constant reference to the vector containing all the parsed box elements.

### getPoints
- **Description**: Retrieves the stored point elements.
- **Return Type**: `const std::vector<Point>&`
- **Behavior**: Returns a constant reference to the vector containing all the parsed point elements.

## XML Writing Methods

### openOutputFile
- **Description**: Opens the output file for writing and initializes the XML structure.
- **Behavior**: Writes the opening `<layout>` tag to the file and ensures the file is ready for subsequent writes.

### closeOutputFile
- **Description**: Closes the output file after completing the write operations.
- **Behavior**: Writes the closing `</layout>` tag and properly closes the file.

### writeLineElement
- **Description**: Writes a staged line element to the XML file.
- **Parameters**:
  - `line` (Line): The line element to write.
- **Behavior**: Outputs the XML representation of the line element to the file.

### writeBoxElement
- **Description**: Writes a staged box element to the XML file.
- **Parameters**:
  - `box` (Box): The box element to write.
- **Behavior**: Outputs the XML representation of the box element to the file.

### writePointElement
- **Description**: Writes a staged point element to the XML file.
- **Parameters**:
  - `point` (Point): The point element to write.
- **Behavior**: Outputs the XML representation of the point element to the file.

## Private Methods

### parseVec2
- **Description**: Parses a 2D vector (vec2 or ivec2) from the XML data.
- **Parameters**:
  - `stream` (std::istringstream&): The input stream containing the XML data.
  - `vector` (std::array<float, 2>&): The array to store the parsed values.
- **Behavior**: Extracts the `<x>` and `<y>` values and stores them in the provided array.

### parseVec3
- **Description**: Parses a 3D vector (vec3 or ivec3) from the XML data.
- **Parameters**:
  - `stream` (std::istringstream&): The input stream containing the XML data.
  - `vector` (std::array<float, 3>&): The array to store the parsed values.
- **Behavior**: Extracts the `<x>`, `<y>`, and `<z>` values and stores them in the provided array.

## Container Design

The `GUIFile` class uses the following containers for managing GUI elements:
- `std::vector<Line>`: Stores line elements.
- `std::vector<Box>`: Stores box elements.
- `std::vector<Point>`: Stores point elements.

### Container Management
- Containers are cleared when switching between reading and writing operations to optimize memory usage.
- Elements are staged using setter methods (`addLine`, `addBox`, `addPoint`) and accessed through getter methods (`getLines`, `getBoxes`, `getPoints`) without directly modifying the stored data.

## Future Enhancements

The current implementation is designed to handle simple XML structures. As client requirements evolve, the following enhancements might be considered:
- Support for hierarchical XML structures.
- Error handling improvements for more robust parsing of malformed XML files.
- Extending the GUI element types to support additional shapes or attributes.

## UML Diagram



## Usage Example

Here is a basic example of how to use the `GUIFile` class:

```cpp
GUIFile guiFile;
guiFile.addLine({0.0f, 0.0f}, {1.0f, 1.0f}, {255.0f, 0.0f, 0.0f}); // Staging a line
guiFile.addBox({0.0f, 0.0f}, {2.0f, 2.0f}, {0.0f, 255.0f, 0.0f});  // Staging a box
guiFile.addPoint({1.0f, 1.0f}, {0.0f, 0.0f, 255.0f});              // Staging a point

guiFile.readFile("input.xml");                                      // Reading from an XML file

const auto& lines = guiFile.getLines();                             // Accessing parsed lines
const auto& boxes = guiFile.getBoxes();                             // Accessing parsed boxes
const auto& points = guiFile.getPoints();                           // Accessing parsed points
