#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::unique_ptr;
using std::vector;
using std::queue;
using std::sort;
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
    int n,oo;

    Bipartite(int N): oo(N + 1), 
        seen(N), edges(N, vInt()), d(N),
        couple(N, S), match(N), n(N), side(N) {}

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
        } int m = 0;
        for(int v = 0; v < n; v++){
            if(side[v]) continue;
            if(couple[v] != S) 
                couple[couple[v]] = v,
                match[v] = true, ++m;
        }
        return m;
    }

    int AugmentingPath(int u){
        seen[u] = true, path.push(u);
        for(auto& v : edges[u])
            if(couple[v] == S){
                match[u] = true;
                return couple[v] = u;
            }
        for(auto& v : edges[u]){
            auto& w = couple[v];
            if(!seen[w] and d[w] == d[u] + 1)
                if(AugmentingPath(w) != S){
                    match[u] = true,
                    couple[v] = u;
                    return u;
                }
        }
        return S;
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
                    q.push(w);
            }
        }
    }

    vector<int> MVC(){ queue<int> q;
        for(int u = 0; u < n; u++){
            seen[u] = false;
            if(!side[u]) continue;
            if(match[u]) continue;
            seen[u] = true,
            q.push(u);
        }
        while(!q.empty()){
            int u = q.front(); q.pop();
            if(!side[u]){
                auto& v = couple[u];
                if(v != S)
                    seen[v] = true,
                    q.push(v);
                continue;
            }
            for(auto& v : edges[u]){
                if(seen[v]) continue;
                if(v != couple[u])
                    seen[v] = true,
                    q.push(v);
            }
        }
        vector<int> mvc;
        for(int v = 0; v < n; v++){
            if(side[v]) continue;
            auto& w = couple[v];
            if(w == S) continue;
            if(!seen[w])
                mvc.push_back(w);
            else 
                mvc.push_back(v);
        }
        return mvc;
    }

    void AddEdge(int u, int v){
        edges[u].push_back(v),
        edges[v].push_back(u);
    }

    void SetRight(int v){
        side[v] = false;
    }

    void SetLeft(int u){
        side[u] = true;
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    unique_ptr<Bipartite> b;
    int c,n,r;
    while(cin >> r >> c >> n){
        if(!r and !c and !n) break;
        b.reset(new Bipartite(r + c));
        for(int e = 0; e < n; e++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            b->AddEdge(u, r + v),
            b->SetRight(r + v), 
            b->SetLeft(u);
        }
        cout << b->MaximumMatching();
        vector<int> mvc = b->MVC();
        sort(mvc.begin(), mvc.end());
        for(auto& u : mvc){ 
            cout << ' ';
            if(u >= r) u -= r,
                cout << 'c' << ++u;
            else 
                cout << 'r' << ++u;
        }
        cout << '\n';
    }
    return 0;
}