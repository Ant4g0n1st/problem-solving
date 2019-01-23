#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::sort;
using std::cin;
using std::max;

namespace Constants{

    const float Z = 1e3;
    const int L = 3;

};

using namespace Constants;

namespace DataStructures{

    struct Triangle{
        
        vector<int> sides;
        int scale;

        Triangle(): scale(), sides(L) {}

        bool operator==(const Triangle& other) const{
            return scale == other.scale;
        }

        bool operator<(const Triangle& other) const{
            return scale < other.scale;
        }

    };

};

using DataStructures::Triangle;

namespace DynamicProgramming{

    int LongestChain(vector<Triangle>& v){
        sort(v.begin(), v.end());
        vector<vector<int>> dp;
        const int n = v.size();
        auto maxChain = 1;
        dp.assign(n, vector<int>(L + 1, 1));
        for(int l = n - 1; l >= 0; l--){
            const auto& p = v[l];
            for(int r = l + 1; r < n; r++){
                const auto& q = v[r];
                if(p == q) continue;
                for(int x = 0; x < L; x++){
                    auto& a = dp[l][(x + 1) % L];
                    auto& b = dp[l][(x + 2) % L];
                    for(int y = 0; y < L; y++){
                        if(p.sides[x] == q.sides[y]){
                            a = max(a, dp[r][y] + 1);
                            b = max(b, dp[r][y] + 1);
                            dp[l][L] = max(dp[l][L], dp[r][y] + 1);
                        }
                    }
                }
            }
            for(int x = 0; x <= L; x++){
                if(maxChain < dp[l][x]){
                    maxChain = dp[l][x];
                }
            }
        }
/*
        for(const auto& t : v){
            for(const auto& s : t.sides){
                cout << s << ' ';
            }
            cout << t.scale << std::endl;
        }
        cout << std::endl;
        for(int l = 0; l < L; l++){
            for(int r = 0; r < n; r++){
                cout << dp[r][l] << ' '; 
            }
            cout << std::endl;
        }
        cout << std::endl;
*/
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
            float s; cin >> s, s *= Z;
            t.scale = int(s);
        }
        cout << LongestChain(triangles),
        cout << '\n';
    }
    return 0;
}
