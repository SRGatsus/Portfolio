#include "stdafx.h"

void Mul(int** matrix, int n, int m, int multiplier) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			matrix[i][j] *= multiplier;
}