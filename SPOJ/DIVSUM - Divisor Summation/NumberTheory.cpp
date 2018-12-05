#include <iostream>
#include <array>

using std::array;
using std::cout;
using std::cin;

namespace Constants{

    const int S = 5e5 + 1;

};

using namespace Constants;

namespace Math{

    typedef long long int Long;

    array<int, S> sieve;

    void FillSieve(){
        sieve.fill(int());
        for(int k = 4; k < S; k += 2) sieve[k] = 2;
        for(int a = 3; a * a < S; a += 2){
            if(sieve[a]) continue;
            for(int b = a * a; b < S; b += 2 * a){
                if(!sieve[b]) sieve[b] = a;
            }
        }
    }
    
    Long Pow(Long n, Long k){
        Long p = 1LL;
        for(; k; k >>= 1){
            if(k & 1) p *= n;
            n = n * n;
        }
        return p;
    }

    Long Geometric(Long n, Long k){
        return (Pow(n, ++k) - 1LL) / (n - 1LL);
    }

    Long DivisorSum(int n){
        Long sum = 1LL; 
        while(sieve[n]){
            int p = sieve[n], k = 0;
            while(n % p == 0){
                n /= p, k++;
            }
            sum *= Geometric(p, k);
        }
        if(n > 1LL) sum *= n + 1;
        return sum;
    }

};

using Math::DivisorSum;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    Math::FillSieve();
    int t; cin >> t;
    for(int k = 0; k < t; k++){
        Math::Long n; cin >> n;
        auto s = DivisorSum(n);
        cout << s - n << '\n';
    }
    return 0;
}
