#ifndef __OA_CQUEUE_H__
#define __OA_CQUEUE_H__

namespace onart {
	/// <summary>
	/// Ư���� ������ ���� ����� �� �ִ� ���� ť�Դϴ�. �⺻������ ��� ��Ʈ�� 0���� �� �ֽ��ϴ�.
	/// </summary>
	template <class T, unsigned L>
	class CircularQueue
	{
	private:
		int out = 0;
		T body[L];
	public:
		CircularQueue();
		/// <summary>
		/// ť�� ���Ҹ� �����Ͽ� �����մϴ�.
		/// </summary>
		void push(const T&);
		/// <summary>
		/// ť�� ���ϴ� ����ŭ�� ���Ҹ� �����Ͽ� �����մϴ�.
		/// </summary>
		/// <param name="src">���� ���</param>
		/// <param name="length">������ �����Դϴ�. ť ���̺��� ū ��� ť ���̸�ŭ�� �����մϴ�.</param>
		void push(const T* src, unsigned length = L);
		/// <summary>
		/// ť���� ���� �� ���Ҹ� ���� �����մϴ�.
		/// </summary>
		T pop();
		/// <summary>
		/// ť�� ������ ���Լ�������� �����մϴ�. ������ ���̸� ������� �ʴ� ��� ��ü�� �����մϴ�.
		/// </summary>
		/// <param name="dst">�����ؼ� �ٿ����� ��</param>
		/// <param name="length">������ �����Դϴ�. ť ���̺��� ū ��� ť ��ü�� �����մϴ�.</param>
		void copy(T* dst, unsigned length = L);
	};

	/// <summary>
	/// Ư���� ������ ���� ����� �� �ִ� ���� ť�Դϴ�. �⺻������ ��� ��Ʈ�� 0���� �� ������ ���̸� �ٲپ� ��Ȱ���� �� �ֽ��ϴ�.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class CircularQueue<T, 0> {
	private:
		unsigned length;
		int out = 0;
		T* body;
	public:
		/// <summary>
		/// ť�� ���̸� �ٲߴϴ�. ������ ���Ҵ� �������� �ʽ��ϴ�.
		/// </summary>
		void resize(unsigned length);
		/// <summary>
		/// ���ϴ� ������ ť�� �������� �Ҵ��Ͽ� �����մϴ�.
		/// </summary>
		CircularQueue(unsigned length);
		~CircularQueue();
	};
	
}

#endif // !__OA_CQUEUE_H__


