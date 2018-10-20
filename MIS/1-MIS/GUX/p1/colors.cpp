// File:        colors.cpp
// Author:      Daniel Klimaj (xklima22)
// Description: Initializes and sets colors.

#include "colors.h"

// Init static variables
char *Colors::m_color_names[Colors::COLORS];
Pixel *Colors::m_colors      = 0;
Pixel Colors::m_background;
Pixel Colors::m_foreground;

///
/// \brief Initialize colors.
/// \param shell Widget for which colors are initialized
///
bool Colors::Init(Widget shell)
{
  InitNames();
  XColor col_screen, col_exact;
  XWindowAttributes win_attrs;
  m_colors = (Pixel*)XtMalloc((Cardinal)(sizeof(Pixel) * Colors::COLORS));

  Display  *display = XtDisplay(shell);
  Screen   *screen  = XtScreen(shell);
  Colormap  colmap  = DefaultColormapOfScreen(screen);

  XGetWindowAttributes(display, RootWindowOfScreen(screen), &win_attrs);

  for(unsigned int i=0; i<Colors::COLORS; i++)
  {
    if(XAllocNamedColor(display, colmap, m_color_names[i],
    &col_screen, &col_exact) == 0)
    {
      fprintf(stderr, "Failed to initialize color '%s'\n", m_color_names[i]);
      return false;
    }
    m_colors[i] = col_screen.pixel;
  }
  m_background = m_colors[0];
  m_foreground = m_colors[1];

  return true;
}

///
/// \brief Get background color.
/// \return Background color
///
Pixel Colors::Background()
{
  return m_background;
}

///
/// \brief Get foreground color.
/// \return Foreground color
///
Pixel Colors::Foreground()
{
  return m_foreground;
}

///
/// \brief Set background color.
/// \param i Id of background color
///
void Colors::SetBackground(unsigned int i)
{
  if(i < Colors::COLORS)
    m_background = m_colors[i];
}

///
/// \brief Set foreground color.
/// \param i Id of foreground color
///
void Colors::SetForeground(unsigned int i)
{
  if(i < Colors::COLORS)
    m_foreground = m_colors[i];
}

///
/// \brief Get color names.
/// \return Color names
///
char **Colors::Names()
{
  return m_color_names;
}

///
/// \brief Get color name.
/// \return i Id of color
///
char *Colors::Name(unsigned int i)
{
  return m_color_names[i];
}

///
/// \brief Initilize vector of color names.
/// \return List of color names
///
void Colors::InitNames()
{
  for(int i=0; i<Colors::COLORS; ++i)
  {
    m_color_names[i] = (char*) malloc(10);
  }
  strcpy(m_color_names[0], "white");
  strcpy(m_color_names[1], "black");
  strcpy(m_color_names[2], "yellow");
  strcpy(m_color_names[3], "green");
  strcpy(m_color_names[4], "red");
  strcpy(m_color_names[5], "blue");
}
