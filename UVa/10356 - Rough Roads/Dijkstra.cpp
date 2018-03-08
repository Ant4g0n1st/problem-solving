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
const int B = 2;

struct Graph{

    struct Edge{

        int v,w;

        Edge(int V, int W): v(V), w(W) {}

    };

    typedef forward_list<Edge> List;

    struct Path : Edge{

        bool b;

        Path(int V, int W, bool B): b(B), Edge(V, W) {}

        bool operator<(const Path& other) const{
            return w > other.w;
        }

    };

    vector<vBool> seen;
    vector<List> edges;
    vector<vInt> d;
    int n;

    Graph(int N): edges(N), n(N) {}

    int ShortestPath(int s, int t){
        d.assign(n, vInt(B, oo));
        seen.assign(n, vBool(B));
        priority_queue<Path> q;
        q.emplace(s, 0, false);
        d[s][false] = 0;
        while(!q.empty()){
            auto b = q.top().b;
            auto u = q.top().v; 
            auto w = q.top().w; q.pop();
            if(u == t and !b) return w;
            if(seen[u][b]) continue;
            seen[u][b] = true;
            for(auto& e : edges[u]){
                auto v = e.v;
                auto r = !b;
                auto z = e.w;
                if(seen[v][r]) continue;
                if(w + z < d[v][r]){
                    Path p(v, w + z, r);
                    d[v][r] = w + z,
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

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int n,m,z; z = 0;
    while(cin >> n >> m){
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u;
            int v; cin >> v;
            int w; cin >> w;
            g->AddEdge(u, v, w);
        }
        cout << "Set #" << ++z,
        cout << '\n';
        int s = 0, t = --n, w;
        w = g->ShortestPath(s, t);
        if(w < oo) cout << w;
        else cout << '?';
        cout << '\n';
    }
    return 0;
}