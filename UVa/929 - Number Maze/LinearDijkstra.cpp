#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::max;

typedef forward_list<int> List;
typedef vector<int> vInt;

const int oo = numeric_limits<int>::max();

struct Graph{

    vector<List> edges;
    vector<bool> seen;
    vector<int> w;
    vector<int> d;
    int m,n;

    Graph(int N): d(N, oo), 
        edges(N), m(-oo), n(N), seen(N), w(N) {}

    int ShortestPath(int s, int t){
        vector<queue<int>> q(++m);
        int in = 1, k, p = w[s];
        q[d[s] = w[s]].push(s);
        while(in){ p %= m;
            if(q[p++].empty()) continue;
            auto u = q[--p].front();
            q[p].pop(), --in;
            if(seen[u]) continue;
            if(u == t) break;
            seen[u] = true;
            for(auto& v : edges[u]){
                if(seen[v]) continue;
                if(d[u] + w[v] < d[v])
                    k = (p + w[v]) % m,
                    q[k].push(v), ++in,
                    d[v] = d[u] + w[v];
            }
        }
        return d[t];
    }

    void SetWeight(int u, int weight){
        m = max(m, w[u] = weight);
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v);
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
    unique_ptr<Graph> g;
    int z; cin >> z; 
    for(int y = 0; y < z; y++){
        int m,n,x; cin >> m >> n, x = 0;
        vector<vInt> node(m, vInt(n));
        g.reset(new Graph(m * n));
        for(auto& row : node)
            for(auto& k : row){ k = x++; 
                int w; cin >> w;
                g->SetWeight(k, w);
            }
        for(int a = 0; a < m; a++)
            for(int b = 0; b < n; b++)
                for(auto& p : adj){
                    int r = a + p.a;
                    if(m <= r) continue;
                    if(r < 0) continue;
                    int c = b + p.b;
                    if(n <= c) continue;
                    if(c < 0) continue;
                    int u = node[a][b];
                    int v = node[r][c];
                    g->AddEdge(u, v);
                }
        int s = 0, t = node[--m][--n];
        cout << g->ShortestPath(s, t),
        cout << '\n';
    }
    return 0;
}