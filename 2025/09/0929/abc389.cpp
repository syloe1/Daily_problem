//https://atcoder.jp/contests/abc389
//A
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    string s;cin>>s;
    int a = s[0] - '0';
    int b = s[2] - '0';
    cout<<a*b<<'\n';
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using i128 = __int128_t;
int main() {
    i64 x;cin>>x;
    i128 f = 1;
    for(int n = 1; n <= 20; n++) {
        f *= n; 
        if(f == x) {
            cout<<n<<"\n";
            return 0;
        }
    }
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    int q; cin>>q; 
    deque<i64> len;
    deque<i64> head;
    i64 shift = 0;
    while(q--) {
        int type;cin>>type;
        if(type == 1) {
            int l;cin>>l;
            if(len.empty()) {
                head.push_back(0);
            } else {
                head.push_back(head.back() + len.back());
            }
            len.push_back(l);
        } else if(type == 2) {
            i64 m = len.front();
            len.pop_front();
            head.pop_front();
            shift += m;
        } else if(type == 3) {
            int k;cin>>k;
            cout<<head[k-1]-shift<<'\n';
        }
    }
    return 0;
}
//D
