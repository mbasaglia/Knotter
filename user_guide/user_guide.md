Introduction
=======================

Interlace patterns are a kind of design hystorically used as a decorations in 
many places and by different cultures.

A good description of the algorithm used to render them and dsign tips can be 
found at [Celtic Knotwork: the Ultimate Tutorial](http://www.entrelacs.net/)

![An illumination on the cover of the Quran with an interlaced frame](img/quran.jpg "An illumination on the cover of the Quran with an interlaced frame")



Editing
=======================

A knot is produced from a graph where the edges define interactions between 
two knot lines.

Adding nodes
-----------------------

Double clicking on an empty spot on the drawing area will add a node.
If you double click on an edge, the new node will split that edge in two.

Adding edges
----------------------

To toggle an edge between two nodes, drag the mouse from the first node to
the second one while holding the right mouse button.

To create a complete subgraph from the selected nodes, use the *Link selected*
command.

Changing edge type
----------------------

Knotter supports three types of edge:

* Default  - The two lines cross at the edge midpoint, one on top of the other
* Inverted - Like default but with the other line on top
* Hole     - The line turn back without crossing
* Wall     - The lines follow the edge without crossing

![Screenshot showing the edge types](img/edge_types.png "From left to right: Default, Inverted, Hole, Wall")

The type of an edge is changed with the mouse wheel.

Quick graph design
----------------------

Adding nodes and edges one at the time can be very tedious, knotter provides
the *Edge list* tool to make the process easier.

When this is enabled, a single click will create a node and each new node is 
connected to the previous one by an edge.

To break the edge chain, click with the right button, you'll be able to start
a new chain from the next node.

Selection and transformation
----------------------------

Only nodes can be selected and moved, the edges will follow their vertices.

A single node is selected by left clicking it, multiple nodes can be selected
by holding Shift or Control.

Dragging the mouse from an empty spot will select the nodes within a 
rectangual area.

Dragging one of the selected nodes will move the selection, while moving 
nodes can be transformed using the mouse wheel (will scale the selection)
or Shift + mouse wheel (will rotate the selection around the node under the 
cursor). These transformations are available when pasting nodes.

Selected nodes can be mirrored using the horizontal and vertical flip commands.

Snapping 
----------------------

To allow precise designs knotter offers a grid that, when enabled, will snap
nodes to the closest point.

There are three types of grid:
* Square      - A regular square grid.
* Triangular1 - A grid where the elements are equilateral triangles with a horizontal base.
* Triangular2 - A grid where the elements are equilateral triangles with a vertical side.

![Screenshot showing some nodes on a grid](img/grid.png "The grid allows precise editing")

Style
=======================
The *Knot Style* dialog allows the configuration of display and shape options.
Changes are visible in the preview area of the dialog.

* Curve Control Point Distance - How much the knot line follows its direction when 
                                 exiting from an edge.
* Crossing Gap - Distance between the end points when a lines are crossing.
* Minimum Cusp Angle - Minimum angle between two edge to make the knot line 
                       display a cuspid rather than a smooth curve.
* Curve Style  - Various cuspid shapes
![Screenshot showing the various curve shapes](img/cusp_shapes.png "Curve shapes avaliable in knotter")
* Point Style  - How the sharp points are rendered
    * Bevel - Flat
    * Miter - with a sharp point
    * Round - Smooth

![Screenshot of the display style settings](img/style1.png)
![Screenshot of the shape style settings](img/style2.png)


Save and Load
=======================
Knotter can save and load knots in its own human-readable format.

Export
----------------------
The Export dialog gives many options to export the knot as Image

### Vector

* Only shape - Only export the shape of the knot as a thin line, discarding 
                any other style option

### Raster

* Antialias   - If checked, the output image will be supersampled.
* Compression - The compression level, the result depends on the exported format.
    * 0%   = Large files
    * 100% = Small files 
* Width/Height - The resulting image size, if they are different from the 
                 actual knot it will be scaled to fit those.
* Keep Ratio   - When enabled, editing Width/Height will preserve their ratio.
* Reset Size   - Change Width and Height back to the default.

![Export dialog](img/export.png)

