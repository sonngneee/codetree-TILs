#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int map[101];
int N, M;

int isTrue = 0;

int tempmap[101];
int bsize;

void Input() {
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> map[i];
	}
}

void Del() {
	vector<int> del;
	vector<int> temp;	// 임시저장용
	int now = map[0];
	temp.push_back(0);
	int cnt = 1;

	for (int i = 1; i < bsize; i++) {
		int next = map[i];

		if (now == next) {
			cnt++;
			temp.push_back(i);
			now = next;
			if (i == bsize - 1) {
				if (cnt >= M) {
					for (int p = 0; p < temp.size(); p++) {
						del.push_back(temp[p]);
					}
				}
			}
		}
		else {
			if (cnt >= M) {
				for (int p = 0; p < temp.size(); p++) {
					del.push_back(temp[p]);
				}
			}
			cnt = 1;
			temp.clear();
			temp.push_back(i);
			now = next;
		}

		if ((i == bsize - 1) && (M == 1)) {
			del.push_back(i);
		}

	}

	if (del.size() == 0) {
		isTrue = 1;
		return;
	}

	for (auto p : del) {
		map[p] = 0;
	}

	cnt = 0;
	
	for (int i = 0; i < bsize; i++) {
		if (map[i] == 0) continue;
		tempmap[cnt] = map[i];
		cnt++;
	}
	memcpy(map, tempmap, sizeof(map));
	bsize = cnt;
}

void Print() {
	cout << bsize << '\n';
	for (int i = 0; i < bsize; i++) {
		cout << map[i] << '\n';
	}
}

void Process() {
	bsize = N;
	while (!isTrue) {
		memset(tempmap, 0, sizeof(tempmap));
		Del();
	}
}
int main() {
	//freopen("sample_input.txt", "r", stdin);
	Input();
	Process();
	Print();
	
	return 0;
}