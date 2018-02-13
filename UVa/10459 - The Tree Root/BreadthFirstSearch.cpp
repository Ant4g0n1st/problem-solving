#include <forward_list>
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::fill;
using std::swap;
using std::cin;

typedef forward_list<int> List;

const int oo = numeric_limits<int>::max();
const int S = -1;

struct Graph{

    vector<List> edges;
    vector<int> w;
    int n;

    Graph(int N): edges(N), n(N), w(N) {}

    void Roots(List& best, List& worst){ 
        queue<int> q; int a,b;
        vector<int> deg(n);
        for(auto& adj : edges)
            for(auto& u : adj) ++deg[u];
        for(int u = 0; u < n; u++)
            if(--deg[u] == 0) q.push(u);
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u]){
                if(--deg[v] == 0)
                    w[v] = w[u] + 1,
                    q.push(v);
            } swap(a, b), a = u;
        }
        if(w[a] > w[b]) b = S;
        fill(w.begin(), w.end(), oo);
        q.push(a), w[a] = 0;
        if(b != S) w[b] = 0,
            q.push(b); 
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(w[u] + 1 < w[v])
                    w[v] = w[u] + 1,
                    q.push(v);
        }
        int m = -oo;
        for(int u = n - 1; u >= 0; u--){
            if(m < w[u]) 
                worst.clear(), m = w[u];
            if(m == w[u])
                worst.push_front(u);
            if(!w[u]) 
                best.push_front(u);
        }
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
        for(int u = 0; u < n; u++){
            int m; cin >> m;
            for(int k = 0; k < m; k++){
                int v; cin >> v, --v;
                g->AddEdge(u, v);
            }
        }
        List best,worst;
        g->Roots(best, worst);
        cout << "Best Roots  :";
        for(auto& u : best)
            cout << ' ' << ++u;
        cout << '\n';
        cout << "Worst Roots :";
        for(auto& u : worst)
            cout << ' ' << ++u;
        cout << '\n';
    }
    return 0;
}