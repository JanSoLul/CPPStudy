// 보편 참조 (universal reference) parameter에 대한 type 추론 과정에서 
// L-Value 인수들은 특별하게 취급된다.
// ※ L-Value: 표현식 이후에도 사라지지 않는 값. 이름을 지니는 변수
// ※ R-Value: 표현식 이후에는 사라지는 값. 임시 변수

// Case 2: ParamType이 보편 참조 (universal reference, &&)인 경우
/**
/* 1. 만일 expr이 L-Value이면 T와 ParamType 둘 다 L-Value 참조로 추론된다.
/*	이는 이중으로 비정상적인 상황이다. 첫째로, template type 추론에서 T가
/*  참조 타입으로 추론되는 경우는 이것이 유일하다. 둘째로, ParamType의 선언
/*  구문은 R-Value 참조와 같은 모습이지만, 추론된 타입은 L-Value 참조이다.
/*  
/* 2. 만일 expr이 R-Value이면, 정상적인(Case 1) 규칙들이 적용된다.
*/

template <typename T>
void f(T&& param);		// param이 보편 참조 (universal reference)

int x = 27;
const int cx = x;
const int& rx = x;

f(x);					// x는 L-Value, 따라서 T는 int&, param의 타입 역시 int &
f(cx);					// cx는 L-Value, 
						// 따라서 T는 const int&, param의 타입 역시 const int &
f(rx);					// rx는 L-Value, 
						// 따라서 T는 const int&, param의 타입 역시 const int &
f(27);					// 27은 R-Value, 따라서 T는 int, param의 타입은 int&&

// 보편 참조(universal reference)가 관여하는 경우에는 L-Value 인수와 R-Value에 대해
// 서로 다른 추론 규칙들이 적용된다.

// 보편 참조(universal reference)가 아닌 parameter에 대해서는 그런 일이 절대 발생하지 않는다.