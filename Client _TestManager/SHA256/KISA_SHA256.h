/**
@file KISA_SHA_256.h
@brief SHA256 암호 알고리즘
@author Copyright (c) 2013 by KISA
@remarks http://seed.kisa.or.kr/
*/

#ifndef SHA256_H
#define SHA256_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#undef BIG_ENDIAN
#undef LITTLE_ENDIAN

#if defined(USER_BIG_ENDIAN)
	#define BIG_ENDIAN
#elif defined(USER_LITTLE_ENDIAN)
	#define LITTLE_ENDIAN
#else
	#if 0
		#define BIG_ENDIAN
	#elif defined(_MSC_VER)
		#define LITTLE_ENDIAN
	#else
		#error
	#endif
#endif

typedef unsigned long Ulong;
typedef Ulong* Ulong_PTR;

typedef unsigned int Uint;
typedef Uint* Uint_PTR;

typedef signed int SINT;
typedef SINT* SINT_PTR;

typedef unsigned char UCHAR;
typedef UCHAR* UCHAR_PTR;

typedef unsigned char BYTE;

#define SHA256_DIGEST_BLOCKLEN	64
#define SHA256_DIGEST_VALUELEN	32

typedef struct{
	Uint uChainVar[SHA256_DIGEST_VALUELEN / 4];
	Uint uHighLength;
	Uint uLowLength;
	Uint remain_num;
	BYTE szBuffer[SHA256_DIGEST_BLOCKLEN];
} SHA256_INFO;

/**
@brief 연쇄변수와 길이변수를 초기화하는 함수
@param Info : SHA256_Process 호출 시 사용되는 구조체
*/
void SHA256_Init( OUT SHA256_INFO *Info );

/**
@brief 연쇄변수와 길이변수를 초기화하는 함수
@param Info : SHA256_Init 호출하여 초기화된 구조체(내부적으로 사용된다.)
@param pszMessage : 사용자 입력 평문
@param inLen : 사용자 입력 평문 길이
*/
void SHA256_Process( OUT SHA256_INFO *Info, IN const BYTE *pszMessage, IN Uint uDataLen );

/**
@brief 메시지 덧붙이기와 길이 덧붙이기를 수행한 후 마지막 메시지 블록을 가지고 압축함수를 호출하는 함수
@param Info : SHA256_Init 호출하여 초기화된 구조체(내부적으로 사용된다.)
@param pszDigest : 암호문
*/
void SHA256_Close( OUT SHA256_INFO *Info, OUT BYTE *pszDigest );

/**
@brief 사용자 입력 평문을 한번에 처리
@param pszMessage : 사용자 입력 평문
@param pszDigest : 암호문
@remarks 내부적으로 SHA256_Init, SHA256_Process, SHA256_Close를 호출한다.
*/
void SHA256_Encrpyt( IN const BYTE *pszMessage, IN Uint uPlainTextLen, OUT BYTE *pszDigest );

#ifdef  __cplusplus
}
#endif

#endif