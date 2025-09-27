//https://atcoder.jp/contests/abc391
//A
#include <bits/stdc++.h>
using namespace std;
int main() {
    string d;cin>>d; 
    unordered_map<string, string> ops = {
        {"N", "S"},
        {"S", "N"},
        {"E", "W"},
        {"W", "E"}, 
        {"NE", "SW"},
        {"NW", "SE"},
        {"SE", "NW"}, 
        {"SW", "NE"}
    };
    cout<<ops[d]<<"\n";
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n,m;cin>>n>>m;
    vector<string> s(n), t(m);
    for(int i = 0; i < n; i++) cin>>s[i];
    for(int i = 0; i < m; i++) cin>>t[i];
    for(int a = 0; a <= n- m; a++) {
        for(int b = 0; b <= n - m; b++) {
            bool f = true; 
            for(int i = 0; i < m; i++) {
                for(int j = 0; j < m; j++) {
                    if(s[a+i][b+j] != t[i][j]) {
                        f=false; break;
                    }
                }
                if(!f) break;
            }
            if(f) {
                cout<<a+1<<" "<<b+1<<"\n";
                return 0;
            }
        }
    }
    return 0;
}
//C
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n, q;cin>>n>>q;
    vector<int> pp(n+1),ne(n+1);
    for(int i = 1; i <= n; i++) {
        pp[i] = i;
        ne[i] = 1;
    }
    int cnt = 0 ;
    while(q--) {
        int type;cin>>type;
        if(type == 1) {
            int p, h;cin>>p>>h;
            int old = pp[p];
            if(ne[old] >= 2) cnt--;
            ne[old]--;
            if(ne[old] >= 2) cnt++;
            if(ne[h] >= 2) cnt--;
            ne[h]++;
            if(ne[h] >= 2) cnt++;
            pp[p] = h;
        } else {
            cout<<cnt<<"\n";
        }
    }
    return 0;
}
//E
#include<bits/stdc++.h>
using namespace std;
const int inf=1e9;
char ch;
int n,m,dcnt,id[3000005],a[3000005],f[3000005][2];
vector<int> v[3000005];
bool cmp0(int x,int y){
	return f[x][0]<f[y][0];
}
bool cmp1(int x,int y){
	return f[x][1]<f[y][1];
}
void build(int l,int r,int fa){
	int x=++dcnt;
	v[fa].push_back(x);
	if(l==r){
		id[x]=l;
		return;
	}
	int mid1=(r-l+1)/3+l-1,mid2=(r-l+1)/3*2+l-1;
	build(l,mid1,x);
	build(mid1+1,mid2,x);
	build(mid2+1,r,x);
}
void dfs(int x){
	if(!v[x].size()){
		f[x][a[id[x]]]=0;
		return;
	} 
	for(int i=0;i<v[x].size();i++){
		dfs(v[x][i]);
	}
	sort(v[x].begin(),v[x].end(),cmp0);
	if(id[v[x][0]]) f[x][0]=min(f[v[x][0]][0],1)+min(f[v[x][1]][0],1);
	else f[x][0]=f[v[x][0]][0]+f[v[x][1]][0];
	sort(v[x].begin(),v[x].end(),cmp1);
	if(id[v[x][0]]) f[x][1]=min(f[v[x][0]][1],1)+min(f[v[x][1]][1],1);
	else f[x][1]=f[v[x][0]][1]+f[v[x][1]][1];
}
int main(){
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	cin>>n;
	m=pow(3,n);
	for(int i=1;i<=m;i++){
		cin>>ch;
		a[i]=ch-'0';
	}
	build(1,m,0);
	for(int i=1;i<=dcnt;i++){
		f[i][0]=f[i][1]=inf;
	}
	dfs(1);
	if(!f[1][0]) cout<<f[1][1];
	else cout<<f[1][0];
	return 0; 
} 
