#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>

using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Graph{

    struct Edge{

        int v,k;

        Edge(int V, int K): k(K), v(V) {}

    };

    typedef forward_list<Edge> List;

    struct Pair{

        int u,v;

        Pair(int U, int V): u(U), v(V) {} 

    };

    vector<vector<int>> bi;
    vector<List> edges;
    vector<Pair> pairs;
    vector<bool> used;
    vector<int> stack;
    vector<int> l;
    vector<int> t;
    int k,m,n,p;

    Graph(int N): edges(N), m(S), n(N) {}

    void AddEdge(int u, int v){
        pairs.push_back({u, v}), ++m,
        edges[u].emplace_front(v, m),
        edges[v].emplace_front(u, m);
    }

    void Tarjan(int u, int w = S){
        l[u] = t[u] = ++k;
        auto q = 0;
        for(auto& e : edges[u]){
            auto& v = e.v;
            auto& x = e.k;
            if(v == w and !q++)
                continue;
            if(!used[x]){
                stack[++p] = x,
                used[x] = true;
            }
            if(!t[v]){ Tarjan(v, u);
                if(t[u] <= l[v])
                    AddComponent(x);
                if(l[v] < l[u])
                    l[u] = l[v];
            }
            if(t[v] < l[u])
                l[u] = t[v];
        }
    }

    vector<int> PigeonValues(){
        l.resize(n), t.resize(n), 
        used.resize(++m), p = S,
        stack.resize(m), k = 0;
        vector<int> pigeon(n);
        for(int u = 0; u < n; u++){
            if(!t[u]) Tarjan(u);
            AddComponent();
        }
        vector<bool> seen(n);
        for(auto& comp : bi){
            for(auto& u : comp)
                if(!seen[u]) 
                    seen[u] = true,
                    ++pigeon[u];
            for(auto& u : comp)
                seen[u] = false;
        }
        return pigeon;
    }

    void AddComponent(int k = S){
        if(p == S) return;
        bi.emplace_back();
        auto& s = bi.back();
        while(p != S){
            auto& x = stack[p];
            auto& q = pairs[x];
            s.push_back(q.u),
            s.push_back(q.v);
            if(stack[p--] == k)
                break;
        }
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        if(!n and !m) break;
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        while(true){
            int u; cin >> u;
            int v; cin >> v;
            if(u < 0) break;
            g->AddEdge(u, v);
        }
        vector<vector<int>> bucket;
        vector<bool> used(n);
        bucket.resize(n);
        auto v = g->PigeonValues();
        for(int u = 0; u < n; u++){
            auto& b = bucket[v[u]];
            b.push_back(u),
            used[u] = true;
        }
        for(int k = n; k > 0; k--){
            if(!used[k]) continue;
            for(auto& u : bucket[k]){
                if(!m) continue;
                cout << u << ' ',
                cout << k, --m,
                cout << '\n';
            }
        }
        cout << '\n';
    }
    return 0;
}