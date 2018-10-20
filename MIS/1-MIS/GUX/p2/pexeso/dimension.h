// File:   dimension.h
// Author: Daniel Klimaj; xklima22@stud.fit.vutbr.cz

#ifndef DIMENSION_H
#define DIMENSION_H

#include <QString>

class Dimension
{
public:
    Dimension(int rows, int cols);
    int size();
    int rows();
    int cols();
    void set(int rows, int cols);
    QString toString();

private:
    int m_cols;
    int m_rows;
};

#endif // DIMENSION_H
