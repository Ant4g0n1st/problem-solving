#include <forward_list>
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using std::forward_list;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::swap;
using std::cin;

namespace Constants{

    const int oo = std::numeric_limits<int>::max();
    const int S = -1;

};

using namespace Constants;

struct Tree{

    typedef forward_list<int> List;
    typedef vector<int> vInt; 

    vector<List> edges;
    vector<vInt> p;
    vector<int> d;
    int l,n;

    Tree(int N): edges(N), l(), n(N){
        for(l = 0; N; N >>= 1, l++);
    }

    int Lift(int u, int h){
        for(int k = 0; h; h >>= 1, k++){
            if(!(h & 1)) continue;
            if(p[u][k] != S)
                u = p[u][k];
        }
        return u;
    }

    void BinaryLifting(int root = 0){
        queue<int> q; q.push(root),
        p.assign(n, vInt(l, S));
        d.assign(n, oo), d[root] = 0;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto& v : edges[u])
                if(d[u] + 1 < d[v])
                    p[v].front() = u,
                    d[v] = d[u] + 1,
                    q.push(v);
        }
        for(int k = 1; k < l; k++)
            for(int u = 0; u < n; u++){
                auto& w = p[u][k - 1];
                if(w == S) continue;
                p[u][k] = p[w][k - 1];
            }
    }

    int LCA(int u, int v){
        if(d[u] < d[v]) swap(u, v);
        u = Lift(u, d[u] - d[v]);
        if(u == v) return v;
        for(int k = l - 1; k >= 0; k--)
            if(p[u][k] != p[v][k])
                u = p[u][k], 
                v = p[v][k];
        return p[u].front();
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 1; y <= z; y++){
        int n; cin >> n;
        Tree* t = new Tree(n);
        for(int u = 0; u < n; u++){
            int m; cin >> m;
            for(int k = 0; k < m; k++){
                int v; cin >> v, --v;
                t->AddEdge(u, v);
            }
        }
        t->BinaryLifting();
        int q; cin >> q;
        cout << "Case " << y << ":\n";
        for(int k = 0; k < q; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            int lca = t->LCA(u, v);
            cout << ++lca << '\n';
        }
    }
    return 0;
}