#include "trap.h"

int f[110][110][110];
int d[110]={32 12 1 -1 2 4 56 1 -10 20};
int s[110];
int n,m;
int ans=612;
int max(int x,int y){
	if (x>y) return x;
	return y;
}
int main(){

	n = 10, m = 7;
	s[0]=0;
	for (int i=1;i<n;i++){
		s[i]=s[i-1]+d[i];
	}
	for (int i=0;i<=n;i++)
		for (int j=0;j<=n;j++)
			for (int k=0;k<=n;k++)
				f[i][j][k]=-1e8;
	f[0][0][0]=0;
	
	for (int i=1;i<=n;i++){
		f[i][0][0]=0;
		for (int j=1;j<=min(i,m);j++){
			for (int k=0;k<=j;k++)
				f[i][j][k]=max(f[i][j][k],max(f[i-1][j][k],f[i-1][j-1][k]));
			for (int k=1;k<=j;k++)
			    f[i][j][k]=max(f[i][j][k],f[i-1][j-1][k-1]+d[0]-2*s[i-1]);
			for (int k=0;k<j;k++)
			    f[i][j][k]=max(f[i][j][k],f[i-1][j-1][k+1]+d[0]+2*s[i-1]);
		}
	}
	nemu_assert(f[n][m][0] == ans);
	cout<<f[n][m][0]<<endl;
	return 0;
}
