#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<algorithm>

using namespace std;

int N;
int map[21][21];
int ans;

void Input() {
	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
}

int CheckCoin(int r, int c) {
	int cnt = 0;

	for (int i = r; i < r + 3; i++) {
		for (int j = c; j < c + 3; j++) {
			if (r >= N || c >= N) continue;
			cnt += map[i][j];
		}
	}
	return cnt;
}



void Process() {
	int cnt = 0;
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			cnt = CheckCoin(r, c);
			ans = max(cnt, ans);
		}
	}
}

int main() {

	//freopen("sample_input.txt", "r", stdin);
	Input();
	Process();
	cout << ans;
	return 0;
}