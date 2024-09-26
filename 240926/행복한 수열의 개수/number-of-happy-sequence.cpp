#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<algorithm>

using namespace std;

int n, m;
int map[101][101];


void Input() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> map[i][j];
		}
	}
}

int Checkhappy(int r) {
	int now = map[r][0];
	int cnt = 1;

	for (int i = 1; i < n; i++) {
		if (cnt >= m) return 1;

		if (now != map[r][i]) {
			cnt = 1;
			now = map[r][i];
			continue;
		}
		cnt++;
	}
	if (cnt >= m) return 1;
	return 0;
}

int Checkhappy2(int c) {
	int now = map[0][c];
	int cnt = 1;

	for (int i = 1; i < n; i++) {
		if (cnt >= m) return 1;

		if (now != map[i][c]) {
			cnt = 1;
			now = map[i][c];
			continue;
		}
		cnt++;
	}
	if (cnt >= m) return 1;
	return 0;
}
int ans;

void Process() {
	for (int i = 0; i < n; i++) {
		ans += Checkhappy(i);
	}
	for (int j = 0; j < n; j++) {
		ans += Checkhappy2(j);
	}
	cout << ans;
}

int main() {

	// freopen("sample_input.txt", "r", stdin);
	Input();
	Process();

	return 0;
}