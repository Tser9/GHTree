
#include <iostream>
#include <vector>
#include <ctime>
//#include <fstream>
using namespace std;

const int MAXN = 3000;
const int MAXM = 9000000;
const int oo = (1 << 29);
int n = 100;
int arrayq[MAXN][MAXN];/* = {
{ 0, 8, 9, 7, 0, 0, 0 },
{ 6, 0, 0, 5, 7, 0, 0 },
{ 6, 0, 0, 4, 0, 9, 0 },
{ 6, 0, 8, 0, 4, 6, 8 },
{ 6, 0, 8, 9, 0, 2, 0 },
{ 6, 0, 8, 9, 10, 0, 11 },
{ 6, 0, 8, 9, 10, 0 , 0 },
};*/
int Rebro[2 * MAXM], Flow[2 * MAXM], Cpct[2 * MAXM], Ntt[2 * MAXM],Lastq[MAXN + 2], Now[MAXN + 2], d[MAXN + 2], QQ[MAXN + 2];
int  E, Qh, Qt;

void add_edge(int u, int v, int c) {
    Rebro[E] = v; 
    Flow[E] = 0; 
    Cpct[E] = c; 
    Ntt[E] = Lastq[u]; 
    Lastq[u] = E++;
    Rebro[E] = u; 
    Flow[E] = 0; 
    Cpct[E] = c; 
    Ntt[E] = Lastq[v]; 
    Lastq[v] = E++;
}

bool bfs(int s, int t) {
    for (int i = 1; i <= n; i++) d[i] = 0;
    Qh = Qt = 0;
    QQ[Qh++] = s;
    d[s] = 1;
    while (Qt < Qh && !d[t]) {
        int u = QQ[Qt++];
        for (int e = Lastq[u]; e; e = Ntt[e]) {
            int v = Rebro[e];

            if (!d[v] && Flow[e] < Cpct[e]) {
                d[v] = d[u] + 1;
                QQ[Qh++] = v;
            }
        }
    }

    return d[t];
}
int dfs(int u, int t, int F) {
    if (u == t) return F;
    for (int e = Now[u], f = 0; e; e = Now[u] = Ntt[e]) {
        int v = Rebro[e];
        if (d[u] + 1 == d[v] && Flow[e] < Cpct[e] && (f = dfs(v, t, min(F, Cpct[e] - Flow[e]))))
        {
            Flow[e] += f;
            Flow[e ^ 1] -= f;
            return f;
        }
    }
    return 0;
}
int maxFlow(int s, int t) {
    for (int e = 2; e < E; e++) {
        Flow[e] = 0;
    }
    int F = 0, f = 0;
    while (bfs(s, t)) {
        for (int i = 0; i <= n; i++) Now[i] = Lastq[i];
        while ((f = dfs(s, t, oo))) { F += f; }
    }
    return F;
}
vector<pair<int, int>> GH_t[MAXN];
int p[MAXN];
void GHTree() {

    fill(p, p + n + 1, 1);
    for (int u = 2; u <= n; u++) {
        int mxf = maxFlow(u, p[u]);
        GH_t[u].push_back(pair<int, int>(p[u], mxf));
        GH_t[p[u]].push_back(pair<int, int>(u, mxf));
        for (int v = u + 1; v <= n; v++) {
            if (d[v] && p[v] == p[u]) {
                p[v] = u;
            }
        }
    }
}

void print_GHTree() {
    for (int u = 1; u <= n; u++) {
        for (auto e : GH_t[u]) {
            cout << u << "<-->" << e.first << " : " << e.second << '\n';
        }
    }
}
int main() {
    srand(time(0));
    E = 2;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            arrayq[i][j] = rand() % 50;
            if (arrayq[i][j] % 3 == 0)
                arrayq[i][j] = 0;
            else if (arrayq[i][j] != 0) { add_edge(i + 1, j + 1, arrayq[i][j]); }
        }arrayq[i][i] = 0;
        for (int j = i; j >= 0; j--) {
            arrayq[i][j] = arrayq[j][i];
        }

    }
    /*for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << arrayq[i][j] << ' ';
        }cout << endl;
    }*/
    GHTree();
    print_GHTree();
}
