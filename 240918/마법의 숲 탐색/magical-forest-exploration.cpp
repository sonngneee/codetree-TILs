#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;


//정령은 어떤 방향에서든 골렘에 탑승할 수 있지만 골렘에서 내릴 때에는 정해진 출구를 통해서만 내릴 수 있습니다.
//
// 더 이상 움직이지 못할 때까지 해당 과정을 반복
// (1) 남쪽으로 한 칸 내려갑니다.
//  초록색 칸들이 비어있을 때에만 남쪽으로 내려갈 수 있습니다.
// (1)의 방법으로 이동할 수 없으면 서쪽 방향으로 회전하면서 내려갑니다.
// 초록색 칸들이 비어있을 때만 가능
// 서쪽 방향으로 한 칸 이동을 한 뒤 내려가
// 출구가 반시계방향으로 이동
//(1)과 (2)의 방법으로 이동할 수 없으면 동쪽 방향으로 회전하면서 내려갑니다.
// 동쪽 방향으로 한칸 이동
// 출구 시계방향 이동
// 한칸 아래로
// 이동할 수 없는 가장 남쪽 도착 >> 정령 이동
// 만약 현재 위치하고 있는 골렘의 출구가 다른 골렘과 인접하고 있다면 
// 해당 출구를 통해 다른 골렘으로 이동할 수 있습니다. // 
// 모든 칸 중 갈수있는 가장 남쪽의 칸
// 정령의 위치가 최종 위치
// 
//
// 단, 골렘의 몸 일부가 여전히 숲을 벗어난 상태 >>
// 모든 골렘 초기화
// 다음 골렘부터 새롭게 숲 탐색 시작
// ? 이 경우에는 정령이 도달하는 최종 위치를 답에 포함시키지 않음
//
//정령의 최종 위치의 행 번호의 합을 구해야 함 (누적)
// 각 정령들이 최종적으로 위치한 행의 총합을 구하는 프로그램을 작성하세요. 
//  숲이 다시 텅 비게 돼도 행의 총합은 누적되는 것에 유의합니다.


int R, C, K; // 숲의 크키, 정령의 수

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
int ck_dir[3] = { 2, 3, 1 };
int visited[74][71];
int ans;
int is_possible;
struct map_forF {
	int exit_info, gol_num;
};

map_forF map_FF[74][71];
int map[74][71];

struct Ginfo {
	int c, d;
};

struct fairy_info {
	int r, c, dir, num;
};

struct pos {
	int r, c;
};

struct pos_num {
	int r, c, num;
};

vector <Ginfo> golem_info; // {{c, d}, ...}

int is_lr;

void input() {
	cin >> R >> C >> K;
	int c, d; // 골렘이 출발하는 열(중앙), 골렘의 출구 방향 정보

	for (int i = 0; i < K; i++) {
		cin >> c >> d;
		golem_info.push_back({ c-1, d });
	}
}


void move_fairy(fairy_info* st) {
	// 만약 현재 위치하고 있는 골렘의 출구가 다른 골렘과 인접하고 있다면 
	// 해당 출구를 통해 다른 골렘으로 이동할 수 있습니다. // 
	// 모든 칸 중 갈수있는 가장 남쪽의 칸
	// 정령의 위치가 최종 위치

	// map_FF

	int max_idx = st->r;
	pos_num now, next;
	queue <pos_num> q; // exit

	// visited[st->r][st->c] = 1;
	// visited 리셋

	for (int i = 0; i < R+3; i++) {
		memset(visited[i], 0, sizeof(visited[i]));
	}

	q.push({ st->r + dy[st->dir] , st->c + dx[st->dir] , st->num});
	visited[st->r + dy[st->dir]][st->c + dx[st->dir]] = 1;

	while (!q.empty()) {
		
		now = q.front();
		q.pop();
		max_idx = max(max_idx, now.r);

		if (map_FF[now.r][now.c].exit_info == 1) {

			for (int i = 0; i < 4; i++) {
				next.r = now.r + dy[i];
				next.c = now.c + dx[i];
				if (next.r < 0 || next.c < 0 || next.r >= R+3 || next.c >= C) continue;
				if (visited[next.r][next.c] == 1) continue;
				if (map_FF[next.r][next.c].exit_info == 0) continue;
				if (map_FF[next.r][next.c].gol_num != now.num) continue;

				q.push({ next.r, next.c, now.num });
				visited[next.r][next.c] = 1;

			}

		}

		else if(map_FF[now.r][now.c].exit_info == 2){
			for (int i = 0; i < 4; i++) {
				next.r = now.r + dy[i];
				next.c = now.c + dx[i];
				
				if (next.r < 0 || next.c < 0 || next.r >= R+3 || next.c >= C) continue;
				if (visited[next.r][next.c] == 1) continue;
				if (map_FF[next.r][next.c].exit_info == 0) continue;

				next.num = map_FF[next.r][next.c].gol_num;
				q.push(next);
				visited[next.r][next.c] = 1;

			}

		}

	}

	ans += (max_idx+1-3);
}


int check_green(fairy_info* st) {
	int except_dir;
	pos next;
	int cnt_g = 0;

	if (st->r == R + 3 - 2) return 0;
	if (is_lr == 3) {
		is_lr = 0;
		int i = 2;
		cnt_g = 0;
		except_dir = (ck_dir[i] + 2) % 4;

		for (int j = 0; j < 4; j++) {
			if (except_dir == j) continue;
			next.r = st->r + dy[j] + dy[ck_dir[i]];
			next.c = st->c + dx[j] + dx[ck_dir[i]];

			if (next.r < 0 || next.c < 0 || next.r >= R + 3 || next.c >= C) break;
			if (map_FF[next.r][next.c].exit_info != 0) break;
			cnt_g++;

		}

		if (cnt_g == 3) {
			return ck_dir[i];
		}
		return 0;
	}

	for (int i = 0; i < 3; i++) {
		cnt_g = 0;
		except_dir = (ck_dir[i]+2) % 4;

		for (int j = 0; j < 4; j++) {
			if (except_dir == j) continue;
			next.r = st->r + dy[j] + dy[ck_dir[i]];
			next.c = st->c + dx[j] + dx[ck_dir[i]];

			if (next.r < 0 || next.c < 0 || next.r >= R+3 || next.c >= C) break;
			if (map_FF[next.r][next.c].exit_info != 0) break;
			cnt_g++;

		}

		if (cnt_g == 3) {
			return ck_dir[i];
		}
	}

	return 0;
}


// move_dir = 남(2), 서(3), 동(1)
void move_green(int md, fairy_info* st) {

	

	if (md == 2) {
		// 남쪽이라면 그대로 내려가기
		// map 초기화
		map[st->r][st->c] = 0;
		for (int i = 0; i < 4; i++) {
			map[st->r + dy[i]][st->c + dx[i]] = 0;
		}

		map_FF[st->r][st->c] = { 0, 0 };
		for (int i = 0; i < 4; i++) {
			map_FF[st->r + dy[i]][st->c + dx[i]] = { 0, 0 };
		}

		// st 위치 업데이트
		st->r += dy[md];
		st->c += dx[md];

		// map 업데이트
		map[st->r][st->c] = 1;
		for (int i = 0; i < 4; i++) {
			map[st->r + dy[i]][st->c + dx[i]] = 1;
		}

		map[st->r + dy[st->dir]][st->c + dx[st->dir]] = 2;

		map_FF[st->r][st->c] = { 1, st->num };
		for (int i = 0; i < 4; i++) {
			map_FF[st->r + dy[i]][st->c + dx[i]] = { 1, st->num };
		}

		map_FF[st->r + dy[st->dir]][st->c + dx[st->dir]] = { 2, st->num };
		return;
	}

	// 다른방향이라면 한 칸 이동(move_idx) + 회전 + 내려가기
	
	// 이동+내려갈 칸이 비지 않을 경우 return

	// if (map[st->r + dy[md] + dy[2] * 2][st->c + dx[md] + dx[2] * 2] != 0 || map[st->r + dy[md] * 2 + dy[2]][st->c + dx[md] * 2 + dx[2]] != 0) return;
	if (map_FF[st->r + dy[md] + dy[2] * 2][st->c + dx[md] + dx[2] * 2].exit_info != 0 || map_FF[st->r + dy[md] * 2 + dy[2]][st->c + dx[md] * 2 + dx[2]].exit_info != 0) {
		is_lr = md;
		return;
	}


	// map 초기화
	map[st->r][st->c] = 0;
	for (int i = 0; i < 4; i++) {
		map[st->r + dy[i]][st->c + dx[i]] = 0;
	}

	map_FF[st->r][st->c] = { 0, 0 };
	for (int i = 0; i < 4; i++) {
		map_FF[st->r + dy[i]][st->c + dx[i]] = { 0, 0 };
	}

	// st 위치 업데이트
	st->r  = st->r + dy[md] + dy[2];
	st->c  = st->c +dx[md] + dx[2];

	// map 업데이트
	map[st->r][st->c] = 1;
	for (int i = 0; i < 4; i++) {
		map[st->r + dy[i]][st->c + dx[i]] = 1;
	}

	map_FF[st->r][st->c] = { 1, st->num };
	for (int i = 0; i < 4; i++) {
		map_FF[st->r + dy[i]][st->c + dx[i]] = { 1, st->num };
	}


	// st 방향 업데이트
	if (md == 3) {
		st->dir = (st->dir + 3) % 4;
	}
	else if (md == 1) {
		st->dir = (st->dir + 1) % 4;
	}
	// map 업데이트
	map[st->r + dy[st->dir]][st->c + dx[st->dir]] = 2;

	map_FF[st->r + dy[st->dir]][st->c + dx[st->dir]] = { 2, st->num };


	return;
}



void move_golem(fairy_info* st) {
	// 더 이상 움직이지 못할 때까지 해당 과정을 반복

	// 이동할 수 없는 가장 남쪽 도착 >> 정령 이동

	is_possible = 1; // 움직이는거 가능할때만
	int move_dir = 0;

	while (is_possible) {

		// 각 칸들이 비어있는지 확인
		// move_dir = 남, 서, 동
		// move_dir = 남(2), 서(3), 동(1)
		if (is_lr == 3) {
			move_dir = check_green(st);
		}
		else if (is_lr == 1) {
			is_lr = 0;
			move_dir = 0;
		}
		else {
			is_lr = 0;
			move_dir = check_green(st);
		}

		if (move_dir == 0) {
			// 더이상 움직일 곳이 없는 경우
			is_possible = 0;
			continue;
		}
		
		// 비어있다면 한 칸 이동 + 회전
		move_green(move_dir, st);
	}
}

int check_map(fairy_info st) {
	if (map[st.r][st.c] != 0) return 1;
	pos next;
	for (int i = 0; i < 4; i++) {
		next.r = st.r + dy[i];
		next.c = st.c + dx[i];
		if (next.r < 0 || next.c < 0 || next.r >= R || next.c >= C) return 1;
		if (map[next.r][next.c] != 0) return 1;
	}

	if (map_FF[st.r][st.c].exit_info != 0) return 1;
	// pos next;
	for (int i = 0; i < 4; i++) {
		next.r = st.r + dy[i];
		next.c = st.c + dx[i];
		if (next.r < 0 || next.c < 0 || next.r >= R || next.c >= C) return 1;
		if (map_FF[next.r][next.c].exit_info != 0) return 1;
	}
	return 0;
}

void process() {
	fairy_info st;
	int is_reset = 0;

	for (int g = 0; g < K; g++) {
		// 맨 처음 위치 정보 
		st = { 1, golem_info[g].c, golem_info[g].d, g+1 }; // 골렘,요정 번호 1~

		// 첫 골렘이 map에 올라갈 수 있는지 확인
		// is_reset = check_map(st);

		//if (is_reset == 1) {
		//	// ** map 리셋~!
		//	// 
		//	// 단, 골렘의 몸 일부가 여전히 숲을 벗어난 상태 >>
		//	// 모든 골렘 초기화
		//	// 다음 골렘부터 새롭게 숲 탐색 시작
		//	// ? 이 경우에는 정령이 도달하는 최종 위치를 답에 포함시키지 않음
		//	for (int i = 0; i < R; i++) {
		//		// map_FF
		//		memset(map[i], 0, sizeof(map[i]));
		//		memset(map_FF[i], 0, sizeof(map_FF[i]));
		//	}
		//	continue;
		//}

		map[st.r][st.c] = 1;
		for (int i = 0; i < 4; i++) {
			map[st.r + dy[i]][st.c + dx[i]] = 1;
		}
		map[st.r + dy[st.dir]][st.c + dx[st.dir]] = 2;

		map_FF[st.r][st.c] = { 1, g};
		for (int i = 0; i < 4; i++) {
			map_FF[st.r + dy[i]][st.c + dx[i]] = { 1, g };
		}
		map_FF[st.r + dy[st.dir]][st.c + dx[st.dir]] = { 2, g };

		// 골렘 이동
		move_golem(&st);

		// 근데~~ st.r <4 이면? 리셋~~~ㅇ리셋~~

		if (st.r < 4) {
			for (int i = 0; i < R+3; i++) {
				memset(map[i], 0, sizeof(map[i]));
				memset(map_FF[i], 0, sizeof(map_FF[i]));
			}
			continue;
		}

		// 정령 이동
		move_fairy(&st);

	}
}


int main() {

	//freopen("input.txt", "r", stdin);
	input();
	process();
	cout << ans;


}