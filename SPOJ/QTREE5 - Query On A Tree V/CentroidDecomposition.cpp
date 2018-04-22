#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>
#include <set>

using std::forward_list;
using std::unique_ptr;
using std::multiset;
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
    typedef multiset<Int> mInt;

    struct Pair{

        int v,w;

        Pair(int V, int W): v(V), w(W) {}

    };

    typedef forward_list<Pair> fPair;

    vector<mInt> subtree;
    vector<bool> color;
    vector<fInt> edges;
    vector<int> parent;
    vector<int> size;
    vector<bool> cut;
    vector<fPair> up;
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
        dc[centroid] = 0,
        q.push(centroid);
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& v : edges[u]){
                if(cut[v]) continue;
                if(dc[u] + 1 < dc[v])
                    dc[v] = dc[u] + 1,
                    q.push(v);
            }
            Pair x(centroid, dc[u]);
            up[u].push_front(x);
        }
        for(auto& v : edges[centroid])
            if(!cut[v])
                Decompose(v, centroid);
        return centroid;
    }

    void CentroidDecomposition(){
        subtree.resize(n), dc.resize(n),
        parent.resize(n), up.resize(n),
        color.resize(n), cut.resize(n),
        size.resize(n), Decompose();
    }

    void PaintItWhite(int u){
        for(auto& p : up[u])
            subtree[p.v].insert(p.w);
    }

    int ClosestWhite(int u){
        auto min = oo;
        for(auto& p : up[u]){
            auto& s = subtree[p.v];
            if(s.empty()) continue;
            auto w = *s.begin();
            if(w + p.w < min)
                min = w + p.w;
        }
        return min < oo ? min : S;
    }

    void PaintItBlack(int u){
        for(auto& p : up[u]){
            auto& s = subtree[p.v];
            auto f = s.find(p.w);
            if(f == s.end())
                continue;
            s.erase(f);
        }       
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

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

    void ToggleColor(int u){
        if(color[u])
            PaintItBlack(u);
        else
            PaintItWhite(u);
        color[u] = !color[u];
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        unique_ptr<Tree> t;
        t.reset(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            t->AddEdge(u, v);
        }
        t->CentroidDecomposition();
        int q; cin >> q;
        for(int k = 0; k < q; k++){
            int z; cin >> z;
            int u; cin >> u, --u;
            if(!z){
                t->ToggleColor(u);
                continue;
            }
            cout << t->ClosestWhite(u),
            cout << '\n';
        }
    }
    return 0;
}