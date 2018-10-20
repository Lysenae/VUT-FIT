/*
 * Description: PRL #3: Mesh Multiplication
 * Author:      Daniel Klimaj (xklima22@stud.fit.vutbr.cz)
 */

#include <mpi.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>

using namespace std;

const int MTX_R = 1; // Rows (K-dimension)
const int MTX_C = 2; // Cols (N-dimension)
const int MTX_M = 3; // M-dimension
const int MTX_A = 4; // Matrix A cell
const int MTX_B = 5; // Matrix B cell
const int MTX_X = 6; // Result

const bool GET_TIME = false;

class Processor
{
public:
  Processor(int id, int rows, int cols, int m);
  int above()          { return m_id < m_cols ? 0 : m_id - m_cols; }
  int below()          { return m_id + m_cols; }
  int left()           { return m_id % m_cols == 0 ? 0 : m_id - 1; }
  int right()          { return m_id + 1; }
  bool has_right_ngb() { return m_id % m_cols != m_cols - 1; }
  bool has_below_ngb() { return m_id < (m_rows * m_cols) - m_cols; }
  void inc_c(int val)  { m_c += val; }
  int c()              { return m_c; }
  int m()              { return m_m; }
  int id()             { return m_id; }

private:
  int m_id;
  int m_rows;
  int m_cols;
  int m_c;
  int m_m;
};

Processor::Processor(int id, int rows, int cols, int m)
{
  m_id   = id;
  m_rows = rows;
  m_cols = cols;
  m_c    = 0;
  m_m    = m;
}

enum class MtxDim
{
  Row,
  Col
};

class Matrix
{
public:
  Matrix(string fname, MtxDim dim);
  bool is_valid() { return m_valid; }
  int rows()      { return m_valid ? m_rows : 0; }
  int cols()      { return m_valid ? m_cols : 0; }
  int cell(int r, int c);

private:
  vector<vector<int> *> m_mtx;
  int  m_rows;
  int  m_cols;
  bool m_valid;
};

// Create new matrix from file
Matrix::Matrix(string fname, MtxDim dim)
{
  vector<int> vcols;
  int cols      = 0;
  int dimi      = 0;
  bool parse_ok = true;

  m_cols  = 0;
  m_rows  = 0;
  m_valid = false;

  ifstream infile(fname);
  if(infile.good())
  {
    int i = 0;
    for(string line; getline(infile, line);)
    {
      if(i == 0) // First value in the file
      {
        try
        {
          dimi = stoi(line, nullptr, 10);
        }
        catch (const std::invalid_argument&)
        {
          parse_ok = false;
        }
        if(dimi == 0)
        {
          parse_ok = false;
          break;
        }
        ++i;
        continue;
      }
      if(!parse_ok) break;

      // Parse matrix body
      m_mtx.push_back(new vector<int>());
      string tmps = "";
      int tmpi;
      size_t ptr;
      for(int j=0; j<line.size(); ++j)
      {
        if(isspace(line[j]) && tmps != "")
        {
          tmpi = stoi(tmps, &ptr, 10);
          if(ptr != tmps.size())
          {
            parse_ok = false;
            break;
          }
          m_mtx[m_mtx.size()-1]->push_back(tmpi);
          tmps = "";
        }
        else
        {
          tmps += line[j];
        }

        if(j == line.size()-1 && tmps != "")
        {
          tmpi = stoi(tmps, &ptr, 10);
          if(ptr != tmps.size())
          {
            parse_ok = false;
            break;
          }
          m_mtx[m_mtx.size()-1]->push_back(tmpi);
          tmps = "";
        }
      }
      if(!parse_ok) break;
      vcols.push_back(m_mtx[m_mtx.size()-1]->size());
      cols = m_mtx[m_mtx.size()-1]->size();
      ++i;
    }

    // Assign fields and compare rows/columns with first value in the file
    if(parse_ok)
    {
      m_valid = true;
      for(int c : vcols)
      {
        if(c != cols)
        {
          m_valid = false;
          break;
        }
      }
      m_rows  = vcols.size();
      m_cols  = cols;
      if((dim == MtxDim::Row && m_rows != dimi) ||
      (dim == MtxDim::Col && m_cols != dimi))
      {
        m_valid = false;
      }
    }
  }
}

// Get value of the cell A[r,c]
int Matrix::cell(int r, int c)
{
  if(r > 0 && r <= m_rows && c > 0 && c <= m_cols)
    return m_mtx[r-1]->at(c-1);
  return 0;
}

const int DATA_SIZE = 1;
const int ROOT      = 0;

int main(int argc, char **argv)
{
  int p_count;
  int p_id;
  int data;

  chrono::high_resolution_clock::time_point started;
  chrono::high_resolution_clock::time_point done;

  MPI::Init(argc, argv);
  p_count = MPI::COMM_WORLD.Get_size();
  p_id    = MPI::COMM_WORLD.Get_rank();

  // Processor 0 reads and validates matrices, sends values to other
  // processors if validation is successful
  if(p_id == 0)
  {
    // Read and check matrices
    Matrix A("mat1", MtxDim::Row);
    Matrix B("mat2", MtxDim::Col);

    if(!A.is_valid())
    {
      cerr << "Invalid matrix A" << endl;
      MPI::COMM_WORLD.Abort(-1);
    }
    if(!B.is_valid())
    {
      cerr << "Invalid matrix B" << endl;
      MPI::COMM_WORLD.Abort(-1);
    }
    if(A.cols() != B.rows())
    {
      cerr << "M dimensions of matrices do not match" << endl;
      MPI::COMM_WORLD.Abort(-1);
    }

    started = chrono::high_resolution_clock::now();
    int r = A.rows();
    int c = B.cols();
    int m = A.cols(); // or B.rows()

    // Inform processors about dimensions
    for(int i=0; i<p_count; ++i)
    {
      MPI::COMM_WORLD.Send(&r, DATA_SIZE, MPI::INT, i, MTX_R);
      MPI::COMM_WORLD.Send(&c, DATA_SIZE, MPI::INT, i, MTX_C);
      MPI::COMM_WORLD.Send(&m, DATA_SIZE, MPI::INT, i, MTX_M);
    }

    // Send A matrix
    for(int i=1; i<=A.rows(); ++i)
    {
      for(int j=1; j<=A.cols(); ++j)
      {
        data = A.cell(i, j);
        MPI::COMM_WORLD.Send(&data, DATA_SIZE, MPI::INT, (i-1)*c, MTX_A);
      }
    }

    // Send B matrix
    for(int i=1; i<=B.cols(); ++i)
    {
      for(int j=1; j<=B.rows(); ++j)
      {
        data = B.cell(j, i);
        MPI::COMM_WORLD.Send(&data, DATA_SIZE, MPI::INT, i-1, MTX_B);
      }
    }
  }

  // Each processor (including 0)
  int r, c, m, a, b, x;
  // Receive result dimensions
  MPI::COMM_WORLD.Recv(&r, DATA_SIZE, MPI::INT, ROOT, MTX_R);
  MPI::COMM_WORLD.Recv(&c, DATA_SIZE, MPI::INT, ROOT, MTX_C);
  MPI::COMM_WORLD.Recv(&m, DATA_SIZE, MPI::INT, ROOT, MTX_M);

  Processor proc(p_id, r, c, m);

  // Calculate
  for(int i=0; i<proc.m(); ++i)
  {
    MPI::COMM_WORLD.Recv(&a, DATA_SIZE, MPI::INT, proc.left(), MTX_A);
    MPI::COMM_WORLD.Recv(&b, DATA_SIZE, MPI::INT, proc.above(), MTX_B);
    proc.inc_c(a*b);

    // Send to neighbors
    if(proc.has_right_ngb())
    {
      MPI::COMM_WORLD.Send(&a, DATA_SIZE, MPI::INT, proc.right(), MTX_A);
    }
    if(proc.has_below_ngb())
    {
      MPI::COMM_WORLD.Send(&b, DATA_SIZE, MPI::INT, proc.below(), MTX_B);
    }
  }

  // Send result to processor 0
  x = proc.c();
  MPI::COMM_WORLD.Send(&x, DATA_SIZE, MPI::INT, ROOT, MTX_X);

  // Processor 0 outputs result
  if(proc.id() == 0)
  {
    vector<int> rslt;
    for(int i=0; i<p_count; ++i)
    {
      MPI::COMM_WORLD.Recv(&data, DATA_SIZE, MPI::INT, i, MTX_X);
      rslt.push_back(data);
    }

    cout << r << ":" << c << endl;
    for(int i=0; i<rslt.size(); ++i)
    {
      cout << rslt[i];
      if(i % c == c-1)
        cout << endl;
      else
        cout << " ";
    }

    done = std::chrono::high_resolution_clock::now(); // end time
    if(GET_TIME)
    {
      cout << "Execution time: " <<
        chrono::duration_cast<chrono::nanoseconds>(done-started).count() << endl;
    }
  }

  MPI::Finalize();
  return 0;
}
