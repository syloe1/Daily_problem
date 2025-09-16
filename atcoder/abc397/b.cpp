#include <bits/stdc++.h>
using namespace std;
string s;int ans = 0;
char c = 'i';
int main() {
    cin>>s;
    for(char i : s) {
        if(i==c) c = (c=='i'?'o':'i');
        else ans++;
    }
    cout<<ans+(c=='o')<<'\n';
    return 0;
}