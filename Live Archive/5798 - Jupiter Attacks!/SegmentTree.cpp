#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::cin;

typedef long long int Long;

namespace Math{

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

using Math::Multiply;
using Math::Subtract;
using Math::Add;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    Long b,n,p,q;
    while(cin >> b >> p >> n >> q){
        if(!b and !p and !n and !q) break;
        vector<Long> pow(n);
        pow.front() = 1LL;
        Math::SetMod(p);
        for(int k = 1; k < n; k++){
            pow[k] = Multiply(pow[k - 1], b);
        }
    }
    return 0;
}
