/*
我们用哈希表（unordered_map）或 map 统计每个整数出现的次数。
然后扫描所有人，找出唯一出现的数里最大的，并记录其编号。
*/
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int n;cin>>n;vector<ll> a(n);
    ll ans = -1, ax = -1;
    unordered_map<ll, int> mp;
    for(int i = 0; i < n; i++) {
        cin>>a[i];
        mp[a[i]]++;
    }
    for(int i = 0; i < n; i++) {
        if (mp[a[i]] == 1) {
            if(a[i] > ax) {
                ax = a[i]; ans = i + 1;
            }
        }
    }
    cout<<ans<<'\n'; return 0;
}