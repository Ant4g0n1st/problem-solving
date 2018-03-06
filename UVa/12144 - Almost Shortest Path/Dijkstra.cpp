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
using std::vector;
using std::cout;
using std::cin;

typedef long long int Long;

const Long oo = numeric_limits<int>::max();
const Long S = -1;

struct Graph{

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef forward_list<Edge> List;

    struct Path : Edge{

        Path(int V, Long W): Edge(V, W) {}

        bool operator<(const Path& other) const{
            return w > other.w;
        }

    };

    vector<List> g;
    vector<List> r;
    int n;

    Graph(int N): g(N), n(N), r(N) {}

    vector<Long> ShortestPaths(int s,
                vector<List>& edges){
        priority_queue<Path> q;
        vector<Long> d(n, oo);
        vector<bool> seen(n);
        q.emplace(s, d[s] = 0);
        while(!q.empty()){
            auto u = q.top().v; q.pop();
            if(seen[u]) continue;
            seen[u] = true;
            for(auto& e : edges[u]){
                if(seen[e.v]) continue;
                if(d[u] + e.w < d[e.v]){
                    d[e.v] = d[u] + e.w;
                    Path p(e.v, d[e.v]);
                    q.push(p);
                }
            }
        }
        return d;
    }

    int AlmostShortest(int s, int t){
        auto ds = ShortestPaths(s, g);
        auto dt = ShortestPaths(t, r);
        priority_queue<Path> q;
        vector<Long> d(n, oo);
        vector<bool> seen(n);
        q.emplace(s, d[s] = 0);
        while(!q.empty()){
            auto u = q.top().v; q.pop();
            if(seen[u]) continue;
            if(u == t) break;
            seen[u] = true;
            for(auto& e : g[u]){
                auto v = e.v;
                if(seen[v]) 
                    continue;
                auto sp = ds[u] + dt[v];
                if(sp + e.w == ds[t])
                    continue;
                if(d[u] + e.w < d[v]){
                    d[v] = d[u] + e.w;
                    Path p(v, d[v]);
                    q.push(p);
                }
            }
        }
        return d[t];
    }

    void AddEdge(int u, int v, Long w){
        g[u].emplace_front(v, w),
        r[v].emplace_front(u, w);
    } 

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        if(!n and !m) break;
        unique_ptr<Graph> g;
        int s; cin >> s;
        int t; cin >> t;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u;
            int v; cin >> v;
            int w; cin >> w;
            g->AddEdge(u, v, w);
        }
        Long w;
        w = g->AlmostShortest(s, t),
        cout << (w < oo ? w : S),
        cout << '\n';
    }
    return 0;
}