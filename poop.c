#include "stdio.h"
int main(int argc, char const *argv[])
{
	freopen("input.txt", "r", stdin);
	int i,j, lala = 0;
	for (i = 1000000000; lala < 40; ++i)
	{
		int ctr = 0;
		for (j = 2; j*j <= i && !ctr; ++j)
		{
			if(i%j == 0)
				ctr++;
		}
		if(!ctr)
		{
			printf("%d\n", i);
			lala++;
		}
	}
	return 0;
}