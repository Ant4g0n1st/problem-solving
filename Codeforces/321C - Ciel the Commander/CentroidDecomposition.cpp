#include <forward_list>
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

using std::forward_list;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::cout;
using std::cin;
using std::max;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct Tree{

    typedef forward_list<int> List;

    vector<List> edges;
    vector<int> parent;
    vector<bool> cut;
    vector<int> size;
    vector<int> tag;
    int n;

    Tree(int N): edges(N), n(N) {}

    void Decompose(int w = 0, int p = S){
        queue<int> q; q.push(w);
        auto min = Subtree(w);
        auto centroid = w;
        while(!q.empty()){
            auto u = q.front(); q.pop();
            auto s = size[w] - size[u];
            for(auto& v : edges[u]){
                if(cut[v]) continue;
                if(v != parent[u])
                    s = max(s, size[v]),
                    q.push(v);
            }
            if(s < min)
                centroid = u, 
                min = s;
        }
        cut[centroid] = true;
        if(p != S)
            tag[centroid] = tag[p] + 1;
        for(auto& v : edges[centroid])
            if(!cut[v])
                Decompose(v, centroid);
    }

    void CentroidDecomposition(){
        parent.resize(n), tag.resize(n),
        size.resize(n), cut.resize(n),
        Decompose();
    }

    int Subtree(int u, int p = S){
        parent[u] = p; auto s = 0;
        for(auto& v : edges[u]){
            if(cut[v]) continue;
            if(v == p) continue;
            s += Subtree(v, u);
        }
        return size[u] = ++s;
    }

    void AddEdge(int u, int v){
        edges[u].push_front(v),
        edges[v].push_front(u);
    }

    int GetTag(int u = 0){
        return tag[u];
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        unique_ptr<Tree> t;
        t.reset(new Tree(n));
        for(int k = 1; k < n; k++){
            int u; cin >> u, --u;
            int v; cin >> v, --v;
            t->AddEdge(u, v);
        }
        t->CentroidDecomposition(),
        cout << char('A' + t->GetTag());
        for(int u = 1; u < n; u++){
            char r = 'A' + t->GetTag(u);
            cout << ' ' << r;
        }
        cout << '\n';
    }
    return 0;
}