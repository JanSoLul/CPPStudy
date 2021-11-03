// Template 타입 추론 과정에서 배열이나 함수 이름에 해당하는 인수는 pointer로 취급한다.
// 단, 그런 인수가 참조를 초기화하는 데 쓰이는 경우에는 pointer로 취급하지 않는다.

// 배열 인수-> 배열 타입의 함수 parameter라는 것은 없다. 물론 다음과 같은 구문 자체는 가능하다.

void myFunc(int param[]);
// 그러나 이 경우 배열 선언은 하나의 pointer 선언으로 취급된다.
// 즉, myFunc의 선언은 사실상 다음과 같은 의미이다.

// void myFunc(int* param);
// 이처럼 배열 parameter 선언이 pointer parameter처럼 취급되므로, template 함수에 값으로
//전달되는 배열의 타입은 pointer 타입으로 추론된다.
/*
template <typename T>
void f(T param);
const char name[] = "J.P.Briggs";
f(name);
*/
// name은 배열이지만 T와 param은 const char*로 추론된다.
// 즉, template f의 호출에서 타입 parameter T는 const char*로 추론된다.

// 그런데 한 가지 교묘한 요령이 있다. 비록 함수의 parameter를 진짜 배열로 선언할 수는 없지만,
//배열에 대한 참조로 선언할 수는 있다.
/*
template <typename T>
void f(T& param);
const char name[] = "J.P.Briggs";
f(name);
*/
// 이렇게 하면 T에 대해 추론된 타입은 배열의 실제 타입이 된다! 그 타입은 배열의 크기를
//포함하므로, 이 예에서는 T는 const char [13]으로 추론되고 f의 parameter(그 배열에 대한 참조)의
//타입은 const char (&)[13]으로 추론된다.
// 흥미롭게도, 배열에 대한 참조를 선언하는 능력을 이용하면 배열에 담긴 원소들의 개수를 추론하는
//template을 만들 수 있다.

/** 배열의 크기를 compile 시점 상수로서 돌려주는 template함수
* (배열 매개변수에 이름을 붙이지 않은 것은, 이 template에 필요한 것은
* 배열에 담긴 원소의 개수뿐이기 때문이다.)
*/
#include <iostream>

template <typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept {
	return N;
}
// ※constexpr: 함수를 constexpr로 선언하면 함수 호출의 결과를 compile 도중에 사용할 
//수 있게 된다. 그러면 다음 예처럼 중괄호 초기화 구문으로 정의된 기존 배열과 같은 크기
//(원소 개수)의 새 배열을 선언하는 것이 사용할 수 있게 된다.
int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };	//keyVal의 원소 개수 7개

std::array<int, arraySize(keyVals)> mappedVals;

// arraySize를 noexcept로 선언한 것은 compiler가 더 나은 코드를 산출하는 데
//도움을 주려는 것이다.

// 함수 타입도 함수 pointer로 취급할 수 있으며, 지금까지 배열에 대한 타입 추론과 관련해서
//논의한 모든 것은 함수에 대한 타입 추록에도 적용된다.
void someFunc(int, double);

template <typename T>
void f1(T param);		// f1의 param은 call by value

template <typename T>
void f2(T& param);		// f2의 param은 call by reference

f1(someFunc);			// param은 함수 pointer로 추론된다.
						// 타입은 void (*)(int, double)
f2(someFunc);			// param은 함수 참조로 추론된다./
						// 타입은 void (&)(int, double)