// ---------------------------------------------------------------------------
// Битовое поле
// ---------------------------------------------------------------------------

#include "tbitfield.h"

/**
 * @brief Конструктор битового поля заданной длины.
 * @param bitLen Длина битового поля.
 */
TBitField::TBitField(size_t bitLen)
{
  BitLen = bitLen;
  MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);

  try 
  {
    pMem = new TELEM[MemLen];
  } 
  catch (const std::bad_alloc&) 
  {
    throw;
  }

  for (size_t i = 0; i < MemLen; i++) 
  {
    pMem[i] = 0;
  }
}

/**
 * @brief Конструктор копирования.
 * @param other Копируемое битовое поле.
 */
TBitField::TBitField(const TBitField& other)
{
  BitLen = other.BitLen;
  MemLen = other.MemLen;

  try 
  {
    pMem = new TELEM[MemLen];
  } 
  catch (const std::bad_alloc&) 
  {
    throw;
  }

  for (size_t i = 0; i < MemLen; i++) 
  {
    pMem[i] = other.pMem[i];
  }
}

/**
 * @brief Деструктор битового поля.
 */
TBitField::~TBitField()
{
  if (pMem != nullptr)
    delete[] pMem;
  pMem = nullptr;
}

/**
 * @brief Получить индекс элемента памяти для бита n.
 * @param n Индекс бита.
 * @return Индекс элемента памяти.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
int TBitField::GetMemIndex(const int n) const
{
  if (n < 0 || n >= BitLen) 
  {
    throw std::out_of_range("Index is out of range");
  }
  size_t bitsPerElem = sizeof(TELEM) * 8;
  return n / bitsPerElem;
}

/**
 * @brief Получить маску для бита n.
 * @param n Индекс бита.
 * @return Маска для бита.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
TELEM TBitField::GetMemMask(const int n) const
{
  if (n < 0 || n >= BitLen) 
  {
    throw std::out_of_range("Index is out of range");
  }
  int bitsPerElem = sizeof(TELEM) * 8;
  return static_cast<TELEM>(1) << (n % bitsPerElem);
}

// ---------------------------------------------------------------------------

/**
 * @brief Получить длину битового поля.
 * @return Длина битового поля.
 */
int TBitField::GetLength() const noexcept
{
  return BitLen;
}

/**
 * @brief Установить бит с номером n.
 * @param n Номер бита.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
void TBitField::SetBit(const int n)
{
  if (n < 0 || n >= BitLen) 
  {
    throw std::out_of_range("Index is out of range");
  }
  int memIndex = GetMemIndex(n);
  pMem[memIndex] |= GetMemMask(n);
}

/**
 * @brief Очистить бит с номером n.
 * @param n Номер бита.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
void TBitField::ClrBit(const int n)
{
  if (n < 0 || n >= BitLen) 
  {
    throw std::out_of_range("Index is out of range");
  }
  int memIndex = GetMemIndex(n);
  pMem[memIndex] &= ~GetMemMask(n);
}

/**
 * @brief Получить значение бита с номером n.
 * @param n Номер бита.
 * @return true если бит установлен, иначе false.
 * @throw std::out_of_range Если индекс вне диапазона.
 */
bool TBitField::GetBit(const int n) const
{
  if (n < 0 || n >= BitLen) 
  {
    throw std::out_of_range("Index is out of range");
  }
  int memIndex = GetMemIndex(n);
  return (pMem[memIndex] & GetMemMask(n)) != 0;
}

// ---------------------------------------------------------------------------

/**
 * @brief Оператор присваивания.
 * @param other Битовое поле для присваивания.
 * @return Ссылка на текущее битовое поле.
 */
TBitField& TBitField::operator=(const TBitField& other)
{
  if (this != &other) 
  {
    if (MemLen != other.MemLen) 
    {
      TELEM* newMem = nullptr;
      try 
      {
        newMem = new TELEM[other.MemLen];
      } 
      catch (const std::bad_alloc&) 
      {
        throw;
      }
      delete[] pMem;
      pMem = newMem;
      MemLen = other.MemLen;
    }
    BitLen = other.BitLen;
    for (size_t i = 0; i < MemLen; i++) 
    {
      pMem[i] = other.pMem[i];
    }
  }
  return *this;
}

/**
 * @brief Оператор сравнения.
 * @param other Битовое поле для сравнения.
 * @return true если поля равны, иначе false.
 */
bool TBitField::operator==(const TBitField& other) const noexcept
{
  if (BitLen != other.BitLen)
    return false;

 const int bitsPerElem = sizeof(TELEM) * 8;
 const int fullElems = BitLen / bitsPerElem;

  for (int i = 0; i < fullElems; ++i) 
  {
    if (pMem[i] != other.pMem[i])
      return false;
  }

  return true;
}

/**
 * @brief Оператор неравенства.
 * @param other Битовое поле для сравнения.
 * @return true если поля не равны, иначе false.
 */
bool TBitField::operator!=(const TBitField& other) const noexcept
{
  return !(*this == other);
}

// ---------------------------------------------------------------------------

/**
 * @brief Оператор "или" для битовых полей.
 * @param other Второе битовое поле.
 * @return Результат операции "или".
 */
TBitField TBitField::operator|(const TBitField& other) const
{
  const int resultLen = std::max(BitLen, other.BitLen);
  TBitField result(resultLen);

  const int minMemLen = std::min(MemLen, other.MemLen);

  for (int i = 0; i < minMemLen; i++) 
  {
    result.pMem[i] = pMem[i] | other.pMem[i];
  }
  if (MemLen > other.MemLen) 
  {
    for (int i = other.MemLen; i < MemLen; i++) 
    {
      result.pMem[i] = pMem[i];
    }
  } 
  else 
  {
    for (int i = MemLen; i < other.MemLen; i++) 
    {
      result.pMem[i] = other.pMem[i];
    }
  }
  return result;
}

/**
 * @brief Оператор "и" для битовых полей.
 * @param other Второе битовое поле.
 * @return Результат операции "и".
 */
TBitField TBitField::operator&(const TBitField& other) const
{
  const int resultLen = std::max(BitLen, other.BitLen);
  TBitField result(resultLen);

  const int minMemLen = std::min(MemLen, other.MemLen);

  for (int i = 0; i < minMemLen; i++) 
  {
    result.pMem[i] = pMem[i] & other.pMem[i];
  }
  // Остальные элементы уже инициализированы нулями
  return result;
}

/**
 * @brief Оператор отрицания для битового поля.
 * @return Результат операции отрицания.
 */
TBitField TBitField::operator~() const noexcept
{
  TBitField result(BitLen);
  const int bitsPerElem = sizeof(TELEM) * 8;

  for (int i = 0; i < MemLen; i++) 
  {
    result.pMem[i] = ~pMem[i];
  }
  int unusedBits = MemLen * bitsPerElem - BitLen;

  if (unusedBits > 0) 
  {
    const TELEM mask = (static_cast<TELEM>(1) << (bitsPerElem - unusedBits)) - 1;
    result.pMem[MemLen - 1] &= mask;
  }
  return result;
}

// ---------------------------------------------------------------------------

/**
 * @brief Перегрузка оператора ввода для битового поля.
 * @param istr Входной поток.
 * @param bf Битовое поле.
 * @return Ссылка на входной поток.
 */
std::istream& operator>>(std::istream& istr, TBitField& bf)
{
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  const size_t bitSize = bf.GetLength();
  std::string inputString(bitSize, '0');
  std::string tempString(bitSize, '0');

  getline(std::cin, tempString);

  for (size_t j = 0; j < tempString.size(); j++) 
  {
    inputString[j] = tempString[j];
  }

  for (size_t i = 0; i < bitSize; i++) 
  {
    if (inputString[i] == '1') 
    {
      bf.SetBit(i);
    } 
    else if (inputString[i] == '0') 
    {
      bf.ClrBit(i);
    } 
    else 
    {
      break;
    }
  }

  if (std::cin.fail()) 
  {
    std::cin.ignore();
    std::cin.clear();
  }
  return istr;
}

/**
 * @brief Перегрузка оператора вывода для битового поля.
 * @param ostr Выходной поток.
 * @param bf Битовое поле.
 * @return Ссылка на выходной поток.
 */
std::ostream& operator<<(std::ostream& ostr, const TBitField& bf)
{
  std::string outputStr;
  const size_t bitSize = bf.GetLength();

  for (size_t i = 0; i < bitSize; i++) 
  {
    outputStr.append(bf.GetBit(i) ? "1" : "0");
  }

  std::cout << outputStr << std::endl;

  return ostr;
}
