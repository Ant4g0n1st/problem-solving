#include <iostream>
#include <string>
#include <array>

using std::ios_base;
using std::string;
using std::array;
using std::cout;
using std::swap;
using std::cin;

namespace Constants{

    const int N = 2e2 + 1;
    const int Z = '@';

};

using namespace Constants;

namespace Strings{

    array<int, N> fail;
    array<char, N> s;

    void Failure(int n){
        int v = fail.front() = int();
        for(int u = 1; u < n; u++){
            while(v > 0){
                if(s[u] == s[v]){
                    break;
                }
                v = fail[v - 1];
            }
            if(s[u] == s[v]) v++;
            fail[u] = v;
        }
    }

    int Longest(string& u, string& v){
        int m = int();
        for(const auto& c : v){
            s[m++] = c; 
        }
        s[m++] = Z;
        for(const auto& c : u){
            s[m++] = c; 
        }
        Failure(m);
        return fail[m - 1];
    }

};

using Strings::Longest;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int t; cin >> t;
    while(t--){
        int l; cin >> l;
        int n; cin >> n;
        int min = l; 
        string u,v;
        cin >> std::ws;
        cin >> v;
        for(int k = 1; k < n; k++){
            swap(u, v), cin >> v;
            min -= Longest(u, v);
            min = min + l;
        }
        cout << min << '\n';
    }
    return 0;
}
