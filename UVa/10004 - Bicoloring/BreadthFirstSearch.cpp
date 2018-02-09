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

struct Graph{

    vector<vInt> edges;
    vector<bool> seen;
    vector<int> c;
    int n;

    Graph(int N): c(N), seen(N), edges(N), n(N) {}

    bool IsBipartite(int s = 0){
        queue<int> q; q.push(s);
        seen[s] = true;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& v : edges[u]){
                if(seen[v]){
                    if(c[u] != c[v])
                        continue;
                    return false;
                }
                c[v] = c[u] ^ 1,
                seen[v] = true,
                q.push(v);
            }
        }
        return true;
    }

    void AddEdge(int u, int v){
        edges[u].push_back(v),
        edges[v].push_back(u);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n and n){
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        int m; cin >> m;
        for(int k = 0; k < m; k++){
            int u,v; cin >> u >> v;
            g->AddEdge(u, v);
        }
        if(g->IsBipartite()){
            cout << "BICOLORABLE.\n";
            continue;
        }
        cout << "NOT BICOLORABLE.\n";
    }
    return 0;
}