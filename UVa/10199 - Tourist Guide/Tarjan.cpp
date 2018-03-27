#include <unordered_map>
#include <forward_list>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::unordered_map;
using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::string;
using std::vector;
using std::cout;
using std::sort;
using std::cin;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Graph{

    typedef forward_list<int> List;

    vector<List> edges;
    vector<bool> cut;
    vector<int> m;
    vector<int> t;
    int k,n;

    Graph(int N): edges(N), n(N) {}

    vector<int> CutVertices(){
        m.resize(n), t.resize(n),
        cut.resize(n), k = 0;
        vector<int> vertices; 
        for(int u = 0; u < n; u++){
            if(t[u]) continue;
            cut[u] = Tarjan(u) > 1;
        }
        for(int u = 0; u < n; u++){
            if(!cut[u]) continue;
            vertices.push_back(u);
        }
        return vertices;
    }

    int Tarjan(int u, int p = S){
        auto f = 0, q = 0;
        m[u] = t[u] = ++k;
        for(auto& v : edges[u]){
            if(v == p and !q++)
                continue;
            if(!t[v]){
                ++f, Tarjan(v, u);
                if(t[u] <= m[v])
                    cut[u] = true;
                if(m[v] < m[u])
                    m[u] = m[v];
            }
            if(t[v] < m[u])
                m[u] = t[v];
        }
        return f;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int n,z; z = 0;
    while(cin >> n and n){
        unordered_map<string,int> x;
        unique_ptr<Graph> g; int p;
        vector<string> w(n); p = 0;
        for(auto& s : w)
            cin >> s, x[s] = p++;
        sort(w.begin(), w.end());
        for(int k = 0; k < n; k++)
            x[w[k]] = k;
        g.reset(new Graph(n));
        if(z) cout << '\n';
        int m; cin >> m;
        for(int k = 0; k < m; k++){
            string u; cin >> u;
            string v; cin >> v;
            g->AddEdge(x[u], x[v]);
        }
        auto cut = g->CutVertices();
        cout << "City map #" << ++z,
        cout << ": " << cut.size(),
        cout << " camera(s) found",
        cout << '\n';
        for(auto& u : cut)
            cout << w[u] << '\n';
    }
    return 0;
}