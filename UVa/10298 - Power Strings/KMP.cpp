#include <iostream>
#include <string>
#include <vector>

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
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); string s;
    while(cin >> std::ws >> s){
        if(s == ".") break;
        int p = Period(s);
        int n = s.size();
        if(n % p == 0){
            cout << n / p;
        }else{
            cout << n / n;
        }
        cout << '\n'; 
    }
    return 0;
}
