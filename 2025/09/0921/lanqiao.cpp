//蓝桥杯萌新赛https://www.lanqiao.cn/oj-contest/25xspws-1/
//AC code
//1
#include <bits/stdc++.h>
using namespace std;
int main() {
    string ans ;
    for(int i = 0; i < 6; i++) {
        string s;cin>>s; ans += s[0];
    }
    cout<<ans<<'\n';
    return 0;
}
   
//2
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;cin>>s;
    for(int i = 0; i < s.size(); i++) {
        if(isalpha(s[i])) s[i] = 'L';
        else s[i] = 'Q';
    }cout<<s<<'\n';
    return 0;
}
//3
#include <bits/stdc++.h>
using namespace std;
int main() {
    int a, b, c; cin>>a>>b>>c;
    vector<int> t;
    t.push_back(a);
    t.push_back(b);
    t.push_back(c);
    sort(t.begin(),t.end(), greater<int>());
    cout<<t[1]+max(t[0]-t[1], t[2])<<'\n';
    return 0;
}
//4
#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> a(10);
    for(int i = 0; i < 10; i++) cin>>a[i];
    int s = 0;
    for(int i = 0; i < 10; i++) {
        s += a[i];
        if(s >= 100) {
            int eat = s;
            int neat = s - a[i];
            int df = abs(eat-100), df1 = abs(neat-100);
            if(df < df1) {
                cout<<eat<<'\n';
            } else if(df > df1) {
                cout<<neat<<'\n';
            } else {
                cout<<max(eat, neat)<<'\n';
            }
            return 0;
        }
    }
    cout<<s<<'\n';
    return 0;
}
//5
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    vector<int> A(N), B(N);
    for (int &x : A) cin >> x;
    for (int &x : B) cin >> x;

    sort(B.begin(), B.end());
    int ans = 0;

    for (int a : A) {
        auto it = lower_bound(B.begin(), B.end(), a);
        int md = INT_MAX;
        if (it != B.end()) md = min(md, abs(a - *it));
        if (it != B.begin()) md = min(md, abs(a - *(it - 1)));
        ans = max(ans, md);
    }

    cout << ans << '\n';
    return 0;
}
