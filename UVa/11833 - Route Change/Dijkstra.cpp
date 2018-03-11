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
using std::swap;
using std::cin;

const int oo = numeric_limits<int>::max();

struct Graph{

    struct Edge{

        int v,w;

        Edge(int V, int W): v(V), w(W) {}

    };

    typedef forward_list<Edge> List;

    struct Path : Edge{

        Path(int V, int W): Edge(V, W) {}

        bool operator<(const Path& other) const{
            return w > other.w;
        }

    };

    vector<List> edges;
    vector<bool> seen;
    vector<int> d;
    vector<int> p;
    vector<int> r;
    int n;

    Graph(int N): d(N, oo), 
        edges(N), n(N), p(N), r(N), seen(N) {
        for(int u = --N; u > 0; u--) 
            p[u - 1] = u;
    }

    int ShortestPath(int s, int t){
        priority_queue<Path> q;
        q.emplace(s, d[s] = 0);
        ServiceRoute(t);
        while(!q.empty()){
            auto u = q.top().v; q.pop();
            if(seen[u]) continue;
            if(u == t) break;
            seen[u] = true;
            for(auto& e : edges[u]){
                auto v = e.v;
                auto w = e.w;
                if(v < t){ 
                    auto x = r[v] + w;
                    if(d[u] + x < d[t]){
                        d[t] = d[u] + x;
                        Path p(t, d[t]);
                        q.push(p);
                    }
                    continue;
                }
                if(seen[v]) 
                    continue;
                if(d[u] + w < d[v]){
                    d[v] = d[u] + w;
                    Path p(v, d[v]);
                    q.push(p);
                }
            }
        }
        return d[t];
    }

    void ServiceRoute(int t){
        for(int u = --t; u > 0; u--)
            r[u - 1] = r[u - 1] + r[u];
    }

    void AddEdge(int u, int v, int w){
        edges[u].emplace_front(v, w),
        edges[v].emplace_front(u, w);
        if(u < v) swap(u, v);
        if(p[v] == u) 
            r[v] = w;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int c,m,n,x;
    while(cin >> n >> m >> c >> x){
        if(!n and !m and !c and !x) break;
        unique_ptr<Graph> g(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u;
            int v; cin >> v;
            int w; cin >> w;
            g->AddEdge(u, v, w);
        }
        cout << g->ShortestPath(x, --c);
        cout << '\n';
    }
    return 0;
}