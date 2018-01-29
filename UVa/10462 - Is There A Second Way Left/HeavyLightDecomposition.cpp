#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>

using std::numeric_limits;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::queue;
using std::cout;
using std::sort;
using std::swap;
using std::cin;
using std::max;
using std::min;

typedef long long int Long;
typedef vector<bool> vBool;

const int oo = numeric_limits<int>::max();
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
            p[Ru] = Rv, ++r[Rv];
        } --n;
    }

    int Root(int u){
        if(u == p[u]) return p[u];
        return p[u] = Root(p[u]); 
    }

    bool Joined(int u, int v){
        return Root(u) == Root(v);
    }

    int SetCount(){ return n; }

};

template<class T>
struct SegTree{

    SegTree *left, *right;
    int l,r;
    T v;

    SegTree(int L, int R): v(), 
        l(L), left(NULL), r(R), right(NULL) {}

    T Query(int a, int b){
        if(r < a or b < l) return T();
        if(a <= l and r <= b) return v;
        return left->Query(a, b)
            + right->Query(a, b);
    }

    T Build(vector<T>& Vo){
        if(l == r) return v = Vo[r];
        int h = (l + r) >> 1;
        left = new SegTree(l, h),
        right = new SegTree(h + 1, r);
        return v = left->Build(Vo)
                + right->Build(Vo);
    }

    ~SegTree(){
        if(right) delete right;
        if(left) delete left;
    }

};

struct MaxInt{

    int v; 

    MaxInt(): v(-oo) {}

    MaxInt(int V): v(V) {}

    MaxInt operator+(const MaxInt& other) const{
        return MaxInt(max(v, other.v));
    }

};

typedef SegTree<MaxInt> MaxTree;
typedef vector<MaxInt> vMaxInt;

struct Tree{

    struct Edge{

        int v,w;

        Edge(int V, int W): v(V), w(W) {}

    };

    typedef vector<Edge> vEdge;

    vector<MaxTree*> st;
    vector<vEdge> edges;
    vector<int> level;
    vector<int> super;
    vector<int> size;
    vector<int> idx;
    vector<int> top;
    vector<int> up;
    int chain,n;

    Tree(int N): chain(), edges(N), idx(N), 
        level(N), n(N), size(N), super(N), up(N) {}

    int Heaviest(int u, int v){ MaxInt m,q; 
        if(level[u] < level[v]) swap(u, v);
        while(level[u] > level[v])
            q = Query(idx[u], 
                idx[top[super[u]]], super[u]),
            m = m + q, u = up[top[super[u]]];
        while(super[u] != super[v])
            q = Query(idx[u], 
                idx[top[super[u]]], super[u]),
            m = m + q, u = up[top[super[u]]], 
            q = Query(idx[v], 
                idx[top[super[v]]], super[v]),
            m = m + q, v = up[top[super[v]]];
        if(u == v) return m.v;
        if(idx[u] < idx[v]) swap(u, v);
        q = Query(idx[v], idx[u] - 1, super[v]);
        return (m + q).v;
    }

    void HeavyLight(int root = 0){
        Build(root); vBool seen(n);
        queue<int> q; q.push(root);
        top.assign(chain, S);
        seen[root] = true;
        st.resize(chain);
        while(!q.empty()){
            int u = q.front(); q.pop();
            if(top[super[u]] == S) 
                top[super[u]] = u;
            for(auto& e : edges[u]){
                if(seen[e.v]) continue;
                if(super[e.v] != super[u])
                    level[e.v] = level[u] + 1;
                else level[e.v] = level[u];
                seen[e.v] = true, q.push(e.v); 
            }
        }
        vector<vMaxInt> w(chain);
        for(int k = 0; k < chain; k++)
            w[k].resize(idx[top[k]] + 1);
        for(int u = 0; u < n; u++)
            for(auto& e : edges[u]){ 
                auto& v = e.v;
                if(u != up[v]) continue;
                w[super[v]][idx[v]].v = e.w;
            }
        for(int k = 0; k < chain; k++){
            auto& i = idx[top[k]];
            st[k] = new MaxTree(0, i),
            st[k]->Build(w[k]);
        }
    }

    void AddEdge(int u, int v, int w){
        edges[u].emplace_back(v, w),
        edges[v].emplace_back(u, w);
    }

    MaxInt Query(int a, int b, int c){
        return st[c]->Query(a, b);
    }

    int Build(int u, int p = S){
        up[u] = p; int sub = 0;
        for(auto& e : edges[u]){
            if(e.v == p) continue;
            sub += Build(e.v, u);
        }
        int m = S, s = S;
        for(auto& e : edges[u]){
            if(e.v == p) continue;
            if(m < size[e.v]) 
                m = size[s = e.v];
        }
        if(s != S)
            super[u] = super[s],
            idx[u] = idx[s] + 1;
        else super[u] = chain++; 
        return size[u] = ++sub;
    }

    ~Tree(){
        for(auto& t : st) 
            if(t) delete t;
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
    int t; cin >> t;
    for(int c = 1; c <= t; c++){
        int n,m; cin >> n >> m;
        vector<Edge> edges(m);
        vector<bool> used(m);
        for(auto& e : edges)
            cin >> e.u, --e.u,
            cin >> e.v, --e.v,
            cin >> e.w;
        unique_ptr<DisjointSets> ds;
        Long mst = 0; int taken = 0; 
        sort(edges.begin(), edges.end());
        unique_ptr<Tree> t(new Tree(n));
        ds.reset(new DisjointSets(n));
        for(int k = 0; k < m; k++){ 
            auto& e = edges[k];
            if(ds->Joined(e.u, e.v)) 
                continue; used[k] = true,
            t->AddEdge(e.u, e.v, e.w),
            mst += e.w, ++taken,
            ds->Join(e.u, e.v);
        }
        cout << "Case #" << c << " : ";
        if(ds->SetCount() > 1){
            cout << "No way\n";
            continue;
        }
        if(taken == m){
            cout << "No second way\n";
            continue;
        }
        t->HeavyLight();
        Long best = oo;
        for(int k = 0; k < m; k++){
            if(used[k]) continue;
            auto& e = edges[k];
            Long d = e.w - t->Heaviest(e.u, e.v);
            best = min(best, mst + d);
        }
        cout << best << '\n';
    }
    return 0;
}