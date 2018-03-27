#include <forward_list>
#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>

using std::forward_list;
using std::stringstream;
using std::unique_ptr;
using std::getline;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::ws;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Graph{

    typedef forward_list<int> List;

    vector<List> edges;
    vector<bool> cut;
    vector<int> t;
    vector<int> m;
    int k,n;

    Graph(int N): edges(N), n(N) {}

    int CutVertices(){
        m.resize(n), t.resize(n),
        cut.resize(n), k = 0;
        int vertices = 0;
        for(int u = 0; u < n; u++){
            if(t[u]) continue;
            cut[u] = Tarjan(u) > 1;
        }
        for(int u = 0; u < n; u++)
            if(cut[u]) ++vertices;
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
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n and n){
        unique_ptr<Graph> g;
        string in; cin >> ws;
        g.reset(new Graph(n));
        while(getline(cin, in)){
            stringstream st(in);
            int u,v; st >> u;
            if(!u--) break;
            while(st >> v)
                g->AddEdge(u, --v);
            cin >> ws;
        }
        cout << g->CutVertices(),
        cout << '\n';
    }
    return 0;
}