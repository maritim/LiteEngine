#include <iostream>
#include <cmath>

#include "Matrix.h"
#include "Array.h"

#define PI 3.14159265

Matrix Matrix::Hilbert (int n)
{
	Matrix hilbert (n, n);

	for (int i=0;i<n;i++) {
		for (int j=0;j<n;j++) {
			hilbert [i][j] = ((long double)1) / (i + 1 + j);
		}
	}

	return hilbert;
}

Matrix Matrix::One (int n, int m)
{
	Matrix b (n, m);

	for (int i=0;i<n;i++) {
		for (int j=0;j<m;j++) {
			b [i][j] = 1;
		}
	}

	return b;
}

Matrix Matrix::Zero (int n, int m)
{
	Matrix zero (n, m);

	return zero;
}

Matrix Matrix::Identity (int n)
{
	Matrix identity (n, n);

	for (int i=0;i<n;i++) {
		identity [i][i] = 1;
	}

	return identity;
}

Matrix Matrix::SolveEquationSystem (Matrix A, Matrix b)
{
	// Using Cholesky Decomposition

	int n = b.GetLines ();

	Matrix At = A.Transpose ();
	A = At * A;
	b = At * b;

	Matrix L = A.CholeskyDecomposition ();

	Matrix Y (n, 1);
	Matrix X (n, 1);

	for (int i=0;i<n;i++) {
		Y[i][0] = b[i][0];
		for (int k=0;k<=i-1;k++) {
			Y[i][0] -= L[i][k] * Y[k][0];
		}
		Y[i][0] /= L[i][i];
	}	

	for (int i=n-1;i>=0;i--) {
		X[i][0] = Y[i][0];
		for (int k=i+1;k<n;k++) {
			X[i][0] -= L[k][i] * X[k][0];
		}
		X[i][0] /= L[i][i];
	}

	return X;
}

Matrix Matrix::Translate (float x, float y, float z)
{
	Matrix result = Matrix::Identity (4);

	result [0][3] = x;
	result [1][3] = y;
	result [2][3] = z;

	return result;
}

Matrix Matrix::Scale (float x, float y, float z)
{
	Matrix result (4, 4);

	result [0][0] = x;
	result [1][1] = y;
	result [2][2] = z;
	result [3][3] = 1;

	return result;
}

Matrix Matrix::Rotate (float x, float y, float z)
{
	Matrix result (4, 4);

	float xRad = x * PI / 180;
	float yRad = y * PI / 180;
	float zRad = z * PI / 180;

	float cosa = cos (x);
	float cosb = cos (y);
	float cosc = cos (z);

	float sina = sin (x);
	float sinb = sin (y);
	float sinc = sin (z);

	result [0][0] = cosc * cosb;
	result [0][1] = sinc * cosa + sinb * sina * cosc;
	result [0][2] = sinc * sina - cosa * sinb * cosc;
	result [1][0] = -cosb * sinc;
	result [1][1] = cosa * cosc - sinc * sinb * sina;
	result [1][2] = sina * cosc + cosa * sinb * sinc;
	result [2][0] = sinb;
	result [2][1] = -sina * cosb;
	result [2][2] = cosa * cosb;
	result [3][3] = 1;

	// result [0][0] = cosb * cosc;
	// result [0][1] = - cosb * sinc;
	// result [0][2] = sinb;
	// result [1][0] = sina*sinb*cosc + cosa * sinc;
	// result [1][1] = -sina * sinb * sinc + cosa * cosc;
	// result [1][2] = -sina * cosb;
	// result [2][0] = -cosa * sinb * cosc + sina * sinc;
	// result [2][1] = cosa * sinb * sinc + sina * cosc;
	// result [2][2] = cosa * cosb;
	// result [3][3] = 1;

	return result;
}

Matrix::Matrix () :
	_lines (0),
	_columns (0),
	_matrix (NULL)
{

}

Matrix::Matrix (int lines, int columns) :
	_lines (lines),
	_columns (columns),
	_matrix (NULL)
{
	_matrix = new Array*[_lines];

	for (int i=0;i<_lines;i++) {
		_matrix [i] = new Array (columns);
	}
}

Matrix::Matrix (const Matrix& other) :
	_lines (other._lines),
	_columns (other._columns),
	_matrix (NULL)
{
	_matrix = new Array*[_lines];

	for (int i=0;i<_lines;i++) {
		_matrix [i] = new Array (*other._matrix [i]);
	}
}

Matrix& Matrix::operator=(const Matrix& other)
{
	if (_lines) {
		for (int i=0;i<_lines;i++) {
			delete _matrix [i];
		}

		delete[] _matrix;
	}

	_lines = other._lines;
	_columns = other._columns;

	_matrix = new Array*[_lines];

	for (int i=0;i<_lines;i++) {
		_matrix [i] = new Array (*other._matrix [i]);
	}

	return *this;
}

Matrix& Matrix::operator+= (const Matrix& other)
{
	if (_lines != other._lines || _columns != other._columns) {
		throw "Matrix summation could only be done on matrices with the same size!";
	}

	for (int i=0;i<_lines;i++) {
		for (int j=0;j<_columns;j++) {
			(*_matrix [i])[j] += (*other._matrix[i])[j];
		}
	}

	return *this;
}

Matrix Matrix::operator+ (const Matrix& other)
{
	if (_lines != other._lines || _columns != other._columns) {
		throw "Matrix summation could only be done on matrices with the same size!";
	}

	Matrix result (*this);
	result += other;

	return result;
}

Matrix& Matrix::operator-= (const Matrix& other)
{
	if (_lines != other._lines || _columns != other._columns) {
		throw "Matrix summation could only be done on matrices with the same size!";
	}

	for (int i=0;i<_lines;i++) {
		for (int j=0;j<_columns;j++) {
			(*_matrix [i])[j] -= (*other._matrix[i])[j];
		}
	}

	return *this;
}

Matrix Matrix::operator- (const Matrix& other)
{
	if (_lines != other._lines || _columns != other._columns) {
		throw "Matrix summation could only be done on matrices with the same size!";
	}

	Matrix result (*this);
	result -= other;

	return result;
}

Matrix Matrix::operator*(const Matrix& other)
{
	if (_columns != other._lines) {
		throw "Matrix product could not be done on these matrices!";
	}

	Matrix result (_lines, other._columns);

	for (int i=0;i<_lines;i++) {
		for (int j=0;j<other._columns;j++) {
			for (int k=0;k<_columns;k++) {
				result [i][j] += ((*_matrix [i])[k] * (*other._matrix[k])[j]);
			}
		}
	}

	return result;
}

Matrix& Matrix::operator*=(const float& other)
{
	for (int i=0;i<_lines;i++) {
		for (int j=0;j<_columns;j++) {
			(*_matrix [i])[j] *= other;
		}
	}

	return *this;
}

Matrix Matrix::operator*(const float& other)
{
	Matrix result (*this);
	result *= other;

	return result;
}

Matrix operator*(const float& other, const Matrix& matrix)
{
	Matrix result (matrix);
	result *= other;

	return result;
}

std::ostream &operator<<(std::ostream &out, Matrix m) 
{
	for (int i=0;i<m._lines;i++) {
		for (int j=0;j<m._columns;j++) {
			out << m[i][j] << " ";
		}
		out << "\n";
	}

    return out;
}

Matrix Matrix::Minor (int i, int j)
{
	Matrix minorant (_lines -1, _columns - 1);

	int minorLine = 0;
	for (int k=0;k<_lines;k++) {
		if (k == i) {
			continue ;
		}

		int minorColumn = 0;
		for (int l=0;l<_columns;l++) {
			if (l == j) {
				continue;
			}

			minorant [minorLine] [minorColumn] = (*_matrix [k])[l];
			minorColumn ++;
		}
		minorLine ++;
	}	

	return minorant;
}

long double Matrix::Determinant (void)
{
	if (_lines != _columns) {
		throw "Could not calculate determinant on a non-squared matrix!";
	}

	// First case
	if (_lines == 1) {
		return (*_matrix [0])[0];
	}

	// Second case (usual one)
	if (_lines == 2) {
		return (*_matrix [0])[0] * (*_matrix [1])[1] -
			(*_matrix [1])[0] * (*_matrix [0])[1];
	}

	// Third case for optimization
	if (_lines == 3) {
		long double result = 0;
		for (int i=0;i<3;i++) {
			long double temp = 1;
			for (int j=0;j<3;j++) {
				temp *= (*_matrix [j]) [(i+j) % 3];
			}
			result += temp;

			temp = 1;
			for (int j=0;j<3;j++) {
				temp *= (*_matrix [j]) [(3 + i - j) % 3];
			}
			result -= temp;
		}

		return result;
	}

	std::pair<Matrix, Matrix> LU = LUDecomposition ();

	long double result = 1;
	for (int i=0;i<_lines;i++) {
		result *= LU.first [i][i];
	}

	return result;
}

Matrix Matrix::Transpose (void)
{
	Matrix transpose (_columns, _lines);

	for (int i=0;i<_lines;i++) {
		for (int j=0;j<_columns;j++) {
			transpose [j][i] = (*_matrix [i])[j];
		}
	}

	return transpose;
}

Matrix Matrix::Cofactor (void)
{
	Matrix cofactor (_lines, _columns);

	for (int i=0;i<_lines;i++) {
		for (int j=0;j<_columns;j++) {
			cofactor [i][j] = (((i+j) & 1) ? -1 : 1) * Minor (i, j).Determinant ();
		}
	}

	return cofactor;
}

long double ldabs (long double x) 
{
	if (x < 0) {
		return -x;
	}
	return x;
}

// TODO: If needed, search a better way to find this
bool Matrix::IsInvertible (void)
{
	return ldabs (Determinant ()) > DETERMINANT_PRECISION_EPSILON;
}

Matrix Matrix::Inverse (void)
{
	long double determinant = Determinant ();
	Matrix star = Cofactor ().Transpose ();

	return (star * (1 / determinant));
}

std::pair<Matrix, Matrix> Matrix::LUDecomposition ()
{
	if (_lines != _columns) {
		throw "LU Decomposition can be processed only on squared matrices...";
	}

	int n = _lines;

	Matrix L (n, n);
	Matrix U (n, n);

	for (int i=0;i<n;i++) {
		L[i][0] = (*_matrix [i])[0];
	}
	U [0][0] = 1;
	for (int j=1;j<n;j++) {
		U[0][j] = (*_matrix [0])[j] / L[0][0];
	}

	for (int k=1;k<n;k++) {
		for (int i=k;i<n;i++) {
			L[i][k] = (*_matrix [i])[k];
			for (int p=0;p<=k-1;p++) {
				L[i][k] -= L[i][p] * U[p][k];
			}
		}

		U[k][k] = 1;

		for (int j=k+1;j<n;j++) {
			U [k][j] = (*_matrix[k])[j];
			for (int p=0;p<=k-1;p++) {
				U[k][j] -= L[k][p] * U[p][j];
			}

			if (L[k][k] == 0) {
				// This is a little hack, usually in this case we will throw an
				// error, but we will need this on determinant calculation, and
				// in this case determinant is zero
				return std::make_pair (Matrix::Zero (n, n), Matrix::Zero (n, n));
			}

			U[k][j] /= L[k][k];
		}
	}

	return std::make_pair (L, U);
}

std::pair<Matrix, Matrix> Matrix::QRDecomposition ()
{
	if (_lines != _columns) {
		throw "LU Decomposition can be processed only on squared matrices...";
	}

	int n = _lines;

	Matrix Q (n, n);
	Matrix R (n, n);

	for (int k=0;k<n;k++) {
		for (int j=0;j<=k-1;j++) {
			for (int i=0;i<n;i++) {
				R[j][k] += (*_matrix [i])[k] * Q[i][j];
			}
		}

		long double firstSum = 0;
		long double secondSum = 0;

		for (int i=0;i<n;i++) {
			firstSum += (*_matrix [i])[k] * (*_matrix [i])[k];
		}

		for (int i=0;i<=k-1;i++) {
			secondSum += R[i][k] * R[i][k];
		}

		if (firstSum - secondSum <= 0) {
			return std::make_pair (Matrix::Zero (n, n), Matrix::Zero (n, n));
		}

		R [k][k] = sqrt (firstSum - secondSum);

		for (int i=0;i<n;i++) {
			Q [i][k] = (*_matrix [i])[k];
			for (int j=0;j<=k-1;j++) {
				Q [i][k] -= R [j][k] * Q [i][j];
			}
			Q [i][k] /= R [k][k];
		}
	}

	return std::make_pair (Q, R);
}

Matrix Matrix::CholeskyDecomposition ()
{
	if (_lines != _columns) {
		throw "LU Decomposition can be processed only on squared matrices...";
	}

	int n = _lines;

	Matrix L (n, n);

	for (int j=0;j<n;j++) {
		L [j][j] = (*_matrix [j])[j];
		for (int k=0;k<=j-1;k++) {
			L [j][j] -= L[j][k] * L[j][k];
		}
		
		if (L[j][j] <= 0) {
			return Matrix::Zero (n, n);
		}
		L [j][j] = sqrt ( L [j][j] );

		for (int i=j+1;i<n;i++) {
			L [i][j] = (*_matrix [i])[j];
			for (int k=0;k<=j-1;k++) {
				L [i][j] -= L[i][k] * L[j][k];
			}

			L[i][j] /= L[j][j];
		}
	}

	return L;
}

float* Matrix::C_Matrix () 
{
	float* c_mat = new float [_lines * _columns];

	int matPos = 0;

	for (int i=0;i<_lines;i++) {
		for (int j=0;j<_columns;j++) {
			c_mat [matPos ++] = (float)(*_matrix [i])[j];
		}
	}

	return c_mat;
}

float* Matrix::C_ColMatrix ()
{
	float* c_mat = new float [_lines * _columns];

	int matPos = 0;

	for (int i=0;i<_columns;i++) {
		for (int j=0;j<_lines;j++) {
			c_mat [matPos ++] = (float) (*_matrix [j])[i];
		}
	}

	return c_mat;
}

int Matrix::GetLines (void) const
{
	return _lines;
}

int Matrix::GetColumns (void) const
{
	return _columns;
}

Array& Matrix::operator[](const int index)
{
	if (index < 0 || index >= _lines) {
		throw "Error! Matrix limits excedeed!";
	}

	return * _matrix [index];
}

std::string Matrix::ToString () const
{
	std::string str = "{";

	for (int i=0;i<_lines;i++) {
		str += "{";
		for (int j=0;j<_columns;j++) {
			str += std::to_string ((*_matrix [i])[j]) +
				(j != _columns - 1 ? ", " : "");
		}
		str += " }";
	}
	str += " }";

	return str;
}

Matrix::~Matrix (void)
{
	if (!_lines)
		return ;
	for (int i=0;i<_lines;i++) {
		delete _matrix [i];
	}

	delete[] _matrix;
}