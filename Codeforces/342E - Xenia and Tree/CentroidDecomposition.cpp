#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::max;

typedef unsigned int Int;

namespace Constants{

    const Int oo = std::numeric_limits<int>::max();
    const int S = -1;

};

using namespace Constants;

struct Tree{
    
    typedef forward_list<int> fInt;

    struct Pair{

        int v,w;

        Pair(int V, int W): v(V), w(W) {}

        Pair(): v(S), w(oo) {}

    };

    typedef forward_list<Pair> fPair;
    
    vector<fInt> edges;
    vector<int> parent;
    vector<Int> close;
    vector<int> size;
    vector<bool> cut;
    vector<fPair> z;
    vector<int> dc;
    int n;

    Tree(int N): edges(N), n(N) {}

    int Decompose(int w = 0, int p = S){
        queue<int> q; q.push(w);
        auto min = Subtree(w);
        auto centroid = w;
        while(!q.empty()){
            auto u = q.front(); q.pop();
            auto s = size[w] - size[u];
            for(auto& v : edges[u]){
                if(cut[v]) continue;
                if(v != parent[u])
                    s = max(s, size[v]),
                    q.push(v);
            }
            if(s < min)
                centroid = u, 
                min = s;
            dc[u] = oo;
        }
        cut[centroid] = true,
        q.push(centroid), 
        dc[centroid] = 0;
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u]){
                if(cut[v]) continue;
                if(dc[u] + 1 < dc[v])
                    dc[v] = dc[u] + 1,
                    q.push(v);
            }
            Pair x(centroid, dc[u]);
            z[u].push_front(x);
        }
        for(auto& v : edges[centroid])
            if(!cut[v])
                Decompose(v, centroid);
        return centroid;
    }

    int ClosestRed(int u){
        auto min = oo;
        for(auto& p : z[u])
            if(close[p.v] + p.w < min)
                min = close[p.v] + p.w;
        return min;
    }

    void CentroidDecomposition(){
        parent.resize(n), z.resize(n),
        size.resize(n), dc.resize(n),
        cut.resize(n), Decompose(),
        close.resize(n, oo);
    }

    int Subtree(int u, int p = S){
        parent[u] = p; auto s = 0;
        for(auto& v : edges[u]){
            if(cut[v]) continue;
            if(v == p) continue;
            s += Subtree(v, u);
        }
        return size[u] = ++s;
    }

    void PaintRed(int u = 0){
        for(auto& p : z[u])
            if(p.w < close[p.v])
                close[p.v] = p.w;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n,q;
    while(cin >> n >> q){
        unique_ptr<Tree> t;
        t.reset(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            t->AddEdge(u, v);
        }
        t->CentroidDecomposition(),
        t->PaintRed();
        for(int k = 0; k < q; k++){
            int z; cin >> z, --z;
            int u; cin >> u, --u;
            if(!z){
                t->PaintRed(u);
                continue;
            }
            cout << t->ClosestRed(u),
            cout << '\n';
        }
    }
    return 0;
}