#include <forward_list>
#include <iostream>
#include <bitset>

using std::cout;
using std::cin;

namespace Constants{

    const unsigned int S = 1 << 16;

};

using namespace Constants;

namespace Math{

    typedef long long int Long;

    std::forward_list<int> primes;
    std::bitset<S> sieve;

    void FillPrimes(){ sieve.reset();
        for(int k = 4; k < S; k += 2) sieve.set(k);
        for(int u = 3; u * u < S; u += 2){
            if(sieve[u]) continue;
            for(int v = u * u; v < S; v += 2 * u){
                if(!sieve[v]) sieve.set(v);
            }
        }
        for(int k = S - 1; k > 2; k -= 2){
            if(!sieve[k]) primes.emplace_front(k);
        }
        primes.emplace_front(2);
    }

    bool Factovisors(Long u, Long v){
        if(u == 0) return false;
        if(v == 0) return true;
        for(const Long p : primes){
            Long x = 0, y = 0;
            while(u % p == 0){
                u /= p, x++;
            }
            Long q = p;
            while(q <= v){
                y += v / q,
                q = p * q;
            }
            if(y < x) 
                return false;
            if(u == 1) break;
        }
        return true;
    }

};

using Math::Long;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); Long m,n;
    Math::FillPrimes();
    while(cin >> n >> m){
        cout << m << ' ';
        if(!Math::Factovisors(m, n)){
            cout << "does not",
            cout << " divide ";
        }else{
            cout << "divides ";
        }
        cout << n << "!\n";
    }
    return 0;
}
