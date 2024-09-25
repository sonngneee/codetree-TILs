#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;


// 체스판의 왼쪽 상단은 (1,1)로 시작
// 기사의 초기위치는 (r,c)
//  (r,c)를 좌측 상단으로 하며 h(높이)×w(너비) 크기의 직사각형 형태
// 체력 : k

// 1. 이동 (bfs)
// 한 칸 이동
// 다른 기사가 있다면 그 기사도 함께 연쇄적으로 한 칸 밀려나게 됩니다
// 만약 기사가 이동하려는 방향의 끝에 벽이 있다면 모든 기사는 이동할 수 없게 됩니다.
// 체스판에서 사라진 기사에게 명령을 내리면 아무런 반응이 없게
// 
// 2. 대미지
// 밀려난 기사들은 피해 / 명령을 받은 기사는 피해를 입지 않으며
// w×h 직사각형 내에 놓여 있는 함정의 수만큼
//  체력이 깎이게 되며,
// 현재 체력 이상의 대미지를 받을 경우 기사는 체스판에서 사라지게 됩니다. 
//

struct pos {
    int y, x;
};

struct order {
    int num, d;
};

int L, N, Q; // 체스판 크기, n번 기사, 명령 Q개


int chessmap[41][41]; // 벽,함정 map
int knightmap[41][41]; // 기사 번호 map
int hp[31]; // 기사 체력 + 사망 여부
int attack[31]; // 기사 얼마나 맞았는지
vector<pos> hole; // 함정 위치
queue<order> King; // 왕의 명령
queue<int>slide; // 밀려난 기사 num
vector<pos> knightposinfo[31]; // 기사 y, x 좌표

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
int visited[41][41];
int ans;

void Input() {
    int a;
    cin >> L >> N >> Q;
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            cin >> a;
            chessmap[i][j] = a;
            if (chessmap[i][j] == 1) {
                hole.push_back({ i, j });
            }
        }
    }
    int y, x;
    int r, c, h, w, k;
    for (int i = 1; i <= N; i++) {
        cin >> r >> c >> h >> w >> k;
        y = r - 1, x = c - 1;

        for (int cy = y; cy < y + h; cy++) {
            for (int cx = x; cx < x + w; cx++) {
                knightmap[cy][cx] = i;
                knightposinfo[i].push_back({ cy, cx });
            }
        }
        hp[i] = k;
    }
    int n, d;

    for (int i = 0; i < Q; i++) {
        cin >> n >> d;
        King.push({ n, d });
    }

}

int IsMovePossible(order nowQ) {
    // nowQ = num, d >> 현재 기사 번호, 방향
    pos now, next;
    queue<pos> Qq;

    slide = queue<int>(); // slide 초기화

    // slide 초기화
    memset(visited, 0, sizeof(visited));

    int ksize = knightposinfo[nowQ.num].size();
    for (int i = 0; i < ksize; i++) {
        Qq.push(knightposinfo[nowQ.num][i]);
        visited[knightposinfo[nowQ.num][i].y][knightposinfo[nowQ.num][i].x] = 1;
    }

    slide.push(nowQ.num);

    while (!Qq.empty()) {
        now = Qq.front();
        Qq.pop();

        next.y = now.y + dy[nowQ.d];
        next.x = now.x + dx[nowQ.d];

        if (next.y < 0 || next.x < 0 || next.y >= L || next.x >= L) return 0;// 벽 바깥인 경우
        if (chessmap[next.y][next.x] == 2) return 0;// 벽인 경우
        if (knightmap[next.y][next.x] == 0) continue; // 빈공간일 경우
        if (visited[next.y][next.x] == 1) continue;

        if (knightmap[next.y][next.x] != knightmap[now.y][now.x]) { // 기사 번호가 다를 경우 => Qq.push => continue;
            slide.push(knightmap[next.y][next.x]);
            for (int i = 0; i < knightposinfo[knightmap[next.y][next.x]].size(); i++) {
                Qq.push(knightposinfo[knightmap[next.y][next.x]][i]);
                visited[knightposinfo[knightmap[next.y][next.x]][i].y][knightposinfo[knightmap[next.y][next.x]][i].x] = 1;
            }
            continue;
        }
        Qq.push({ next.y, next.x });
        visited[next.y][next.x] = 1;

    }
    return 1;
}

void Move(order pushk) {
    // 업데이트!
    // knightposinfo 기사 번호에 따른 y,x 좌표
    // hp : 기사 체력
    // knightmap : 기사 번호 map 
    int moveknight = 0;
    while (!slide.empty()) {
        moveknight = slide.front(); // 밀려난 기사 num
        slide.pop();

        if (moveknight == pushk.num) {
            // 이동만 하고 hp 감소 X

            for (int i = 0; i < knightposinfo[moveknight].size(); i++) {
                knightmap[knightposinfo[moveknight][i].y][knightposinfo[moveknight][i].x] = 0;
            }

            for (int i = 0; i < knightposinfo[moveknight].size(); i++) {
                knightposinfo[moveknight][i].y += dy[pushk.d];
                knightposinfo[moveknight][i].x += dx[pushk.d];
                knightmap[knightposinfo[moveknight][i].y][knightposinfo[moveknight][i].x] = moveknight;
            }

            continue;
        }


        for (int i = 0; i < knightposinfo[moveknight].size(); i++) {
            knightmap[knightposinfo[moveknight][i].y][knightposinfo[moveknight][i].x] = 0;
        }

        for (int i = 0; i < knightposinfo[moveknight].size(); i++) {
            knightposinfo[moveknight][i].y += dy[pushk.d];
            knightposinfo[moveknight][i].x += dx[pushk.d];
            knightmap[knightposinfo[moveknight][i].y][knightposinfo[moveknight][i].x] = moveknight;
            // 기사들은 모두 밀린 이후에 대미지를 입게 됩니다.
            if (chessmap[knightposinfo[moveknight][i].y][knightposinfo[moveknight][i].x] == 1) {
                hp[moveknight] -= 1;
                attack[moveknight]++;
            }
        }
    }
}

void Process() {
    order nowQ;
    int ismovepossible = 0;
    for (int q = 0; q < Q; q++) {
        nowQ = King.front(); // 현재 명령
        King.pop();

        //현재 체력 이상의 대미지를 받을 경우 
        if (hp[nowQ.num] <= 0) continue;    // 해당 기사가 죽었으면 다음 명령으로

        ismovepossible = IsMovePossible(nowQ);

        if (ismovepossible == 0) continue; // 움직이지 못하면 다음 명령으로

        Move(nowQ);
    }

    for (int i = 1; i <= N; i++) {
        if (hp[i] > 0) {
            ans += attack[i];
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