#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>
#include <set>

using std::numeric_limits;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::fill;
using std::cin;
using std::set;

typedef vector<int> vInt;

const int oo = numeric_limits<int>::max();
const int S = -1;

struct Edge{

    int u,v;

    Edge(): u(), v() {}

    Edge(int U, int V): u(U), v(V) {}

};

struct Graph{

    vector<set<int>> edges;
    vector<bool> seen;
    vector<int> deg;
    vector<int> w;
    int n;

    Graph(int N): n(N),
        edges(N), deg(N), seen(N), w(N) {}

    int Center(int s = 0){ 
        for(int u = 0; u < n; u++)
            seen[u] = false, deg[u] = 0;
        int center; seen[s] = true; 
        queue<int> q; q.push(s);
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u]){
                if(seen[v]) continue;
                seen[v] = true, 
                q.push(v);
            }
        }
        for(int u = 0; u < n; u++){
            if(!seen[u]) continue;
            for(auto& v : edges[u]) 
                if(seen[v]) ++deg[v];
        }
        for(int u = 0; u < n; u++){
            if(!seen[u]) continue;
            if(!deg[u]){
                seen[u] = true,
                q.push(u);
                break;
            }
            if(--deg[u] == 0) 
                seen[u] = true,
                q.push(u);
        }
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(--deg[v] == 0)
                    seen[v] = true,
                    q.push(v);
            center = u;
        }
        return center;
    }

    int ShortestPaths(int s = 0){
        fill(w.begin(), w.end(), oo);
        queue<int> q; int furthest; 
        q.push(s), w[s] = 0;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(w[u] + 1 < w[v])
                    w[v] = w[u] + 1,
                    q.push(v);
            furthest = u;
        }
        return furthest;
    }

    int RemoveEdge(Edge& r, Edge& p){
        RemoveEdge(r);
        p.u = Center(r.u);
        p.v = Center(r.v);
        AddEdge(p.u, p.v);
        int d = Diameter();
        RemoveEdge(p);
        AddEdge(r.u, r.v);
        return d;
    }

    int Diameter(int s = 0){
        auto f = ShortestPaths(s);
        return w[ShortestPaths(f)];
    }

    void AddEdge(int u, int v){
        edges[u].emplace(v),
        edges[v].emplace(u);
    }   

    void RemoveEdge(Edge& r){
        edges[r.u].erase(r.v),
        edges[r.v].erase(r.u);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int c = 0; c < z; c++){
        int n; cin >> n;
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        vector<Edge> edges(n - 1);
        for(auto& e : edges)
            cin >> e.u, --e.u,
            cin >> e.v, --e.v,
            g->AddEdge(e.u, e.v);
        Edge p,q,r; int min = oo;
        for(auto& e : edges){
            int d = g->RemoveEdge(e, q);
            if(min <= d) continue; 
            min = d, p = q, r = e;
        }
        cout << min << '\n',
        cout << ++r.u << ' ',
        cout << ++r.v << '\n',
        cout << ++p.u << ' ',
        cout << ++p.v << '\n';
    }
    return 0;
}