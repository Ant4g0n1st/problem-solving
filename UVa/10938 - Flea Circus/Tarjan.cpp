#include <iostream>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::swap;
using std::cin;

typedef vector<int> vInt;

const int S = -1;

struct DisjointSets{

    vector<int> p;
    vector<int> r;

    DisjointSets(int N): p(N), r(N) {
        for(int u = 0; u < N; u++) p[u] = u;
    }

    void Join(int u, int v){
        int Ru = Root(u), Rv = Root(v);
        if(r[Ru] < r[Rv]) p[Ru] = Rv;
        if(r[Rv] < r[Ru]) p[Rv] = Ru;
        if(r[Ru] == r[Rv]){
            if(Rv == Ru) return;
            p[Ru] = Rv, ++r[Rv];
        }
    }

    bool Joined(int u, int v){
        return Root(u) == Root(v);
    }

    int Root(int u){
        if(u == p[u]) return p[u];
        return p[u] = Root(p[u]);
    }

};

struct Edge{

    int u,v;

    Edge(): u(), v() {}

    Edge(int U): Edge(U, U) {}

    Edge(int U, int V): u(U), v(V) {}

};

struct Tree{

    struct Query{

        int i,v;

        Query(int V, int I): i(I), v(V) {}

    };

    typedef vector<Query> vQuery;

    unique_ptr<DisjointSets> ds;
    vector<vQuery> queries;
    vector<vInt> edges;
    vector<bool> seen;
    vector<int> up;
    vector<int> a;
    vector<int> d;
    vector<int> r;
    int n,q;

    Tree(int N): a(N), d(N), edges(N), 
        n(N), q(S), queries(N), seen(N), up(N) {}

    int GetResult(int qi = 0){ return r[qi]; }

    void ProcessQueries(int root = 0){
        for(int u = 0; u < n; u++) a[u] = u;
        ds.reset(new DisjointSets(n)),
        r.resize(++q), Tarjan(root);
    }

    void Tarjan(int u, int p = S){
        for(auto& v : edges[u]){
            if(v == p) continue; 
            up[v] = u,
            d[v] = d[u] + 1,
            Tarjan(v, u), 
            ds->Join(v, u),
            a[ds->Root(u)] = u;
        } seen[u] = true;
        for(auto& qu : queries[u]){
            if(!seen[qu.v]) continue;
            r[qu.i] = a[ds->Root(qu.v)];
        }
    }

    int Length(int u, int v, int lca){
        return d[u] + d[v] - 2 * d[lca];
    }

    void AddQuery(int u, int v){ ++q,
        queries[u].push_back({v, q}),
        queries[v].push_back({u, q});
    }

    Edge Meet(int u, int v, int lca){
        if(d[u] < d[v]) swap(u, v);
        auto s = Length(u, v, lca);
        auto w = GoUp(u, s >> 1);
        if(!(s & 1)) return {w};
        return {w, up[w]};
    }

    int GoUp(int u, int s){
        for(; s; s--) u = up[u];
        return u;
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
        unique_ptr<Tree> t(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            t->AddEdge(u, v);
        }
        int q; cin >> q;
        vector<Edge> queries(q); 
        for(auto& e : queries){
            cin >> e.u, --e.u,
            cin >> e.v, --e.v,
            t->AddQuery(e.u, e.v);
        }
        t->ProcessQueries();
        for(int k = 0; k < q; k++){
            auto u = queries[k].u;
            auto v = queries[k].v;
            auto w = t->GetResult(k);
            auto e = t->Meet(u, v, w);
            if(e.u == e.v){
                cout << "The fleas meet at ",
                cout << ++e.u << ".\n";
                continue;
            }
            if(e.u > e.v) swap(e.u, e.v);
            cout << "The fleas jump forever",
            cout << " between " << ++e.u,
            cout << " and " << ++e.v,
            cout << ".\n";
        }
    }
    return 0;
}