//https://www.lanqiao.cn/oj-contest/newbie-29/
//A
#include <iostream>
using namespace std;
int main()
{
  cout<<8<<'\n';
  return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int t;cin>>t;while(t--) {
        ll a, b, c;cin>>a>>b>>c;
        ll L = b-a, R = c - b;
        cout<<max(L, R)-1<<'\n';
    }return 0;
}
//C
#include <iostream>
using namespace std;
const int N=1e5+10;
long long n,m,a[N],ans,sum_1;

int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++) {
        cin>>a[i];
        sum_1+=a[i];
    }
    if(sum_1<m){ // 如果所有人口都不足一个防护罩
        cout<<-1;
        return 0;
    }

    long long sum_2 = 0;
    for(int i=1;i<=n;i++){
        sum_2 += a[i];
        if(sum_2 < m) {
            ans++; // 把第i户迁到前面
        } else {
            sum_2 = 0; // 这一组形成一个防护罩，清零继续
        }
    }

    cout<<ans;
    return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
int main() {
    int n;cin>>n;
    ll s = 0;
    for(int i = 0; i < n; i++) {
        ll x; cin>>x; s += x;
    }
    if(s % 2 == 1) {
        cout<<0<<'\n';
    } else{
        cout<<1LL*(n-1)*n/2<<'\n';
    }
}
//E
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;cin>>n;vector<int> a(n);
    for(int i = 0 ; i < n; i++) cin>>a[i];
    int p = -3, t = 0;
    for(int i = 0; i < n; i++) {
        if(a[i] == 0) {
            if(i - p >= 3) {
                p = i;
            } else t++;
        }
    }cout<<t<<'\n';
    return 0;
}
//F
#include <iostream>
using namespace std;

int main() {
  int x;
  cin>>x;
  cout<<((1<<32-__builtin_clz(x))-x)<<'\n';
  
  return 0;
}