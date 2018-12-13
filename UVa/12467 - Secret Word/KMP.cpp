#include <iostream>
#include <string>
#include <array>

using std::ios_base;
using std::string;
using std::array;
using std::cout;
using std::cin;

namespace Constants{

    const int N = 2e6 + 1;
    const char Z = '@';

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

    string SecretWord(const string& w){
        int m = int(), n = w.size();
        for(int k = 0; k < n; k++){
            s[m++] = w[k];
        }
        s[m++] = Z;
        for(int k = 0; k < n; k++){
            s[m++] = w[n - k - 1];
        }
        Failure(m);
        auto u = int(), v = int();
        for(int k = 1; k <= n; k++){
            if(v < fail[n + k]){
                v = fail[n + k],
                u = n + k;
            }
        }
        string secret;
        for(int k = 0; k < v; k++){
            secret.push_back(s[u - k]);
        }
        return secret;
    }

};

using Strings::SecretWord;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int t; cin >> t;
    for(int k = 0; k < t; k++){
        string s;
        cin >> std::ws;
        cin >> s;
        cout << SecretWord(s),
        cout << '\n';
    }
    return 0;
}
