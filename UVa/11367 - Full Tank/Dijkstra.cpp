#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::priority_queue;
using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::cout;
using std::cin;

typedef vector<bool> vBool;
typedef vector<int> vInt;

const int oo = numeric_limits<int>::max();

struct Graph{

    struct Edge{

        int v,w;

        Edge(int V): v(V), w() {}

        Edge(int V, int W): v(V), w(W) {}

    };

    typedef forward_list<Edge> List;

    struct Path : Edge{

        int f;

        Path(int V) : Edge(V), f() {}

        Path(int V, int W) : Edge(V, W), f() {}

        Path(int V, int W, int F) : Edge(V, W), f(F) {}

        bool operator<(const Path& other) const{
            if(w == other.w) return f < other.f;
            return w > other.w;
        }

    };

    vector<vBool> seen;
    vector<List> edges;
    vector<vInt> d;
    vector<int> w;
    int n;

    Graph(int N): d(), edges(N), n(N), w(N) {}

    int ShortestPath(int s, int t, int c){
        d.assign(n, vInt(++c, oo));
        seen.assign(n, vBool(c));
        priority_queue<Path> q;
        q.emplace(s), d[s][0] = 0, --c;
        while(!q.empty()){
            int f = q.top().f;
            int r = q.top().w;
            int u = q.top().v; q.pop();
            if(seen[u][f]) continue;
            if(u == t) return r;
            seen[u][f] = true;
            if(f < c){ auto g = f + 1;
                if(w[u] + r < d[u][g]){
                    d[u][g] = w[u] + r;
                    Path p(u, d[u][g]);
                    p.f = g, q.push(p);
                }
            }
            for(auto& e : edges[u]){
                if(f < e.w) continue; 
                auto g = f - e.w;
                auto v = e.v;
                if(seen[v][g]) 
                    continue;
                if(r < d[v][g]){
                    Path p(v, r, g);
                    d[v][g] = r,
                    q.push(p);
                }
            }
        }
        return oo;
    }

    void AddEdge(int u, int v, int w){
        edges[u].emplace_front(v, w),
        edges[v].emplace_front(u, w);
    }

    void SetWeight(int u, int weight){
        w[u] = weight;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    unique_ptr<Graph> g;
    int n; cin >> n;
    int m; cin >> m;
    g.reset(new Graph(n));
    for(int u = 0; u < n; u++){
        int w; cin >> w;
        g->SetWeight(u, w);
    }
    for(int k = 0; k < m; k++){
        int u; cin >> u;
        int v; cin >> v;
        int w; cin >> w;
        g->AddEdge(u, v, w);
    }
    int q; cin >> q;
    for(int k = 0; k < q; k++){
        int c; cin >> c;
        int s; cin >> s;
        int t; cin >> t;
        int w = g->ShortestPath(s, t, c);
        if(w == oo)
            cout << "impossible";
        else cout << w;
        cout << '\n';
    }
    return 0;
}