#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Graph{

    struct Edge{

        Edge* r;
        bool b;
        int v;

        Edge(int V, bool B = true): b(B), v(V) {}

        void SetReverse(Edge* e){ r = e; }

        bool Blocked(){ return b; }

        void Unblock(){ 
            b = r->b = false;
        }

    };

    typedef forward_list<Edge*> List;

    vector<List> edges;
    vector<int> m;
    vector<int> s;
    vector<int> t;
    int k,n,x;

    Graph(int N): edges(N), n(N) {}

    ~Graph(){
        for(int u = 0; u < n; u++)
            for(auto& e : edges[u])
                if(e) delete e;
    }

    void Color(int w){ s[w] = x;
        queue<int> q; q.push(w);
        while(!q.empty()){
            auto u = q.front();
            for(auto& e : edges[u]){
                if(e->Blocked())
                    continue;
                int v = e->v;
                if(s[v] != S)
                    continue;
                q.push(v),
                s[v] = x;
            }
            q.pop();
        }
    }

    void FindTrees(){
        m.resize(n), k = 0,
        t.resize(n), x = 0,
        s.resize(n, S);
        for(int u = 0; u < n; u++)
            if(!t[u]) Tarjan(u);
        for(int u = 0; u < n; u++){
            if(s[u] != S) continue;
            Color(u), ++x;
        }
    }

    void Tarjan(int u, int p = S){
        m[u] = t[u] = ++k;
        for(auto& e : edges[u]){
            auto& v = e->v;
            if(v == p) continue;
            if(!t[v]){ 
                Tarjan(v, u);
                if(t[u] < m[v])
                    e->Unblock();
                if(m[v] < m[u])
                    m[u] = m[v];
            }
            if(t[v] < m[u])
                m[u] = t[v];
        }
    }

    bool UniquePath(int u, int v){
        return s[u] == s[v];
    }

    void AddEdge(int u, int v){
        Edge* uv = new Edge(v);
        Edge* vu = new Edge(u);
        edges[u].push_front(uv),
        edges[v].push_front(vu),
        uv->SetReverse(vu),
        vu->SetReverse(uv);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int m,n;
    while(cin >> n >> m){ 
        int q; cin >> q;
        if(!n and !m and !q) continue;
        unique_ptr<Graph> g;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            g->AddEdge(u, v);
        }
        g->FindTrees();
        for(int k = 0; k < q; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            if(g->UniquePath(u, v))
                cout << 'Y';
            else
                cout << 'N';
            cout << '\n';
        }
        cout << "-\n";
    }
    return 0;
}