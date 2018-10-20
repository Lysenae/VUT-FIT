// File:        shapeproperties.h
// Author:      Daniel Klimaj (xklima22)
// Description: Store shape's properties.

#ifndef SHAPEPROPS_H
#define SHAPEPROPS_H

#include <Xm/Xm.h>

class ShapeProperties
{
public:
  ShapeProperties(int type, int border, int width, bool filled, Pixel fg,
    Pixel bg);
  ~ShapeProperties();

  int         Type();
  int         Border();
  int         LineWidth();
  bool        Filled();
  Pixel       Foreground();
  Pixel       Background();
  XPoint     *Point();
  XSegment   *Line();
  XRectangle *Rect();
  XArc       *Ellipse();

  void SetPoint(XPoint *Point);
  void SetLine(XSegment *line);
  void SetRect(XRectangle *rect);
  void SetEllipse(XArc *ellipse);

private:
  int         m_type;
  int         m_border;
  int         m_width;
  bool        m_filled;
  Pixel       m_fg;
  Pixel       m_bg;
  XPoint     *m_point;
  XSegment   *m_line;
  XRectangle *m_rect;
  XArc       *m_ellipse;
};

#endif // SHAPEPROPS_H
