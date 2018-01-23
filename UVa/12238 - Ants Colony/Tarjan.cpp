#include <iostream>
#include <vector>
#include <memory>

using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::cout;
using std::cin;

typedef long long int Long;

const int S = -1;

struct DisjointSets{

    vector<int> p;
    vector<int> r;
    int n;

    DisjointSets(int N): n(N), p(N), r(N) {
        for(int u = 0; u < N; u++) p[u] = u;
    }

    void Join(int u, int v){
        int Ru = Root(u), Rv = Root(v);
        if(r[Ru] < r[Rv]) p[Ru] = Rv;
        if(r[Rv] < r[Ru]) p[Rv] = Ru;
        if(r[Ru] == r[Rv]){
            if(Rv == Ru) return;
            p[Rv] = Ru, ++r[Ru];
        }
    }

    int Root(int u){
        if(u == p[u]) return p[u];
        return p[u] = Root(p[u]);
    }

    bool Joined(int u, int v){
        return Root(u) == Root(v);
    }

};

struct Tree{

    struct Query{

        int v,i;

        Query(): v(), i() {}

        Query(int V, int I): v(V), i(I) {}

    };

    typedef vector<Query> vQuery;

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef vector<Edge> vEdge;

    unique_ptr<DisjointSets> ds;
    vector<vQuery> queries;
    vector<vEdge> edges;
    vector<bool> seen;
    vector<Long> s;
    vector<Long> r;
    vector<int> a;
    int n,q;

    Tree(int N): n(N), a(N), s(N), seen(N), q(S),
        queries(N, vQuery()), edges(N, vEdge()), ds() {}

    void ProcessQueries(int root = 0){
        for(int u = 0; u < n; u++) a[u] = u;
        ds.reset(new DisjointSets(n)),
        r.resize(++q), Tarjan(root);
    }

    void Tarjan(int u, int p = S){
        for(auto& e : edges[u]){
            if(e.v == p) continue;
            s[e.v] = s[u] + e.w, 
            Tarjan(e.v, u),
            ds->Join(u, e.v), 
            a[ds->Root(u)] = u;
        } seen[u] = true;
        for(auto& qu : queries[u]){
            if(!seen[qu.v]) continue;
            int lca = a[ds->Root(qu.v)];
            r[qu.i] = Weight(u, qu.v, lca);
        }
    }

    Long Weight(int u, int v, int lca){
        return s[u] + s[v] - 2LL * s[lca];
    }

    void AddEdge(int u, int v, Long w){
        edges[u].push_back({v, w}),
        edges[v].push_back({u, w});
    }

    void AddQuery(int u, int v){ ++q,
        queries[u].push_back({v, q}),
        queries[v].push_back({u, q});
    }

    Long GetResult(int qi = 0){
        return r[qi];
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n and n){
        unique_ptr<Tree> t(new Tree(n));
        for(int u = 1; u < n; u++){
            int v; Long w;
            cin >> v >> w,
            t->AddEdge(u, v, w);
        } 
        int q; cin >> q;
        for(int k = 0; k < q; k++){
            int u,v; cin >> u >> v,
            t->AddQuery(u, v);
        }
        t->ProcessQueries();
        cout << t->GetResult();
        for(int k = 1; k < q; k++)
            cout << ' ' << t->GetResult(k);
        cout << '\n';
    }
    return 0;
}