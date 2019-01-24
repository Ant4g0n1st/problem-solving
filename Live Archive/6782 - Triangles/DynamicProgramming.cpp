#include <functional>
#include <algorithm>
#include <iostream>
#include <vector>

using std::function;
using std::vector;
using std::cout;
using std::sort;
using std::cin;
using std::max;

namespace Constants{

    const int L = 3;

};

using namespace Constants;

namespace DataStructures{

    struct Triangle{
        
        vector<int> sides;
        float scale;

        Triangle(): scale(), sides(L) {}

        bool operator<(const Triangle& other) const{
            return scale < other.scale;
        }
        
        int& operator[](int k){
            return sides[k];
        }

    };

};

using DataStructures::Triangle;

namespace DynamicProgramming{

    int LongestChain(vector<Triangle>& t){
        sort(t.begin(), t.end());
        function<int(int,int)> f;
        vector<vector<int>> dp;
        const int n = t.size();
        dp.assign(n, vector<int>(L + 1));
        f = [&] (int l, int s){
            if(dp[l][s]) return dp[l][s];
            auto& v = dp[l][s] = 1;
            for(int r = l + 1; r < n; r++){
                if(!(t[l] < t[r])) continue;
                for(int x = 0; x < L; x++){
                    if(x == s) continue;
                    for(int y = 0; y < L; y++){
                        if(t[l][x] != t[r][y]){
                            continue;
                        }
                        int u = f(r, y);
                        v = max(v, ++u);
                    }
                }
            }
            return v;
        };
        auto maxChain = int();
        for(int k = 0; k < n; k++){
            maxChain = max(maxChain, f(k, L));
        }
        return maxChain;
    }

};

using DynamicProgramming::LongestChain;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        vector<Triangle> triangles(n);
        for(auto& t : triangles){
            for(auto& side : t.sides){
                cin >> side; 
            }
            cin >> t.scale;
        }
        cout << LongestChain(triangles),
        cout << '\n';
    }
    return 0;
}
