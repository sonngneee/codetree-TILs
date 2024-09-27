#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<algorithm>

using namespace std;

int n, T;

// int map[2][201];
int map[402];

void Input() {
	cin >> n >> T;
	int a = 0;
	for (int r = 0; r < 2; r++) {
		for (int i = 0; i < n; i++) {
			cin >> map[a];
			a++;
		}
	}
	/*for (int r = 0; r < 2; r++) {
		for (int i = 0; i < n; i++) {
			cin >> map[r][i];

			}
	}*/
}


//void Process() {
//	int temp1, temp2 = 0;
//
//	for (int t = 0; t < T; t++) {
//		temp1 = map[0][n - 1];
//		for (int i = n-1; i >= 0 ; i--) {
//			map[0][i] = map[0][i - 1];
//		}
//		temp2 = map[1][n - 1];
//		map[1][n - 1] = temp1;
//
//		temp1 = map[1][0];
//		for (int i = 0; i < n - 2; i++) {
//			map[1][i] = map[1][i + 1];
//		}
//		map[1][n - 2] = temp2;
//		map[0][0] = temp1;
//	}
//}

void Process() {
	int temp = 0;
	for (int t = 0; t < T; t++) {
		temp = map[2 * n - 1];
		for (int i = 2*n - 1; i > 0; i--) {
			map[i] = map[i - 1];
		}
		map[0] = temp;
	}

}
void Print() {
	for (int i = 0; i <= 2*n - 1; i++) {
		cout << map[i] << ' ';
		if (i == n - 1) cout << '\n';
		
	}
}

int main() {

	//freopen("sample_input.txt", "r", stdin);

	Input();
	Process();
	Print();

	return 0;
}