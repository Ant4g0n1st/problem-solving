#include <forward_list>
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::upper_bound;
using std::vector;
using std::queue;
using std::cout;
using std::sort;
using std::cin;
using std::max;

typedef long long int Long;

namespace Constants{

    const Long oo = std::numeric_limits<Long>::max();
    const int S = -1;

};

using namespace Constants;

struct Tree{

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef std::forward_list<Edge> List;

    struct Level{

        std::unique_ptr<vector<Long>> v;
        int c,r;
        Long d;

        Level(): c(), d(), r(), v() {}

        int Query(Long k){
            auto ub = upper_bound(v->begin(), v->end(), k);
            return ub - v->begin(); 
        }

        void Reset(){
            v.reset(new vector<Long>());
        }

        void Sort(){
            sort(v->begin(), v->end());
        }

        void Add(Long k){
            v->push_back(k);
        }

    };

    vector<vector<Level>> levels;
    vector<List> edges;
    vector<int> parent;
    vector<int> size;
    vector<bool> cut;
    vector<Long> dc;
    int n;

    Tree(int N): edges(N), n(N) {}

    void Decompose(int w = 0){
        auto min = Subtree(w), z = w;
        queue<int> q; q.push(w);
        while(!q.empty()){
            auto u = q.front(); q.pop();
            auto s = size[w] - size[u];
            for(const auto& e : edges[u]){
                if(cut[e.v]) continue;
                if(e.v != parent[u])
                    s = max(s, size[e.v]),
                    q.push(e.v);
            }
            if(s < min) min = s, z = u;
            levels[u].emplace_back(),
            dc[u] = oo;
        }
        auto& l = levels[z].back();
        cut[l.c = l.r = z] = true,
        dc[z] = l.d = 0, l.Reset();
        for(const auto& x : edges[z]){
            if(cut[x.v]) continue;
            auto& r = levels[x.v].back();
            r.Reset(), q.push(x.v), 
            dc[x.v] = x.w;
            while(!q.empty()){
                auto u = q.front(); q.pop();
                for(const auto& e : edges[u]){
                    if(cut[e.v]) continue;
                    if(dc[u] + e.w < dc[e.v])
                        dc[e.v] = dc[u] + e.w,
                        q.push(e.v);
                }
                auto& s = levels[u].back();
                l.Add(dc[u]), r.Add(dc[u]),
                s.r = x.v, s.c = z,
                s.d = dc[u];
            }
            r.Sort();
        }
        l.Add(l.d), l.Sort();
        for(const auto& e : edges[z])
            if(!cut[e.v]) Decompose(e.v);
    }

    int Query(int u, Long m){
        int s = levels[u].size();
        int paths = 0;
        for(int k = 0; k < s; k++){
            auto& l = levels[u][k];
            if(m < l.d) continue;
            auto d = m - l.d;
            paths += levels[l.c][k].Query(d);
            if(u == l.c) continue;
            paths -= levels[l.r][k].Query(d);
        }
        return paths;
    }

    void CentroidDecomposition(){
        levels.resize(n), cut.resize(n),
        parent.resize(n), dc.resize(n),
        size.resize(n), Decompose();
    }

    void AddEdge(int u, int v, Long w){
        edges[u].emplace_front(v, w),
        edges[v].emplace_front(u, w);
    }

    int Subtree(int u, int p = S){
        parent[u] = p; auto s = 0;
        for(auto& e : edges[u]){
            if(cut[e.v]) continue;
            if(e.v == p) continue;
            s += Subtree(e.v, u);
        }
        return size[u] = ++s;
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n,q;
    while(cin >> n >> q){
        std::unique_ptr<Tree> t;
        t.reset(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            Long w; cin >> w;
            t->AddEdge(u, v, w);
        }
        t->CentroidDecomposition();
        for(int k = 0; k < q; k++){
            int u; cin >> u, --u;
            Long l; cin >> l;
            cout << t->Query(u, l),
            cout << '\n';
        }
    }
    return 0;
}