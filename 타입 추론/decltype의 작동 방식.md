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