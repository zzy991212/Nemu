#include "trap.h"

int f[110][110][110];
int d[110]={32,12,1,-1,2,4,56,1,-10,20};
int s[110];
int n,m;
int ans=612;
int max(int x,int y){
	if (x>y) return x;
	return y;
}
int min(int x,int y){
	if (x<y) return x;
	return y;
}
int main(){
	n = 10, m = 7;
	s[0]=0;
	int i,j,k;
	for (i=1;i<n;i++){
		s[i]=s[i-1]+d[i];
	}
	for (i=0;i<=n;i++)
		for (j=0;j<=n;j++)
			for (k=0;k<=n;k++)
				f[i][j][k]=-1e8;
	f[0][0][0]=0;
	
	for (i=1;i<=n;i++){
		f[i][0][0]=0;
		for (j=1;j<=min(i,m);j++){
			for (k=0;k<=j;k++)
				f[i][j][k]=max(f[i][j][k],max(f[i-1][j][k],f[i-1][j-1][k]));
			for (k=1;k<=j;k++)
			    f[i][j][k]=max(f[i][j][k],f[i-1][j-1][k-1]+d[0]-2*s[i-1]);
			for (k=0;k<j;k++)
			    f[i][j][k]=max(f[i][j][k],f[i-1][j-1][k+1]+d[0]+2*s[i-1]);
		}
	}
	nemu_assert(f[n][m][0] == ans);
	return 0;
}
