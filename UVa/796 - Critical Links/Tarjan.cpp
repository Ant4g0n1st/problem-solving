#include <forward_list>
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::sort;
using std::swap;
using std::cin;
using std::ws;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Graph{

    struct Edge{

        int u,v;

        Edge(int U, int V): u(U), v(V) {
            if(u > v) swap(u, v);
        }

        bool operator<(const Edge& other) const{
            if(u == other.u) return v < other.v;
            return u < other.u;
        }

    };

    typedef forward_list<int> List;

    vector<List> edges;
    vector<Edge> b;
    vector<int> m;
    vector<int> t;
    int k,n;

    Graph(int N): edges(N), n(N) {}

    void Tarjan(int u, int p = S){
        m[u] = t[u] = ++k; 
        auto q = 0;
        for(auto& v : edges[u]){
            if(v == p and !q++) continue;
            if(!t[v]){ Tarjan(v, u);
                if(t[u] < m[v])
                    b.emplace_back(u, v);
                if(m[v] < m[u]) 
                    m[u] = m[v];
            }
            if(t[v] < m[u])
                m[u] = t[v];
        }
    }

    vector<Edge>& CutEdges(){
        m.resize(n), t.resize(n), k = 0;
        for(int u = 0; u < n; u++)
            if(!t[u]) Tarjan(u);
        return b;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int x = 0; x < n; x++){
            int u; cin >> u;
            char a; cin >> ws >> a;
            int m; cin >> m;
            char b; cin >> ws >> b;
            while(m--){
                int v; cin >> v;
                g->AddEdge(u, v);
            }
        }
        auto& b = g->CutEdges();
        sort(b.begin(), b.end());
        cout << b.size() << ' ';
        cout << "critical ",
        cout << "links\n";
        for(auto& bridge : b){
            cout << bridge.u,
            cout << " - ",
            cout << bridge.v,
            cout << '\n';
        }
        cout << '\n';
    }
    return 0;
}