//https://atcoder.jp/contests/abc393
//A
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s1, s2;cin>>s1>>s2;
    if(s1 == "sick" && s2 == "sick") cout<<1<<'\n';
    else if(s1 == "sick" && s2 == "fine") cout<<2<<'\n';
    else if(s1 == "fine" && s2 == "sick") cout<<3<<'\n';
    else cout<<4<<'\n';
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;cin>>s; 
    int n = s.size(); int cnt = 0;
    for(int j = 1; j < n - 1; j++) {
        if(s[j] == 'B') {
            int axd = min(j, (n-1)-j);
            for(int d = 1; d <= axd; ++d) {
                int i = j - d, k = j + d;
                if(s[i] == 'A' && s[k] == 'C') cnt++;
            }
        }
    }cout<<cnt<<'\n'; return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int n, m;cin>>n>>m;
    int cnt = 0;
    unordered_set<ll> ms;
    for(int i = 0; i < m; i++) {
        int u, v;cin>>u>>v; 
        if(u == v) cnt++;
        else {
            int a = min(u, v), b = max(u, v);
            ll v = 1LL * a * n + b;
            if(ms.count(v)) cnt++;
            else {
                ms.insert(v);
            }
        }
    }cout<<cnt<<"\n";return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int n;string s;cin>>n>>s;
    vector<int> pos;
    for(int i = 0; i < n; i++) {
        if(s[i] == '1') pos.push_back(i);
    }
    int k = pos.size();
    if(k <= 1) {
        cout<<0<<'\n';return 0;
    }
    int median = pos[k/2];
    ll res = 0;
    for(int i = 0; i < k; i++) {
        res += abs(pos[i] - (median - k/2 + i));
    }cout<<res<<'\n'; return 0;
}
//E
#include<bits/stdc++.h>         // 引入所有标准库
#define N 1200010               // 定义数组大小上限 N（数组元素个数）
#define V 1000010               // 定义值域上限 V（数组元素最大值）
using namespace std;

// 全局数组
int n, k;                       // n = 数组长度，k = 选择子集大小
int a[N];                        // 输入数组 a[1..n]
int s[V];                        // s[x] = 数组中值为 x 的元素个数
int u[V];                        // u[x] = 元素值 x 的答案（最大 GCD）

signed main(){
    ios::sync_with_stdio(false);  // 加快输入输出
    cin.tie(nullptr), cout.tie(nullptr);

    cin >> n >> k;               // 输入 n 和 k

    // 输入数组，并统计每个值出现次数
    for(int i=1;i<=n;i++){
        cin >> a[i];
        s[a[i]]++;               // s[x] 累加每个元素出现次数
    }

    // 枚举每个可能的公因数 i
    for(int i=1;i<V;i++){
        int t = 0;               // t = i 的倍数在数组中出现的总次数

        // 枚举 i 的所有倍数 j = i, 2i, 3i, ...
        for(int j=i; j<V; j+=i) 
            t += s[j];           // 累加 j 出现次数，得到 t

        // 如果 i 的倍数数量 >= k，则 i 可以作为最大 GCD
        if(t >= k) 
            for(int j=i; j<V; j+=i) 
                u[j] = i;        // 更新所有 j 的答案为 i
    }

    // 输出每个元素 a[i] 的答案 u[a[i]]
    for(int i=1;i<=n;i++) 
        cout << u[a[i]] << "\n";

    return 0;
}
