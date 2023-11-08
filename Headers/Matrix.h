#ifndef __MATRIX_H__
#define __MATRIX_H__
class Matrix {
	int rows, cols;
	std::vector<std::vector<double>> matrix;
public:
	Matrix(int rows = 0, int cols = 0);
	Matrix(std::vector<std::vector<double>> matrix);

	Matrix copy();
	Matrix dot(Matrix op);
	Matrix singleColumnMatrixFromVector(std::vector<double> vec);
	Matrix addBias();
	Matrix activation();
	Matrix crossover(Matrix partner);

	std::vector<double> softmax(std::vector<double> input);
	std::vector<double> toVector();

	void randomize(double min, double max);
	void mutate(double mutation_rate);
	void draw();

	double getContent(int row, int col);
	double relu(double x);
	double leakyRelu(double x);
	double sigmoid(double x);

	int getRows();
	int getCols();
};
#endif