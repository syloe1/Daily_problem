//https://www.lanqiao.cn/oj-contest/newbie-27/
//A
#include <iostream>
using namespace std;
int main()
{
  int cnt = 0;
  for(int i = 3; i <= 2025; i++) {
    if(i % 3 == 0 || i % 8 == 0 || i % 38 == 0) cnt++;
  }cout<<cnt<<'\n';
  return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;cin>>s;
    int cnt = 0, tmp = 0;
    for(auto & ch : s) {
        if(ch == 'a') tmp++;
        else cnt = max(cnt, tmp), tmp = 0;
    }
    cnt = max(cnt, tmp);
    cout<<string(cnt+1, 'a')<<'\n';
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;cin>>n;
    string s;cin>>s;
    vector<int> pos;
    for(int i = 0; i < n; i++) {
        if(s[i] == '1') pos.push_back(i);
    }
    for(int i = 0; i < n; i++) {
        if(s[i] == '0') {
            int l = -1, r = pos.size();
            while(l+1 < r) {
                int mid = (l+r) >> 1;
                if(pos[mid] < i) l = mid;
                else r = mid;
            }
            int ans = INT_MAX;
            if(l != -1) ans = min(ans, i-pos[l]);
            if(r != pos.size()) ans = min(ans, pos[r] - i);
            if(ans == INT_MAX) ans = -1;
            cout<<ans<<" ";
        }
    }cout<<"\n"; 
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int n,k;cin>>n>>k;
    vector<ll> a(n);
    for(int i = 0; i < n; i++) cin>>a[i];
    sort(a.begin(), a.end());
    vector<ll> pre(n+1, 0);
    for(int i = 0; i < n; i++) pre[i+1] = pre[i] + a[i];
    ll dif = 0;
    for(int j = 0; j < k; j++) {
        dif += (ll)j * a[j] - pre[j];
    }
    ll ans = dif;
    for(int i = k; i < n; i++) {
        dif -= (pre[i] - pre[i-k+1]) - (ll)(k-1)*a[i-k];
        dif += (ll)(k-1)*a[i] - (pre[i] - pre[i-k+1]);
        ans = min(ans, dif);
    }cout<<ans<<'\n'; return 0;
}
//E
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main() {
    int n;cin>>n;
    vector<ll> a(n), b(n), c(n);
    for(int i = 0 ; i < n; i++) cin>>a[i];
    for(int i = 0;  i < n; i++) cin>>b[i];
    for(int i = 0 ; i < n; i++) c[i] = a[i] - b[i];
    sort(c.begin(), c.end());
    ll ans = 0;
    for(int i = 0; i < n; i++) {
        int pos = upper_bound(c.begin(), c.end(), -c[i]) - c.begin();
        ll cnt = n - pos;
        if(c[i] + c[i] > 0) cnt--;
        ans+=cnt;
    }cout<<ans<<'\n'; return 0;
}
//F
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    long long M = 0;
    for (int i = 0; i < N; i++) {
        long long x;
        cin >> x;
        M |= x;  // 所有口红色号取按位或
    }

    // 找到最小正整数 B
    // 方法：取 M 的按位取反，找最低位的 1
    long long B = (~M) & (-(~M));

    cout << B << "\n";
    return 0;
}
