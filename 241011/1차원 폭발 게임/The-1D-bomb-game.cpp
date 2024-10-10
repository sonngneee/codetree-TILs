#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

int N, M;
vector<int> bomb;
int isEnd = 0;

void Input() {
	cin >> N >> M;
	int a;
	for (int i = 0; i < N; i++) {
		cin >> a;
		bomb.push_back(a);
	}
}

void Bomb() {
	int bsize = bomb.size();
	vector<int> b;
	vector<int> temp;
	int now = bomb[0];
	int cnt = 1;

	temp.push_back(0);
	for (int i = 1; i < bsize; i++) {
		int next = bomb[i];
		if (now == next) {
			cnt++;
			temp.push_back(i);
			now = next;
			if ((i == bsize - 1) && temp.size() >= M) {
				for (int j = 0; j < cnt; j++) {
					b.push_back(temp[j]);
				}
			}
		}
		else {
			if (cnt >= M) {
				for (int j = 0; j < cnt; j++) {
					b.push_back(temp[j]);
				}
			}
			temp.clear();
			temp.push_back(i);
			cnt = 1;
			now = next;
		}
	}
	
	bsize = b.size();
	if (bsize == 0) {
		isEnd = 1;
		return;
	}
	for (auto d : b) {
		bomb[d] = 0;
	}
	int bombsize = bomb.size();
	for (int i = bombsize - 1; i >=0; i--) {
		if (bomb[i] == 0) {
			bomb.erase(bomb.begin() + i);
		}
	}
	if(bomb.size() == 0) isEnd = 1;

}

void Process() {
	while (! isEnd) {
		Bomb();
	}

}
int main() {

	//freopen("input.txt", "r", stdin);
	Input();
	
	Process();

	int ccnt = bomb.size();
	cout << ccnt<<'\n';
	for (int i = 0; i < ccnt; i++) {
		cout << bomb[i] << '\n';
	}

	
	return 0;
}