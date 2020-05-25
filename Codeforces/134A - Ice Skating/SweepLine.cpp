#include <algorithm>
#include <iostream>
#include <numeric>
#include <memory>
#include <vector>

using std::vector;
using std::pair;
using std::sort;
using std::cout;
using std::cin;

namespace Types{

    typedef std::pair<int,int> Drift;

};

using namespace Types;

namespace DataStructures{

    struct DisjointSets{

        vector<int> p;
        vector<int> r;
        int n;

        DisjointSets(int N): n(N), p(N), r(N) {
            std::iota(p.begin(), p.end(), int());
        }

        int Root(int u){
            if(u == p[u]) return p[u];
            return p[u] = Root(p[u]);
        }

        void Join(int u, int v){
            u = Root(u), v = Root(v);
            if(u == v) return; --n;
            if(r[u] < r[v]) p[u] = v;
            if(r[v] < r[u]) p[v] = u;
            if(r[u] == r[v]){
                p[u] = v, ++r[v];
            }
        }

        int ComponentCount(){
            return n;
        }

    };

};

namespace Solution{

    using DataStructures::DisjointSets;

    void SortByY(vector<Drift>& drifts, vector<int>& id){
        auto compare = [&drifts] (int i, int j) {
            return drifts[i].second < drifts[j].second;
        };
        sort(id.begin(), id.end(), compare);
    }

    void SortByX(vector<Drift>& drifts, vector<int>& id){
        auto compare = [&drifts] (int i, int j) {
            return drifts[i].first < drifts[j].first;
        };
        sort(id.begin(), id.end(), compare);
    }

    int MinDrifts(vector<Drift>& drifts){
        const int n = drifts.size();
        vector<int> id(n);
        std::iota(id.begin(), id.end(), int());
        std::unique_ptr<DisjointSets> ds;
        ds.reset(new DisjointSets(n));
        Solution::SortByX(drifts, id);
        for(int k = 1; k < n; k++){
            const auto& u = drifts[id[k - 1]];
            const auto& v = drifts[id[k]];
            if(u.first == v.first){
                ds->Join(id[k - 1], id[k]);
            }
        }
        Solution::SortByY(drifts, id);
        for(int k = 1; k < n; k++){
            const auto& u = drifts[id[k - 1]];
            const auto& v = drifts[id[k]];
            if(u.second == v.second){
                ds->Join(id[k - 1], id[k]);
            }
        }
        return ds->ComponentCount() - 1;
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        vector<Drift> drifts(n);
        for(auto& d : drifts){
            cin >> d.first >> d.second;
        }
        cout << Solution::MinDrifts(drifts),
        cout << '\n';
    }
    return 0;
}
