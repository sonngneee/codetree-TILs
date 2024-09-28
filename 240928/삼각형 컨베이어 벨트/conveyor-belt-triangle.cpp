#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>

using namespace std;

int n, T;
vector<int>Q;

int main() {
	int a = 0;
	cin >> n >> T;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> a;
			Q.push_back(a);
		}
	}

	for (int t = 0; t < T; t++) {
		int temp = Q.back();
		Q.pop_back();
		Q.insert(Q.begin(), temp);
	}

	for (int i = 0; i < n * n; i++) {
		cout << Q[i] << ' ';
		if ((i + 1) % n == 0) cout << '\n';
	}
	return 0;
}