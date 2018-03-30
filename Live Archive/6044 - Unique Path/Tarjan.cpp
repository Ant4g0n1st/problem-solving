#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::forward_list;
using std::unique_ptr;
using std::ios_base;
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

        Edge(int V, bool B = false): v(V), b(B) {}

        void SetReverse(Edge* e){ r = e; }

        bool Blocked(){ return b; }

        void Block(){ 
            b = true, r->b = true;
        }

    };

    typedef forward_list<Edge*> List;

    vector<List> edges;
    vector<bool> seen;
    vector<int> size;
    vector<int> m;
    vector<int> s;
    vector<int> t;
    int k,n,z;

    Graph(int N): edges(N), n(N) {}

    ~Graph(){
        for(int u = 0; u < n; u++)
            for(auto& e : edges[u])
                if(e) delete e;
    }

    int Paths(int w){ int x = 0;
        queue<int> q; q.push(w);
        seen[s[w]] = true, ++x;
        while(!q.empty()){
            auto u = q.front();
            for(auto& e : edges[u]){
                auto& v = e->v;
                if(seen[s[v]])
                    continue;
                seen[s[v]] = true,
                q.push(v), ++x;
            }
            q.pop();
        }
        return (x * (x - 1)) >> 1;
    }

    void Color(int w){ s[w] = z;
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
                s[v] = z;
            } 
            q.pop();
        }   
    }

    int UniquePaths(){
        m.resize(n), z = 0,
        t.resize(n), k = 0,
        s.assign(n, S);
        for(int u = 0; u < n; u++)
            if(!t[u]) Tarjan(u);
        for(int u = 0; u < n; u++){
            if(s[u] != S) continue; 
            Color(u), z++;
        }
        size.resize(z);
        for(auto& c : s) ++size[c];
        seen.resize(z);
        for(int u = 0; u < n; u++)
            if(--size[s[u]])
                seen[s[u]] = true;
        auto paths = 0;
        for(int u = 0; u < n; u++)
            if(!seen[s[u]])
                paths += Paths(u);
        return paths;
    }

    void Tarjan(int u, int p = S){
        m[u] = t[u] = ++k;
        auto q = 0;
        for(auto& e : edges[u]){
            auto& v = e->v;
            if(p == v and !q++)
                continue;
            if(!t[v]){ 
                Tarjan(v, u);
                if(t[u] < m[v])
                    e->Block();
                if(m[v] < m[u])
                    m[u] = m[v];
            }
            if(t[v] < m[u])
                m[u] = t[v];
        }
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
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 1; y <= z; y++){
        unique_ptr<Graph> g;
        int n; cin >> n;
        int m; cin >> m;
        g.reset(new Graph(n));
        for(int k = 0; k < m; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            g->AddEdge(u, v);
        }
        cout << "Case #" << y << ": ",
        cout << g->UniquePaths(),
        cout << '\n';
    }
    return 0;
}