#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;
using std::cout;
using std::cin;

namespace Types{
    
    typedef long long int Long;

};

using namespace Types;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

namespace Graphs{

    template<class T>
    struct Tree{

        struct Edge{
            
            int v;
            T w;

            Edge(int V, T W): v(V), w(W) {}
    
        };

        typedef std::forward_list<Edge> List;
        
        vector<List> edges; 
        vector<int> stack;
        vector<bool> used;
        vector<bool> f;
        vector<T> path;
        vector<T> d;
        const int n;
        int x;

        Tree(int N): edges(N), f(N), n(N){}

        T MinEnergy(int r = int()){
            stack.resize(n), x = S; 
            path.assign(n, T());
            d.assign(n, T());
            FindLeaves(r);
        }  

        int FindLeaves(int u, int p = S){
            auto pointer = x, z = int();
            for(auto& e : edges[u]){
                if(e.v == p) continue;
                const auto& v = e.v;
                d[v] = d[u] + e.w;
                z += FindLeaves(v, u);
            }
            if(z == 0){
                if(used[u] = f[u]){ 
                    stack[x++] = u;
                }
                return used[u];
            } 
            used[u] = true;
            if(z == 1){ 
                return used[u];
            }
            while(++pointer < x){
                auto& v = stack[pointer];
                path[v] = d[v] - d[u];
            }
            return z;
        } 

        void AddEdge(int u, int v, T w){
            edges[u].emplace_front(v, w),
            edges[v].emplace_front(u, w);
        } 

        void SetFriend(int u){
            f[u] = true;
        }

    };

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int f,n;
    while(cin >> n >> f){
        
    }
    return 0;
}
