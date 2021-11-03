// �� ���� ����� parameter�� ���� Ÿ�� �߷� �������� const �Ǵ� volatile(�Ǵ� �� �� ����)
// �μ��� �� const, �� volatile �μ��� ��޵ȴ�.
// Case 3: ParamType�� pointer�� �ƴϰ� ������ �ƴϴ�.
// ParamType�� pointer�� �ƴϰ� ������ �ƴ϶��, �μ��� �Լ��� ������ ���� (pass-by-value)�Ǵ�
// ��Ȳ�� ���̴�. ���� param�� �־��� �μ��� ���纻, �� ������ ���ο� ��ü�̴�.
// param�� ���ο� ��ü��� ��Ƕ�����, expr���� T�� �߷еǴ� �������� ������ ���� ��Ģ���� ����ȴ�.
// 1. ����ó��, ���� expr Ÿ���� �����̸�, ���� �κ��� ���õȴ�.
// 2. expr�� �������� ������ ��, ���� expr�� const�̸� �� const ���� �����Ѵ�.
//	  ���� volatile�̸� �װ͵� �����Ѵ�.

template <typename T>
void f(T param);		// �̹����� param�� ������ ���޵ȴ�.

int x = 27;
const int cx = x;
const int& rx = x;

f(x);					// T�� param�� Ÿ���� �� �� int
f(cx);					// ������ T�� param�� Ÿ���� �� �� int
f(rx);					// �̹����� T�� param�� Ÿ���� �� �� int

// cx�� rx�� const ���� ���������� �׷��� param�� const�� �ƴϴ�.
// 
// param�� cx�� rx�� ���纻�̹Ƿ�, �ٽ� ���� param�� cx�� rx�ʹ� ������ �������� ��ü�̹Ƿ�,
// �̴� �翬�� ����̴�.
// 
// ���⼭ ����� ����, const��(�׸��� volatile��) �� ���� parameter�� ���ؼ��� ���õȴٴ� ���̴�.
//
// expr�� const ��ü�� ����Ű�� cosnt �������̰�, param�� ������ ���޵Ǵ� ���� ���?
const char* const ptr = "Fun with pointers";	// ptr�� const ��ü�� ����Ű�� const pointer
f(ptr);											// const char * const Ÿ���� �μ��� ����

// �� ��� pointer ��ü(ptr)�� ������ ���޵ȴ�. ���� ptr�� ������� ���õȴ�.
// ������, ptr�� ����Ű�� ��ü(���⼭�� ���ڿ�)�� ������� ������ �����ȴ�.
// �� ��� T�� param�� Ÿ���� �Ѵ� const char*�� �߷еȴ�. 