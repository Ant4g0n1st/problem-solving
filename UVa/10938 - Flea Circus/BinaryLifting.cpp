#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::swap;
using std::cin;

typedef vector<int> vInt;

const int S = -1;

struct Edge{

    int u,v;

    Edge(): u(), v() {}

    Edge(int U): Edge(U, U) {}

    Edge(int U, int V): u(U), v(V) {}

};

struct Tree{

    vector<vInt> edges;
    vector<vInt> p;
    vector<int> d;
    int l,n;

    Tree(int N): d(N), edges(N), n(N) {
        for(l = 0; N; N >>= 1, l++);
    }

    void BinaryLifting(int root = 0){
        queue<int> q; q.push(root);
        p.assign(n, vInt(l, S));
        vector<bool> seen(n);
        seen[root] = true;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& v : edges[u]){
                if(seen[v]) continue;
                d[v] = d[u] + 1,
                seen[v] = true,
                p[v][0] = u,
                q.push(v);
            }
        }
        for(int k = 1; k < l; k++)
            for(int u = 0; u < n; u++){
                if(p[u][k - 1] == S) continue;
                p[u][k] = p[p[u][k - 1]][k - 1];
            }
    }

    int PathLength(int u, int v){
        return d[u] + d[v] - 2 * d[LCA(u, v)];
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

    Edge Meet(int u, int v){
        if(d[u] < d[v]) swap(u, v);
        auto s = PathLength(u, v);
        auto a = Lift(u, s >> 1);
        if(!(s & 1)) return {a};
        return {a, p[a][0]};
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
        t->BinaryLifting();
        int q; cin >> q;
        for(int k = 0; k < q; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            auto e = t->Meet(u, v);
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
