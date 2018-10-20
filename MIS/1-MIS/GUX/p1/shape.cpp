// File:        shape.cpp
// Author:      Daniel Klimaj (xklima22)
// Description: Set and draw shapes.

#include "shape.h"

bool Shape::m_fill        = false;
int Shape::m_shape        = Shape::POINT;
int Shape::m_border       = Shape::BORDER_FULL;
int Shape::m_line_width   = 1;

GC Shape::m_input_gc      = 0;
GC Shape::m_draw_gc       = 0;
Pixel Shape::m_fg         = 0;
Pixel Shape::m_bg         = 0;

int Shape::m_shapes_count = 0;
int Shape::m_max_shapes   = 0;

ShapeProperties **Shape::m_shapes = 0;

void Shape::SetShape(int shape)
{
  m_shape = shape;
}

void Shape::SetBorder(int border)
{
  m_border = border;
}

void Shape::SetLineWidth(int width)
{
  m_line_width = width;
}

void Shape::SetFill(bool fill)
{
  m_fill = fill;
}

///
/// \brief Get Xlib's id of border type.
/// \param border int
///
int Shape::Border(int border)
{
  return border == Shape::BORDER_FULL ? LineSolid : LineDoubleDash;
}

///
/// \brief Initialize drawing context.
/// \param w Widget
///
void Shape::InitDrawGC(Widget w)
{
  if(!m_draw_gc)
  {
    m_draw_gc = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
  }
}

///
/// \brief Set input context.
/// \param w Widget
///
void Shape::SetInputGC(Widget w)
{
  if(!m_input_gc)
  {
    m_input_gc = XCreateGC(XtDisplay(w), XtWindow(w), 0, NULL);
    XSetFunction(XtDisplay(w), m_input_gc, GXxor);
    XSetPlaneMask(XtDisplay(w), m_input_gc, ~0);
    XtVaGetValues(w, XmNforeground, &m_fg, XmNbackground, &m_bg, NULL);
    XSetForeground(XtDisplay(w), m_input_gc, m_fg ^ m_bg);
  }
}

///
/// \brief Set drawing context.
/// \param w Widget
///
void Shape::SetDrawGC(Widget w)
{
  Arg al[4];
  int ac;
  XGCValues v;

  if(!m_draw_gc)
  {
    ac = 0;
    XtSetArg(al[ac], XmNforeground, &v.foreground); ac++;
    XtGetValues(w, al, ac);
    m_draw_gc = XCreateGC(XtDisplay(w), XtWindow(w), GCForeground, &v);
  }
}

///
/// \brief Draw selected shape with selected properties.
/// \param w Drawable Widget
/// \param x1 Initial x coord
/// \param y1 Initial y coord
/// \param x2 End x coord
/// \param y2 End y coord
///
void Shape::Draw(Widget w, int x1, int y1, int x2, int y2)
{
  int width, height, x, y;

  if(m_shape == Shape::POINT)
  {
    if(m_line_width == 0)
    {
      XDrawPoint(XtDisplay(w), XtWindow(w), m_draw_gc, x1, y1);
    }
    else
    {
      x      = x1 - m_line_width;
      y      = y1 - m_line_width;
      width  = 2 * m_line_width + 1;
      height = 2 * m_line_width + 1;
      XFillArc(XtDisplay(w), XtWindow(w), m_draw_gc, x, y, width, height,
        Shape::ANGLE1, Shape::ANGLE2);
    }
  }
  else if(m_shape == Shape::LINE)
  {
    XDrawLine(XtDisplay(w), XtWindow(w), m_input_gc, x1, y1, x2, y2);
  }
  else if(m_shape == Shape::RECT)
  {
    width  = Width(x1, x2);
    height = Height(y1, y2);
    x = Min(x1, x2);
    y = Min(y1, y2);
    if(m_fill)
    {
      XFillRectangle(XtDisplay(w), XtWindow(w), m_input_gc, x, y, width, height);
      if(m_border == Shape::BORDER_DOTTED)
      {
        XDrawRectangle(XtDisplay(w), XtWindow(w), m_input_gc, x, y, width, height);
      }
    }
    else
    {
      XDrawRectangle(XtDisplay(w), XtWindow(w), m_input_gc, x, y, width, height);
    }
  }
  else if(m_shape == Shape::ELLIPSE)
  {
    width  = Width(x1, x2);
    height = Height(y1, y2);
    x = Min(x1, x2);
    y = Min(y1, y2);
    if(m_fill)
    {
      XFillArc(XtDisplay(w), XtWindow(w), m_input_gc, x, y, width, height,
        Shape::ANGLE1, Shape::ANGLE2);
      if(m_border == Shape::BORDER_DOTTED)
      {
        XDrawArc(XtDisplay(w), XtWindow(w), m_input_gc, x, y, width, height,
          Shape::ANGLE1, Shape::ANGLE2);
      }
    }
    else
    {
      XDrawArc(XtDisplay(w), XtWindow(w), m_input_gc, x, y, width, height,
        Shape::ANGLE1, Shape::ANGLE2);
    }
  }
}

///
/// \brief Draw all stored shapes.
/// \param w Drawable Widget
///
void Shape::DrawAll(Widget w)
{
  for(unsigned int i=0; i<m_shapes_count; ++i)
  {
    ShapeProperties *shape = m_shapes[i];
    XGCValues gcv;

    gcv.line_style     = Border(shape->Border());
    gcv.line_width     = shape->LineWidth();
    gcv.foreground     = shape->Foreground();
    gcv.background     = shape->Background();
    unsigned long mask = GCForeground | GCBackground | GCLineWidth | GCLineStyle;
    XChangeGC(XtDisplay(w), m_draw_gc, mask, &gcv);
    
    if(shape->Type() == Shape::POINT)
    {
      if(shape->LineWidth() == 0)
      {
        XDrawPoints(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Point(), 1,
          CoordModeOrigin);
      }
      else
      {
        XFillArcs(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Ellipse(), 1);
      }
    }
    else if(shape->Type() == Shape::LINE)
    {
      XDrawSegments(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Line(), 1);
    }
    else if(shape->Type() == Shape::RECT)
    {
      if(shape->Filled())
      {
        XFillRectangles(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Rect(), 1);
        if(shape->Border() == Shape::BORDER_DOTTED)
        {
          XDrawRectangles(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Rect(), 1);
        }
      }
      else
      {
        XDrawRectangles(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Rect(), 1);
      }
    }
    else if(shape->Type() == Shape::ELLIPSE)
    {
      if(shape->Filled())
      {
        XFillArcs(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Ellipse(), 1);
        if(shape->Border() == Shape::BORDER_DOTTED)
        {
          XDrawArcs(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Ellipse(), 1);
        }
      }
      else
      {
        XDrawArcs(XtDisplay(w), XtWindow(w), m_draw_gc, shape->Ellipse(), 1);
      }
    }
  }
}

///
/// \brief Set input context according to selected properties.
/// \param w Drawable Widget
///
void Shape::SetInputStyle(Widget w)
{
  XGCValues gcv;
  gcv.line_style     = Border(m_border);
  gcv.line_width     = m_line_width;
  gcv.foreground     = Colors::Foreground() ^ m_bg;
  gcv.background     = Colors::Background() ^ m_bg;
  unsigned long mask = GCForeground | GCBackground | GCLineWidth | GCLineStyle;
  XChangeGC(XtDisplay(w), m_input_gc, mask, &gcv);
}

///
/// \brief Set drawing context according to selected properties.
/// \param w Drawable Widget
///
void Shape::SetDrawStyle(Widget w)
{
  XGCValues gcv;
  gcv.line_style     = Border(m_border);
  gcv.line_width     = m_line_width;
  gcv.foreground     = Colors::Foreground();
  gcv.background     = Colors::Background();
  unsigned long mask = GCForeground | GCBackground | GCLineWidth | GCLineStyle;
  XChangeGC(XtDisplay(w), m_draw_gc, mask, &gcv);
}

///
/// \brief Store shape.
/// \param x1 Initial x coord
/// \param y1 Initial y coord
/// \param x2 End x coord
/// \param y2 End y coord
///
void Shape::Add(int x1, int y1, int x2, int y2)
{
  if(m_shapes_count + 1 > m_max_shapes)
  {
    m_max_shapes += Shape::ALLOC_STEP;
    m_shapes = (ShapeProperties**)realloc(m_shapes, sizeof(ShapeProperties*) * m_max_shapes);
  }

  m_shapes[m_shapes_count] = new ShapeProperties(
    m_shape, m_border, m_line_width, m_fill,
    Colors::Foreground(), Colors::Background()
  );

  if(m_shape == Shape::POINT)
  {
    if(m_line_width == 0)
    {
      XPoint *pt = (XPoint*) XtMalloc((Cardinal)sizeof(XPoint));
      pt->x = x1;
      pt->y = y1;
      m_shapes[m_shapes_count]->SetPoint(pt);
    }
    else
    {
      XArc *elps   = (XArc*) XtMalloc((Cardinal)sizeof(XArc));
      elps->x      = x1 - m_line_width;
      elps->y      = y1 - m_line_width;
      elps->width  = 2 * m_line_width + 1;
      elps->height = 2 * m_line_width + 1;
      elps->angle1 = Shape::ANGLE1;
      elps->angle2 = Shape::ANGLE2;
      m_shapes[m_shapes_count]->SetEllipse(elps);
    }
  }
  else if(m_shape == Shape::LINE)
  {
    XSegment *line = (XSegment*) XtMalloc((Cardinal)sizeof(XSegment));
    line->x1       = x1;
    line->y1       = y1;
    line->x2       = x2;
    line->y2       = y2;
    m_shapes[m_shapes_count]->SetLine(line);
  }
  else if(m_shape == Shape::RECT)
  {
    XRectangle *rect = (XRectangle*) XtMalloc((Cardinal)sizeof(XRectangle));
    rect->x          = Min(x1, x2);
    rect->y          = Min(y1, y2);
    rect->width      = Width(x1, x2);
    rect->height     = Height(y1, y2);
    m_shapes[m_shapes_count]->SetRect(rect);
  }
  else if(m_shape == Shape::ELLIPSE)
  {
    XArc *elps   = (XArc*) XtMalloc((Cardinal)sizeof(XArc));
    elps->x      = Min(x1, x2);
    elps->y      = Min(y1, y2);
    elps->width  = Width(x1, x2);
    elps->height = Height(y1, y2);
    elps->angle1 = Shape::ANGLE1;
    elps->angle2 = Shape::ANGLE2;
    m_shapes[m_shapes_count]->SetEllipse(elps);
  }
  ++m_shapes_count;
}

void Shape::ClearAll()
{
  for(unsigned int i=0; i<m_shapes_count; i++)
  {
    delete m_shapes[i];
  }

  m_shapes       = 0;
  m_max_shapes   = 0;
  m_shapes_count = 0;
}

///
/// \brief Calculate shape's width.
/// \param x1 First x coord
/// \param x2 Second x coord
///
int Shape::Width(int x1, int x2)
{
  return x1 >= x2 ? x1 - x2 : x2 - x1;
}

///
/// \brief Calculate shape's height.
/// \param y1 First y coord
/// \param y2 Second y coord
///
int Shape::Height(int y1, int y2)
{
  return y1 >= y2 ? y1 - y2 : y2 - y1;
}

///
/// \brief Get lower value.
/// \param f First coord
/// \param s Second coord
///
int Shape::Min(int f, int s)
{
  return f <= s ? f : s;
}
