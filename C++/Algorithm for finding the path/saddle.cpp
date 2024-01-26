#include "stdafx.h"

bool FindSaddleElement(int **matrix, int n, int m, int &saddle) {
	for (int i = 0; i < n; i++) {
		int min_row = matrix[i][0];
		int col_ind = 0;
		for (int j = 1; j < m; j++)
			if (min_row > matrix[i][j]) {
				min_row = matrix[i][j];
				col_ind = j;
			}

		int k;
		for (k = 0; k < n; k++)
			if (min_row < matrix[k][col_ind])
				break;

		if (k == n) {
			saddle = min_row;
			return true;
		}
	}
	return false;
}