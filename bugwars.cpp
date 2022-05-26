// Refueling: Given the locations of cities and the amount of fuel at each of them, compute the maximum number of cities that can be visited.

#include "bits/stdc++.h"
template<class T>bool umax(T& a,T b){if(a<b){a=b;return 1;}return 0;}

const int N=707;
int dp[N][N][2];

int solution(vector<int> &a, vector<int> &x) {
    int n=x.size(),ans=0;
    memset(dp,-1,sizeof dp);
    for(int i=0;i<n;i++)
        dp[i][i][0]=dp[i][i][1]=a[i];
    for(int len=0;len+1<n;len++){
        for(int i=0;i<n;i++){
            int j=i+len;
            //k=0
            if(i and dp[i][j][0]>=x[i]-x[i-1])
                umax(dp[i-1][j][0],dp[i][j][0]+a[i-1]-(x[i]-x[i-1]));
            if(j+1<n and dp[i][j][0]>=x[j+1]-x[i])
                umax(dp[i][j+1][1],dp[i][j][0]+a[j+1]-(x[j+1]-x[i]));
            //k=1
            if(i and dp[i][j][1]>=x[j]-x[i-1])
                umax(dp[i-1][j][0],dp[i][j][1]+a[i-1]-(x[j]-x[i-1]));
            if(j+1<n and dp[i][j][1]>=x[j+1]-x[j])
                umax(dp[i][j+1][1],dp[i][j][1]+a[j+1]-(x[j+1]-x[j]));
        }
    }
    for(int i=0;i<n;i++)
        for(int j=i;j<n;j++)
            if(max(dp[i][j][0],dp[i][j][1])>=0)
                ans=max(ans,j-i+1);
    return ans;
}
