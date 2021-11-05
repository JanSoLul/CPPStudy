## auto의 타입 추론 규칙

* **auto 타입 추론은 대체로 template 타입 추론과 같지만, auto 타입 추론은 중괄호 초기치가 std::initializer_list 를 나타낸다고 가정하는 반면 template 타입 추론은 그렇지 않다는 차이가 있다.**

  ​	auto를 이용해서 변수를 선언할 때 auto는 template의 T와 동일한 역할을 하며, 변수의 타입 지정자(type specifier)는 ParamType과 동일한 역할을 한다.

  ```c++
  auto x = 27;		// 여기서 x의 타입 지정자는 그냥 auto 자체이다.
  const auto cx = x;	// 타입 지정자는 const auto이다.
  const auto& rx = x;	// 타입 지정자는 const auto&이다.
  
  // 이 예들에서 x, cx, rx의 타입들을 추론할 때, compiler는 마치 선언마다 template 함수 하나와 해당 초기화 표현식으로 그 template 함수를 호출하는 구문이 존재하는 것처럼 행동한다.
  
  template <typename T>
  void func_for_x(T param);		// x의 타입을 추론하기 위한 개념적인 template
  
  func_for_x(27);					// 개념적인 호출: param에 대해 추론된 타입이
  								//바로 x의 타입이다.
  
  template <typename T>
  void func_for_cx(const T param);// cx의 타입을 추론하기 위한 개념적인 template
  
  func_for_cx(x);					// 개념적인 호출: param에 대해 추론된 타입이
  								//곧 cx의 타입이다.
  ```

  * **Case 1:** 타입 지정자가 포인터나 참조 타입이지만 보편 참조는 아닌 경우.

  * **Case 2:** 타입 지정자가 보편 참조인 경우

  * **Case 3:** 타입 지정자가 포인터도 아니고 참조도 아닌 경우.

    Case 1과 Case 3의 예는 위에서 이미 보았다.

  ```c++
  auto x = 27;			// Case 3 (x는 포인터도 아니고 참조도 아님)
  const auto cx = x;		// Case 3 (cx 역시 둘 다 아님)
  cosnt auto& rx = x;		// Case 1 (rx는 보편 참조가 아닌 참조)
  
  auto&& uref1 = x;		// x는 int이고 L-Value이므로
  						// uref1의 타입은 int&
  auto&& uref2 = cx;		// cx는 const int이고 L-Value이므로
  						// uref2의 타입은 const int&
  auto&& uref3 = 27;		// 27은 int이고 R-Value이므호
  						// uref3의 타입은 int&&
  
  const char name[] = "R. N. Briggs";	// name의 타입은 const char [13]
  auto arr1 = name;					// arr1의 타입은 const char*
  auto& arr2 = name;					// arr2의 타립은 const char (&)[13]
  
  void some Func(int, double);
  
  auto func1 = someFunc;				// func1의 타입은 void (*)(int, double)
  auto& func2 = someFunc;				// func2의 타입은 void (&)(int, double)
  ```

  ​	이 예들에서 보듯이, auto의 타입 추론은 template 타입 추론과 똑같이 작동한다.

* **함수의 return 타입이나 lambda 매개변수에 쓰인 auto에 대해서는 auto 타입 추론이 아니라 template 타입 추론이 적용된다.**

  ​	C++14에서는 함수의 return 타입을 auto로 지정해서 compiler가 추론하게 만들 수 있으며, lambda의 매개변수 선언에 auto를 사용하는 것도 가능하다.

  ​	그러나 auto의 그러한 용법들에는 auto 타입 추론이 아니라 template 타입 추론의 규칙들이 적용된다.

  ​	그래서 중괄호 초기치를 돌려주는 함수의 return 타입을 auto로 지정하면 compiler이 실패한다. C++14 lambda의 매개변수 타입 명세에 auto를 사용하는 경우데ㅗ 마찬가지 이유로 compile이 실패한다.

  ```c++
  // return 타입이 auto인 경우
  auto createInitList(void){
      return {1, 2, 3};		// error! {1, 2, 3}의 타입을 추론할 수 없다.
  }
  
  // lambda의 매개변수 타입 명세에 auto를 사용하는 경우
  std::vector<int> v;
  auto resetV = [&v](const auto& newValue) {v= newValue;};
  resetV({1, 2, 3});			// error! {1, 2, 3}의 타입을 추론할 수 없다.
  ```

  