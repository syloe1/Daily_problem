//https://ac.nowcoder.com/acm/contest/117763#question
//A
#include <iostream>
using namespace std;
int main() {
    int a, b, c;cin>>a>>b>>c;
    if((c - b == b - a ) && (b - a == 1)) {
        cout<<"Yes\n";
    } else{
        cout<<"No\n";
    }
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n; cin>>n;vector<int> a(n), b(n);
    int x = INT_MAX, y = INT_MIN;
    int ma = 0, mb = 0;
    for(int i = 0 ; i < n; i++) {
        cin>>a[i]; if(a[i] < x) {
            ma = i; x = a[i];
        }
    }
    for(int i = 0; i < n; i++) {
        cin>>b[i];
        if(b[i] > y) {
            mb = i; y = b[i];
        }
    }
    
    cout<<ma+1<<" "<<mb+1<<endl;
    return 0;
}

//C
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    i64 n, k; int x; cin>>n>>k>>x;
    vector<int> a(n);
    for(int i = 0; i < n ;i++) cin>>a[i];
    int shift = k % x ;
    rotate(a.begin(), a.begin()+(x-shift), a.begin()+x);
    for(int i = 0; i < n; i++) {
        if(i) cout<<" ";
        cout<<a[i];
    }cout<<"\n";
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;

// 快速求逆元 (p 和 11 互质时)
int modInverse(int a, int mod = 11) {
    a %= mod;
    for (int x = 1; x < mod; x++) {
        if ((a * x) % mod == 1) return x;
    }
    return -1; // 不存在逆元
}

int getLen(long long x) {
    int len = 0;
    while (x) {
        len++;
        x /= 10;
    }
    return max(1, len);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    // 预计算 10^len % 11
    vector<int> pow10mod11(11);
    pow10mod11[0] = 1;
    for (int i = 1; i <= 10; i++) {
        pow10mod11[i] = (pow10mod11[i - 1] * 10) % 11;
    }

    // 统计余数出现次数
    vector<long long> cnt(11, 0);
    for (int i = 0; i < n; i++) {
        cnt[a[i] % 11]++;
    }

    long long ans = 0;
    for (int i = 0; i < n; i++) {
        int b = a[i] % 11;
        int len = getLen(a[i]);
        int p = pow10mod11[len];

        if (p % 11 == 0) continue; // 无解

        int inv = modInverse(p, 11);
        int target = ((-b * inv) % 11 + 11) % 11;

        // 所有 a 满足条件
        ans += cnt[target];

        // 排除 (i,i) 自己
        if (a[i] % 11 == target) ans--;
    }

    cout << ans << "\n";
    return 0;
}
//E
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, k;
    if (!(cin >> n >> k)) return 0;

    if (n == 1) {
        // 只有排列 [1]，L=1, R=1 -> L+R=2
        if (k == 2) cout << 1 << "\n";
        else cout << -1 << "\n";
        return 0;
    }

    ll minv = 3 * n - 1;
    ll maxv = n + n * (n + 1) / 2; // 等于 3n-1 + sum(1..n-2)

    if (k < minv || k > maxv) {
        cout << -1 << "\n";
        return 0;
    }

    ll rem = k - minv; // 需要从 {1..n-2} 中凑的和
    vector<char> vis(n + 1, 0); // vis[i]=1 表示 i 被选到 B 中

    // 从大到小贪心选择
    for (ll i = n - 2; i >= 1; --i) {
        if (rem >= i) {
            vis[i] = 1;
            rem -= i;
        }
    }

    // 按模板输出
    bool first = true;
    auto out = [&](ll v){
        if (!first) cout << ' ';
        first = false;
        cout << v;
    };

    out(n);
    for (ll i = 1; i <= n - 2; ++i) {
        if (!vis[i]) out(i);
    }
    out(n - 1);
    for (ll i = n - 2; i >= 1; --i) {
        if (vis[i]) out(i);
    }
    cout << '\n';
    return 0;
}

//F
/*
 * problem: 小红的排列构造.cpp
 * author: duchuan
 * date: 2025-09-28 19:00:10
 */
#include<bits/stdc++.h>
using i64=long long;
using u64=unsigned long long;
using namespace std;
void solve()
{
    int n,k;
    cin>>n>>k;
    int sum=n-k;
    if(sum<1||sum>n/2){
        cout<<"-1\n";
    }else{
        vector<int>ans(n+1);
        int pos=1;
        for(int i=1;i<sum;i++){
            ans[pos]=pos+1;
            ans[pos+1]=pos;
            pos+=2;
        }
        if(pos<=n){
            for(int i=pos;i<n;i++){
                ans[i]=i+1;
            }
            ans[n]=pos;
        }
        for(int i=1;i<=n;i++){
            cout<<ans[i]<<" \n"[i==n];
        }
    }
}
#define tests int _;cin>>_;while(_--)solve();
#define test solve();
int main()
{
    cin.tie(nullptr)->sync_with_stdio(false);
    test
    return 0 ^ 0;
}
