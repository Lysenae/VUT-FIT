// File:        shape.h
// Author:      Daniel Klimaj (xklima22)
// Description: Set and draw shapes.

#ifndef SHAPE_H
#define SHAPE_H

#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <cstdlib>

#include "colors.h"
#include "shapeproperties.h"

class Shape
{
public:
  static void SetShape(int shape);
  static void SetBorder(int border);
  static void SetLineWidth(int width);
  static void SetFill(bool fill);
  static int Border(int border);

  static void InitDrawGC(Widget w);
  static void SetInputGC(Widget w);
  static void SetDrawGC(Widget w);

  static void Draw(Widget w, int x1, int y1, int x2, int y2);
  static void DrawAll(Widget w);

  static void SetInputStyle(Widget w);
  static void SetDrawStyle(Widget w);

  static void Add(int x1, int y1, int x2, int y2);

  static void ClearAll();

  static const int POINT         = 0;
  static const int LINE          = 1;
  static const int RECT          = 2;
  static const int ELLIPSE       = 3;
  static const int BORDER_FULL   = 0;
  static const int BORDER_DOTTED = 1;
  static const int ANGLE1        = 0;
  static const int ANGLE2        = 23040; // 360*64
  static const int ALLOC_STEP    = 10;

private:
  static bool m_fill;
  static int m_shape;
  static int m_border;
  static int m_line_width;
  static int m_shapes_count;
  static int m_max_shapes;

  static GC m_input_gc;
  static GC m_draw_gc;
  static Pixel m_fg;
  static Pixel m_bg;

  static ShapeProperties **m_shapes;

  static int Width(int x1, int x2);
  static int Height(int y1, int y2);
  static int Min(int x1, int x2);
};

#endif // LINES_H
