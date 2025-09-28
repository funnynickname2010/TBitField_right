// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля
// ---------------------------------------------------------------------------

#include "tset.h"

/**
 * @brief Конструктор множества.
 * @param maxPower Максимальная мощность множества.
 */
TSet::TSet(size_t maxPower)
  : bitField(maxPower)
{
  this->maxPower = static_cast<int>(maxPower);
}

/**
 * @brief Конструктор копирования множества.
 * @param other Копируемое множество.
 */
TSet::TSet(const TSet& other)
  : bitField(other.bitField)
{
  maxPower = other.maxPower;
}

/**
 * @brief Конструктор преобразования из битового поля.
 * @param bitField Битовое поле.
 */
TSet::TSet(const TBitField& bitField)
  : bitField(bitField)
{
  maxPower = bitField.GetLength();
}

/**
 * @brief Преобразование множества к битовому полю.
 * @return Битовое поле.
 */
TSet::operator TBitField()
{
  return bitField;
}

/**
 * @brief Получить максимальную мощность множества.
 * @return Максимальная мощность.
 */
int TSet::GetMaxPower() const noexcept
{
  return maxPower;
}

/**
 * @brief Проверить наличие элемента в множестве.
 * @param elem Элемент.
 * @return 1 если элемент присутствует, 0 иначе.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
int TSet::IsMember(int elem) const
{
    if (elem < 0 || elem >= maxPower) 
    {
        throw std::out_of_range("Wrong element index");
    }
    return bitField.GetBit(elem);
}

/**
 * @brief Включить элемент в множество.
 * @param elem Элемент.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
void TSet::InsElem(int elem)
{
    if (elem < 0 || elem >= maxPower) 
    {
        throw std::out_of_range("Wrong element index");
    }
    bitField.SetBit(elem);
}

/**
 * @brief Удалить элемент из множества.
 * @param elem Элемент.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
void TSet::DelElem(int elem)
{
    if (elem < 0 || elem >= maxPower) 
    {
        throw std::out_of_range("Wrong element index");
    }
    bitField.ClrBit(elem);
}

/**
 * @brief Оператор присваивания.
 * @param other Множество для присваивания.
 * @return Ссылка на текущее множество.
 */
TSet& TSet::operator=(const TSet& other)
{
  if (this != &other) 
  {
    maxPower = other.maxPower;
    bitField = other.bitField;
  }
  return *this;
}

/**
 * @brief Оператор сравнения.
 * @param other Множество для сравнения.
 * @return 1 если множества равны, 0 иначе.
 */
int TSet::operator==(const TSet& other) const
{
  return (maxPower == other.maxPower) && (bitField == other.bitField);
}

/**
 * @brief Оператор неравенства.
 * @param other Множество для сравнения.
 * @return 1 если множества не равны, 0 иначе.
 */
int TSet::operator!=(const TSet& other) const
{
  return !(*this == other);
}

/**
 * @brief Оператор объединения с другим множеством.
 * @param other Другое множество.
 * @return Результат объединения.
 */
TSet TSet::operator+(const TSet& other)
{
  size_t resultMaxPower = std::max(maxPower, other.maxPower);
  TSet result(resultMaxPower);
  result.bitField = bitField | other.bitField;
  return result;
}

/**
 * @brief Оператор объединения с элементом.
 * @param elem Элемент.
 * @return Множество с добавленным элементом.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
TSet TSet::operator+(int elem)
{
    if (elem < 0 || elem >= maxPower) 
    {
        throw std::out_of_range("Wrong element index");
    }
    TSet result(*this);
    result.InsElem(elem);
    return result;
}

/**
 * @brief Оператор разности с элементом.
 * @param elem Элемент.
 * @return Множество без указанного элемента.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
TSet TSet::operator-(int elem)
{
    if (elem < 0 || elem >= maxPower) 
    {
        throw std::out_of_range("Wrong element index");
    }
    TSet result(*this);
    result.DelElem(elem);
    return result;
}

/**
 * @brief Оператор пересечения с другим множеством.
 * @param other Другое множество.
 * @return Результат пересечения.
 */
TSet TSet::operator*(const TSet& other)
{
  const size_t resultSetMaxPower = std::max(maxPower, other.maxPower);
  const size_t resultSetMinPower = std::min(maxPower, other.maxPower);
  TSet result(resultSetMaxPower);

  for (size_t i = 0; i < resultSetMinPower; ++i) 
  {
    if (bitField.GetBit(static_cast<int>(i)) && other.bitField.GetBit(static_cast<int>(i))) 
    {
      result.bitField.SetBit(static_cast<int>(i));
    }
  } 

  for (size_t i = resultSetMinPower; i < resultSetMaxPower; ++i) 
  {
    result.bitField.ClrBit(static_cast<int>(i));
  }

  return result;
}

/**
 * @brief Оператор дополнения множества.
 * @return Дополнение множества.
 */
TSet TSet::operator~()
{
  TSet result(*this);
  result.bitField = ~bitField;
  return result;
}

/**
 * @brief Перегрузка оператора ввода множества.
 * @param istr Входной поток.
 * @param set Множество.
 * @return Ссылка на входной поток.
 */
std::istream& operator>>(std::istream& istr, TSet& set)
{
  std::vector<int> inputVector;
  int temp;

  if (istr.peek() == '{') 
  {
    istr.ignore(1);
  }

  while (istr >> temp) 
  {
    inputVector.push_back(temp);
    if (istr.peek() == ',') 
    {
      istr.ignore(1);
    } 
    else if (istr.peek() == '}') 
    {
      istr.ignore(1);
      break;
    }
  }

  for (size_t i = 0; i < inputVector.size(); ++i) {
    set.InsElem(inputVector[i]);
  }

  return istr;
}

/**
 * @brief Перегрузка оператора вывода множества.
 * @param ostr Выходной поток.
 * @param set Множество.
 * @return Ссылка на выходной поток.
 */
std::ostream& operator<<(std::ostream& ostr, const TSet& set)
{
  ostr << '{';
  bool first = true;
  for (int i = 0; i < set.maxPower; ++i)
  {
    if (set.bitField.GetBit(i)) 
    {
      if (!first) 
      {
        ostr << ',';
      }
      ostr << i;
      first = false;
    }
  }
  ostr << '}';
  return ostr;
}
// ---------------------------------------------------------------------------
