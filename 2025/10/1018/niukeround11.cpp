//每日一题 https://www.nowcoder.com/practice/98204dce2782410a822ac53c5025b88a?channelPut=w251acm
// 遍历每一列，每一列的字符组成一个字符串，相同的字符串可以在变换的情况下是的全为 1
// 不同的字符串不可能通过变换使得全为 1  （ 能发现这一点就好了 ）
#include<bits/stdc++.h>
using namespace std;
const int N=3e3+6;
unordered_map<string,int> unmp;
int n,m;
string str[N];
int main(){
    cin>>n>>m;
    for(int i=0;i<n;i++){
        cin>>str[i];
    }
    for(int i=0;i<m;i++){
        string s="";
        for(int j=0;j<n;j++){
            s+=str[j][i];
        }
        unmp[s]++;
    }
    int ans=0;
    for(auto &[k,v]:unmp){
        ans=max(ans,v);
    }
    cout<<ans;

    return 0;
}

//https://ac.nowcoder.com/acm/contest/64593
//A
int main() {
    int t;cin>>t;while(t--) {
        int m, x;cin>>m>>x;cout<<(x-1)%m+1<<"\n";
    }
    return 0;
}
//B
int main() {
    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;
        vector<int> a(n + 1);
        for (int i = 1; i <= n; ++i) cin >> a[i];
        
        for (int i = 0; i < k; ++i) {
            int u, v;
            cin >> u >> v;
            a[u] += 1;
            a[v] -= 1;
        }
        
        bool ok = true;
        for (int i = 1; i < n; ++i) {
            if (a[i] > a[i + 1]) {
                ok = false;
                break;
            }
        }
        
        cout << (ok ? "Yes" : "No") << "\n";
    }
    return 0;
}
//C
int main() {
    string s;cin>>s;
    int n = s.size();
    i64 ans = 0;
    for(int i = 0; i < n; i++) {
        int dif1 = 0, dif2 = 0;
        for(int j = i; j < n; j++) {
           int len = j - i;
           char e1 = (len % 2 == 0 ? '0' : '1') ;
           char e2 = (len % 2 == 0 ? '1' : '0');
           if(s[j] != e1) dif1++;
           if(s[j] != e2) dif2++;
           ans += min(dif1, dif2);
        }
    }cout<<ans<<'\n';
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    int m = n - k;

    vector<vector<int>> dp(n, vector<int>(m + 1, 0));
    for (int i = 0; i < n; ++i) dp[i][1] = 1;

    // 仅计算到长度 m
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (a[i] % a[j] == 0) {
                for (int len = 2; len <= m; ++len)
                    dp[i][len] = (dp[i][len] + dp[j][len - 1]) % MOD;
            }
        }
    }

    long long ans = 0;
    for (int i = 0; i < n; ++i)
        ans = (ans + dp[i][m]) % MOD;

    cout << ans << "\n";
}
