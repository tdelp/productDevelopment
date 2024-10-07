# productDevelopment
Private repository for Software Development in C++

# GUI Renderer Project

## Overview
This project implements a GUI rendering system that uses XML files to stage and render graphical elements such as lines, boxes, and points using SDL2. The system includes classes for handling file input/output, XML parsing, and graphical rendering.

## Class Documentation

### GUIFile
The `GUIFile` class is responsible for managing the loading and saving of GUI elements from and to XML files. It handles three main types of graphical elements: lines, boxes, and points.

#### Public Methods

- **`GUIFile()`**: Constructor that initializes the `GUIFile` object.
- **`~GUIFile()`**: Destructor that cleans up any resources.
- **`void addLine(Line line)`**: Adds a `Line` object to the internal list of lines.
    - **Parameters:** `line` - A `Line` object representing a line element with start and end positions and color.
- **`void addBox(Box box)`**: Adds a `Box` object to the internal list of boxes.
    - **Parameters:** `box` - A `Box` object representing a box element with minimum and maximum bounds and color.
- **`void addPoint(Point point)`**: Adds a `Point` object to the internal list of points.
    - **Parameters:** `point` - A `Point` object representing a point element with position and color.
- **`bool writeToFile(const std::string& filename)`**: Writes the staged GUI elements to an XML file.
    - **Parameters:** `filename` - The name of the file to which the XML data will be written.
    - **Returns:** `true` if the file was written successfully, `false` otherwise.
- **`bool readFromFile(const std::string& filename)`**: Reads GUI elements from an XML file and stages them.
    - **Parameters:** `filename` - The name of the file from which the XML data will be read.
    - **Returns:** `true` if the file was read successfully, `false` otherwise.
- **`const std::vector<Line>& getLines() const`**: Retrieves the list of lines parsed from the XML file.
- **`const std::vector<Box>& getBoxes() const`**: Retrieves the list of boxes parsed from the XML file.
- **`const std::vector<Point>& getPoints() const`**: Retrieves the list of points parsed from the XML file.

### Testing
The project includes comprehensive tests to validate the XML read and write operations, as well as the rendering logic using the `Screen` class.

#### Running the Tests
- **XML Read/Write Test:** Validates the ability of the `GUIFile` class to write GUI elements to an XML file and then read them back, ensuring data integrity.
- **Rendering Test:** Verifies that the GUI elements are correctly displayed on the screen using the SDL-based `Screen` class.

To run the tests, execute the `test_gui_file` program. Ensure that SDL2 is correctly installed and linked.

### UML Diagram
Below is a UML diagram that represents the relationship between the main classes used in this project: `GUIFile`, `Screen`, and `XMLParser`.

![UML Diagram](uml_diagram.png)

- **`GUIFile`**: Manages XML file operations and staging of GUI elements.
- **`Screen`**: Handles rendering of the graphical elements on the screen using SDL.
- **`XMLParser`**: Provides functionality to parse XML files and extract GUI elements.

### Dependencies
- **SDL2**: Required for graphical rendering.
- **Standard C++ libraries**: Required for file I/O operations and data handling.

### Getting Started
1. **Build the Project:** Compile the source files with your preferred C++ compiler and link the SDL2 library.
2. **Run the Tests:** Use the provided `test_gui_file` program to verify that the XML and rendering functionalities work correctly.
3. **Usage:** Modify the XML input file to render different GUI elements as desired.


