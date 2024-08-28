#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>
#include <algorithm>

using namespace std;


int map[21][21];
int visited[21][21];
int n, m;  // , 폭탄 종류

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

int isFin = 0;
int score;

struct pos {
    int y, x;
};

struct forsort {

    int B_cnt, red_cnt, rowmax_notRed, colmin_nowRed; // 빨간폭탄, 빨간색 제외 행이 가장 큰 칸, 열이 가장 작은 칸
    vector<pos> bomb_connect; // 이어진 폭탄 저장용

};

vector<forsort> bomb_list; // 한 라운드에 터질 폭탄 비교용

bool cmp(forsort a, forsort b) {
    if (a.red_cnt == b.red_cnt) {
        if (a.rowmax_notRed == b.rowmax_notRed) {

            return a.colmin_nowRed < b.colmin_nowRed;
        }
        return a.rowmax_notRed > b.rowmax_notRed;
    }
    return a.red_cnt < b.red_cnt;
}


void input() {

    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> map[i][j];
        }
    }
}

//
// -2 == 빈칸
//-1은 해당 칸에 검은색 돌이, 
// 0은 빨간색 폭탄이, 
// 1이상 m이하의 숫자는 빨간색과는 다른 서로 다른 색의 폭탄
// 
// 더 이상 폭탄 묶음이 없을 때까지 계속 반복
// 
// 1. 가장 큰 폭탄 묶음을 찾음
//        폭탄 묶음이란 2개 이상의 폭탄으로 이루어져 있어야 하며, 
//        1- 모두 같은 색깔의 폭탄으로만 이루어져 있거나 
//        2- 빨간색 폭탄을 포함하여 정확히 2개의 색깔로만 이루어진 폭탄
//        단, 빨간색으로만 이루어져 있는 경우 X
//        빨간색이 아닌 두가지의 색 X
//        모두 연결되어 있어야함 (상하좌우)
//  
// 2. 폭탄 제거
//        검은 돌 제외
//    2. 1. 점수 구하기 (터진 폭탄의 개수*터진폭탄의 개수
// )
// 
//    3. 중력 작용 : 폭탄들 아래로 떨어짐
//            돌이 있으면 막혀서 못 내려옴
// 
// 4. 반시계 방향으로 90만큼 회전
// 
// 5. 중력 작용
// 
// 6. 반복
// 

void bfs(int y, int x, forsort* Bundle) {
    queue<pos>q;
    pos now, next;

    q.push({ y, x });
    visited[y][x] = 1;
    while (!q.empty()) {
        now = q.front();
        q.pop();
        Bundle->B_cnt++;
        Bundle->bomb_connect.push_back({ now.y, now.x });

        if (map[now.y][now.x] == 0) Bundle->red_cnt++;
        else {
            Bundle->rowmax_notRed = max(Bundle->rowmax_notRed, now.y);
            Bundle->colmin_nowRed = min(Bundle->colmin_nowRed, now.x);

        }

        for (int i = 0; i < 4; i++) {
            next.y = now.y + dy[i];
            next.x = now.x + dx[i];

            if (next.y < 0 || next.y >= n || next.x < 0 || next.x >= n) continue;
            if (visited[next.y][next.x] == 1) continue;
            // 빨간색 제외의 다른 색인경우
            if ((map[next.y][next.x] != 0) && (map[y][x] != map[next.y][next.x])) continue;
            // 돌 + 빈칸인경우
            if (map[next.y][next.x] < 0) continue;

            q.push(next);
            visited[next.y][next.x] = 1;
        }
    }

}



void findMaxBomb() {

    int bomb_cnt = 1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (map[i][j] < 1 || visited[i][j] == 1) continue;
            forsort Bundle = { 0, 0, 0, 30, {} };

            bfs(i, j, &Bundle);

            // visited에서 red 0으로 변환
            for (int ry = 0; ry < n; ry++) {
                for (int rx = 0; rx < n; rx++) {
                    if (map[ry][rx] == 0) {
                        visited[ry][rx] = 0;
                    }
                }
            }

            // 가장 큰 폭탄 업데이트, 리스트 저장
            if (Bundle.B_cnt > bomb_cnt) {
                bomb_cnt = Bundle.B_cnt;
                bomb_list.clear();
                bomb_list.push_back(Bundle);
            }
            else if (Bundle.B_cnt == bomb_cnt && bomb_cnt != 1) {
                bomb_list.push_back(Bundle);
            }

        }
    }
    if (bomb_cnt == 1) {
        isFin = 1;
    }
}

void killBomb() {
    for (pos d : bomb_list[0].bomb_connect) {
        map[d.y][d.x] = -2;
    }
}

void gravity() {
    // 아래 방향으로 끌고오기
    int temp = 0;
    int next = 0;

    for (int j = 0; j < n; j++) {
        next = 0;
        for (int i = n - 1; i > 0; i--) {
            if (map[i][j] != -2) continue;
            for (int c = i - 1; c >= 0; c--) {

                if (map[c][j] == -2) continue;
                if (map[c][j] == -1) {
                    next = 1;
                    break;
                }

                temp = map[c][j];
                map[c][j] = -2;
                map[i][j] = temp;
                break;
            }
            if (next == 1) break;
        }
    }
}

void turn() {
    int tempMap[21][21] = { 0 };
    memcpy(tempMap, map, sizeof(tempMap));

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            map[y][x] = tempMap[x][n - y - 1];
        }
    }
}



void process() {

    findMaxBomb();
    if (isFin == 1) {
        return;
    }
    sort(bomb_list.begin(), bomb_list.end(), cmp);
    killBomb();
    score += bomb_list[0].bomb_connect.size() * bomb_list[0].bomb_connect.size();
    gravity();
    turn();
    gravity();

}

void init() {
    // visited 초기화!
    for (int i = 0; i < n; i++) {
        memset(visited[i], 0, sizeof(visited[i]));
    }



}


int main() {

    //freopen("sample_input.txt", "r", stdin);

    input();

    while (!isFin) {
        init();
        process();
    }
    cout << score;

    return 0;
}