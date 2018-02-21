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
    int n;

    Graph(int N): d(N, oo), edges(N), n(N), seen(N) {}

    int ShortestPath(int s, int t){
        for(int u = 0; u < n; u++)
            seen[u] = false, d[u] = oo;
        priority_queue<Path> q;
        q.emplace(s, d[s] = 0);
        while(!q.empty()){
            auto u = q.top().v; q.pop();
            if(seen[u]) continue;
            if(u == t) break;
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
        return d[t];
    }

    void AddEdge(int u, int v, int w){
        edges[u].emplace_front(v, w);
    }

    vector<int> ShortestPaths(int t){
        vector<int> w(n, oo);
        for(int s = 0; s < n; s++)
            w[s] = ShortestPath(s, t);
        return w;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 0; y < z; y++){
        int e,n; cin >> n >> e;
        int m,t; cin >> t >> m;
        unique_ptr<Graph> g;
        g.reset(new Graph(n)), --e;
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            int w; cin >> w;
            g->AddEdge(u, v, w);
        }
        vector<int> w; int c = 0;
        w = g->ShortestPaths(e);
        for(auto& k : w)
            if(k <= t) ++c;
        if(y) cout << '\n';
        cout << c << '\n';
    }
    return 0;
}