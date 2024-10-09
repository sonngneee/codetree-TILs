#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

int N;
int map[101];
struct pos {
	int s, e;
};
int sizeans;

vector<pos> v;

void Input() {
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> map[i];
	}
	int s, e;
	cin >> s >> e;
	v.push_back({ s-1, e-1 });
	cin >> s >> e;
	v.push_back({ s-1, e-1 });
	sizeans = N;
}

void Process() {
	
	for (int i = 0; i < 2; i++) {
		pos del = v[i];
		int temp[100] = { 0 };

		for (int st = del.s; st <= del.e; st++) {
			if (map[st] == 0) break;
			map[st] = 0;
			sizeans--;
		}
		int poi = 0;
		for (int u = 0; u < N; u++) {
			if (map[u] == 0) continue;
			temp[poi] = map[u];
			poi++;
		}
		memset(map, 0, sizeof(map));
		for (int u = 0; u < N; u++) {
			if (temp[u] == 0) break;
			map[u] = temp[u];
		}
	}
	int i = 0;
	cout << sizeans<<'\n';
	while (true) {
		if (map[i] == 0) break;
		cout << map[i] << '\n';
		i++;
	}
}

int main() {

	//freopen("input.txt", "r", stdin);
	Input();
	
	Process();
	
	return 0;
}