#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include<algorithm>

using namespace std;

int n, m;
int map[21][21];

void Input() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> map[i][j];
		}
	}
}

struct pos {
	int y, x;
};


int Calculation(pos st, pos ed) {
	int cnt =0;

	for (int i = st.y; i <= ed.y; i++) {
		for (int j = st.x; j <= ed.x; j++) {
			if (map[i][j] <= 0) return 0;
			cnt++;
		}
	}
	return cnt;
}

int ans;

void Process() {
	pos st, ed;
	int maxval = 0;
	for (int sy = 0; sy < n; sy++) {
		for (int sx = 0; sx < m; sx++) {
			st = { sy, sx };
			for (int ey = st.y; ey < n;  ey++) {
				for (int ex = st.x; ex < m; ex++) {
					ed = { ey, ex };

					ans = max(ans, Calculation(st, ed));
				}
			}
		}
	}
}

int main() {

	//freopen("sample_input.txt", "r", stdin);
	Input();
	Process();
	if (ans == 0) cout << -1;
	else cout << ans;
	return 0;
}