// Template type 추론 도중에 참조 type의 인수들은 비참조로 취급된다 -> 참조성이 무시된다.

/*
함수 template의 선언은 대체로 이런 모습이다.
template<typename T>
void f(ParamType param);

그리고 이를 호출하는 코드는 대체로 이런 모습이다.
f(expr);	// 어떤 표현식으로 f를 호출
*/

// compile 도중 compiler는 expr을 이용해서 두 가지 type을 추론하는데,
// 하나는 T에 대한 type 추론이고, 하나는 ParamType에 대한 type 추론이다.
// 이 두가지 type이 다른 경우가 많은데, 이는 ParamType에 흔히 const나 참조 한정(& or &&)
// 같은 수식어들이 붙기 때문이다.

// Case 1: ParamType이 pointer or 참조 type이지만 보편 참조 (universal reference)는 아닌 경우
// 1. 만일 expr이 참조 type이면 참조 부분을 무시한다.
// 2. 그런 다음 expr의 type을 ParamType에 대해 pattern-matching 방식으로 대응시켜서 T의 type을 결정한다.
namespace ReferenceType{
	// 함수 template
	// param은 reference type
	template <typename T>
	void f(T& param) {}

	int x = 27;
	const int cx = x;	// cx는 x의 const int
	const int& rx = x;	// rx는 const int인 x에 대한 참조

	// 여러 호출에서 param과 T에 대한 추론된 타입
	f(x);				// T는 int, param의 type은 const int&
	f(cx);				// T는 const int, param의 type은 const int&
	f(rx);				// T는 const int, param의 type은 const int&

	// 2, 3번째 호출에서 cx와 rx에 const 값이 지정되었기 때문에 T는 const int로 추론된다.
	// 3번째 호출에서 rx의 type이 const int& 이지만 T는 비참조로 추론되었다.
}

namespace ConstReferenceType {
	// f의 param type을 T&에서 const T&로 바꾸면 상황이 조금 달라진다.
	template <typename T>
	void f(T& param) { }

	int x = 27;
	const int cx = x;	// cx는 x의 const int
	const int& rx = x;	// rx는 const int인 x에 대한 참조

	f(x);				// T는 int, param의 type은 const int&
	f(cx);				// T는 int, param의 type은 const int&
	f(rx);				// T는 int, param의 type은 const int&

	// 이전처럼 타입 추론에서 rx의 참조성은 무시된다.
}

namespace PointerType {
	// param이 참조가 아니라 pointer(or const를 가리키는 pointer)라도 
	// type 추론은 기본적으로 같은 방식으로 진행된다.
	template <typename T>
	void f(T* param);

	int x = 27;
	const int* px = &x;	// px는 const int로서의 x를 가리키는 pointer

	f(&x);				// T는 int, param type은 int *
	f(px);				// T는 const int, param type은 cont int *
}
