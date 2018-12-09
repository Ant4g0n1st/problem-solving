#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <queue>
#include <array>

using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::max;

namespace Constants{

    const int oo = std::numeric_limits<int>::max();
    const int S = 1e6 + 5e5 + 1;
    const int P = -1;

};

using namespace Constants;

namespace Math{

    typedef long long int Long;    

    std::array<int, S> sieve;

    void FillSieve(){
        sieve.fill(int());
        for(int k = 4; k < S; k += 2) sieve[k] = 2;
        for(int u = 3; u * u < S; u += 2){
            if(sieve[u]) continue;
            for(int v = u * u; v < S; v += 2 * u){
                if(!sieve[v]) sieve[v] = u;
            }
        }
    }

    bool IsBeautiful(int v){
        while(sieve[v]){
            int p = sieve[v], k = 0;
            while(v % p == 0){
                v /= p, k++;
            }
            if((p & 1) and (k & 1)){
                return false;
            }
        }
        return v <= 2;
    }

};

using Math::Long;

namespace DataStructures{

    template<class T>
    struct SegTree{

        SegTree *right;
        SegTree *left;
        bool erase;
        int l,r;
        T v;

        SegTree(int L, int R): erase(false), 
            left(nullptr), right(nullptr),
            l(L), r(R), v() {}

        ~SegTree(){
            if(right) delete right;
            if(left) delete left;
        }

        T Update(int x, T u){ 
            LazyPropagation();
            if(x < l or r < x) return v;
            if(x <= l and r <= x)
                return v = u + v;
            return v = left->Update(x, u)
                    + right->Update(x, u);
        }

        T Query(int a, int b){ 
            LazyPropagation();
            if(b < l or r < a) return T();
            if(a <= l and r <= b) 
                return v;
            return left->Query(a, b)
                + right->Query(a, b);

        }

        T Build(){
            if(l == r) return v = T();
            int h = (l + r) >> 1;
            left = new SegTree(l, h++),
            right = new SegTree(h, r);
            return v = left->Build()
                    + right->Build();
        }

        void LazyPropagation(){
            if(erase == false) return;
            if(right) right->Erase();
            if(left) left->Erase();
            erase = false, v = T();
        }
        
        void Erase(){
            erase = true;
        }

    };

    struct Pair{
    
        Long p,v;
        
        Pair(Long V, Long P = 1LL): p(P), v(V) {}
        
        Pair(): p(), v() {}

        Pair operator+(const Pair& other) const{
            return { v + other.v, p + other.p };
        }

    }; 

    typedef SegTree<Pair> PairTree;

};

namespace Graphs{

    using DataStructures::PairTree;

    typedef std::forward_list<int> List;

    struct Tree{
        
        unique_ptr<PairTree> dp;
        vector<List> edges;
        vector<int> parent;
        vector<int> size;
        vector<bool> cut;
        vector<Long> dc;
        vector<int> wc;
        vector<int> w;
        int n;

        Tree(int N): edges(N), n(N), w(N) {}

        Long Decompose(int z = int()){
            int min = Subtree(z), c = z;
            queue<int> q; q.push(z);
            while(!q.empty()){
                auto u = q.front(); q.pop();
                auto s = size[z] - size[u];
                for(auto& v : edges[u]){
                    if(cut[v]) continue;
                    if(v != parent[u]){
                        s = max(s, size[v]),
                        q.push(v);
                    }
                }
                if(s < min){
                    min = s,
                    c = u;
                }
            }
            dp->Update(w[c], { 1LL });
            Long sp = Long();
            cut[c] = true;
            for(auto& v : edges[c]){
               if(cut[v]) continue;
                wc[v] = w[v];
                dc[v] = 1;
                sp += Paths(v);
                wc[c] = w[c];
                dc[c] = 1;
                wc[v] = wc[c] + w[v];
                dc[v] = dc[c] + 1;
                AddPaths(v);
            }
            dp->Erase();
            for(auto& v : edges[c]){
                if(!cut[v]){
                    sp += Decompose(v);
                }
            }
            if(w[c] > 0) sp++;
            return sp;
        }

        Long InterestingPaths(){
            dp.reset(new PairTree(-n, n)),
            parent.resize(n), dp->Build(),
            size.resize(n), dc.resize(n),
            cut.resize(n), wc.resize(n);
            return Decompose(); 
        }

        Long Paths(int u, int p = P){
            Long sp = Long();
            for(auto& v : edges[u]){
                if(cut[v]) continue;
                if(v == p) continue;
                wc[v] = wc[u] + w[v];
                dc[v] = dc[u] + 1;
                sp += Paths(v, u);
            }
            auto q = dp->Query(-wc[u], n);
            sp += (q.v + dc[u] * q.p);
            return sp;
        }

        void SetWeight(int u, int weight){
            w[u] = weight;
        }

        void AddPaths(int u, int p = P){
            for(auto& v : edges[u]){
                if(cut[v]) continue;
                if(v == p) continue;
                dc[v] = dc[u] + 1;
                AddPaths(v, u);
            }
            dp->Update(wc[u], { dc[u] });
        }

        int Subtree(int u, int p = P){
            parent[u] = p; int s = 0;
            for(auto& v : edges[u]){
                if(cut[v]) continue;
                if(v == p) continue;
                s += Subtree(v, u);
            }
            return size[u] = ++s; 
        }

        void AddEdge(int u, int v){
            edges[u].emplace_front(v),
            edges[v].emplace_front(u);
        }

    };

};

using Graphs::Tree;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    unique_ptr<Tree> t;
    Math::FillSieve();
    int n; cin >> n;
    t.reset(new Tree(n));
    for(int k = 1; k < n; k++){
        int u; cin >> u, --u;
        int v; cin >> v, --v;
        t->AddEdge(u, v); 
    }
    for(int u = 0; u < n; u++){
        int w; cin >> w;
        if(Math::IsBeautiful(w)){
            t->SetWeight(u, +1);
        }else{
            t->SetWeight(u, -1);
        }
    }
    cout << t->InterestingPaths(),
    cout << '\n';
    return 0;
}
