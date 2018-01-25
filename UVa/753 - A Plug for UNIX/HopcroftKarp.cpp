#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <map>

using std::unique_ptr;
using std::string;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::map;
using std::ws;

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
            if(couple[v] != S) ++m,
                match[v] = true;
        }
        return m;
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

struct Edge{

    int u,v;

    Edge(): u(), v() {}

    Edge(int U, int V): u(U), v(V) {}

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int t; cin >> t;
    for(int c = 0; c < t; c++){
        map<string, int> mp;
        map<string, int> md;
        int p = 0; string a;
        int n; cin >> n;
        vector<int> rp;
        for(int k = 0; k < n; k++){
            cin >> ws >> a;
            if(!mp.count(a)) mp[a] = p++;
            rp.push_back(mp[a]);
        }
        int d = 0; string b;
        int m; cin >> m;
        vector<Edge> dp;
        for(int k = 0; k < m; k++){
            cin >> ws >> a;
            if(!mp.count(a)) md[a] = d++;
            cin >> ws >> b;
            if(!mp.count(b)) mp[b] = p++;
            dp.push_back({md[a], mp[b]});
        }
        int x; cin >> x;
        vector<Edge> pp;
        for(int k = 0; k < x; k++){
            cin >> ws >> a;
            if(!mp.count(a)) mp[a] = p++;
            cin >> ws >> b; 
            if(!mp.count(b)) mp[b] = p++;
            pp.push_back({mp[a], mp[b]});
        }
        vector<vInt> edges(p, vInt());
        for(auto& e : pp) 
            edges[e.u].push_back(e.v);
        vector<vInt> adj(p, vInt(p));
        for(int w = 0; w < p; w++){
            queue<int> q; q.push(w);
            vector<bool> seen(p);
            seen[w] = true;
            while(!q.empty()){ 
                int u = q.front(); q.pop();
                for(auto& v : edges[u]){
                    if(seen[v]) continue;
                    seen[v] = true,
                    q.push(v);
                } adj[w][u] = 1;
            }
        }
        unique_ptr<Bipartite> bp;
        bp.reset(new Bipartite(d + p));
        for(int v = 0; v < p; v++)
            bp->SetRight(d + v);
        for(int u = 0; u < d; u++)
            bp->SetLeft(u);
        for(auto& e : dp)
            for(int k = 0; k < n; k++){
                if(!adj[e.v][rp[k]]) continue;
                bp->AddEdge(e.u, d + rp[k]);
            }
        if(c) cout << '\n';
        cout << d - bp->MaximumMatching(),
        cout << '\n';
    }
    return 0;
}