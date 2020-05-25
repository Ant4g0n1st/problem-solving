#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;

typedef long long int Long;

namespace Constants{

    const Long oo = std::numeric_limits<Long>::max();

};

using namespace Constants;

struct MaxLong{

    Long v;

    MaxLong(Long V): v(V) {}

    MaxLong(): v(-oo) {}

    void Reset(Long u = -oo){
        return void(v = u);
    }

    bool IsNeutral(){
        return !(v > -oo);
    }

    MaxLong operator+(const MaxLong& other) const{
        return { other.v < v ? v : other.v };
    }

};

template<class T>
struct SegTree{

    SegTree *right;
    SegTree *left;
    int l,r;
    T v,z;

    SegTree(int L, int R): z(),
        l(L), left(), r(R), right(), v() {}

    ~SegTree(){
        if(right) delete right;
        if(left) delete left;
    }

    T Update(int a, int b, T u){
        if(Disjoint(a, b)) return Value();
        if(Contained(a, b)){ z = u;
            return Value();
        }
        Propagate();
        return v = left->Update(a, b, u)
                + right->Update(a, b, u);
    }

    T Query(int a, int b){
        if(Disjoint(a, b)) return T();
        if(Contained(a, b)) 
            return Value();
        Propagate();
        return left->Query(a, b)
            + right->Query(a, b);
    }

    T Build(vector<T>& u){
        if(l == r) return v = u[r];
        auto h = (l + r) >> 1;
        left = new SegTree(l, h++),
        right = new SegTree(h, r);
        return v = left->Build(u)
                + right->Build(u);
    }

    bool Contained(int a, int b){
        return a <= l and r <= b;
    }

    bool Disjoint(int a, int b){
        return b < l or r < a;
    }

    void Propagate(){
        if(l == r) return;
        if(z.IsNeutral()) 
            return;
        left->z = v = z,
        right->z = z,
        z.Reset();
    }

    T Value(){
        if(z.IsNeutral())
            return v;
        return z;
    }

};

typedef SegTree<MaxLong> MaxTree;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        vector<MaxLong> stairs(n--);
        unique_ptr<MaxTree> st;
        for(auto& k : stairs) 
            cin >> k.v;
        st.reset(new MaxTree(0, n)),
        st->Build(stairs);
        int q; cin >> q;
        for(int k = 0; k < q; k++){
            int w; cin >> w, --w;
            MaxLong h,u; cin >> h.v;
            u = st->Query(0, w),
            cout << u.v << '\n',
            u.Reset(u.v + h.v),
            st->Update(0, w, u);
        }
    }
    return 0;
}