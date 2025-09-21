//https://atcoder.jp/contests/abc394
//A
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;cin>>s;
    for(auto e : s) {
        if(e == '2') cout<<e;
    }
    return 0;
}
//B
#include <bits/stdc++.h>
using namespace std;
bool cmp(string a, string b) {
    return a.size() <= b.size();
}
int main() {
    string ans = "";
    int t;cin>>t; vector<string> ss;
    while(t--) {
        string s;cin>>s; ss.push_back(s);
    }
    sort(ss.begin(),ss.end(), cmp);
    for(auto e : ss) {
        ans += e;
    }cout<<ans<<'\n';return 0;
}
//C
#include<bits/stdc++.h>
using namespace std;
string s;
int n;
signed main(){
	cin>>s;
	n=s.size();
	int pos=-1;
	for(int i=0;i<n;i++){
		if(s[i]=='W'){
			if(pos==-1) pos=i;
		}else if(s[i]=='A'){
			if(~pos){
				cout<<"A";
				for(int j=pos;j<i;j++) cout<<"C";
				pos=-1;
			}else{
				cout<<"A";
			}
		}else{
			if(~pos){
				for(int j=pos;j<i;j++) cout<<s[j];
				pos=-1;
			}
			cout<<s[i];
		}
	}
	if(~pos){
		for(int j=pos;j<n;j++) cout<<s[j];
		pos=-1;
	}
	return 0;
}
//D
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;cin>>s;
    stack<char> st;
    for(char c : s) {
        if(!st.empty()) {
            char top = st.top();
            if((top == '(' && c==')') ||
               (top == '[' && c==']') || 
               (top == '<' && c=='>')) {
                st.pop();continue;
              }
        }
        st.push(c);
    }
    cout<<(st.empty() ? "Yes" : "No")<<'\n'; return 0;
}
//E
