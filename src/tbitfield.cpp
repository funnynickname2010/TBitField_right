// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(size_t len)
{
    BitLen = len;
    MemLen = this->GetMemIndex(len) + 1;
    pMem = new TELEM[MemLen];
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];

    for (size_t i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return floor(BitLen / sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return (1 << (n % sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    pMem[this->GetMemIndex(n)] = pMem[this->GetMemIndex(n)] | this->GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    pMem[this->GetMemIndex(n)] = pMem[this->GetMemIndex(n)] & (~this->GetMemMask(n));
}

bool TBitField::GetBit(const int n) const // получить значение бита
{
    return pMem[this->GetMemIndex(n)] & this->GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    // Test for self assignment
    if (this != &bf) {

        // Not the same size
        if (MemLen != bf.MemLen) {
            MemLen = bf.MemLen;
            pMem = new TELEM[bf.MemLen];
        }

        // Copy the data
        for (size_t i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }

        BitLen = bf.BitLen;
    }

    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    bool result = 1;

    if (MemLen != bf.MemLen || BitLen != bf.BitLen) {
        result = 0;
    }
    else {
        for (size_t i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                result = 0;
                break;
            }
        }
    }

    return result;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    size_t shorter_MemLen = 0;
    size_t longer_MemLen = 0;

    // The pointer will point to the longer bitfield's pMem
    TELEM* longer_pMem = nullptr;

    // Checking out which bitfield is shorter
    // This is needed in order to organize cycles after
    if (MemLen < bf.MemLen) {
        shorter_MemLen = MemLen;
        longer_MemLen = bf.MemLen;
        longer_pMem = bf.pMem;
    }
    else {
        shorter_MemLen = bf.MemLen;
        longer_MemLen = MemLen;
        longer_pMem = pMem;
    }

    // Declaring the result with the size of the longer bitfield
    TBitField result(longer_MemLen * sizeof(TELEM));

    // Cycling through the shorter and longer bitfield, until we hit
    // the end of the shorter bitfield
    for (size_t i = 0; i < shorter_MemLen; i++) {
        result.pMem[i] = this->pMem[i] | bf.pMem[i];
    }

    // Cycling through the rest of the longer bitfield
    for (size_t i = shorter_MemLen; i < longer_MemLen; i++) {
        result.pMem[i] = longer_pMem[i];
    }

    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    size_t shorter_MemLen = 0;
    size_t longer_MemLen = 0;

    // Checking out which bitfield is shorter
    // This is needed in order to organize cycles after
    if (MemLen < bf.MemLen) {
        shorter_MemLen = MemLen;
        longer_MemLen = bf.MemLen;
    }
    else {
        shorter_MemLen = bf.MemLen;
        longer_MemLen = MemLen;
    }

    // Declaring the result with the size of the longer bitfield
    TBitField result(longer_MemLen * sizeof(TELEM));

    // Cycling through the shorter and longer bitfield, until we hit
    // the end of the shorter bitfield
    for (size_t i = 0; i < shorter_MemLen; i++) {
        result.pMem[i] = this->pMem[i] & bf.pMem[i];
    }

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);

    for (size_t i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }

    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    size_t bit_size = bf.GetLength();
    std::string input_string = "";
    std::string temp_string = "";
    input_string.resize(bit_size, '0');
    temp_string.resize(bit_size, '0');

    //std::cout << "String size: " << input_string.size() << "\t" << input_string << std::endl;

    //std::cout << "Please input the bitfield: ";

    getline(std::cin, temp_string);

    for (size_t j = 0; j < temp_string.size(); j++)
    {
        input_string[j] = temp_string[j];
    }

    for (size_t i = 0; i < bit_size; i++)
    {
        if (input_string[i] == '1')
        {
            bf.SetBit(i);
        }
        else if (input_string[i] == '0')
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

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{

    std::string output_str = "";
    size_t bit_size = bf.GetLength();

    for (size_t i = 0; i < bit_size; i++)
    {
        if (bf.GetBit(i))
        {
            output_str.append("1");
        }
        else
        {
            output_str.append("0");
        }
    }

    std::cout << output_str << std::endl;

    return ostr;
}
