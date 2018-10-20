// File:   dimension.cpp
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#include "dimension.h"

///
/// \brief Constructor
/// \param rows
/// \param cols
///
Dimension::Dimension(int rows, int cols)
{
    set(rows, cols);
}

///
/// \brief Get size = rows * cols
/// \return Size
///
int Dimension::size()
{
    return m_cols * m_rows;
}

///
/// \brief Get number of rows
/// \return Number of rows
///
int Dimension::rows()
{
    return m_rows;
}

///
/// \brief Get number of columns
/// \return Number of columns
///
int Dimension::cols()
{
    return m_cols;
}

///
/// \brief Set number of rows and columns
/// \param rows
/// \param cols
///
void Dimension::set(int rows, int cols)
{
    m_rows = rows;
    m_cols = cols;
}

///
/// \brief String representation of size RxC
/// \return String representation
///
QString Dimension::toString()
{
    return QString::number(m_rows) + "x" + QString::number(m_cols);
}
