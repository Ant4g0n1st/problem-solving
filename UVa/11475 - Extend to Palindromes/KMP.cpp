#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const string S = "@";

};

using namespace Constants;

namespace Strings{

    vector<int> Failure(const string& w){
        int n = w.size(), v = 0;
        vector<int> fail(n, 0);
        for(int u = 1; u < n; u++){
            while(v > 0){
                if(w[u] == w[v]){
                    break;
                }
                v = fail[v - 1];
            }
            if(w[u] == w[v]) v++;
            fail[u] = v;
        }
        return fail;
    }

    void ShortestPalindrome(string& w){
        string r(w.rbegin(), w.rend()); 
        r.append(S), r.append(w);
        auto fail = Failure(r);
        auto k = fail.back();
        auto it = r.begin();
        auto n = w.size();
        w.append(it + k, it + n);
    }

};

using Strings::ShortestPalindrome;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); string w;
    while(cin >> std::ws >> w){
        ShortestPalindrome(w);
        cout << w << '\n';
    }
    return 0;
}
