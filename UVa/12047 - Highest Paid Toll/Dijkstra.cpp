#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::priority_queue;
using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::cout;
using std::cin;

typedef unsigned long long int Long;

namespace Constants{

    using std::numeric_limits;

    const Long oo = numeric_limits<long long int>::max();
    const int S = -1;

};

using namespace Constants;

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

    Long Heaviest(int s, int t, Long k){
        auto ds = ShortestPaths(s, g);
        auto dt = ShortestPaths(t, r);
        Long heaviest = oo;
        for(int u = 0; u < n; u++){
            if(ds[u] >= oo) 
                continue;
            for(auto& e : g[u]){
                auto v = e.v;
                if(ds[v] >= oo) 
                    continue;
                auto w = ds[u] + dt[v];
                if(w + e.w < k){
                    if(heaviest == oo){
                        heaviest = e.w;
                        continue;
                    }
                    if(heaviest < e.w)
                        heaviest = e.w;
                }
            }
        }
        return heaviest;
    }

    void AddEdge(int u, int v, Long w){
        g[u].emplace_front(v, w),
        r[v].emplace_front(u, w);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        int m,n; cin >> n >> m;
        int s; cin >> s, --s;
        int t; cin >> t, --t;
        unique_ptr<Graph> g;
        Long p; cin >> p;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            Long w; cin >> w;
            g->AddEdge(u, v, w);
        }
        Long h;
        h = g->Heaviest(s, t, p);
        if(h < oo)
            cout << h;
        else
            cout << S;
        cout << '\n';
    }
    return 0;
}