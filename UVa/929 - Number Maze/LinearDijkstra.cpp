#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::max;

typedef vector<bool> vBool;
typedef vector<int> vInt;

const int oo = numeric_limits<int>::max();

struct Node{

    int r,c;

    Node(): r(), c() {}

    Node(int R, int C): r(R), c(C) {}

    bool operator==(const Node& other) const{
        return c == other.c and r == other.r;
    }

};

struct Pair{

    int a,b;

    Pair(int A, int B): a(A), b(B) {}

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    vector<Pair> adj;
    adj.emplace_back(0, -1);
    adj.emplace_back(0, +1);
    adj.emplace_back(-1, 0);
    adj.emplace_back(+1, 0);
    int z; cin >> z; 
    for(int y = 0; y < z; y++){
        int m,n,x; cin >> m >> n, x = 0;
        vector<vInt> w(m, vInt(n));
        for(auto& row : w)
            for(auto& k : row) 
                cin >> k, x = max(x, k);
        vector<vBool> seen(m, vBool(n));
        vector<vInt> d(m, vInt(n, oo));
        vector<queue<Node>> q(++x);
        int in = 1, k = w[0][0];
        Node t(m - 1, n - 1);
        q[w[0][0]].emplace(),
        d[0][0] = w[0][0];
        while(in){ k %= x;
            if(q[k++].empty()) continue;
            auto u = q[--k].front();
            q[k].pop(), --in;
            if(seen[u.r][u.c]) continue;
            seen[u.r][u.c] = true;
            if(u == t) break;
            for(auto& p : adj){
                int r = u.r + p.a;
                if(t.r < r) continue;
                if(r < 0) continue;
                int c = u.c + p.b;
                if(t.c < c) continue;
                if(c < 0) continue;
                int du = d[u.r][u.c];
                if(du + w[r][c] < d[r][c]){
                    int i = (k + w[r][c]) % x;
                    q[i].emplace(r, c), ++in,
                    d[r][c] = du + w[r][c];
                }
            }
        }
        cout << d[t.r][t.c] << '\n';
    }
    return 0;
}