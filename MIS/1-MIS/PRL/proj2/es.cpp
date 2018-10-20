/*
 * Description: PRL #2: Enumeration Sort
 * Author:      Daniel Klimaj (xklima22@stud.fit.vutbr.cz)
 */

#include <mpi.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

const int MASTER = 0;
const int REG_X  = 1;
const int REG_Y  = 2;
const int REG_Z  = 3;

const bool GET_TIME = false;

class Processor
{
public:
  Processor(int p_id, int p_count);
  bool hasNextNeighbor();
  int prevNeighbor();
  int nextNeighbor();
  void incC();
  void setX(int x, int ix);
  void setY(int y, int iy);
  void setZ(int z);
  int c()  { return this->m_c; }
  int x()  { return this->m_x; }
  int y()  { return this->m_y; }
  int z()  { return this->m_z; }
  int ix() { return this->m_ix; }
  int iy() { return this->m_iy; }
private:
  int m_id;
  int m_c;    // reg C
  int m_x;    // reg X
  int m_y;    // reg Y
  int m_z;    // reg Z
  int m_ix;   // reg ix
  int m_iy;   // reg iy
  int m_pcnt;
};

/// Initialize processor data
Processor::Processor(int p_id, int p_count)
{
  this->m_id   = p_id;
  this->m_c    = 1;
  this->m_x    = -1;
  this->m_y    = -1;
  this->m_z    = -1;
  this->m_ix   = -1;
  this->m_iy   = -1;
  this->m_pcnt = p_count;
}

/// Check if processor has neighbor with bigger id
bool Processor::hasNextNeighbor()
{
  return this->m_id < (this->m_pcnt - 1);
}

/// Get previous processor id
int Processor::prevNeighbor()
{
  return this->m_id - 1;
}

/// Get next processor id
int Processor::nextNeighbor()
{
  return this->m_id + 1;
}

/// Increase register C
void Processor::incC()
{
  this->m_c += 1;
}

/// Set registers X and ix
void Processor::setX(int x, int ix)
{
  this->m_x  = x;
  this->m_ix = ix;
}

/// Set registers Y and iy
void Processor::setY(int y, int iy)
{
  this->m_y  = y;
  this->m_iy = iy;
}

/// Set register Z
void Processor::setZ(int z)
{
  this->m_z = z;
}

/// Read numbers from specified file and return them as vector
vector<int>parseNumbers(string fname)
{
  vector<int> result;
  int current;
  fstream f;
  f.open(fname.c_str(), ios::in);
  if(!f.good())
  {
    cerr << "Failed to open file with numbers" << endl;
    return result;
  }
  else
  {
    while(!f.eof())
    {
      current = f.get();
      if(f.eof())
        break;
      result.push_back(current);
    }
  }
}

/// Create string from vector of numbers
string vecJoin(vector<int> v)
{
  string s = "";
  for(unsigned int i=0; i<v.size(); ++i)
  {
    s += to_string(v[i]);
    if(i < v.size()-1) s += " ";
  }
  return s;
}

int main(int argc, char **argv)
{
  int p_count;
  int p_id;
  int nums;
  int data[2];
  int rslt;

  MPI::Init(argc, argv);
  p_count = MPI::COMM_WORLD.Get_size();
  p_id    = MPI::COMM_WORLD.Get_rank();
  nums    = p_count - 1;

  // Master processor
  if(p_id == 0)
  {
    vector<int> numbers = parseNumbers("numbers");
    cout << vecJoin(numbers) << endl;

    // Error - amount of input data is not equal to number of processors-1
    if(numbers.size() != nums)
    {
      cerr << "Size of numbers does not corresponds to number of processors" <<
        endl;
      MPI::COMM_WORLD.Abort(-1);
    }

    auto started = chrono::high_resolution_clock::now(); // start time
    // Assign numbers to the slaves and send all numbers to the first processor
    for(int i=1; i<p_count; ++i)
    {
      data[0] = numbers[i-1];
      data[1] = i;
      MPI::COMM_WORLD.Send(&data, 2, MPI::INT, i, REG_X);
      MPI::COMM_WORLD.Send(&data, 2, MPI::INT, 1, REG_Y);
    }

    // Get results from slave processors
    for(int i=0; i<nums; ++i)
    {
      MPI::COMM_WORLD.Recv(&rslt, 1, MPI::INT, i+1, REG_Z);
      cout << rslt << endl;
    }
    auto done = std::chrono::high_resolution_clock::now(); // end time
    if(GET_TIME)
    {
      cout << "Execution time: " <<
        chrono::duration_cast<chrono::nanoseconds>(done-started).count() << endl;
    }
  }
  else // Slave processors
  {
    Processor proc(p_id, p_count);
    int x[2], y[2], r[2], z;

    // Initialize X register with it's rank
    MPI::COMM_WORLD.Recv(&x, 2, MPI::INT, MASTER, REG_X);
    proc.setX(x[0], x[1]);

    // Get other values and compare them to the value in register X
    for(unsigned int i=0; i<nums; i++)
    {
      MPI::COMM_WORLD.Recv(&y, 2, MPI::INT, proc.prevNeighbor(), REG_Y);
      proc.setY(y[0], y[1]);

      // Increase counter if X > Y
      if(proc.x() > proc.y())
      {
        proc.incC();
      }
      // Increase counter if X = Y and ix > iy
      else if(proc.x() == proc.y() && proc.ix() > proc.iy())
      {
        proc.incC();
      }

      // Send data to the next processor to the right (if possible)
      if(proc.hasNextNeighbor())
      {
        MPI::COMM_WORLD.Send(&y, 2, MPI::INT, proc.nextNeighbor(), REG_Y);
      }
    }

    // Set Z register according to C
    z = proc.x();
    MPI::COMM_WORLD.Send(&z, 1, MPI::INT, proc.c(), REG_Z);
    MPI::COMM_WORLD.Recv(&z, 1, MPI::INT, MPI::ANY_SOURCE, REG_Z);
    proc.setZ(z);

    // Send result to the master
    z = proc.z();
    MPI::COMM_WORLD.Send(&z, 1, MPI::INT, MASTER, REG_Z);
  }

  MPI::Finalize();
  return 0;
}
