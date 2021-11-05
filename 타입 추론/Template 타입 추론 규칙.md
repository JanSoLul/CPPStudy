## Template 타입 추론 규칙

* **template 타입 추론 도중에 reference 타입의 인수들은 비참조로 취급된다. 즉, 참조성이 무시된다.**

  ​	함수 template의 선언은 대체로 이런 모습이다.

  ```c++
  template<typename T>
  void f(ParamType param);
  ```

  ​	그리고 이를 호출하는 코드는 대체로 이런 모습이다.

  ```c++
  f(expr);	// 어떤 표현식으로 f를 호출
  ```

  ​	compile 도중 compiler는 expr을 이용해서 두 가지 타입을 추론하는데, 하나는 T에 대한 타입 추론이고, 또 하나는 ParamType에 대한 타입 추론이다.

  ​	이 두 타입이 다른 경우가 많은데, 이는 ParamType에 흔히 const나 참조 한정사(&나 &&)같은 수식어들이 붙기 때문이다. 예를 들어 template 선언이 다음과 같다고 하자.

  ```c++
  template<typename T>
  void f(const T&  param);
  ```

   그리고 이를 다음과 같이 호출한다고 하자.

  ```c++
  int x = 0;
  f(x);
  ```

  ​	이 경우 T는 int로 추론되나 ParamType은 const int&로 추론된다.

  ​	T에 대해 추론된 타입은 expr의 타입 뿐만 아니라 ParamType의 형태에도 의존한다. 그 형태에 따라 총 세 가지 경우로 나뉜다.

  1. ParamType이 pointer 또는 참조 타입이지만 보편 참조(Universal Reference)는 아닌 경우
  2. ParamType이 보편 참조(Universal Reference)인 경우
  3. ParamType이 pointer도 아니고 참조도 아닌 경우

  * **Case 1: ParamType이  pointer 또는 참조 타입이지만 보편 참조는 아니다.**
    1. 만일 expr이 참조 타입이면 참조 부분을 무시한다.
    2. 그런 다음 expr의 타입을 ParamType에 대해 patter-matching 방식으로 대응시켜서 T의 타입을 결정한다.

  ```c++
  template <typename T>
  void f(T& param);		// param은 참조 타입
  
  int x = 27;
  const int cx = x;
  const int& rx = x;
  
  f(x);					// T는 int, param의 타입은 int&
  f(cx);					// T는 const int, param의 타입은 const int&
  f(rx);					// T는 const int, param의 타입은 const int&
  ```

  

  ​	둘째, 셋째 호출에서 cx와 rx에 const 값이 대입되었기 때문에 T는 const int로 추론된다.

  ​	셋째 호출에서, 비록 rx의 타입이 참조이지만 T는 비참조로 추론되었다!

  ------------

  ​	f의 매개변수의 타입을 T&에서 const T&로 바꾸면 상황이 조금 달라진다.

  ```c++
  template <typename T>
  void f(const T& param);	// 이제는 param이 const에 대한 참조
  
  int x = 27;
  const int cx = x;
  const int& rx = x;
  
  f(x);					// T는 int, param의 타입은 const int&
  f(cx);					// T는 int, param의 타입은 const int&
  f(rx);					// T는 int, param의 타입은 const int&
  ```

  ​	이전처럼, 타입 추론 과정에서 rx의 참조성은 무시된다.

  ---

  ​	param이 참조가 아니라 pointer(또는 const를 가리키는 pointer)라도 타입 추론은 본질적으로 같은 방식으로 진행된다.

  ```c++
  template <typename T>
  void f(T* param);		// 이번에는 param이 pointer
  
  int x = 27;
  const int *px = &x;
  
  f(&x);					// T는 int, param의 타입은 int*
  f(px);					// T는 const int, param의 타입은 const int*
  ```

  ---

  **보편 참조(universal reference) 매개변수에 대한 타입 추론 과정에서 L-Value 인수들은 특별하게 취급된다.**

  ​		※ **L-Value**: 표현식 이후에도 사라지지 않는 값. 이름을 지닌 변수

  ​		※ **R-Value**: 표현식 이후에는 사라지는 값. 임시 변수

  * **Case 2: ParamType이 보편 참조(universal reference, &&)인 경우**
    1. 만일 expr이 L-Value이면 T와 ParamType 둘 다 L-Value 참조로 추론된다. 이는 이중으로 비정상적인 상황이다. 첫째로, template 타입 추론에서 T가 참조 타입으로 추론되는 경우는 이것이 유일하다. 둘째로, ParamType의 선언 구문은 R-Value 참조와 같은 모습이지만, 추론된 타입은 L-Value 참조이다.
    2. 만일 expr이 R-Value이면, '정상적인'(즉, Case 1)규칙들이 적용된다.

  ```c++
  template <typename T>
  void f(T&& param);			// 이번에는 param이 보편 참조
  
  int x = 27;
  const int cx = x;
  const int& rx = x;
  
  f(x);						// x는 L-Value, 따라서 T는 int&, param의 타입 역시 int&
  f(cx);						// cx는 L-Value, 따라서 T는 const int&, param의 타입 역시 
  							//const int&
  f(rx);						// rx는 L-Value, 따라서 T는 const int&, param의 타입 역시
  							//const int&
  f(27);						// 27은 R-Value, 따라서 T는 int, 그러므로 param의 타입은 int&&
  ```

  ​	보편 참조가 관여하는 경우에는 L-Value 인수와 R-Value 인수에 대해 서로 다른 추론 규칙들이 적용된다.

  ​	보편 참조가 아닌 매개변수들에 대해서는 그런 일이 절대 발생하지 않는다.

  ---

  **값 전달 방식의 매개변수에 대한 타입 추론 과정에서 const 또는 volatile(또는 그 둘 다인) 인수는 비 const, 비 volatile 인수로 취급된다.**

  * **Case 3: ParamType이 포인터도 아니고 참조도 아님**

    ​	ParamType이 포인터도 아니고 참조도 아니라면, 인수가 함수에 값으로 전달(pass by value)되는 상황인 것이다. 따라서 param은 주어진 인수의 본사본, 즉 완전히 새로운 객체이다.

    ​	param이 새로운 객체라는 사실 때문에, expr에서 T가 추론되는 과정에서 다음과 같은 규칙들이 적용된다.

    1. 이전처럼, 만일 expr의 타입이 참조이면, 참조 부분은 무시된다.
    2. expr의 참조성을 무시한 후, 만일 expr이 const이면 그 const 역시 무시한다. 만일 volatile이면 그것도 무시한다.

  ```c++
  template <typename T>
  void f(T param);		// 이번에는 param이 값으로 전달된다.
  
  int x = 27;
  const int cx = x;
  cosnt int& rx = x;
  
  f(x);					// T와 param의 타입은 둘 다 int
  f(cx);					// T와 param의 타입은 둘 다 int
  f(rx);					// T와 param의 타입은 둘 다 int
  ```

  ​	cx와 rx가 const 값을 지정하지만, 그래도 param은 const가 아니다.

  ​	param은 cx나 rx의 복사본이므로, 다시 말해 param은 cx나 rx와는 완전히 독립적인 객체이므로 이는 당연한 결과이다.

  ​	여기서 알아야할 것은 const가 값 전달 매개변수에 대해서만 무시된다는 점이다.

  ​	expr이 const 객체를 가리키는 cosnt 포인터이고 param에 값으로 전달되는 경우는 어떨까?

  ```c++
  template <typename T>
  void f(T param);		// 인수는 param에 여전히 값으로 전달된다.
  
  // ptr은 const 객체를 가리키는 const 포인터
  const char* const ptr = "Fun with pointers";
  f(ptr);					// const char* const 타입의 인수를 전달	
  ```

  ​	이 경우 포인터 자체(ptr)는 값으로 전달된다. 따라서 ptr의 상수성은 무시된다.

  ​	하지만, ptr이 다리키는 객체(여기서는 문자열)의 상수성은 여전히 보존된다.

  ​	그 결과 T와 param의 타입은 둘 다 const char*로 추론된다.

  ---

  * **template 타입 추론 과정에서 배열이나 함수 이름에 해당하는 인수는 포인터로 취급된다. 단, 그런 인수가 참조를 초기화하는데 쓰이는 경우에는 포인터로 취급되지 않는다.**

    ​	배열 타입의 함수 매개변수라는 것은 없다. 물론 다음과 같은 구문 자체는 적법하다.

    ```c++
    void myFunc(int param[]);
    ```

    ​	그러나 이 경우 배열 선언은 하나의 포인터 선언으로 취급된다. 즉, myFunc의 선언은 사실상 다음과 같은 의미이다.

    ```c++
    void myFunc(int* param);
    ```

    ​	이처럼 배열 매개변수 선언이 포인터 매개변수처럼 취급되므로, template 함수에 값으로 전달되는 배열의 타입은 포인터 타입으로 추론된다.

    ​	즉, template f의 호출에서 타입 매개변수 T는 const char*로 추론된다.

    ```c++
    template <tyepname T>
    void f(T param);
    
    const char name[] = "J. P. Briggs";
    
    f(name);
    ```

    ​	name은 배열이지만 T와 param은 const char*로 추론된다.

    ​	그런데 한 가지 교묘한 요령이 있다. 비록 함수의 매개변수를 진짜 배열로 선언할 수는 없지만, 배열에 대한 참조로 선언할 수는 있다.

    ```c++
    template <typename T>
    void f(T& param);
    
    f(name);				// 배열을 f에 전달
    ```

    ​	이렇게하면 T에 대해 추론된 타입은 배열의 실제 타입이 된다. 그 탕비은 배열의 크기를 포함하므로, 이 예에서 T는 const char [13]으로 추론되고 f의 매개변수(그 배열에 대한 참조)의 타입은 const char (&)[13]으로 추론된다.

    ​	흥미롭게도, 배열에 대한 참조를 선언하는 능력을 이용하면 배열에 담긴 원소들의 개수를 추론하는 template을 만들 수 있다.

    ```c++
    // 배열의 크기를 compile 시점 상수로서 돌려주는 template 함수
    // (배열 매개변수에 이름을 붙이지 않은 것은 이 template에 필요한 것은 배열에 담신 원소의 개수이기 때문이다.)
    template <typename T, std::size_t N>
    constexpr std::size_t arraySize(T (&)[N]) noexcept{
        return N;
    }
    // 이 함수를 constexpr로 선언하면 함수 호출의 결과를 compile 도중에 사용할 수 있게 된다.
    // 그러면 다음 예처럼 중괄호 초기화 구문으로 정의된, 기존 배열과 같은 크기의 새 배열을 선언하는 것이 가능해진다.
    // arraySize를 noexcept로 선언한 것은 compiler가 더 나은 코드를 산출하는 데 도움을 주려는 것이다.
    
    int keyVals[] = {1, 3, 7, 9, 11, 22, 35};		// keyVals의 원소 개수는 7개
    
    std::array<int, arraySize(keyVals)> mappedVals;	// mappedVals의 크기는 7
    ```

    + **함수 인수**

      ​	C++에서 포인터로 붕괴하는 것이 배열만은 아니다. 함수 타입도 함수 포인터로 취급될 수 있으며, 지금까지 배열에 대한 타입 추론과 관련해서 논의한 모든 것은 함수에 대한 타입 추론에, 그리고 함수 포인터로 취급되는 것에 적용된다.

    ```c++
    void sumeFunc(int, double);
    
    template <typename T>
    void f1(T param);		// f1의 param은 값 전달 방식
    
    template <typename T>
    void f2(T& param);		// f2의 param은 참조 전달 방식
    
    f1(someFunc);			// param은 함수 포인터로 추론됨
    						// 타입은 void (*)(int, double)
    f2(someFunc);			// param은 함수 참조로 추론됨
    						// 타입은 void (&)(int, double)
    ```

    ​	실제 응용에서 이 점 때문에 뭔가 달라지는 경우는 드문지만, 배열에서 포인터로의 붕괴도 알고 있다면 함수에서 포인터로의 붕괴도 알아 두는 것이 좋다.
