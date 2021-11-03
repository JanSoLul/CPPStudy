// 값 전달 방식의 parameter에 대한 타입 추론 과정에서 const 또는 volatile(또는 그 둘 다인)
// 인수는 비 const, 비 volatile 인수로 취급된다.
// Case 3: ParamType이 pointer도 아니고 참조도 아니다.
// ParamType이 pointer도 아니고 참조도 아니라면, 인수가 함수에 값으로 전달 (pass-by-value)되는
// 상황인 것이다. 따라서 param은 주어진 인수의 복사본, 즉 완전히 새로운 객체이다.
// param이 새로운 객체라는 사실때문에, expr에서 T가 추론되는 과정에서 다음과 같은 규칙들이 적용된다.
// 1. 이전처럼, 만일 expr 타입이 참조이면, 참조 부분은 무시된다.
// 2. expr의 참조성을 무시한 후, 만일 expr이 const이면 그 const 역시 무시한다.
//	  만일 volatile이면 그것도 무시한다.

template <typename T>
void f(T param);		// 이번에는 param이 값으로 전달된다.

int x = 27;
const int cx = x;
const int& rx = x;

f(x);					// T와 param의 타입은 둘 다 int
f(cx);					// 여전히 T와 param의 타입은 둘 다 int
f(rx);					// 이번에도 T와 param의 타입은 둘 다 int

// cx와 rx가 const 값을 지정하지만 그래도 param은 const가 아니다.
// 
// param은 cx나 rx의 복사본이므로, 다시 말해 param은 cx나 rx와는 완전히 독립적인 객체이므로,
// 이는 당연한 결과이다.
// 
// 여기서 명실할 것은, const가(그리고 volatile이) 값 전달 parameter에 대해서만 무시된다는 점이다.
//
// expr이 const 객체를 가리키는 cosnt 포인터이고, param에 값으로 전달되는 경우는 어떨까?
const char* const ptr = "Fun with pointers";	// ptr은 const 객체를 가리키는 const pointer
f(ptr);											// const char * const 타입의 인수를 전달

// 이 경우 pointer 자체(ptr)는 값으로 전달된다. 따라서 ptr의 상수성은 무시된다.
// 하지만, ptr이 가리키는 객체(여기서는 문자열)의 상수성은 여전히 보존된다.
// 그 결과 T와 param의 타입은 둘다 const char*로 추론된다. 