#include "trap.h"

long long a[]={10,12,12343,1000000000,10,10};
long long b[]={10,8,43,1000000000,11,11};
long long x[]={8,8,4543,1,2,9};
long long y[]={5,7,39,1,1,1};
long long n[]={3,2,123212,1000000000,5,10};
long long ans[6];
long long check[]={70,77,177177,999999999,55,10};

long long min(long long a,long long b){
	if (a < b) return a;
	return b;
}
int main(){
	int T=6;
	int t;
	for (t=0;t<T;t++){
		long long tmp=a[t]+b[t]-n[t];
		if (tmp<x[t]+y[t]) {
			ans[t]=x[t]*y[t];
		}else {
			ans[t]=a[t]*b[t];
			long long t1=a[t]-x[t],t2=b[t]-y[t];
			if (t1<=n[t]) {
				ans[t]=min(ans[t],x[t]*(b[t]-(n[t]-t1)));
			}else {
				ans[t]=min(ans[t],(a[t]-n[t])*b[t]);
			}
			if (t2<=n[t]){
				ans[t]=min(ans[t],y[t]*(a[t]-(n[t]-t2)));
			}else {
				ans[t]=min(ans[t],(b[t]-n[t])*a[t]);
			}
		}
		nemu_assert(ans[t] == check[t]);
	}
	
	return 0;
}
