// Template type �߷� ���߿� ���� type�� �μ����� �������� ��޵ȴ� -> �������� ���õȴ�.

/*
�Լ� template�� ������ ��ü�� �̷� ����̴�.
template<typename T>
void f(ParamType param);

�׸��� �̸� ȣ���ϴ� �ڵ�� ��ü�� �̷� ����̴�.
f(expr);	// � ǥ�������� f�� ȣ��
*/

// compile ���� compiler�� expr�� �̿��ؼ� �� ���� type�� �߷��ϴµ�,
// �ϳ��� T�� ���� type �߷��̰�, �ϳ��� ParamType�� ���� type �߷��̴�.
// �� �ΰ��� type�� �ٸ� ��찡 ������, �̴� ParamType�� ���� const�� ���� ����(& or &&)
// ���� ���ľ���� �ٱ� �����̴�.

// Case 1: ParamType�� pointer or ���� type������ ���� ���� (universal reference)�� �ƴ� ���
// 1. ���� expr�� ���� type�̸� ���� �κ��� �����Ѵ�.
// 2. �׷� ���� expr�� type�� ParamType�� ���� pattern-matching ������� �������Ѽ� T�� type�� �����Ѵ�.
namespace ReferenceType{
	// �Լ� template
	// param�� reference type
	template <typename T>
	void f(T& param) {}

	int x = 27;
	const int cx = x;	// cx�� x�� const int
	const int& rx = x;	// rx�� const int�� x�� ���� ����

	// ���� ȣ�⿡�� param�� T�� ���� �߷е� Ÿ��
	f(x);				// T�� int, param�� type�� const int&
	f(cx);				// T�� const int, param�� type�� const int&
	f(rx);				// T�� const int, param�� type�� const int&

	// 2, 3��° ȣ�⿡�� cx�� rx�� const ���� �����Ǿ��� ������ T�� const int�� �߷еȴ�.
	// 3��° ȣ�⿡�� rx�� type�� const int& ������ T�� �������� �߷еǾ���.
}

namespace ConstReferenceType {
	// f�� param type�� T&���� const T&�� �ٲٸ� ��Ȳ�� ���� �޶�����.
	template <typename T>
	void f(T& param) { }

	int x = 27;
	const int cx = x;	// cx�� x�� const int
	const int& rx = x;	// rx�� const int�� x�� ���� ����

	f(x);				// T�� int, param�� type�� const int&
	f(cx);				// T�� int, param�� type�� const int&
	f(rx);				// T�� int, param�� type�� const int&

	// ����ó�� Ÿ�� �߷п��� rx�� �������� ���õȴ�.
}

namespace PointerType {
	// param�� ������ �ƴ϶� pointer(or const�� ����Ű�� pointer)�� 
	// type �߷��� �⺻������ ���� ������� ����ȴ�.
	template <typename T>
	void f(T* param);

	int x = 27;
	const int* px = &x;	// px�� const int�μ��� x�� ����Ű�� pointer

	f(&x);				// T�� int, param type�� int *
	f(px);				// T�� const int, param type�� cont int *
}
