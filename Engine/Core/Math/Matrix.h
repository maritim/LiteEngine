#ifndef MATRIX_H
#define MATRIX_H

#include "Core/Interfaces/Object.h"

#include <string>

#include "Array.h"
//#include "Memory.h"

#define DETERMINANT_PRECISION_EPSILON (1e-3)

class Matrix : public Object
{
//	TRACK_MEMORY(MEMTYPE_MATH)

private:
	int _lines;
	int _columns;
	Array** _matrix;
public:
	static Matrix Hilbert (int n);
	static Matrix One (int n, int m);
	static Matrix Zero (int n, int m);
	static Matrix Identity (int n);

	static Matrix SolveEquationSystem (Matrix A, Matrix b);

	static Matrix Translate (float x, float y, float z);
	static Matrix Scale (float x, float y, float z);
	static Matrix Rotate (float x, float y, float z);

public:
	Matrix ();
	Matrix (int lines, int columns);
	Matrix (const Matrix& other);
    
    ~Matrix ();

    Matrix& operator=(const Matrix& other);
	Matrix& operator+=(const Matrix& other);
	Matrix operator+(const Matrix& other);
	Matrix& operator-=(const Matrix& other);
	Matrix operator-(const Matrix& other);
//	Matrix& operator*=(const Matrix& other);
	Matrix operator*(const Matrix& other);	

	Matrix& operator*=(const float& other);
	Matrix operator*(const float& other);
	friend Matrix operator*(const float& other, const Matrix& matrix);
	friend std::ostream &operator<<(std::ostream &out, Matrix v);

	Array& operator[](const int index);

	long double Determinant ();
	Matrix Transpose ();
	bool IsInvertible ();
	Matrix Inverse ();

	Matrix Minor (int i, int j);
	Matrix Cofactor ();

	std::pair<Matrix, Matrix> LUDecomposition ();
	std::pair<Matrix, Matrix> QRDecomposition ();
	Matrix CholeskyDecomposition ();

	float* C_Matrix ();
	float* C_ColMatrix ();

	int GetLines () const;
	int GetColumns () const;

	std::string ToString () const;
};

#endif