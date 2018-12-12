#include <iostream>
#include <string>
#include <vector>

using std::ios_base;
using std::string;
using std::vector;
using std::cout;
using std::cin;

namespace Strings{

    int Period(const string& s){
        int n = s.size(), v = 0;
        vector<int> fail(n, 0);
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
        return n - fail.back();
    }

};

using Strings::Period;

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int t; cin >> t;
    for(int k = 0; k < t; k++){
        if(k > 0) cout << '\n';
        string text;
        cin >> std::ws >> text;
        int p = Period(text);
        if(!(text.size() % p)){
            cout << p << '\n'; 
            continue;  
        }
        cout << text.size(),
        cout << '\n';
    }
    return 0;
}
