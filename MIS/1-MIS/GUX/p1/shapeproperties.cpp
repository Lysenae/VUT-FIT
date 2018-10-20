// File:        shapeproperties.cpp
// Author:      Daniel Klimaj (xklima22)
// Description: Store shape's properties.

#include "shapeproperties.h"

ShapeProperties::ShapeProperties(int type, int border, int width, bool filled,
Pixel fg, Pixel bg)
{
  m_type    = type;
  m_border  = border;
  m_width   = width;
  m_filled  = filled;
  m_fg      = fg;
  m_bg      = bg;
  m_point   = NULL;
  m_line    = NULL;
  m_rect    = NULL;
  m_ellipse = NULL;
}

ShapeProperties::~ShapeProperties()
{
  if(m_point != NULL)
  {
    XtFree((char*)m_point);
  }
  else if(m_line != NULL)
  {
    XtFree((char*)m_line);
  }
  else if(m_rect != NULL)
  {
    XtFree((char*)m_rect);
  }
  else if(m_ellipse != NULL)
  {
    XtFree((char*)m_ellipse);
  }
}

int ShapeProperties::Type()
{
  return m_type;
}

int ShapeProperties::Border()
{
  return m_border;
}

int ShapeProperties::LineWidth()
{
  return m_width;
}

bool ShapeProperties::Filled()
{
  return m_filled;
}

Pixel ShapeProperties::Foreground()
{
  return m_fg;
}

Pixel ShapeProperties::Background()
{
  return m_bg;
}

XPoint *ShapeProperties::Point()
{
  return m_point;
}

XSegment *ShapeProperties::Line()
{
  return m_line;
}

XRectangle *ShapeProperties::Rect()
{
  return m_rect;
}

XArc *ShapeProperties::Ellipse()
{
  return m_ellipse;
}

void ShapeProperties::SetPoint(XPoint *point)
{
  m_point = point;
}

void ShapeProperties::SetLine(XSegment *line)
{
  m_line = line;
}

void ShapeProperties::SetRect(XRectangle *rect)
{
  m_rect = rect;
}

void ShapeProperties::SetEllipse(XArc *ellipse)
{
  m_ellipse = ellipse;
}
