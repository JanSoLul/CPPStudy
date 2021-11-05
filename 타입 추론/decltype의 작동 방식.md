## decltype의 작동 방식

* **decytype은 항상 변수나 표현식의 타입을 아무 수정 없이 보고한다.**

  ​	C++에서는 decltype은 함수의 return 타입이 그 parameter 타입들에 의존하는 함수 template을 선언할 때 주로 쓰인다.

  ​	예를 들어 container 하나와 index 하나를 받고, 우선 사용자를 인증한 후 operator[]를 통해서 container의 한 요소를 돌려주는 함수를 작성한다고 하자.

  ​	그러면 함수의 return 타입은 반드시 그러한 operator[]의 return 타입과 동일해야 한다. 즉 container의 operator[]의 return 타입이 container에 따라 다를 수 있다.

  ​	decltype을 이용하면 그런 함수의 반환 타입을 손쉽게 표현할 수 있다.

  ```c++
  template <typename Container, typename Index>
  auto authAndAccess(Container& c, Index i) -> decltype(c[i]){
      authenticateUser();
      return c[i];
  }
  ```

  ​	이름 앞에 auto를 지정하는 것은 타입 추론과는 아무런 관력이 없다.

  ​	이 auto는 C++의 trailing return type 구문이 쓰인다는 것을 나타낼 뿐이다.

  ​	**※trailing return type** : 함수의 return 타입을 parameter 목록 다음에 ("->" 다음 위치에) 쓰인다는 것을 나타낼 뿐이다.

  ​	만일 통상적인 방식으로 함수 이름 앞에서 return 타입을 지정한다면, 아직 선언되지 않았으므로 c와 i는 사용할 수 없다.

  

* **C++14는 decltype(auto)를 지원한다. decltype(auto)는 auto처럼 초기치로부터 타입을 추론하지만, 그 타입 추론 관정에서 decltype의 규칙들을 적용한다.**

  ​	C++11은 lambda 함수가 한 문장으로 이루어져 있다면 그 return 타입의 추론을 허용하며, C++14는 허용 범위를 더욱 확장해서 모든 lambda와 모든 함수의 return 타입 추론을 허용한다. (심지어 return 문이 여러개인 함수도 허용한다.[단, 모든 return 문의 타입 추론 결과가 일치해야 한다.])

  ​	따라서, authAndAccess(위의 예제에 등장한 함수)의 경우 C++14에서는 trailing return type을 생략하고 그냥 함수 이름 앞의 auto만 남겨 두어도 된다.

  ​	그런 형태의 선언에서는 실제로 auto가 타입 추론이 일어남을 뜻하는 용도로 쓰인다.(compiler가 함수의 구현으로부터 함수의 return 타입을 추론한다.)
  
  ​	함수의 return 타입에 auto가 지정되어 있으면 compiler는 template 타입 추론을 적용한다. 지금 예에서는 그것이 문제가 된다. 이러한 문제를 해결하기 위해 C++14에서는 decltype(auto) 지정자가 도입되었다. 언뜻 보면 모순처럼 보이지만, 좀 더 생각해보면 합당하다.
  
  ​	auto는 해당 타입이 추론되어야 함을 뜻하고, decltype은 그 추론 과정에서 decltype 타입 추론 규칙들이 적용되어야 함을 뜻한다.
  
  ```c++
  template <typename Container, typename Index>
  auto authAndAccess(Container& c, Index i){
      authenticateUser();
      return c[i];					//return 타입은 c[i]로부터 추론된다.
  }
  
  // T객체들을 담은 container에 대한 operator[] 연산은 대부분의 경우에는 T&를 돌려준다.
  // 문제는 template 타입 추론 과정에서 초기화 표현식의 참조성이 무시된다는 점이다.
  
  std::deque<int> d;
  authAndAccess(d, 5) = 10;			// 사용자를 인증하고, d[5]를 돌려주고,
  									// 그런 다음 10을 d[5]에 대입한다.
  									// 이 code는 compile되지 않는다.
  // 여기서 d[5]는 int&를 돌려주나, authAndAccess에 대한 auto return 타입 추론 과정에서 참조가 제거되기 때문에 결국 return 타입은 int가 된다.
  // 이런 문제를 해결하기 위해 decltype(auto) 지정자를 사용할 수 있다.
  
  template <typename Container, typename Index>
  decltype(auto) autoAndAccess(Container& c, Index i){
      authenticateuser();
      return c[i];
  }
  // 이제 authAndAccess의 return 타입은 실제로 c[i]의 return 타입과 일치한다.
  ```
  
  ​	decltype(auto)를 함수 return 타입에만 사용할 수 있는 것은 아니다.
  
  ​	변수를 선언할 때에도, 초기화 표현식에 decltype 타입 추론 규칙들을 적용하고 싶은 경우라면 이 지정자가 유용하다.
  
  ```c++
  Widget w;
  const Widget& cw = w;
  auto myWidget1 = cw;			// auto 타입 추론: myWidget1의 타입은 Widget
  decltype(auto) myWidget2 = cw;	// decltype 타입 추론: myWidget2의 타입은 const Widget&
  ```
  
  ​	위에서 언급하고 설명하지 않은 authAndAccess를 다듬는 방법에 대해서 알아보자.
  
  ```c++
  template <typename Container, typename Index>
  decltype(auto) authAndAccess(Container& c, Index i);
  
  /*
  container c는 비 const 객체에 대한 L-Value 참조로서 함수에 전달된다.
  이는 함수가 돌려준 container 요소를 client(함수를 호출한 쪽)가 수정할 수 있게 하기 위한 것이다.
  문제는, 이 때문에 함수에 R-Value Container는 전달할 수 없다는 것이다.
  R-Value를 L-Value 참조에 묶을 수는 없다. (const에 대한 L-Value 참조에는 묶을 수 있지만 지금은 그런 경우가 아니다.)
  R-Value도 넘길 수 있게 하려면, 보편 참조를 사용해야 한다.
  */
  
  template <typename Container, typename Index>
  decltype(auto) authAndAccess(Container&& c, Index i){
      authenticateUser();
      return std::forward<Container>(c)[i];
  }
  ```
  
  

* **decltype은 타입이 T이고 이름이 아닌 L-Value 표현식에 대해서는 항상 T& 타입을 보고한다.**

  ​	decltype을 이름에 적용하면 그 이름에 대해 선언된 타입이 산출된다. 대체로 이름은 L-Value 표현식이나, 그 점이 decltype의 행동에 영향을 주지는 않는다.

  ​	그런데 이름보다 복잡한 L-Value 표현식에 대해서는 일반적으로 decltype이 항상 L-Value 참조를 보고한다. 즉, 이름이 아닌, 그리고 타입이 T인 어떤 좌측값 표현식에 대해 decltype은 T&를 보고한다.

  ​	어차피 대부분의 L-Value 표현식에는 태생적으로 L-Value 참조가 포함되어 있으므로, 이 점 때문에 뭔가가 달라지는 경우는 드물다. 그러나 이러한 작동 방식이 뭔가 차이를 만드는 경우가 있기는 있다.

  ​	다음 코드에서,

  ```c++
  int x = 0;
  ```

  ​	x는 변수의 이름이므로 decltype(x)는 int이다. 그러나 x를 괄호로 감싸서 "(x)"를 만들면 이름보다 복잡한 표현식이 된다. 이름으로서의 x는 하나의 L-Value이며, C++는 (x)라는 표현식도 L-Value로 정의한다.

  ​	따라서 decltype((x))는 int&이다. 이름을 괄호로 감싸면 decltype이 보고하는 타입이 달라지는 것이다.

  ​	C++11에서는 이것이 그냥 드물게 만나는 신기한 현상 정도이다. 그러나 decltype(auto)를 지원하는 C++14에서는 return문 작성 습관의 사소한 차이 때문에 함수의 return 타입 추론 결과가 달라지는 사태가 벌어질 수 있다.

  ```c++
  decltype(auto) f1(void){
      int x = 0;
      return x;		// decltype(x)는 int이므로 f1은 int를 반환
  }
  
  decltype(auto) f2(void){
      int x = 0;
      return (x);		// decltype((x))는 int&이므로 f2는 int&를 반환
  }
  ```

  ​	f2가 f1과는 다른 타입을 돌려준다는 점 뿐만아니라 자신의 지역 변수에 대한 참조를 돌려준다는 점도 주목해야 한다.

  ​	이 예의 주된 교훈은 decltype(auto)는 아주 조심해서 사용해야 한다는 것이다. 그렇기만 보통의 경우 decltype은 기대한 바로 그 타입을 산출한다. 특히 decltype을 이름에 적용할 때에는 반드시 그렇다. 이 경우 decltype은 주어진 이름의 선언된 타입(declared type)을 돌려준다.