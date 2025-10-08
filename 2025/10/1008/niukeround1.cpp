//https://ac.nowcoder.com/acm/contest/60245
//A
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    if (!(cin >> n)) return 0;
    int size = 4 * n;
    
    // 前 3n 行：左 n 个 '*'，中间 2n 个 '.'，右 n 个 '*'
    string leftStars(n, '*');
    string midDots(2 * n, '.');
    string rightStars = leftStars;
    string line1 = leftStars + midDots + rightStars;
    for (int i = 0; i < 3 * n; ++i) {
        cout << line1 << '\n';
    }
    
    // 最下方 n 行：第 i 行（i 从 1 到 n）
    // 格式：i 个 '.' + n 个 '*' + (2n-2i) 个 '.' + n 个 '*' + i 个 '.'
    for (int i = 1; i <= n; ++i) {
        string lead(i, '.');
        string stars(n, '*');
        int midLen = 2 * n - 2 * i;
        string mid = (midLen > 0) ? string(midLen, '.') : string();
        string trail = lead;
        cout << lead << stars << mid << stars << trail << '\n';
    }
    
    return 0;
}
//B
/*
for each value x:
    x is red cnt is r[x]
    x is blue cnt is b[x]
    legal match cnt is r[x] * b[x]
*/
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
    int n;cin>>n;
    vector<i64> a(n);
    for(int i = 0; i < n; i++) cin>>a[i];
    string color;cin>>color;
    unordered_map<i64, i64> red, blue;
    for(int i = 0; i < n; i++) {
        if(color[i] == 'R') red[a[i]]++;
        else blue[a[i]]++;
    }
    i64 ans = 0;
    for(auto &p : red) {
        i64 v = p.first;
        if(blue.count(v)) {
            ans += p.second * blue[v];
        }
    }cout<<ans<<'\n';
    return 0;
}
//C
/*
final string:
A: 0101010..
B: 1010101..
只关心1的位置交换次数是 实际距离与目标距离之和
cost = forsum(原位置 - 目标位置)
如果1多，只能10101
如果0多，只能01010
or 都尝试
*/
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
i64 cal(vector<int>& pos, int st, int n) {
    vector<int> target;
    for(int i = st; i < n; i += 2) {
        target.push_back(i);
    }
    i64 cost = 0;
    for(int i = 0; i < pos.size(); i++) {
        cost += abs(pos[i] - target[i]);
    }
    return cost;
}
int main() {
    string s;cin>>s;
    int n = s.size();
    vector<int> ones;
    for(int i = 0; i < n; i++) {
        if(s[i] == '1') {
            ones.push_back(i);
        }
    }
    int cnt1 = ones.size();
    int cnt0 =  n - cnt1;
    i64 ans = LLONG_MAX;
    if(abs(cnt1-cnt0) > 1) {
        cout<<"-1"<<'\n';
        return 0;
    }
    if(cnt1 == cnt0) {
        ans = min(cal(ones, 0, n), cal(ones, 1, n));
    } else if(cnt1>cnt0) {
        ans = cal(ones, 0, n);
    } else {
        ans = cal(ones, 1,n);
    }cout<<ans<<'\n';
    return 0;
}
//D
/*

一个数被9整除的充要条件是 各位数字之和能被9整除
dp[r]表示处理完若干位后，所有子序列中，数字和Mod9=r的子序列数量
for c , value d = c - '0'
旧的子序列可以选择加上它， newmod = (oldmod + d) % 9
ndp[r] = dp[r] + for old from o to 8 (dp[old]) (if (old + d) % 9 == r)
*/
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
const int mod = 1e9+7;
int main() {
    string s;cin>>s;
    vector<i64> dp(9,0);
    for(char c : s) {
        int d = c - '0';
        vector<i64> ndp = dp;
        for(int i = 0; i < 9; i++) {
            int nx = (i+d) % 9;
            ndp[nx] = (ndp[nx] + dp[i]) % mod;
        }
        ndp[d%9] = (ndp[d%9] + 1) % mod;
        dp.swap(ndp);
    }
    cout<<dp[0]%mod<<'\n';
    return 0;
}
