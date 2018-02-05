#include <iostream>
#include <vector>
#include <memory>
#include <queue>

using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;

typedef vector<int> vInt;

struct Graph{

    vector<vInt> edges;
    int n;

    Graph(int N): n(N), edges(N) {}

    vector<int> TopoSort(){
        queue<int> q; int m = n;
        vector<int> order(n);
        vector<int> in(n);
        for(auto& adj : edges)
            for(auto& u : adj) ++in[u];
        for(int u = 0; u < n; u++)
            if(!in[u]) q.push(u);
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(--in[v] == 0)
                    q.push(v);
            order[--m] = u;
        }
        return order;
    }

    void AddEdge(int u, int v){
        edges[u].push_back(v);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        if(!n and !m) break;
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            g->AddEdge(v, u);
        }
        auto order = g->TopoSort();
        cout << ++order.front();
        for(int k = 1; k < n; k++)
            cout << ' ' << ++order[k];
        cout << '\n';
    }
    return 0;
}