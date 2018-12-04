#include <iostream>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;

namespace Math{

    typedef long long int Long;

    struct ModLong{

        static Long m;

        Long k;

        ModLong(Long K): k(K % m) {}

        ModLong(): k() {}

        void ModInverse(Long a, Long b, ModLong& x, ModLong& y){
            if(b == 0){
                x = ModLong(1LL), y = ModLong();
                return;
            }
            ModInverse(b, a % b, x, y);
            auto z = ModLong(a / b);
            z = x - y * z,
            x = y, y = z;
        }

        ModLong ModInverse(){
            ModLong x,y;
            ModInverse(k, m, x, y);
            return x;
        }
    
        ModLong operator-(const ModLong& other) const{
            return (*this) + ModLong(m - other.k);
        }

        ModLong operator+(const ModLong& other) const{
            return ModLong(k + other.k);
        }

        ModLong operator*(const ModLong& other) const{
            return ModLong(k * other.k);
        }

        ModLong operator/(ModLong& other) const{
            return (*this) * other.ModInverse();
        }

    };

    Long ModLong::m = Long();

};

using namespace Math;

namespace DataStructures{

    template<class T>
    struct SegTree{

        SegTree *right;
        SegTree *left;
        int l,r;
        T v;

        SegTree(int L, int R): l(L), r(R), v(),
            left(nullptr), right(nullptr) {}
        
        ~SegTree(){
            if(right) delete right;
            if(left) delete left;
        }

        T Update(int x, T u){
            if(x < l or r < x) return v;
            if(x <= l and r <= x)
                return v = u;
            return v = left->Update(x, u)
                    + right->Update(x, u);
        }

        T Query(int a, int b){
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
            return left->Build()
                + right->Build();
        }

    };

    typedef SegTree<ModLong> ModTree;

};

using namespace DataStructures;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); Long b,p;
    while(cin >> b >> p){
        int n,q; cin >> n >> q;
        if(b + p + n + q == 0) 
            break;
        unique_ptr<ModTree> st;
        ModLong::m = p;
        vector<ModLong> pow(n);
        pow.front().k = 1LL;
        ModLong z(b);
        for(int k = 1; k < n; k++){
            pow[k] = pow[k - 1] * z;
        }
        st.reset(new ModTree(1, n));
        st->Build();
        for(int k = 0; k < q; k++){
            char opt; cin >> opt;
            if(opt == 'E'){
                int x; Long u; 
                cin >> x >> u;
                st->Update(x, pow[n - x] * ModLong(u));
            }else{
                int l; cin >> l;
                int r; cin >> r;
                auto v = st->Query(l, r) / pow[n - r];
                cout << v.k << '\n';
            }
        }
        cout << "-\n";
    }
    return 0;
}
