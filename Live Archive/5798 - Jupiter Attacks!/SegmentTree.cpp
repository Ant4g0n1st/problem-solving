#include <iostream>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;

typedef long long int Long;

namespace ModMath{

    static Long m;

    Long Multiply(Long a, Long b){
        return ((a % m) * (b % m)) % m;
    } 

    Long Add(Long a, Long b){
        return ((a % m) + (b % m)) % m;
    } 

    Long Subtract(Long a, Long b){
        return Add(a, m - (b % m));
    }

    void Bezout(Long a, Long b, Long &x, Long &y){
        if(b == 0){
            x = 1LL, y = 0;
            return;
        }
        Bezout(b, a % b, x, y);
        Long z = Multiply(a / b, y);
        z = Subtract(x, z),
        x = y, y = z;
    }

    Long ModInverse(Long a){
        Long x,y;
        Bezout(a, m, x, y);
        return x;
    }
    
    Long Divide(Long a, Long b){
        return Multiply(a, ModInverse(b));
    }

    void SetMod(Long mod){
        m = mod;
    }

};

using namespace ModMath;

struct SegTree{

    SegTree *left,*right;
    int l,r;
    Long v;

    SegTree(int L, int R): l(L), r(R), v() {}

    Long Query(int a, int b){
        if(b < l or r < a) return Long(); 
        if(l <= a and b <= r) return v;
        return Add(left->Query(a, b),
                right->Query(a, b));
    }

    Long Update(int x, int u){
        if(x < l or r < x) return v; 
        if(l <= x and x <= r)
            return v = u;
        return Add(left->Update(x, u),
                right->Update(x, u));
    }

    Long Build(){
        if(l == r) return v = Long();
        int h = (l + r) >> 1;
        left = new SegTree(l, h++),
        right = new SegTree(h, r);
        return left->Build()
            + right->Build();
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    Long b,n,p,q;
    while(cin >> b >> p >> n >> q){
        if(!b and !p and !n and !q) break;
        unique_ptr<SegTree> st;
        vector<Long> pow(n);
        ModMath::SetMod(p);
        pow.front() = 1LL;
        for(int k = 1; k < n; k++){
            pow[k] = Multiply(pow[k - 1], b);
        }
        st.reset(new SegTree(1, n));
    }
    return 0;
}
