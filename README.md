# Polymorphic GUI Element System with Factory and Rasterization

## Project Overview

This project implements a modular, reusable, and low-coupling graphical user interface (GUI) system. The design includes an abstract base class for all GUI elements, a factory pattern for element creation, and a `Screen` class for rasterization of elements. Additionally, the system supports a triangle rasterization algorithm to draw filled triangles. The project follows key software engineering principles such as low coupling, high cohesion, and future extensibility.

## Key Features

### 1. Abstract Base Class for GUI Elements (`Element`)
The `Element` class serves as an interface for all GUI elements. Derived elements like `LineElement`, `BoxElement`, `PointElement`, and `TriangleElement` implement the common interface, ensuring uniform handling.
- **Polymorphism**: Supports future polymorphic behaviors for interactions (e.g., mouse clicks, keyboard inputs, animations).

### 2. Factory Pattern (`ElementFactory`)
The factory centralizes the creation of GUI elements. Client code interacts with the factory instead of directly constructing elements.
- **Low Coupling**: The factory encapsulates the logic for creating elements, allowing for easy future extensions.

### 3. Screen Rasterization
The `Screen` class handles all pixel drawing and rasterization. It includes methods like `drawSafeLine`, `drawSafeBox`, and `drawSafeTriangle` for drawing on the screen.
- **Filled Triangle Rasterization**: Implements a cross-product method to test if a pixel lies inside a triangle.

### 4. Extensibility for Future Interaction
The design allows future extensions for handling clickable elements, time-driven updates, and keyboard inputs through polymorphic methods in the `Element` class.

### 5. Low Coupling & High Cohesion
- **Low Coupling**: Elements are decoupled from drawing and creation logic, promoting independent development and easier maintenance.
- **High Cohesion**: Each class serves a single, well-defined purpose, ensuring clear responsibilities and maintainable code.

## UML Diagram

```plaintext
+-----------------------------------+
|              Element              |
+-----------------------------------+
| +draw(Screen&): void              |
| +writeToFile(std::ofstream&): void|
+-----------------------------------+
              /_\
               |
+-----------------------+   +-----------------------+   +-----------------------+
|     LineElement       |   |     BoxElement        |   |   TriangleElement      |
+-----------------------+   +-----------------------+   +-----------------------+
| -start: array<float,2>|   | -min: array<float,2>  |   | -v0: array<float,2>    |
| -end: array<float,2>  |   | -max: array<float,2>  |   | -v1: array<float,2>    |
| -color: array<float,3>|   | -color: array<float,3>|   | -v2: array<float,2>    |
+-----------------------+   +-----------------------+   +-----------------------+
| +draw(Screen&): void  |   | +draw(Screen&): void  |   | +draw(Screen&): void   |
| +writeToFile(ofstream)|   | +writeToFile(ofstream)|   | +writeToFile(ofstream) |
+-----------------------+   +-----------------------+   +-----------------------+

+---------------------------------------------+
|              ElementFactory                 |
+---------------------------------------------+
| +createLine(start, end, color): Element*    |
| +createBox(min, max, color): Element*       |
| +createTriangle(v0, v1, v2, color): Element*|
+---------------------------------------------+

## Screen Class (Rasterization Engine)

```plaintext
+---------------------------------------------+
|              Screen                         |
+---------------------------------------------+
| +setSafePixel(position, color): void        |
| +drawSafeLine(start, end, color): void      |
| +drawSafeBox(min, max, color): void         |
| +drawSafeTriangle(v0, v1, v2, color): void  |
+---------------------------------------------+

+-----------------------+    +--------------------+    +-----------------------+
|  Read XML File        | -> |  Create Elements   | -> |   Draw Elements on     |
|                       |    | via ElementFactory |    |      Screen            |
+-----------------------+    +--------------------+    +-----------------------+
                                   |
                                   v
                             +---------------------+
                             |  Save to XML        |
                             +---------------------+
