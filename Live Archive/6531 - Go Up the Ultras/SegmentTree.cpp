#include <forward_list>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using std::numeric_limits;
using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const int oo = numeric_limits<int>::max();
    const int D = 150000;
    const int S = -1;

};

using namespace Constants;

template<class T>
struct SegTree{

    SegTree *left,*right;
    int l,r;
    T v;

    SegTree(int L, int R): 
        l(L), left(), r(R), right(), v() {}

    ~SegTree(){
        if(right) delete right;
        if(left) delete left;
    }

    int First(int a, int b, T& k){
        if(a > r or l > b) 
            return S;
        if(l == r){
            if(k < v) 
                return r;
            return S;
        }
        auto q = S;
        if(k < left->v)
            q = left->First(a, b, k);
        if(q != S) 
            return q;
        if(k < right->v)
            q = right->First(a, b, k);
        return q;
    }

    T Build(vector<T>& beg){
        if(l == r) return v = beg[r];
        auto h = (r + l) >> 1;
        left = new SegTree(l, h),
        right = new SegTree(++h, r);
        return v = left->Build(beg)
                + right->Build(beg);
    }

    int Last(int a, int b, T& k){
        if(a > r or l > b) 
            return S;
        if(l == r){
            if(k < v) 
                return r;
            return S;
        }
        auto q = S;
        if(k < right->v)
            q = right->Last(a, b, k);
        if(q != S) 
            return q;
        if(k < left->v)
            q = left->Last(a, b, k);
        return q;
    }

    T Query(int a, int b){
        if(a > r or l > b) 
            return T();
        if(a <= l and r <= b)
            return v;
        return left->Query(a, b)
            + right->Query(a, b);
    }

};

struct Pair{

    int max,min;

    Pair(int A, int B): min(A), max(B) {}

    Pair(): max(-oo), min(oo) {}

    Pair operator+(const Pair& other) const{
        return {
            std::min(min, other.min),
            std::max(max, other.max)
        };
    }

    bool operator<(const Pair& other) const{
        return max < other.max;
    }

};

typedef SegTree<Pair> PairTree;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        vector<Pair> peak(n);
        int m = n - 1;
        for(auto& p : peak){
            int k; cin >> k;
            p.min = k,
            p.max = k;
        }
        forward_list<int> ultras;
        unique_ptr<PairTree> st;
        PairTree* pt;
        pt = new PairTree(0, n),
        pt->Build(peak), 
        st.reset(pt);
        for(int k = m; k > 0; k--){ 
            auto& p = peak[k]; 
            bool ultra = true;
            auto r = st->First(k + 1, n, p);
            if(r != S){ Pair q;
                q = st->Query(k + 1, r);
                if(p.min - q.min < D)
                    ultra = false;
            }else ultra &= (p.min >= D);
            auto l = st->Last(0, k - 1, p);
            if(l != S){ Pair q;
                q = st->Query(l, k - 1);
                if(p.min - q.min < D)
                    ultra = false;
            }else ultra &= (p.min >= D);
            if(ultra)
                ultras.push_front(k);
        }
        cout << ++ultras.front(),
        ultras.pop_front();
        for(auto& u : ultras)
            cout << ' ' << ++u;
        cout << '\n';
    }
    return 0;
}