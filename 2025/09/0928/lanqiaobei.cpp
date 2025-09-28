//https://www.lanqiao.cn/oj-contest/newbie-26/
//A
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
  string ans = "snake";
  sort(ans.begin(), ans.end());
  cout<<ans<<'\n';
  return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    int n; string s, t;cin>>n>>s>>t;
    int s0 = 0, s1 = 0, t0= 0, t1 = 0;
    for(char c : s) {
        if(c=='0') s0++;
        else s1++;
    }
    for(char c : t) {
        if (c == '0') t0++;
        else t1++;
    }
    int ans = min(s0, t1) + min(s1, t0);
    cout<<ans<<"\n";
    return 0;
}
//C
#include<bits/stdc++.h>
using namespace std;
int main()
{
    string s; cin >> s;
    int lr = 0;
    int ud = 0;
    int change = 0;
    for(int i = 0; i < s.length(); i++)
    {
        if( s[i] == 'L') lr--;
        else if(s[i] == 'R') lr++;
        else if(s[i] == 'U') ud++;
        else if(s[i] == 'D') ud--;
    }
    int abslr = abs(lr);
    int absud = abs(ud);
    if((abslr + absud)%2 != 0) cout << -1;
    else 
    {
        change = (abslr + absud) / 2;
        cout << change << endl;
    }

    return 0;
}
//D
#include <iostream>
#include <algorithm>
using namespace std;

int a[100005], n;

bool check(int m) {
  for (int i=1; i<=n; i++)
    if (a[i] < (i-1)/m+1)
      return false;
  return true;
}

int get(int l, int r) {
  while (l != r) {
    int m = (l+r)/2;
    if (check(m))
      r = m;
    else
      l = m+1;
  }
  return l;
}

int main() {
  cin>>n;
  for (int i=1; i<=n; i++)
    cin>>a[i];
  sort(a+1, a+1+n);
  cout<<get(1, n)<<'\n';
  
  return 0;
}
//E
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define int long long
int n, k, sum1;
signed main()
{
    cin >> n >> k;
    vector<int> v(n);
    for (int i = 0; i < n; v[i] %= k, i++)
        cin >> v[i];
    sort(v.begin(), v.end());
    sum1 = v.back() - v[0];
    for (int i1 = 1; i1 < v.size(); i1++)
        sum1 = min(sum1, v[i1 - 1] + k - v[i1]);
    cout << sum1;
}
//F
#include<cstdio>
#include<iostream>
#include<vector>
#include<cmath>
#include<map>
#include<algorithm>

typedef long long LL;

using namespace std;

const int P = 1e9 + 7;

LL a,b,c,n;

LL qmi(LL a ,LL b , int P)
{
    LL res = 1;
    
    while(b)
    {
        if(b & 1) res = res * a % P;
        a = a * a % P;
        b >>= 1;
    }
    
    return res;
}

void solve()
{
    cin >> a >> b >> c >> n;
    
    int m = qmi(2,n,P-1);
    LL res = qmi(a * b % P * c % P , m,P) % P;
    
    cout << res;
}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0) , cout.tie(0);

    int T = 1;
    cin >> T;
    while(T --)
    {
        solve();
        cout << '\n';
    }
    return 0;
}
