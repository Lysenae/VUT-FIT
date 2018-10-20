// File:   theme.cpp
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "theme.h"

///
/// \brief Constructor
///
Theme::Theme()
{
    m_theme = THEME_NONE;
}

///
/// \brief Set current theme
/// @param theme Theme ID
///
void Theme::set(int theme)
{
    switch(theme)
    {
        case 1:
            m_theme = THEME_ANIMALS;
            break;
        case 2:
            m_theme = THEME_NATURE;
            break;
        case 3:
            m_theme = THEME_FRUIT;
            break;
        default:
            m_theme = THEME_NONE;
            break;
    }
}

///
/// \brief Get current theme
/// @return Theme type enum
///
ThemeType Theme::get()
{
    return m_theme;
}

///
/// \brief Get image name for given id in selected theme.
/// @param i id
/// @return Resource identificator for image
///
QString Theme::imageName(int i)
{
    if(i >= 0 && i <= 18 && m_theme != THEME_NONE)
    {
        QString s = ":/";
        if(m_theme == THEME_ANIMALS)
            s += "a";
        else if(m_theme == THEME_NATURE)
            s += "n";
        else if(m_theme == THEME_FRUIT)
            s += "f";
        s += QString::number(i);
        s += ".jpg";
        return s;
    }
    return ":/x.jpg";
}

///
/// \brief Theme ID to string.
/// @return String representation of theme name
///
QString Theme::toString()
{
    switch(m_theme)
    {
        case THEME_ANIMALS:
            return "Animals";
        case THEME_NATURE:
            return "Nature";
        case THEME_FRUIT:
            return "Fruit";
        default:
            return "None";
    }
}
