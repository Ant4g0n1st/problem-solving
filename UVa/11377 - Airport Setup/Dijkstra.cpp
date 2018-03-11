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
const int S = -1;

struct Graph{

    typedef forward_list<int> List;

    struct Path{

        int v,w;

        Path(int V, int W): v(V), w(W) {}

        bool operator<(const Path& other) const{
            return w > other.w;
        }

    };

    vector<List> edges;
    vector<bool> seen;
    vector<int> a;
    vector<int> d;
    int n;

    Graph(int N): a(N, true), 
        d(N), edges(N), n(N), seen(N) {}

    int ShortestPath(int s, int t){
        priority_queue<Path> q;
        Reset(), d[s] = a[s],
        q.emplace(s, d[s]);
        while(!q.empty()){
            auto u = q.top().v; q.pop();
            if(seen[u]) continue;
            if(u == t) break;
            seen[u] = true;
            for(auto& v : edges[u]){
                if(seen[v]) continue;
                if(!a[v]){
                    if(d[u] < d[v]){
                        Path p(v, d[u]);
                        d[v] = d[u],
                        q.push(p);
                    }
                    continue;
                }
                if(d[u] + a[v] < d[v]){
                    d[v] = d[u] + a[v];
                    q.emplace(v, d[v]);
                }
            }
        }
        return d[t];
    }

    void Reset(){
        for(int u = 0; u < n; u++)
            seen[u] = false, d[u] = oo;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

    void SetAirport(int u){
        a[u] = false;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 1; y <= z; y++){
        unique_ptr<Graph> g;
        int n; cin >> n; 
        int m; cin >> m;
        int x; cin >> x;
        g.reset(new Graph(n));
        for(int k = 0; k < x; k++){
            int u; cin >> u, --u, 
            g->SetAirport(u);
        }
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            g->AddEdge(u, v);
        }
        cout << "Case " << y << ":\n";
        int q; cin >> q;
        for(int k = 0; k < q; k++){
            int s; cin >> s, --s;
            int t; cin >> t, --t;
            int p = g->ShortestPath(s, t);
            if(s == t) p = 0;
            if(p < oo)
                cout << p;
            else
                cout << S;
            cout << '\n';
        }
        cout << '\n';
    }
    return 0;
}