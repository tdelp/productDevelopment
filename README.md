# GUI Layout Application

This document serves as a guide to understanding the codebase for the GUI layout application, detailing how the `GUIFile`, `Layout`, `Parse`, `Element` classes, and the `ElementFactory` function together to create a modular and interactive GUI system in SDL. This application dynamically renders GUI elements from an XML configuration file, adjusting their states based on user interactions.

## Project Structure

- **GUIFile**: Manages drawing logic for different elements.
- **Layout**: Organizes GUI elements and controls rendering behavior based on activity states.
- **Parse**: Parses an XML file into nested layouts and elements for display.
- **Element**: Base class for drawable elements.
- **ElementFactory**: Creates element instances based on XML input data.

## Components

### 1. GUIFile
`GUIFile` is the main drawing interface for the application. It encapsulates various drawable elements, such as lines, boxes, points, and triangles, and provides methods to handle each element's rendering logic. The implementation ensures safe rendering with bounds-checking for each element type.

### 2. Layout
The `Layout` class represents a logical container for `Element` objects and other nested `Layout`s. Key features of `Layout` include:

- **Element Management**: Supports adding, removing, and rendering elements based on user interactions.
- **Nested Layouts**: Allows layouts within layouts, enabling complex UI structures.
- **Dynamic Rendering**: Manages the position and size of layouts based on the `sX`, `sY`, `eX`, `eY` attributes defined in the XML configuration. This flexibility allows for positioning layouts relative to parent dimensions.
- **Active State**: The `setActive` method toggles layout visibility based on user interaction.

### 3. Parse
The `Parse` class handles reading XML data to dynamically build the layout structure. It loads and parses elements by reading `vec2` and `vec3` tags to set position and color values, respectively. The parser recursively loads nested layouts, using `ElementFactory` to instantiate specific elements based on tag types.

**Parsing Process**:
- **Root Layout**: Initiates parsing from the root layout defined in the XML.
- **Element Parsing**: Extracts and instantiates elements like lines, points, boxes, and triangles based on tags.
- **Attribute Parsing**: Reads specific attributes (`sX`, `sY`, `eX`, `eY`, and `active`) for layout positioning.

### 4. Element
`Element` is an abstract base class for drawable components. Derived classes (`LineElement`, `BoxElement`, `PointElement`, and `TriangleElement`) implement the `draw` and `isInside` methods to define each element’s behavior:

- **LineElement**: Draws lines using the Bresenham algorithm.
- **BoxElement**: Draws boxes with boundary checks.
- **PointElement**: Represents a single pixel point on the screen.
- **TriangleElement**: Uses the cross-product method to check if a point is inside the triangle.

### 5. ElementFactory
The `ElementFactory` class creates instances of `Element` subclasses based on XML tags. This factory pattern encapsulates element creation logic, keeping the parser code clean and extensible.

## Main Application Flow

The application initializes SDL, creates a `Screen` object, and parses an XML file (`input.xml`) into a `rootLayout`. The main loop checks the mouse position to toggle nested layouts and renders elements based on their active states. The `Layout::render` method recursively draws all active elements.

**Mouse Interaction**:
- Checks if the mouse is within a triangle’s bounds to toggle the nested layout’s `active` state.


# Application Demo Modifications

- **Root Layout**: Contains a triangle and a box.
- **Nested Layout**: Includes three elements - a triangle, a line, and a point.
- **Interaction**: Moving the mouse cursor inside the triangle toggles the activation of the nested layout.

## Run Instructions

1. **Build the project.**
2. Place `input.xml` in the working directory.
3. Run the application. Use the SDL window to interact with elements.

---

# XML Configuration and Interaction Demo

To simulate interactions and understand layout nesting behavior:

1. Start the application and observe root elements (e.g., triangle, box).
2. Hover over the triangle to toggle nested layout visibility.
3. Adjust layout positions by modifying `sX`, `sY`, `eX`, `eY` values in the XML configuration.

This setup enables a responsive and interactive GUI system that can be easily extended by modifying the XML configuration file.

## link to visual diagram showing relational spacing
[Link](diagram.jpg)

# Relational Spacing in GUI Layout

This document explains how relational spacing works for the XML file provided, focusing on how `sX`, `sY`, `eX`, and `eY` attributes determine the position and size of nested layouts.

## XML Structure and Relational Spacing

### Root Layout

The root `<layout>` element contains two direct child elements (a triangle and a box) and one nested layout.

1. **Triangle**:
   - Defined by three points with absolute coordinates:
     - `(600, 100)`, `(700, 250)`, and `(650, 400)`.
   - Since this triangle is a direct child of the root layout, its coordinates are based on the entire screen area (absolute positioning).

2. **Box**:
   - Defined with absolute coordinates:
     - Top-left corner: `(800, 150)`, bottom-right corner: `(1000, 350)`.
   - Like the triangle, this box is positioned within the root layout’s dimensions, relative to the screen.

3. **Nested Layout**:
   - The nested layout has attributes `sX`, `sY`, `eX`, and `eY`, which define its position and size relative to the root layout.
   - **Relational Coordinates**:
     - `sX: 0.1`, `sY: 0.1`: The starting position of the nested layout is 10% of the root layout’s width and height from the top-left corner.
     - `eX: 0.5`, `eY: 0.5`: The ending position of the nested layout extends to 50% of the root layout’s width and height.
   - This nested layout will, therefore, occupy the area from `(10%, 10%)` to `(50%, 50%)` of the screen (or root layout), creating a contained region within the root layout.

### Elements within the Nested Layout

The nested layout contains a triangle, a line, and a point. These elements have absolute coordinates, but they are positioned within the bounds of the nested layout.

1. **Triangle in Nested Layout**:
   - Defined by three points:
     - `(100, 150)`, `(500, 250)`, and `(350, 200)`.
   - Since these coordinates are within the nested layout, they are based on the dimensions of the nested layout area (from `(10%, 10%)` to `(50%, 50%)` of the root layout).

2. **Line in Nested Layout**:
   - Defined with two endpoints:
     - Start: `(150, 150)`, End: `(400, 400)`.
   - These coordinates are also within the nested layout’s bounds.

3. **Point in Nested Layout**:
   - Defined by a single position:
     - `(250, 250)`.
   - This point is positioned within the nested layout’s coordinates and will render inside the area defined by the nested layout’s `sX`, `sY`, `eX`, and `eY`.

## Summary

The `sX`, `sY`, `eX`, and `eY` attributes in each `<layout>` tag allow nested layouts to be positioned relative to their parent layout. This approach enables flexible, relational positioning that automatically adjusts based on the dimensions of the parent layout, supporting responsive and adaptive GUI designs.


## XML Input Described Above

```xml
<layout>
    <triangle>
        <vec2><x>600</x><y>100</y></vec2>
        <vec2><x>700</x><y>250</y></vec2>
        <vec2><x>650</x><y>400</y></vec2>
        <vec3><x>255</x><y>100</x><z>50</z></vec3>
    </triangle>
    <box>
        <vec2><x>800</x><y>150</y></vec2>
        <vec2><x>1000</x><y>350</y></vec2>
        <vec3><x>100</x><y>150</x><z>200</z></vec3>
    </box>
    <layout>
        <sX>0.1</sX>
        <sY>0.1</sY>
        <eX>0.5</eX>
        <eY>0.5</eY>
        <active>false</active>
        <triangle>...</triangle>
        <line>...</line>
        <point>...</point>
    </layout>
</layout>