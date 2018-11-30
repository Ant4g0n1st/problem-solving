#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <set>

using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;
using std::set;

namespace Constants{

    const int oo = std::numeric_limits<int>::max();

};

using namespace Constants;

struct Graph{

    struct Edge{

        int v,w;

        Edge(int V, int W): v(V), w(W) {}

    };

    typedef forward_list<Edge> List;

    struct Path : Edge{

        Path(int V, int W): Edge(V, W) {}

        bool operator<(const Path& other) const{
            if(w == other.w) return v < other.v;
            return w < other.w;
        }

    };

    vector<List> edges;
    vector<bool> seen;
    set<Path> best;
    vector<int> d;
    int n;

    Graph(int N): edges(N), n(N){}

    set<Path>::iterator Search(int u){
        return best.lower_bound({u, d[u]});
    }

    int ShortestPath(int s, int t){
        seen.assign(n, false),
        d.assign(n, oo),
        best.clear();
        best.emplace(s, d[s] = 0);
        while(!best.empty()){
            auto b = best.begin();
            auto u = b->v;
            best.erase(b);
            if(seen[u]) continue;
            if(u == t) break;
            seen[u] = true;
            for(const auto& e : edges[u]){
                if(seen[e.v]) continue;
                if(d[u] + e.w < d[e.v]){
                    auto lb = Search(e.v);
                    d[e.v] = d[u] + e.w;
                    Path p(e.v, d[e.v]);
                    if(lb != best.end())
                        best.erase(lb);
                    best.insert(p);
                }
            }
        }
        return d[t];
    }

    void AddEdge(int u, int v, int w){
        edges[u].emplace_front(v, w),
        edges[v].emplace_front(u, w);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int c = 1; c <= z; c++){
        std::unique_ptr<Graph> g;
        int m,n; cin >> n >> m;
        int s,t; cin >> s >> t;
        g.reset(new Graph(n)); 
        for(int k = 0; k < m; k++){
            int u; cin >> u;
            int v; cin >> v;
            int w; cin >> w;
            g->AddEdge(u, v, w);
        }
        cout << "Case #" << c << ": ";
        int w = g->ShortestPath(s, t);
        if(w < oo) cout << w << '\n';
        else cout << "unreachable\n";
    }
    return 0;
}
