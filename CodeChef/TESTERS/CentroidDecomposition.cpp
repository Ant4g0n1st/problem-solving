#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>
#include <array>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;

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
            if(x <= l and r <= r)
                return v = u + v;
            return v = left->Update(x, u)
                    + right->Update(x, u);
        }

        T Query(int u, int v){ 
            LazyPropagation();
            if(v < l or r < u) return T();
            if(u <= l and r <= v) 
                return v;
            return left->Query(u, v)
                + right->Query(u, v);

        }

        T Build(){
            if(l == r) return v = T();
            int h = (l + r) >> 1;
            left = new SegTree(l, h++),
            right = new SegTree(h, r);
            return left->Build()
                + right->Build();
        }

        void LazyPropagation(){
            if(!erase) return;
            if(right) right->Erase();
            if(left) left->Erase();
            erase = false, v = T();
        }
        
        void Erase(){
            erase = true;
        }

    };

    struct Pair{
    
        Long v;
        int p; 
        
        Pair(int P, Long V): p(P), v(V) {}
        
        Pair(): p(), v() {}

        Pair operator+(const Pair& other) const{
            return { p + other.p, v + other.v };
        }

    }; 

    typedef SegTree<Pair> PairTree;

};

namespace Graph{

    using DataStructures::PairTree;

    typedef std::forward_list<int> List;

    struct Tree{
        
        unique_ptr<PairTree> dp;
        vector<List> edges;
        vector<int> parent;
        vector<int> size;
        vector<bool> cut;
        vector<int> dc;
        int n;

        Tree(int N): edges(N), n(N) {
            dp.reset(new PairTree(-n, n));
        }

        Long BeautifulPaths(){}

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

int main(){
    //std::ios_base::sync_with_stdio(0),
    //cout.tie(0), cin.tie(0);
    Math::FillSieve();
    return 0;
}
