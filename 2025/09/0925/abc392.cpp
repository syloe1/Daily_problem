//https://atcoder.jp/contests/abc392
//A
#include <iostream>
using namespace std;
int main() {
    int a, b, c;cin>>a>>b>>c;
    if(a*b == c) {
        cout<<"Yes";
    } else if(a * c == b) {
        cout<<"Yes";
    } else if(b * c == a) {
        cout<<"Yes";
    } else cout<<"No";
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, m;cin>>n>>m;vector<int> a;
    vector<pair<int, int>> ms;
    for(int i = 1; i <= n; i++) {
        ms.push_back({i, 1});
    }
    for(int i = 0; i < m; i++) {
        int x;cin>>x;
        ms[x-1].second = 0;
    }
    vector<int> ans;
    for(auto e : ms) {
        if(e.second == 1){
            ans.push_back(e.first);
        }
    }
    cout<<ans.size()<<'\n';
    for(int i = 0; i < ans.size(); i++) {
        if(i> 0) cout<<" ";
        cout<<ans[i];
    }return 0;
}
//C
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    // 读取每个人盯着的人
    vector<int> P(N + 1);  // P[i]表示第i个人盯着的人
    for (int i = 1; i <= N; ++i) {
        cin >> P[i];
    }
    
    // 读取每个人的号码布数字
    vector<int> Q(N + 1);  // Q[i]表示第i个人的号码布数字
    for (int i = 1; i <= N; ++i) {
        cin >> Q[i];
    }
    
    // 建立映射：号码布数字 -> 对应的人
    vector<int> bib_to_person(N + 1);
    for (int i = 1; i <= N; ++i) {
        bib_to_person[Q[i]] = i;
    }
    
    // 计算结果
    vector<int> S(N + 1);
    for (int i = 1; i <= N; ++i) {
        // 穿着号码布i的人 -> 这个人盯着的人 -> 被盯着的人的号码布数字
        S[i] = Q[P[bib_to_person[i]]];
    }
    
    // 输出结果
    for (int i = 1; i <= N; ++i) {
        if (i > 1) cout << ' ';
        cout << S[i];
    }
    cout << endl;
    
    return 0;
}
//D
#include<bits/stdc++.h>
#define N 105
#define S 100010
using namespace std;
int n;
unordered_set<int> se[N];
double p[N][S],maxx;
signed main(){
	cin>>n;
	for(int i=1,m,x;i<=n;i++){
		cin>>m;
		double tmp=1.0/m;
		for(int j=1;j<=m;j++)
			cin>>x,p[i][x]+=tmp,se[i].insert(x);
	}
	for(int i=1;i<n;i++){
		for(int j=i+1;j<=n;j++){
			double ans=0;
			for(int k:se[i])
				ans+=p[i][k]*p[j][k];
			maxx=max(maxx,ans);
		}
	}
	cout<<fixed<<setprecision(18)<<maxx;
	return 0;
}