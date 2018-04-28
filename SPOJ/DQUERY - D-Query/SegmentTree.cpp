#include <iostream>
#include <memory>
#include <vector>
#include <map>

using std::make_shared;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::cin;
using std::map;

namespace Constants{

    const int S = -1;

};

using namespace Constants;

struct SegTree{

    typedef shared_ptr<SegTree> SharedTree;

    SharedTree right;
    SharedTree left;
    int l,r,v;

    SegTree(int L, int R): 
        l(L), left(), r(R), right(), v() {}

    int Build(){
        if(l == r) return ++v;
        auto h = (l + r) >> 1;
        left = make_shared<SegTree>(l, h++),
        right = make_shared<SegTree>(h, r);
        return v = left->Build() 
                + right->Build();
    }

    static void Update(SharedTree& t, 
        SharedTree& n, int x, int u = 0){
        if(t->Disjoint(x, x))
            return void(n = t);
        n = make_shared<SegTree>(*t);
        if(t->Contained(x, x)) 
            return void(n->v = u);
        Update(t->right, n->right, x),
        Update(t->left, n->left, x),
        n->v = n->right->v,
        n->v += n->left->v;
    }

    int Query(int a, int b){
        if(Disjoint(a, b)) return int();
        if(Contained(a, b)) return v;
        return left->Query(a, b)
            + right->Query(a, b);
    }

    bool Contained(int a, int b){
        return a <= l and r <= b;
    }

    bool Disjoint(int a, int b){
        return b < l or r < a;
    }

};

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); int n;
    while(cin >> n){
        vector<SegTree::SharedTree> st; st.resize(n),
        st.front() = make_shared<SegTree>(1, n);
        map<int,int> m; st.front()->Build();
        for(int k = 0; k < n; k++){
            int z; cin >> z;
            int p = S;
            if(m.count(z)) 
                p = m[z];
            m[z] = k;
            if(!k) continue;
            st[k] = st[k - 1];
            if(!++p) continue;
            SegTree::Update(st[k], st[k], p);
        }
        int q; cin >> q;
        for(int k = 0; k < q; k++){
            int l; cin >> l;
            int r; cin >> r;
            cout << st[r - 1]->Query(l, r),
            cout << '\n';
        }
    }
    return 0;
}