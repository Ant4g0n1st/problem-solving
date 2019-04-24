#include <iostream>

using std::cout;
using std::cin;

namespace Constants{

    const int B = 5;

};

using namespace Constants;

int main(){
    std::ios_base::sync_with_stdio(0),
    cout.tie(0), cin.tie(0); 
    int t; cin >> t;
    for(int k = 0; k < t; k++){
        int n; cin >> n;
        int z = int();
        int p = B; 
        while(p <= n){
            z += n / p;
            p = p * B;
        }
        cout << z << '\n';
    }
    return 0;
}
