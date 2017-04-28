#include "gameDef.h"

long long int power(long long int a, long long int b, long long int m)
{
	if(b == 0)
		return (1%m);
	long long int ans = power(a, b/2, m);
	ans = (ans*ans)%m;
	if(b%2)
		ans = (ans*a)%m;
	return ans;
}

long long int phi(long long int x)
{
	long long int ans = x;
	int i;
	for (i = 2; i*i <= x; ++i)
	{
		if(x%i == 0)
		{
			while(x%i == 0)
				x/=i;
			ans/=i;
			ans*=(i - 1);
		}
	}
	if(x > 1)
	{
		ans/=x;
		ans*=(x - 1);
	}
	return ans;
}

void single_prime_encode(char plaintext[], char ciphertext[], int n)
{
	long long int e = PUBLIC_KEY;
	long long int m;
	sscanf(plaintext, "%lld", &m);
	sprintf(ciphertext, "%lld", power(m, e, n));
}

void single_prime_decode(char plaintext[], char ciphertext[], int n)
{
	long long int e = PUBLIC_KEY;
	long long int phi_n = n - 1;
	long long int phi_phi_n = phi(phi_n);
	long long int d = power(e, phi_phi_n - 1, phi_n);
	long long int c;
	sscanf(ciphertext, "%lld", &c);
	sprintf(plaintext, "%lld", power(c, d, n));
}

void encrypt_scheme_1(char plaintext[], char ciphertext[])
{
	single_prime_encode(plaintext, ciphertext, 1000000787);
}

void decrypt_scheme_1(char ciphertext[], char plaintext[])
{
	single_prime_decode(plaintext, ciphertext, 1000000787);
}

void encrypt_scheme_2(char plaintext[], char ciphertext[])
{
	single_prime_encode(plaintext, ciphertext, 1000000663);
}

void decrypt_scheme_2(char ciphertext[], char plaintext[])
{
	single_prime_decode(plaintext, ciphertext, 1000000663);
}