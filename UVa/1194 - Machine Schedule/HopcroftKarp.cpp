#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;

typedef vector<int> vInt;

const int S = -1;

struct Bipartite{

    vector<int> couple;
    vector<vInt> edges;
    vector<bool> match;
    vector<bool> seen;
    vector<bool> side;
    queue<int> path;
    vector<int> d;
    vector<int> p;
    int n,oo;

    Bipartite(int N): d(N), 
        oo(N + 1), p(N), n(N), match(N),
        couple(N, S), edges(N), seen(N), side(N) {}

    void SetRight(int v){ side[v] = false; }

    void SetLeft(int u){ side[u] = true; }

    int MaximumMatching(){
        while(true){    
            ShortestPaths(); int m = 0;
            for(int u = 0; u < n; u++){
                if(!side[u]) continue;
                if(match[u]) continue;
                if(AugmentingPath(u) != S) ++m;
                while(!path.empty())
                    seen[path.front()] = false,
                    path.pop();
            } if(!m) break;
        } int matching = 0;
        for(int v = 0; v < n; v++){
            if(side[v]) continue;
            if(couple[v] != S) ++matching;
        }
        return matching;
    }

    void ShortestPaths(){ queue<int> q;
        for(int u = 0; u < n; u++){
            d[u] = oo;
            if(!side[u]) continue;
            if(match[u]) continue; 
            q.push(u),
            d[u] = 0; 
        }
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& v : edges[u]){
                auto& w = couple[v];
                if(w == S) continue;
                if(d[u] + 1 < d[w])
                    d[w] = d[u] + 1,
                    q.push(w), 
                    p[w] = u;
            }
        }
    }

    int AugmentingPath(int u){
        seen[u] = true, path.push(u);
        for(auto& v : edges[u]){
            auto& w = couple[v];
            if(w == S){
                match[u] = true;
                return w = u;
            }
        }
        for(auto& v : edges[u]){
            auto& w = couple[v];
            if(seen[w] or p[w] != u)
                continue;
            if(AugmentingPath(w) != S){
                match[u] = true;
                return w = u;
            }       
        }
        return S;
    }

    void AddEdge(int u, int v){
        edges[u].push_back(v);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n and n){
        unique_ptr<Bipartite> b;
        int k,m; cin >> m >> k;
        b.reset(new Bipartite(n + m));
        for(int i = 0; i < k; i++){
            int t; cin >> t; 
            int u; cin >> u;
            int v; cin >> v;
            if(!u or !v) continue;
            b->AddEdge(u, n + v),
            b->SetRight(n + v),
            b->SetLeft(u);
        }
        cout << b->MaximumMatching(),
        cout << '\n';
    }
    return 0;
}