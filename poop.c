#include "stdio.h"
int main(int argc, char const *argv[])
{
	freopen("input.txt", "r", stdin);
	char buffer[10];
	scanf("%s", buffer);
	int d;
	sscanf(buffer, "ARENA %d", &d);
	printf("%d\n", d);
	return 0;
}