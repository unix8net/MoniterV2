#ifndef CIRCULAR_DEQUE_HPP_
#define CIRCULAR_DEQUE_HPP_

#include <cstddef>
/*==================================================================
 *    ���ߣ�Creater
 *    ��飺˫��ѭ������,���ڴ��ڵ�֡����
 *    ��ҳ�� unix8.net
 *    ���ڣ�2014-04-19
/*==================================================================*/

template <typename T>
inline bool operator !=(const T& x, const T& y) { return !(x == y); }

template <typename T>
inline bool operator <=(const T& x, const T& y) { return !(y < x); }

template <typename T>
inline bool operator >(const T& x, const T& y) { return y < x; }

template <typename T>
inline bool operator >=(const T& x, const T& y) { return !(x < y); }


template <typename T>
class CircularDeque
{
public:
  typedef T value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef T* pointer;
  typedef const T* const_pointer;

  CircularDeque(size_type buf_sz = 0) :
      start(new T[buf_sz + 1]),finish(start + buf_sz + 1),
      first(start + ((buf_sz + 1) >> 1)), last(first) {}

  CircularDeque(const CircularDeque& x) :
      start(new T[x.finish - x.start]), finish(start + (x.finish - x.start)),
      first(start + (x.first - x.start)),last(start + (x.last - x.start))
  {
    T* f1 = first, * f2 = x.first, * l2 = x.last;
    for(; f2 != l2; *f1++ = *f2++)
    {
      if(f2 == x.finish)
        f2 = x.start;
      if(f1 == finish)
        f1 = start;
    }
  }

  // ѭ�����е���������
  ~CircularDeque() { delete [] start; }

    // ����һ������
  CircularDeque& operator = (const CircularDeque& x)
  {
    if(&x != this)
    {
        if(capacity() < x.size())
        {
          delete [] start;
          size_type cap = x.finish - x.start;
          start = new T[cap];
          finish = start + cap;
            first = start + (x.first - x.start);
          }
      T* f1 = first, * f2 = x.first, * l2 = x.last;
      for(; f2 != l2; *f1++ = *f2++)
      {
        if(f2 == x.finish)
          f2 = x.start;
        if(f1 == finish)
          f1 = start;
      }
        if(f1 == finish)
        f1 = start;
        last = f1;
    }
    return *this;
  }

  // Ԥ���ռ�,��boost::circular_buffer�ĳ�Ա����reserve����Ϊ��ͬ,
  // ��std::vector�ĳ�Ա����reserve����Ϊ����
  void reserve(size_type new_sz)
  {
    if(capacity() < new_sz)
    {
        size_type buf = new_sz + 1;
        T* new_start = new T[buf],
                * new_finish = new_start + buf;
            T* new_last = new_start + (buf >> 1),
                    * new_first = new_last;
            T* f2 = first, * l2 = last;
      for(; f2 != l2; *new_last++ = *f2++)
      {
        if(f2 == finish)
          f2 = start;
        if(new_last == new_finish)
          new_last = new_start;
      }
        if(new_last == new_finish)
        new_last = new_start;
        start = new_start;
        finish = new_finish;
        first = new_first;
            last = new_last;
    }
  }

  // ���е�������, ����������ɵ�Ԫ�ظ���
  size_type capacity() const { return finish - start - 1; }

  // �����е�ǰ�洢��Ԫ�ظ���
  size_type size() const
  {
    size_type cap = finish - start;
    return (last - first + cap) % cap;
  }

    // �ж϶����Ƿ��
  bool empty() const { return first == last; }

  // �ж϶����Ƿ���, ��Ȼciecular_deque�����̶�, ��Ӧ���д˳�Ա����
  bool full() const
  {
    T* tlast = (last == finish ? start : last);
    return tlast + 1 == first;
  }

  // ��ն���������Ԫ��, ������������ÿ��Ԫ��, �����ƶ�ָ�����
  void clear() { last = first = (start + ((finish - start) >> 1)); }

  // ������������, ��������ָ��Ϳ�����
  void swap(CircularDeque& x)
  {
    pswap(start, x.start);
    pswap(finish, x.finish);
    pswap(first, x.first);
    pswap(last, x.last);
  }
  // �������±�����κ�һ��Ԫ��
  reference operator [](difference_type i)
  {
    difference_type idx = first - start + i,
            cap = finish - start;
    if(idx >= cap)
      idx -= cap;
    return *(start + idx);
  }

  const_reference operator [](difference_type i) const
  {
    difference_type idx = first - start + i,
            cap = finish - start;
    if(idx >= cap)
      idx -= cap;
    return *(start + idx);
  }

  // ���ö�ͷԪ��, ������п�, ����޷�Ԥ��
  reference front()
  {
    if(first == finish)
      return *start;
    return *first;
  }
  void get_n_front(pointer p, size_t n)
  {
      pointer ts = first;
      for(size_t i = 0; i < n; ++i)
      {
        if(ts == finish)
        {
            *(p + i) = *start;
            ts = start;
        }
        else
            *(p + i) = *ts;
        ++ts;
      }
  }

  const_reference front() const
  {
    if(first == finish)
      return *start;
    return *first;
  }

  // ���ö�βԪ��, ������п�, ����޷�Ԥ��
  reference back()
  {
    if(last == start)
      return *(finish - 1);
    return *(last - 1);
  }

  const_reference back() const
  {
    if(last == start)
      return *(finish - 1);
    return *(last - 1);
  }

  // ��һ��Ԫ�ز��뵽��ͷǰ, ��Ϊ�¶�ͷ
  void push_front(const_reference x)
  {
    if(first == start)
      first = finish;
    *--first = x;
  }
  void push_n_front(pointer p, size_t n)
  {
      for(size_t i = 0; i < n;
          push_front(*( p + i)), ++i);
  }

  // ��һ��Ԫ�ز��뵽��β��, ��Ϊ�¶�β
  void push_back(const_reference x)
  {
    if(last == finish)
      last = start;
    *last++ = x;
  }
  //����n��Ԫ�ص���β
  void push_n_back(pointer p, size_t n)
  {
      for(size_t i = 0; i < n;
          push_back(*( p + i)), ++i);
  }

  // ɾ����ͷԪ��, ������п�, ������ָ�����
  void pop_front()
  {
    if(first == finish)
      first = start;
    ++first;
  }
  void remove_n_front(size_t n)
  {
      for(size_t i = 0; i < n; ++i)
          pop_front();
  }
  void remove_n_back(size_t n)
  {
      for(size_t i =0; i < n; ++i)
          pop_back();
  }

  // ɾ����βԪ��, ������п�, ������ָ�����
  void pop_back()
  {
    if(last == start)
      last = finish;
    --last;
  }

  // ... �д���Ӹ��๫�г�Ա����, �ù�����������
protected:
    T* start, * finish, * first, * last;

    template <typename U>
  void pswap(U& x, U& y) { U t = x;  x = y, y = t; }

  // ������������
    friend void swap(CircularDeque& x, CircularDeque& y) { x.swap(y); }

  // �ж����������Ƿ����
    friend bool operator ==(const CircularDeque& x, const CircularDeque& y)
    {
        const T* f1 = x.first, * l1 = x.last,
                * f2 = y.first, * l2 = y.last;
        if(l1 == x.start)
          l1 = x.finish - 1;
        if(l2 == y.start)
          l2 = y.finish - 1;
        for(; f1 != l1 && f2 != l2 && *f1 == *f2; ++f1, ++f2)
        {
          if(f1 == x.finish)
            f1 = x.start;
          if(f2 == y.finish)
            f2 = y.start;
        }
        return f1 == l1 && f2 == l2;
    }

  // �Ƚ��������д�С
    friend bool operator <(const CircularDeque& x, const CircularDeque& y)
    {
        const T* f1 = x.first, * l1 = x.last,
                * f2 = y.first, * l2 = y.last;
        if(l1 == x.start)
          l1 = x.finish - 1;
        if(l2 == y.start)
          l2 = y.finish - 1;
        for(; f1 != l1 && f2 != l2 && *f1 == *f2; ++f1, ++f2)
        {
          if(f1 == x.finish)
            f1 = x.start;
          if(f2 == y.finish)
            f2 = y.start;
        }
        return (f1 == l1 && f2 != l2) ||
                (f1 != l1 && f2 != l2 && *f1 < *f2);
    }
};  // CircularDeque

#endif  // CircularDeque
