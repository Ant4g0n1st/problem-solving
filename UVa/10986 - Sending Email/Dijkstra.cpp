#include <iostream> 
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::priority_queue;
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

    typedef vector<Edge> vEdge; 

    struct Path : Edge{

        Path(int V, int W): Edge(V, W) {} 

        bool operator<(const Path& other) const{
            if(w == other.w) return v < other.v;
            return w > other.w;
        }

    };

    vector<vEdge> edges;
    vector<bool> seen;
    vector<int> d;
    int n;

    Graph(int N): n(N), d(N, oo), edges(N), seen(N) {}

    int ShortestPath(int s, int t){ 
        priority_queue<Path> q;
        q.emplace(s, 0), d[s] = 0;
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
        edges[u].emplace_back(v, w),
        edges[v].emplace_back(u, w);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    unique_ptr<Graph> g;
    int t; cin >> t;
    for(int c = 1; c <= t; c++){
        int m,n; cin >> n >> m;
        int a,b; cin >> a >> b; 
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u;
            int v; cin >> v;
            int w; cin >> w;
            g->AddEdge(u, v, w);
        }
        cout << "Case #" << c << ": ";
        int w = g->ShortestPath(a, b);
        if(w < oo) cout << w << '\n';
        else cout << "unreachable\n";
    }
    return 0;
}