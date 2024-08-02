#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<algorithm>
#include<cmath>

using namespace std;

long long n;
int pp[1000001];
int leader;
float tmate;
long long cnt_pp;

void input() {
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> pp[i];
	}
	cin >> leader >> tmate;
}

void process() {
	for (int i = 0; i < n; i++) {
		pp[i] = max(0, pp[i] - leader);
		cnt_pp +=1;
		if (pp[i] == 0) continue;
		cnt_pp += ceil(pp[i] / tmate);
	}
}

int main() {
	input();
	process();
	cout << cnt_pp;

	return 0;
}