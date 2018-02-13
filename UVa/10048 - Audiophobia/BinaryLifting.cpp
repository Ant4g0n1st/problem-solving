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
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::sort;
using std::swap;
using std::cin;
using std::max;

typedef vector<int> vInt;

const int oo = numeric_limits<int>::max();
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

struct Tree{

    struct Edge{

        int v,w;

        Edge(int V, int W): v(V), w(W) {}

    };

    typedef forward_list<Edge> List;

    vector<List> edges;
    vector<vInt> m;
    vector<vInt> p;
    vector<int> d;
    int l,n;

    Tree(int N): d(N), edges(N), n(N) {
        for(l = 0; N; N >>= 1, l++);
    }

    void BinaryLifting(){
        m.assign(n, vInt(l, -oo)),
        p.assign(n, vInt(l, S));
        vector<bool> seen(n);
        for(int w = 0; w < n; w++){
            if(seen[w]) continue;
            seen[w] = true;
            queue<int> q; 
            q.push(w);
            while(!q.empty()){
                int u = q.front(); q.pop();
                for(auto& e : edges[u]){
                    if(seen[e.v]) continue;
                    d[e.v] = d[u] + 1,
                    seen[e.v] = true,
                    m[e.v][0] = e.w,
                    p[e.v][0] = u,
                    q.push(e.v);
                }
            }
        }
        for(int k = 1; k < l; k++)
            for(int u = 0; u < n; u++){
                if(p[u][k - 1] == S) continue;
                p[u][k] = p[p[u][k - 1]][k - 1],
                m[u][k] = m[p[u][k - 1]][k - 1];
                if(m[u][k] < m[u][k - 1])
                    m[u][k] = m[u][k - 1];
            }
    }

    int Lift(int u, int h, int& g){ g = -oo;
        for(int k = 0; h; h >>= 1, k++){
            if(p[u][k] == S) continue;
            if(!(h & 1)) continue;
            g = max(g, m[u][k]),
            u = p[u][k];
        }
        return u;
    }

    int Lift(int u, int h){
        for(int k = 0; h; h >>= 1, k++){
            if(!(h & 1)) continue;
            if(p[u][k] != S) u = p[u][k];
        }
        return u;
    }

    int LCA(int u, int v){
        if(d[u] < d[v]) swap(u, v);
        u = Lift(u, d[u] - d[v]);
        if(u == v) return v;
        for(int k = l - 1; k >= 0; k--)
            if(p[u][k] != p[v][k])
                u = p[u][k], v = p[v][k];
        return p[u][0];
    }

    int Heaviest(int u, int v){
        int mu,mv; int lca = LCA(u, v);
        Lift(u, d[u] - d[lca], mu),
        Lift(v, d[v] - d[lca], mv);
        return max(mu, mv);
    }

    void AddEdge(int u, int v, int w){
        edges[u].emplace_front(v, w),
        edges[v].emplace_front(u, w);
    }

};

struct Edge{

    int u,v,w;

    Edge(): u(), v(), w() {}

    Edge(int U, int V, int W): u(U), v(V), w(W) {}

    bool operator<(const Edge& other) const{
        return w < other.w;
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int c = 0; int n,m,q;
    while(cin >> n >> m >> q){ 
        if(!n and !m and !q) break;
        vector<Edge> edges(m);
        for(auto& e : edges)
            cin >> e.u, --e.u,
            cin >> e.v, --e.v,
            cin >> e.w;
        unique_ptr<DisjointSets> ds;
        sort(edges.begin(), edges.end());
        unique_ptr<Tree> t(new Tree(n));
        ds.reset(new DisjointSets(n));
        for(auto& e : edges){
            if(ds->Joined(e.u, e.v))
                continue;
            t->AddEdge(e.u, e.v, e.w),
            ds->Join(e.u, e.v);
        }
        t->BinaryLifting();
        if(c++) cout << '\n';
        cout << "Case #" << c << '\n';
        for(int k = 0; k < q; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            if(!ds->Joined(u, v)){
                cout << "no path\n";
                continue;
            }
            cout << t->Heaviest(u, v),
            cout << '\n';
        }
    }
    return 0;
}