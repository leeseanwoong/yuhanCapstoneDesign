#include "KISA_SHA256.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const Uint SHA256_K[64] = 
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

#if defined(_MSC_VER)
	#define ROTL_Ulong(x, n) _lrotl((x), (n))
	#define ROTR_Ulong(x, n) _lrotr((x), (n))
#else
	#define ROTL_Ulong(x, n) ((Ulong)((x) << (n)) | (Ulong)((x) >> (32 - (n))))
	#define ROTR_Ulong(x, n) ((Ulong)((x) >> (n)) | (Ulong)((x) << (32 - (n))))
#endif

#define ENDIAN_REVERSE_Ulong(dwS)	( (ROTL_Ulong((dwS),  8) & 0x00ff00ff)	\
									| (ROTL_Ulong((dwS), 24) & 0xff00ff00) )

#if defined(BIG_ENDIAN)
	#define BIG_B2D(B, D)		D = *(Ulong_PTR)(B)
	#define BIG_D2B(D, B)		*(Ulong_PTR)(B) = (Ulong)(D)
	#define LITTLE_B2D(B, D)	D = ENDIAN_REVERSE_Ulong(*(Ulong_PTR)(B))
	#define LITTLE_D2B(D, B)	*(Ulong_PTR)(B) = ENDIAN_REVERSE_Ulong(D)
#elif defined(LITTLE_ENDIAN)
	#define BIG_B2D(B, D)		D = ENDIAN_REVERSE_Ulong(*(Ulong_PTR)(B))
	#define BIG_D2B(D, B)		*(Ulong_PTR)(B) = ENDIAN_REVERSE_Ulong(D)
	#define LITTLE_B2D(B, D)	D = *(Ulong_PTR)(B)
	#define LITTLE_D2B(D, B)	*(Ulong_PTR)(B) = (Ulong)(D)
#else
	#error ERROR : Invalid DataChangeType
#endif

#define RR(x, n)		ROTR_Ulong(x, n)
#define SS(x, n)		(x >> n)

#define Ch(x, y, z)		((x & y) ^ ((~x) & z))
#define Maj(x, y, z)	((x & y) ^ (x & z) ^ (y & z))
#define Sigma0(x)		(RR(x,  2) ^ RR(x, 13) ^ RR(x, 22))
#define Sigma1(x)		(RR(x,  6) ^ RR(x, 11) ^ RR(x, 25))

#define RHO0(x)			(RR(x,  7) ^ RR(x, 18) ^ SS(x,  3))
#define RHO1(x)			(RR(x, 17) ^ RR(x, 19) ^ SS(x, 10))

		
//*********************************************************************************************************************************
// o SHA256_Transform()	: 512 비트 단위 블록의 메시지를 입력 받아 연쇄변수를 갱신하는 압축 함수로써
//						  4 라운드(64 단계)로 구성되며 8개의 연쇄변수(a, b, c, d, e, f, g, h)를 사용
// o 입력				: Message		- 입력 메시지의 포인터 변수
//						  ChainVar		- 연쇄변수의 포인터 변수
// o 출력				: 
//*********************************************************************************************************************************
void SHA256_Transform(Ulong_PTR Message, Ulong_PTR ChainVar)
{
	Ulong a, b, c, d, e, f, g, h, T1, X[64];
	Ulong j;

#define FF(a, b, c, d, e, f, g, h, j) {							\
	T1 = h + Sigma1(e) + Ch(e, f, g) + SHA256_K[j] + X[j];		\
	d += T1;													\
	h = T1 + Sigma0(a) + Maj(a, b, c);							\
}

#if defined(BIG_ENDIAN)
	#define GetData(x)	x
#else
	#define GetData(x)	ENDIAN_REVERSE_Ulong(x)
#endif

	for (j = 0; j < 16; j++)
		X[j] = GetData(Message[j]);

	for (j = 16; j < 64; j++)
		X[j] = RHO1(X[j - 2]) + X[j - 7] + RHO0(X[j - 15]) + X[j - 16];

	a = ChainVar[0];
	b = ChainVar[1];
	c = ChainVar[2];
	d = ChainVar[3];
	e = ChainVar[4];
	f = ChainVar[5];
	g = ChainVar[6];
	h = ChainVar[7];

	for (j = 0; j < 64; j += 8)
	{
		FF(a, b, c, d, e, f, g, h, j + 0);
		FF(h, a, b, c, d, e, f, g, j + 1);
		FF(g, h, a, b, c, d, e, f, j + 2);
		FF(f, g, h, a, b, c, d, e, j + 3);
		FF(e, f, g, h, a, b, c, d, j + 4);
		FF(d, e, f, g, h, a, b, c, j + 5);
		FF(c, d, e, f, g, h, a, b, j + 6);
		FF(b, c, d, e, f, g, h, a, j + 7);
	}

	ChainVar[0] += a;
	ChainVar[1] += b;
	ChainVar[2] += c;
	ChainVar[3] += d;
	ChainVar[4] += e;
	ChainVar[5] += f;
	ChainVar[6] += g;
	ChainVar[7] += h;
}

//*********************************************************************************************************************************
// o SHA256_Init()		: 연쇄변수와 길이변수를 초기화하는 함수
// o 입력				: Info		-  SHA-256 구조체의 포인터 변수
// o 출력				: 
//*********************************************************************************************************************************
void SHA256_Init( OUT SHA256_INFO *Info )
{
	Info->uChainVar[0] = 0x6a09e667;
	Info->uChainVar[1] = 0xbb67ae85;
	Info->uChainVar[2] = 0x3c6ef372;
	Info->uChainVar[3] = 0xa54ff53a;
	Info->uChainVar[4] = 0x510e527f;
	Info->uChainVar[5] = 0x9b05688c;
	Info->uChainVar[6] = 0x1f83d9ab;
	Info->uChainVar[7] = 0x5be0cd19;

	Info->uHighLength = Info->uLowLength = Info->remain_num = 0;
}

//*********************************************************************************************************************************
// o SHA256_Process()	: 임의의 길이를 가지는 입력 메시지를 512 비트 블록 단위로 나누어 압축함수를 호출하는 함수
// o 입력				: Info		 - SHA-256 구조체의 포인터 변수
//						  pszMessage - 입력 메시지의 포인터 변수
//						  uDataLen	 - 입력 메시지의 바이트 길이
// o 출력				: 
//*********************************************************************************************************************************
void SHA256_Process( OUT SHA256_INFO *Info, IN const BYTE *pszMessage, IN Uint uDataLen )
{

	Uint remain_buffer = Info->remain_num;

	if ((Info->uLowLength += (uDataLen << 3)) < 0)
		Info->uHighLength++;

	Info->uHighLength += (uDataLen >> 29);

	while ((uDataLen + remain_buffer) >= SHA256_DIGEST_BLOCKLEN)
	{
		memcpy((UCHAR_PTR)(Info->szBuffer + remain_buffer), pszMessage, (SINT)SHA256_DIGEST_BLOCKLEN);
		SHA256_Transform((Ulong_PTR)Info->szBuffer, Info->uChainVar);
		pszMessage += (SHA256_DIGEST_BLOCKLEN - remain_buffer);
		uDataLen -= (SHA256_DIGEST_BLOCKLEN - remain_buffer);
		remain_buffer = 0;
	}

	memcpy((UCHAR_PTR)(Info->szBuffer + remain_buffer), pszMessage, uDataLen);
	Info->remain_num = remain_buffer + uDataLen;
}

//*********************************************************************************************************************************
// o SHA256_Close()		: 메시지 덧붙이기와 길이 덧붙이기를 수행한 후 마지막 메시지 블록을 가지고 압축함수를 호출하는 함수
// o 입력				: Info	    - SHA-256 구조체의 포인터 변수
//						  pszDigest	- SHA-256 해쉬값을 저장할 포인터 변수
// o 출력				:
//*********************************************************************************************************************************
void SHA256_Close( OUT SHA256_INFO *Info, IN BYTE *pszDigest )
{
	Ulong i, Index;

	Index = (Info->uLowLength >> 3) % SHA256_DIGEST_BLOCKLEN;
	Info->szBuffer[Index++] = 0x80;

	if (Index > SHA256_DIGEST_BLOCKLEN - 8)
	{
		memset((UCHAR_PTR)Info->szBuffer + Index, 0, (SINT)(SHA256_DIGEST_BLOCKLEN - Index));
		SHA256_Transform((Ulong_PTR)Info->szBuffer, Info->uChainVar);
		memset((UCHAR_PTR)Info->szBuffer, 0, (SINT)SHA256_DIGEST_BLOCKLEN - 8);
	}
	else
		memset((UCHAR_PTR)Info->szBuffer + Index, 0, (SINT)(SHA256_DIGEST_BLOCKLEN - Index - 8));

#if defined(LITTLE_ENDIAN)
	Info->uLowLength = ENDIAN_REVERSE_Ulong(Info->uLowLength);
	Info->uHighLength = ENDIAN_REVERSE_Ulong(Info->uHighLength);
#endif

	((Ulong_PTR)Info->szBuffer)[SHA256_DIGEST_BLOCKLEN / 4 - 2] = Info->uHighLength;
	((Ulong_PTR)Info->szBuffer)[SHA256_DIGEST_BLOCKLEN / 4 - 1] = Info->uLowLength;

	SHA256_Transform((Ulong_PTR)Info->szBuffer, Info->uChainVar);

	for (i = 0; i < SHA256_DIGEST_VALUELEN; i += 4)
		BIG_D2B((Info->uChainVar)[i / 4], &(pszDigest[i]));
}

void SHA256_Encrpyt( IN const BYTE *pszMessage, IN Uint uPlainTextLen, OUT BYTE *pszDigest )
{
	SHA256_INFO info;

	SHA256_Init( &info );

	SHA256_Process( &info, pszMessage, uPlainTextLen );

	SHA256_Close( &info, pszDigest );
}