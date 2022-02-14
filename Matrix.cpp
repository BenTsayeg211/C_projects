#include "Matrix.h"
#include <cmath>
#define SIZE_ERROR "Error: the array size not valid"
#define ALLOC_FAIL "Error: allocation fail"
#define READ_MATRIX_FAIL "Error: the read of the binary file failed"
#define SIZES_NOT_EQUAL "Error: the size of the matrix's not equals"
#define INDEX_OUT_OF_RANGE "Error: the given index is out of range"
#define SMALL_NUMBER 0.1
/**
 * Constructs matrix rows * cols. Initiates all elements to 0.
 * @param rows
 * @param cols
 */
Matrix::Matrix (int rows, int cols)
{
  if (!(rows > 0 && cols > 0))
    {
      std::cerr << SIZE_ERROR << std::endl;
      exit (EXIT_FAILURE);
    }
  _matrix_dims.cols = cols;
  _matrix_dims.rows = rows;
  _my_matrix = new (std::nothrow) float[rows * cols];
  if (_my_matrix == nullptr)
    {
      std::cerr << ALLOC_FAIL << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int x = 0; x < rows; x++)
    {
      for (int y = 0; y < cols; y++)
        {
          _my_matrix[x * cols + y] = 0;
        }
    }
}

/**
 * Constructs 1*1 matrix, where the single element is initiated to 0.
 * Default c'tor
 */
Matrix::Matrix () : Matrix (1, 1)
{}
//Constructs matrix from another matrix.
/**
 * //Copy c'tor
 * Constructs matrix from another matrix.
 * @param other_matrix
 */
Matrix::Matrix (const Matrix &other_matrix)
{
  _matrix_dims.rows = other_matrix.get_rows ();
  _matrix_dims.cols = other_matrix.get_cols ();
  _my_matrix = new (std::nothrow) float[_matrix_dims.rows * _matrix_dims.cols];
  if (_my_matrix == nullptr)
    {
      std::cerr << ALLOC_FAIL << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < _matrix_dims.cols; j++)
        {
          _my_matrix[i * (_matrix_dims.cols) + j] = other_matrix (i, j);
        }
    }
}
/**
 * Destructor
 * destruct the matrix.
 */

Matrix::~Matrix ()
{
  delete[] _my_matrix;
}
/**
 * getter
 * returns the amount of rows (int).
 * @return rows
 */
int Matrix::get_rows () const
{
  return (_matrix_dims.rows);
}
/**
 * getter
 * returns the amount of cols (int).
 * @return cols
 */
int Matrix::get_cols () const
{
  return (_matrix_dims.cols);
}
//transope the matrix
Matrix &Matrix::transpose ()
{
  Matrix helper = Matrix (*this);
  int rows = _matrix_dims.rows;
  _matrix_dims.rows = helper.get_cols ();
  _matrix_dims.cols = rows;
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < _matrix_dims.cols; j++)
        {
          _my_matrix[i * (_matrix_dims.cols) + j] = helper (j, i);
        }
    }
  return *this;
}
//transform to vector form
Matrix &Matrix::vectorize ()
{
  _matrix_dims.rows = _matrix_dims.rows * _matrix_dims.cols;
  _matrix_dims.cols = 1;
  return *this;
}
//print the matrix
void Matrix::plain_print () const
{
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < _matrix_dims.cols; j++)
        {
          std::cout << _my_matrix[i * _matrix_dims.cols + j] << " ";
        }
      std::cout << std::endl;
    }
}
//claculate the dot
Matrix Matrix::dot (const Matrix &other) const
{
  Matrix ret_matrix = Matrix (_matrix_dims.rows, _matrix_dims.cols);
  if (((other.get_rows ()) != (_matrix_dims.rows))
      || (other.get_cols () != (_matrix_dims.cols)))
    {
      std::cerr << SIZES_NOT_EQUAL << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < _matrix_dims.cols; j++)
        {
          ret_matrix._my_matrix[i * (_matrix_dims.cols) + j] =
              _my_matrix[i * (_matrix_dims.cols) + j] * other (i, j);
        }
    }
  return ret_matrix;
}
//claculate the norm of the matrix
float Matrix::norm () const
{
  float arr_sum = 0;
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < _matrix_dims.cols; j++)
        {
          arr_sum += _my_matrix[i * (_matrix_dims.cols) + j] *
                     _my_matrix[i * (_matrix_dims.cols) + j];
        }
    }
  return std::sqrt (arr_sum);
}
/**
 * read input stream to fill the given matrix
 * @param input
 * @param matrix_to_fill
 * @return matrix after fill
 */
std::istream &read_binary_file (std::istream &input, Matrix &matrix_to_fill)
{
  long int total_size = sizeof (float) *
                        (matrix_to_fill._matrix_dims.rows) *
                        (matrix_to_fill._matrix_dims.cols);
  //the size that this need to read
  input.seekg (0, std::istream::beg);
  //intiall to the begin of the file
  input.read ((char *) matrix_to_fill._my_matrix, total_size);
  //read the file
  if (input.tellg () < total_size || input.fail () || !input.good ())
    {
      //in case of fail in reading
      std::cerr << READ_MATRIX_FAIL << std::endl;
      exit (EXIT_FAILURE);
    }
  return input;
}
//operators
/**
 * addition
 * @param other_matrix
 * @return this+other_matrix
 */
Matrix Matrix::operator+ (const Matrix &other_matrix) const
{
  //valid check
  if (((other_matrix.get_rows ()) != (_matrix_dims.rows))
      || (other_matrix.get_cols () != (_matrix_dims.cols)))
    {
      std::cerr << SIZES_NOT_EQUAL << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix new_matrix = Matrix (_matrix_dims.rows, _matrix_dims.cols);
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < _matrix_dims.cols; j++)
        {
          new_matrix._my_matrix[i * _matrix_dims.cols + j] =
              _my_matrix[i * _matrix_dims.cols + j] +
              other_matrix[i * _matrix_dims.cols + j];
        }
    }
  return new_matrix;
}
/**
 * Assignment
 * @param other_matrix
 */
Matrix &Matrix::operator= (const Matrix &other_matrix)
{
  if (this == &other_matrix)
    {
      return *this;
    }
  _matrix_dims.rows = other_matrix._matrix_dims.rows;
  _matrix_dims.cols = other_matrix._matrix_dims.cols;
  delete[] _my_matrix;
  _my_matrix = new (std::nothrow) float[_matrix_dims.rows * _matrix_dims.cols];
  if (_my_matrix == nullptr)
    {
      std::cerr << ALLOC_FAIL << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < other_matrix._matrix_dims.rows *
                      other_matrix._matrix_dims.cols; i++)
    {
      _my_matrix[i] = other_matrix._my_matrix[i];
    }
  return *this;
}
/**
 * Matrix multiplication
 * @param other_matrix
 * @return this*other_matrix
 */
Matrix Matrix::operator* (const Matrix &other_matrix) const
{
  //valid check
  if (_matrix_dims.cols != other_matrix.get_rows ())
    {
      std::cerr << SIZES_NOT_EQUAL << std::endl;
      exit (EXIT_FAILURE);
    }
  Matrix new_matrix = Matrix (_matrix_dims.rows, other_matrix.get_cols ());
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < other_matrix.get_cols (); j++)
        {
          for (int k = 0; k < _matrix_dims.cols; k++)
            {
              new_matrix._my_matrix[i * other_matrix.get_cols () + j] +=
                  _my_matrix[i * _matrix_dims.cols + k] *
                  other_matrix (k, j);
            }
        }
    }
  return new_matrix;
}
//Scalar multiplication on the right
Matrix Matrix::operator* (float c) const
{
  Matrix new_matrix = Matrix (_matrix_dims.rows, _matrix_dims.cols);
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < _matrix_dims.cols; j++)
        {
          new_matrix._my_matrix[i * _matrix_dims.cols + j] =
              c * (_my_matrix[i * _matrix_dims.cols + j]);
        }
    }
  return new_matrix;
}
//Scalar multiplication on the left
Matrix operator* (float c, const Matrix &the_matrix)
{
  return (the_matrix * c);
}
/**
 * //Matrix addition accumulation
 * @param other_matrix
 * @return update this
 */
Matrix &Matrix::operator+= (const Matrix &other_matrix)
{
  //valid check
  if (((other_matrix.get_rows ()) != (_matrix_dims.rows))
      || (other_matrix.get_cols () != (_matrix_dims.cols)))
    {
      std::cerr << SIZES_NOT_EQUAL << std::endl;
      exit (EXIT_FAILURE);
    }
  for (int i = 0; i < _matrix_dims.rows; i++)
    {
      for (int j = 0; j < _matrix_dims.cols; j++)
        {
          _my_matrix[i * _matrix_dims.cols + j] += other_matrix (i, j);
        }
    }
  return *this;
}

/**
 * Parenthesis indexing
 * @param i
 * @param j
 * @return matrix(i,j)
 */
float &Matrix::operator() (int i, int j)
{
  if ((i >= _matrix_dims.rows) || (i < 0)
      || (j >= _matrix_dims.cols) || (j < 0))
    {
      std::cerr << INDEX_OUT_OF_RANGE << std::endl;
      exit (EXIT_FAILURE);
    }
  return _my_matrix[i * _matrix_dims.cols + j];
}
/**
 * Parenthesis indexing-const version
 * @param i
 * @param j
 * @return matrix(i,j)
 */
float Matrix::operator() (int i, int j) const
{
  if ((i >= _matrix_dims.rows) || (i < 0)
      || (j >= _matrix_dims.cols) || (j < 0))
    {
      std::cerr << INDEX_OUT_OF_RANGE << std::endl;
      exit (EXIT_FAILURE);
    }
  return _my_matrix[i * _matrix_dims.cols + j];
}
/**
 * Brackets indexing
 * @param i
 * @return matrix[i]
 */
float &Matrix::operator[] (int i)
{
  return _my_matrix[i];
}
/**
 * Brackets indexing-const version
 * @param i
 * @return matrix[i]
 */
float Matrix::operator[] (int i) const
{
  return _my_matrix[i];
}
/**
 * print the matrix
 * @param out
 * @param the_matrix
 * @return out
 */
std::ostream &operator<< (std::ostream &out,
                          const Matrix &the_matrix)
{
  for (int i = 0; i < the_matrix.get_rows (); i++)
    {
      for (int j = 0; j < the_matrix.get_cols (); j++)
        {
          if (the_matrix[i * the_matrix._matrix_dims.cols + j]
              >= SMALL_NUMBER)
            {
              out << "  ";
            }
          else
            {
              out << "**";
            }
        }
      out << std::endl;
    }
  return out;
}
