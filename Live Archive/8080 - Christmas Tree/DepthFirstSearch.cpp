#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <set>

using std::forward_list;
using std::unique_ptr;
using std::ios_base;
using std::multiset;
using std::vector;
using std::cout;
using std::cin;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Tree{

    typedef forward_list<int> List;

    vector<List> edges;
    vector<int> deg;
    vector<int> s;
    int k,n;

    Tree(int N): deg(N), edges(N), n(N) {}

    int MaxLeft(int k, int root = 0){
        s.resize(n), ++deg[root],
        this->k = k, Size(root);
        return s[root];
    }

    int Size(int u, int p = S){
        if(--deg[u] < k) 
            return ++s[u];
        multiset<int> m;
        for(auto& v : edges[u]){
            if(v == p) continue;
            s[u] += Size(v, u),
            m.insert(s[v]);
        }
        for(auto& size : m)
            if(deg[u]-- > k)
                s[u] -= size;
        return ++s[u];
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u),
        ++deg[u], ++deg[v];
    }

};

int main(){
    ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0);
    int z; cin >> z;
    for(int y = 1; y <= z; y++){
        unique_ptr<Tree> t;
        int k,n; cin >> n >> k;
        t.reset(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            t->AddEdge(u, v);
        }
        cout << "Case " << y << ": ",
        cout << t->MaxLeft(k),
        cout << '\n';
    }
    return 0;
}