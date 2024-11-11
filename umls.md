```plaintext
+-----------------------------------------------------------------+
|                             Layout                              |
+-----------------------------------------------------------------+
| - sX: float                                                     |
| - sY: float                                                     |
| - eX: float                                                     |
| - eY: float                                                     |
| - active: bool                                                  |
| - start: ivec2                                                  |
| - end: ivec2                                                    |
| - elements: vector<unique_ptr<Element>>                         |
| - nestedLayouts: vector<unique_ptr<Layout>>                     |
+-----------------------------------------------------------------+
| + addElement(element: unique_ptr<Element>): void                |
| + addNestedLayout(layout: unique_ptr<Layout>): void             |
| + calculatePosition(parentStart: ivec2, parentEnd: ivec2): void |
| + render(screen: Screen&): void                                 |
+-----------------------------------------------------------------+

                 /_\
                  |
     +------------+------------+
     |                         |
+----------------+     +---------------------+
|    Element     |     |  LayoutFactory      |
+----------------+     +---------------------+
| <<abstract>>   |     | + createLayout(...) |
| + draw(...)    |     | + addElement(...)   |
| + isInside(...)|     | + addNestedLayout() |
+----------------+     +---------------------+
         /_\
          |
        +------------+---------+-------------------+------------------+
        |                      |                   |                  |
+----------------+    +----------------+   +----------------+ +------------------+
|  LineElement   |    |   BoxElement   |   | TriangleElement ||  PointElement    |
+----------------+    +----------------+   +----------------+ +------------------+
| - start: vec2  |    | - min: vec2    |   | - v0: vec2     | | - position: vec2 |
| - end: vec2    |    | - max: vec2    |   | - v1: vec2     | | - color: vec3    |
| - color: vec3  |    | - color: vec3  |   | - v2: vec2     | |                  |
+----------------+    +----------------+   +----------------+ +------------------+
| + draw(...)    |    | + draw(...)    |   | + draw(...)    | | + draw(...)      |
| + isInside(...)|    | + isInside(...)|   | + isInside(...)| | + isInside(...)  |
+----------------+    +----------------+   +----------------+ +------------------+

+---------------------------------------------+
|               ElementFactory                |
+---------------------------------------------+
| + createLine(...) : unique_ptr<Element>     |
| + createBox(...) : unique_ptr<Element>      |
| + createTriangle(...) : unique_ptr<Element> |
| + createPoint(...) : unique_ptr<Element>    |
+---------------------------------------------+
