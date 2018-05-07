#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;

struct Graph{

    typedef std::forward_list<int> List;

    vector<List> edges;
    vector<int> deg;
    int n;

    Graph(int N): deg(N), edges(N), n(N) {}

    bool Brexit(int s, int t){
        vector<bool> seen(n);
        vector<int> left(deg);
        seen[s] = true;
        queue<int> q;
        q.push(s);
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u]){
                if(seen[v]) 
                    continue; --left[v];
                if(2 * left[v] > deg[v])
                    continue;
                seen[v] = true, 
                q.push(v);
            }
        }
        return seen[t];
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u),
        ++deg[u], ++deg[v];
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){
        int t; cin >> t, --t;
        int s; cin >> s, --s;
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            g->AddEdge(u, v);
        }
        if(g->Brexit(s, t))
            cout << "leave";
        else cout << "stay";
        cout << '\n';
    }
    return 0;
}