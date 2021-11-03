// ���� ���� (universal reference) parameter�� ���� type �߷� �������� 
// L-Value �μ����� Ư���ϰ� ��޵ȴ�.
// �� L-Value: ǥ���� ���Ŀ��� ������� �ʴ� ��. �̸��� ���ϴ� ����
// �� R-Value: ǥ���� ���Ŀ��� ������� ��. �ӽ� ����

// Case 2: ParamType�� ���� ���� (universal reference, &&)�� ���
/**
/* 1. ���� expr�� L-Value�̸� T�� ParamType �� �� L-Value ������ �߷еȴ�.
/*	�̴� �������� ���������� ��Ȳ�̴�. ù°��, template type �߷п��� T��
/*  ���� Ÿ������ �߷еǴ� ���� �̰��� �����ϴ�. ��°��, ParamType�� ����
/*  ������ R-Value ������ ���� ���������, �߷е� Ÿ���� L-Value �����̴�.
/*  
/* 2. ���� expr�� R-Value�̸�, ��������(Case 1) ��Ģ���� ����ȴ�.
*/

template <typename T>
void f(T&& param);		// param�� ���� ���� (universal reference)

int x = 27;
const int cx = x;
const int& rx = x;

f(x);					// x�� L-Value, ���� T�� int&, param�� Ÿ�� ���� int &
f(cx);					// cx�� L-Value, 
						// ���� T�� const int&, param�� Ÿ�� ���� const int &
f(rx);					// rx�� L-Value, 
						// ���� T�� const int&, param�� Ÿ�� ���� const int &
f(27);					// 27�� R-Value, ���� T�� int, param�� Ÿ���� int&&

// ���� ����(universal reference)�� �����ϴ� ��쿡�� L-Value �μ��� R-Value�� ����
// ���� �ٸ� �߷� ��Ģ���� ����ȴ�.

// ���� ����(universal reference)�� �ƴ� parameter�� ���ؼ��� �׷� ���� ���� �߻����� �ʴ´�.