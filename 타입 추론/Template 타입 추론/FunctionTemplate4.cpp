// Template Ÿ�� �߷� �������� �迭�̳� �Լ� �̸��� �ش��ϴ� �μ��� pointer�� ����Ѵ�.
// ��, �׷� �μ��� ������ �ʱ�ȭ�ϴ� �� ���̴� ��쿡�� pointer�� ������� �ʴ´�.

// �迭 �μ�-> �迭 Ÿ���� �Լ� parameter��� ���� ����. ���� ������ ���� ���� ��ü�� �����ϴ�.

void myFunc(int param[]);
// �׷��� �� ��� �迭 ������ �ϳ��� pointer �������� ��޵ȴ�.
// ��, myFunc�� ������ ��ǻ� ������ ���� �ǹ��̴�.

// void myFunc(int* param);
// ��ó�� �迭 parameter ������ pointer parameteró�� ��޵ǹǷ�, template �Լ��� ������
//���޵Ǵ� �迭�� Ÿ���� pointer Ÿ������ �߷еȴ�.
/*
template <typename T>
void f(T param);
const char name[] = "J.P.Briggs";
f(name);
*/
// name�� �迭������ T�� param�� const char*�� �߷еȴ�.
// ��, template f�� ȣ�⿡�� Ÿ�� parameter T�� const char*�� �߷еȴ�.

// �׷��� �� ���� ������ ����� �ִ�. ��� �Լ��� parameter�� ��¥ �迭�� ������ ���� ������,
//�迭�� ���� ������ ������ ���� �ִ�.
/*
template <typename T>
void f(T& param);
const char name[] = "J.P.Briggs";
f(name);
*/
// �̷��� �ϸ� T�� ���� �߷е� Ÿ���� �迭�� ���� Ÿ���� �ȴ�! �� Ÿ���� �迭�� ũ�⸦
//�����ϹǷ�, �� �������� T�� const char [13]���� �߷еǰ� f�� parameter(�� �迭�� ���� ����)��
//Ÿ���� const char (&)[13]���� �߷еȴ�.
// ��̷ӰԵ�, �迭�� ���� ������ �����ϴ� �ɷ��� �̿��ϸ� �迭�� ��� ���ҵ��� ������ �߷��ϴ�
//template�� ���� �� �ִ�.

/** �迭�� ũ�⸦ compile ���� ����μ� �����ִ� template�Լ�
* (�迭 �Ű������� �̸��� ������ ���� ����, �� template�� �ʿ��� ����
* �迭�� ��� ������ �������̱� �����̴�.)
*/
#include <iostream>

template <typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept {
	return N;
}
// ��constexpr: �Լ��� constexpr�� �����ϸ� �Լ� ȣ���� ����� compile ���߿� ����� 
//�� �ְ� �ȴ�. �׷��� ���� ��ó�� �߰�ȣ �ʱ�ȭ �������� ���ǵ� ���� �迭�� ���� ũ��
//(���� ����)�� �� �迭�� �����ϴ� ���� ����� �� �ְ� �ȴ�.
int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };	//keyVal�� ���� ���� 7��

std::array<int, arraySize(keyVals)> mappedVals;

// arraySize�� noexcept�� ������ ���� compiler�� �� ���� �ڵ带 �����ϴ� ��
//������ �ַ��� ���̴�.

// �Լ� Ÿ�Ե� �Լ� pointer�� ����� �� ������, ���ݱ��� �迭�� ���� Ÿ�� �߷а� �����ؼ�
//������ ��� ���� �Լ��� ���� Ÿ�� �߷Ͽ��� ����ȴ�.
void someFunc(int, double);

template <typename T>
void f1(T param);		// f1�� param�� call by value

template <typename T>
void f2(T& param);		// f2�� param�� call by reference

f1(someFunc);			// param�� �Լ� pointer�� �߷еȴ�.
						// Ÿ���� void (*)(int, double)
f2(someFunc);			// param�� �Լ� ������ �߷еȴ�./
						// Ÿ���� void (&)(int, double)