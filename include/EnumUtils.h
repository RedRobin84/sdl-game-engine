#pragma once

#include <type_traits>

template<typename T>
class EnumUtils
{
public:
  class Iterator
  {
  public:
    Iterator(T t) : m_value(std::underlying_type<T>::type(t))
    {}

    T operator*(void) const
    {
      return (T)m_value;
    }

    void operator++(void)
    {
      if (m_value >= std::underlying_type<T>::type(T::LAST)) {
        m_value = 0;
        return;
      }
      ++m_value;
    }

    void operator--(void)
    {
      if (m_value <= std::underlying_type<T>::type(T::FIRST)) {
        m_value = std::underlying_type<T>::type(T::LAST);
        return;
      }
      --m_value;
    }

    bool operator!=(Iterator rhs)
    {
      return m_value != rhs.m_value;
    }

  private:
    int m_value;
  };
};

template<typename T>
typename EnumUtils<T>::Iterator begin(EnumUtils<T>)
{
  return typename EnumUtils<T>::Iterator(T::FIRST);
}

template<typename T>
typename EnumUtils<T>::Iterator end(EnumUtils<T>)
{
  return typename EnumUtils<T>::Iterator((T::LAST) + 1);
}
