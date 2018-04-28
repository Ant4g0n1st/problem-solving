#include <forward_list>
#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>
#include <deque>

using std::unique_ptr;
using std::reverse;
using std::vector;
using std::deque;
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

struct ConvexHull{

    struct Line{

        Long b,m;

        Line(Long M, Long B): b(B), m(M) {}

        Long Evaluate(Long x){
            return m * x + b;
        }

    };

    deque<Line> hull;

    ConvexHull(){}

    Long Query(Long x){
        while(hull.size() > 1){
            auto u = hull.begin(), v = u;
            auto g = (++v)->Evaluate(x);
            auto f = u->Evaluate(x);
            if(f <= g) break;
            hull.pop_front();
        }
        return hull.begin()->Evaluate(x);
    }

    void AddLine(Long m, Long b){ 
        Line line(m, b);
        while(hull.size() > 1)
            if(!Irrelevant(line))
                hull.pop_back();
            else break;
        if(hull.empty()){
            hull.push_back(line);
            return;
        }
        auto& z = hull.back(); 
        if(m == z.m and z.b <= b)
            return;
        hull.push_back(line);
    }

    bool Irrelevant(Line& z){
        auto p = hull.end();
        auto q = --p; q--;
        auto x = p->b - q->b;
        x *= z.m - p->m;
        auto y = q->m - p->m;
        y *= p->b - z.b;
        return x > y;
    }

};

struct Tree{

    struct Pair{

        Long w;
        int u;

        Pair(int U, Long W): u(U), w(W) {}

        bool operator<(const Pair& other) const{
            return w > other.w;
        }

    };

    struct Edge{

        Long w;
        int v;

        Edge(int V, Long W): v(V), w(W) {}

    };

    typedef std::forward_list<Edge> List;

    vector<Long> penalty;
    vector<int> parent;
    vector<List> edges;
    vector<Long> cost;
    vector<bool> cut;
    vector<int> size;
    vector<Long> dc;
    int n;

    Tree(int N): edges(N), n(N), penalty(N) {}

    void CentroidDecomposition(int w = 0){
        queue<int> q; q.push(w);
        auto min = Subtree(w);
        auto centroid = w;
        while(!q.empty()){
            auto u = q.front(); q.pop();
            auto s = size[w] - size[u];
            for(auto& e : edges[u]){
                if(cut[e.v]) continue;
                if(e.v == parent[u])
                    continue;
                s = max(s, size[e.v]),
                q.push(e.v);
            }
            if(s < min)
                centroid = u,
                min = s;
            dc[u] = oo;
        }
        cut[centroid] = true,
        dc[centroid] = 0,
        q.push(centroid);
        vector<Pair> z;
        ConvexHull h;
        while(!q.empty()){
            auto u = q.front(); q.pop();
            for(auto& e : edges[u]){
                auto v = e.v;
                if(cut[v]) continue;
                if(dc[u] + e.w < dc[v])
                    dc[v] = dc[u] + e.w,
                    q.push(v);
            }
            z.emplace_back(u, penalty[u]);
        }
        sort(z.begin(), z.end());
        for(auto& p : z)
            h.AddLine(p.w, dc[p.u]);
        reverse(z.begin(), z.end());
        for(auto& p : z){
            auto q = h.Query(p.w);
            if(dc[p.u] + q < cost[p.u])
                cost[p.u] = dc[p.u] + q;
        }
        for(auto& e : edges[centroid]){
            if(cut[e.v]) continue;
            CentroidDecomposition(e.v);
        }
    }

    void AddEdge(int u, int v, Long w){
        edges[u].emplace_front(v, w),
        edges[v].emplace_front(u, w);
    }

    void SetPenalty(int u, Long w){
        penalty[u] = w;
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

    Long SmallestCost(){
        cost.resize(n, oo), 
        parent.resize(n),
        size.resize(n), 
        cut.resize(n), 
        dc.resize(n), 
        CentroidDecomposition();
        Long smallest = 0;
        for(auto& k : cost) 
            smallest += k;
        return smallest;
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        unique_ptr<Tree> t;
        t.reset(new Tree(n));
        for(int u = 0; u < n; u++){
            int w; cin >> w;
            t->SetPenalty(u, w);
        }
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            Long w; cin >> w;
            t->AddEdge(u, v, w);
        }
        cout << t->SmallestCost(),
        cout << '\n';
    }
    return 0;
}