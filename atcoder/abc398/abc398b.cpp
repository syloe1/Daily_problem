/*
统计每个数出现的次数（用 map<int,int>）。

想要葫芦，需要：

至少有一个数出现 ≥3 次（可以作为三张部分）。

至少有另一个不同的数出现 ≥2 次（可以作为对子部分）。

注意排除特例：

如果只有一个数（比如全是 7），即使出现 7 次，也不能构成葫芦，因为葫芦要求 两个不同的数。
*/
#include <bits/stdc++.h>
using namespace std; 
int main() {
    vector<int> a(7);
    for(int i = 0; i < 7; i++) cin>>a[i];
    map<int, int> mp;
    for(int x : a) mp[x]++;
    bool f = false;
    for(auto [x, cnt] : mp) {
        if(cnt >= 3) {
            for(auto [y, cy] : mp) {
                if(x != y && cy >= 2) {
                    f = true;
                }
            }
        }
    }
    cout<<(f ? "Yes\n" :"No\n");
    return 0;
}