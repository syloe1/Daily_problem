/*
计算前缀位移

用 dx, dy 记录当前位置的累计位移。

根据风向更新 (dx,dy)：

N → dx -= 1

S → dx += 1

W → dy -= 1

E → dy += 1

记录烟雾源集合

初始时刻 (0,0) 必须加入集合。

每个时刻，将当前位置 (dx,dy) 插入集合，表示此时可以作为“生成点”。

判断每个时间点 (t+0.5) 是否有烟雾到达 (R,C)

对于当前前缀位移 (dx,dy)：

计算 target = (dx - R, dy - C)

如果 target 在集合中，说明存在某个生成时刻的烟雾能到达 (R,C)，在答案字符串中对应位置置 1；否则置 0。

输出答案

得到长度为 n 的 01 字符串，每位表示对应时间点是否有烟雾。
*/
#include<bits/stdc++.h>
#define debug(a) cout<<#a<<"="<<a<<'\n';
#define il inline
 
using namespace std;
using ll = long long;
using ull = unsigned long long;
 
ll n,r,c,dx,dy;
string s;
unordered_set<ll> vis;
 
il ll hs(ll x,ll y){return x << 32 | (y & 0xffffffffll);}
 
int main(){
	// freopen("test.in","r",stdin);
	// freopen("test.out","w",stdout);
	ios::sync_with_stdio(0),cout.tie(0),cin.tie(0);
    cin>>n>>r>>c>>s;
    vis.insert(hs(0, 0));
    string res;
    for (char a : s) {
        switch (a) {
            case 'N': {dx--; break;}
            case 'S': {dx++; break;}
            case 'W': {dy--; break;}
            case 'E': {dy++; break;}
        }
        ll key = hs(dx - r, dy - c);
        res += vis.count(key) ? '1' : '0';
        vis.insert(hs(dx, dy));
    }
    cout << res;
    return 0;
}