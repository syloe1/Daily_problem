//https://www.lanqiao.cn/oj-contest/newbie-30/
//A
#include <iostream>
using namespace std;
int main()
{
  cout<<16<<'\n';
  return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main()
{
  ll n,m;cin>>n>>m; 
  ll cycleLen = 2 * n - 2;
  ll r = (m-1) % cycleLen;
  int ans;
  if(r < n) {
    ans = r + 1;
  } else {
    ans = 2 * n - 1 - r;
  }
  cout<<ans<<'\n';
  return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int n; ll m; cin>>n>>m;
    vector<ll> a(n);
    for(int i = 0; i < n; i++) cin>>a[i];
    sort(a.begin(), a.end());
    ll mxGap = 0;
    for(int i = 0; i < n - 1; i++) {
        mxGap = max(mxGap, a[i+1] - a[i]);
    }
    mxGap = max(mxGap,a[0] + m - a[n-1]);
    cout<<m-mxGap<<'\n'; return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;cin>>s;
    int ans = 1;
    for(int i = 0; i < s.size() - 1; i++) {
        char c = s[i];
        if(c == 'L') {
            ans++;
            if(i-1 >= 0 && s[i-1] == 'Q' && s[i+1] == 'L') ans++;
        }
    }cout<<ans<<'\n';return 0;
}
//E
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int n; ll k; cin>>n>>k;
    vector<ll> v(n);
    for(int i = 0; i < n; i++) cin>>v[i];
    sort(v.begin(), v.end(),greater<ll>());
    ll cnt = 0, ans = 0;
    for(int i = 0; i < n; i++) {
        cnt++;
        if(cnt * v[i] >= k) {
            ans++;
            cnt = 0;
        }
    }cout<<ans<<'\n'; return 0;
}
//F
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll lcm(ll a, ll b) {
    return a / __gcd(a, b) * b;
}

ll cal_days(ll n, ll x, ll y, ll z) {
    ll xy = lcm(x, y);
    ll yz = lcm(y, z);
    ll xz = lcm(x, z);
    return n / x + n / y + n / z - n / xy - n / yz - n / xz + n / lcm(xy, z);
}

bool check(ll n, ll x, ll y, ll z, ll k) {
    return cal_days(n, x, y, z) >= k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; cin >> t;
    while(t--) {
        ll x, y, z, k;
        cin >> x >> y >> z >> k;

        ll l = 0, r = 1e18+5;
        while(l + 1 < r) {
            ll mid = (l + r) >> 1;
            if(check(mid, x, y, z, k)) r = mid;
            else l = mid;
        }

        cout << r << '\n';
    }
}
