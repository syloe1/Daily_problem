//https://www.lanqiao.cn/oj-contest/newbie-28/
//A
#include <iostream>
using namespace std;
int main()
{
  cout<<"I will fight and win";
  return 0;
}
//B
#include <iostream>
using namespace std;
using ll = long long;
int main()
{
  ll a,b,c;cin>>a>>b>>c;
  if(a > b+c) cout<<"l";
  else if(b > a+c) cout<<"q";
  else if(c > a+b) cout<<"b";
  else cout<<"-1";
  return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int n, q;cin>>n>>q;
    ll s = 0;
    for(int i = 1; i <= n; i++) {
        ll x;cin>>x; s += x;
    }
    while(q--) {
        ll l, r;cin>>l>>r;
        ll len = r - l + 1;
        ll delta;
        if(len % 2 == 0) {
            delta = -(len/2);
        } else {
            delta = -((r-l)/2)+r;
        }
        s += delta;
    }cout<<s<<'\n';return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
int main() {
    int t;cin>>t;while(t--) {
        int n,m;cin>>n>>m;
        vector<int> a(n);
        for(int i = 0 ; i < n; i++) cin>>a[i];
        if(n==1) {
            if(a[0] == m)  cout<<"YES\n";
            else cout<<"NO\n";
        } else{
            int mn = *min_element(a.begin(), a.end());
            int mx = *max_element(a.begin(), a.end());
            if(mn <= m && m <= mx) cout<<"YES\n";
            else cout<<"NO\n";
        }
    }return 0;
}
//E
#include <bits/stdc++.h>
using namespace std;

struct Task {
    int a, b, id;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> A(N), B(N);
    for (int i = 0; i < N; i++) cin >> A[i];
    for (int i = 0; i < N; i++) cin >> B[i];

    vector<Task> front, back;
    for (int i = 0; i < N; i++) {
        if (A[i] <= B[i]) front.push_back({A[i], B[i], i});
        else back.push_back({A[i], B[i], i});
    }

    sort(front.begin(), front.end(), [](Task x, Task y) { return x.a < y.a; });
    sort(back.begin(), back.end(), [](Task x, Task y) { return x.b > y.b; });

    vector<Task> order;
    order.insert(order.end(), front.begin(), front.end());
    order.insert(order.end(), back.begin(), back.end());

    long long timeA = 0, timeB = 0;
    for (auto &t : order) {
        timeA += t.a;
        timeB = max(timeA, timeB) + t.b;
    }

    cout << timeB << "\n";
    return 0;
}
//F
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;  // N 转为字符串
    int L = s.size();

    set<string> st;
    for (int pos = 0; pos <= L; pos++) {
        for (int d = 0; d <= 9; d++) {
            if (pos == 0 && d == 0) continue; // 不能前导零
            string t = s.substr(0, pos) + char('0' + d) + s.substr(pos);
            st.insert(t);
        }
    }

    cout << st.size() << "\n";
    return 0;
}
