#include <iostream>

using namespace std;

int n, m;
int map[201][201];

void Input() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> map[i][j];
		}
	}
}

int maxval = 0;
void check1(int r, int c) {
	
	if (r + 1 >= n || c + 1 >= m) return;

	int sqr = map[r][c] + map[r + 1][c] + map[r][c + 1] + map[r + 1][c + 1];
	for (int i = r; i < r + 2; i++) {
		for (int j = c; j < c + 2; j++) {
			maxval = max(maxval, sqr - map[i][j]);
		}
	}
}

void check2(int r, int c) {
	
	if (r + 2 >= n) return;
	int sqr = 0;

	for (int row = r; row < r + 3; row++) {
		sqr += map[row][c];
	}
	maxval = max(maxval, sqr);
}

void check3(int r, int c) {

	if (c + 2 >= m) return;
	int sqr = 0;

	for (int col = c; col < c + 3; col++) {
		sqr += map[r][col];
	}
	maxval = max(maxval, sqr);
}

void Process() {

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			check1(i, j);
			check2(i, j);
			check3(i, j);
		}
	}
	cout << maxval;
}

int main() {

	Input();
	Process();

	return 0;
}