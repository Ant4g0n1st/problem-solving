#include <iostream>
#include <memory>
#include <vector>
#include <cmath>

using std::vector;
using std::ceil;
using std::cout;
using std::log2;
using std::cin;

namespace DataStructures{

    template<class T>
    struct SegTree{

        struct Node{

            Node *left, *right;
            bool operation;
            int l,r;
            T v;

            Node(): l(), left(nullptr), operation(), r(), right(nullptr), v() {}

            void UpdateOperation(){
                operation = !(left->operation) and !(right->operation);
            }

            void InitChildren(Node* lChild, Node* rChild){
                int h = (l + r) >> 1;
                left = lChild;
                left->r = h++;
                left->l = l;
                right = rChild;
                right->l = h;
                right->r = r;
            }

            void UpdateValue(){
                v = Combine(left->v, right->v);
            }

            void Update(int a, int b, T u){
                if(Disjoint(a, b)) return;
                if(Contained(a, b)){
                    this->v = u;
                    return;
                }
                right->Update(a, b, u);
                left->Update(a, b, u);
                this->UpdateValue();
            }

            bool Contained(int a, int b){
                return a <= l and r <= b;
            }

            bool Disjoint(int a, int b){
                return r < a or b < l;
            }

            T Combine(T x, T y){
                if(operation){
                    return x | y;
                }else{
                    return x ^ y;
                }
            }

            bool IsLeaf(){
                return l == r;
            }

        };

        vector<Node> nodes;
        Node* root;
        int next;

        SegTree(): next(), nodes(), root(nullptr) {}

        void Build(Node* node, vector<T>& a){
            if(node->IsLeaf()){
                node->v = a[node->l];
                return;
            }
            Node* p = NextNode();
            Node* q = NextNode();
            node->InitChildren(p, q);
            Build(p, a), Build(q, a);
            node->UpdateOperation();
            node->UpdateValue();
        }

        T Update(int a, int b, T u){
            root->Update(a, b, u);
            return root->v;
        }

        void Build(vector<T>& a){
            const int n = a.size();
            int l = ceil(log2(n));
            const int m = 1 << l;
            nodes.resize(m << 1);
            root = nodes.data();
            root->r = m - 1;
            root->l = 0;
            a.resize(m);
            this->Build(root, a);
        }

        Node* NextNode(){
            return &nodes[++next];
        }

    };

};

namespace Solution{

    using DataStructures::SegTree;

    void SolveProblem(){
        std::ios_base::sync_with_stdio(0);
        cout.tie(0), cin.tie(0); int m, n;
        std::unique_ptr<SegTree<int>> st;
        while(cin >> n >> m){
            const int p = 1 << n;
            vector<int> v(p);
            for(auto& k : v){
                cin >> k;
            }
            st.reset(new SegTree<int>());
            st->Build(v);
            for(int k = 0; k < m; k++){
                int x; cin >> x, --x;
                int u; cin >> u;
                cout << st->Update(x, x, u);
                cout << '\n';
            }
            cout << '\n';
        }
    }

};

int main(){
    Solution::SolveProblem();
    return 0;
}