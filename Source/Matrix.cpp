#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#include "Random.h"
#include "Matrix.h"

Matrix::Matrix(int rows, int cols){
	this->rows = rows;
	this->cols = cols;
	matrix.resize(rows);
	for (int i = 0; i < rows; i++) {
		matrix[i].resize(cols);
	}
}

Matrix::Matrix(std::vector<std::vector<double>> matrix){
	this->matrix = matrix;
	this->rows = matrix.size();
	this->cols = matrix[0].size();
}

Matrix Matrix::copy() {
	Matrix clone(rows,cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			clone.matrix[i][j] = matrix[i][j];
		}
	}
	return clone;
}

Matrix Matrix::dot(Matrix op) {
	Matrix result(rows, op.cols);
	if (cols == op.rows) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < op.cols; j++) {
				double sum = 0;
				for (int k = 0; k < cols; k++) {
					sum += matrix[i][k] * op.matrix[k][j];
				}
				result.matrix[i][j] = sum;
			}
		}
	}
	return result;
}

Matrix Matrix::singleColumnMatrixFromVector(std::vector<double> vec) {
	Matrix newMatrix(vec.size(),1);
	for (unsigned int i = 0; i < vec.size(); i++) {
		newMatrix.matrix[i][0] = vec[i];
	}
	return newMatrix;
}

Matrix Matrix::addBias() {
	Matrix newMatrix(rows + 1, 1);
	for (int i = 0; i < rows; i++) {
		newMatrix.matrix[i][0] = matrix[i][0];
	}
	newMatrix.matrix[rows][0] = 1;
	return newMatrix;
}

Matrix Matrix::activation() {
	Matrix newMatrix(rows, cols);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			newMatrix.matrix[i][j] = sigmoid(matrix[i][j]);
		}
	}
	return newMatrix;
}

Matrix Matrix::crossover(Matrix partner) {
	Random rand;
	Matrix child(rows, cols);

	int rand_col = rand.getRandom(0,cols);
	int rand_row = rand.getRandom(0,rows);

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (i < rand_row || (i == rand_row && j <= rand_col))
				child.matrix[i][j] = matrix[i][j];
			else
				child.matrix[i][j] = partner.matrix[i][j];
		}
	}

	return child;
}

vector<double> Matrix::softmax(std::vector<double> input) {
	vector <double> result;
	double sum = 0;

	for (double x : input) {
		double exp_x = exp(x);
		result.push_back(exp_x);
		sum += exp_x;
	}

	for (double& val : result) {
		val /= sum;
	}

	return result;
}

vector<double> Matrix::toVector() {
	vector<double> newVec;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			newVec.push_back(matrix[i][j]);
	}
	return newVec;
}

void Matrix::randomize(double min, double max) {
	Random random;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			matrix[i][j] = random.getRandom(min,max);
		}
	}
}

void Matrix::mutate(double mutation_rate){
	Random random;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			double rand = random.getRandom(0,1);
			if (rand < mutation_rate) {
				matrix[i][j] += random.getRandom(-1.25, 1.25);
				if (matrix[i][j] > 1)
					matrix[i][j] = 1;
				if (matrix[i][j] < -1)
					matrix[i][j] = -1;
			}
		}
	}
}

void Matrix::draw() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

double Matrix::getContent(int row, int col) {
	return matrix[row][col];
}

double Matrix::relu(double x) {
	return max(0.0,x);
}

double Matrix::leakyRelu(double x){
	return max(0.01*x, x);
}

double Matrix::sigmoid(double x) {
	return 1 / (1 + exp(-x));
}

int Matrix::getRows() {
	return rows;
}

int Matrix::getCols() {
	return cols;
}