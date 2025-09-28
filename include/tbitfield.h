// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.h - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#ifndef __BITFIELD_H__ 
#define __BITFIELD_H__

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

typedef unsigned int TELEM;

/**
 * @brief Класс TBitField - битовое поле.
 */
class TBitField
{
private:
    int BitLen;      ///< длина битового поля - макс. к-во битов
    TELEM* pMem;     ///< память для представления битового поля
    int MemLen;      ///< к-во эл-тов pMem для представления бит.поля

    /**
     * @brief Получить индекс в pMem для бита n.
     * @param n Номер бита.
     * @return Индекс в массиве pMem.
     */
    int GetMemIndex(const int n) const;

    /**
     * @brief Получить битовую маску для бита n.
     * @param n Номер бита.
     * @return Маска для бита.
     */
    TELEM GetMemMask(const int n) const;

public:
    /**
     * @brief Конструктор битового поля.
     * @param len Размер битового поля.
     */
    TBitField(size_t len);

    /**
     * @brief Конструктор копирования.
     * @param bf Копируемое битовое поле.
     */
    TBitField(const TBitField& bf);

    /**
     * @brief Деструктор.
     */
    ~TBitField();

    /**
     * @brief Получить длину битового поля.
     * @return Длина (количество битов).
     */
    int GetLength(void) const noexcept;

    /**
     * @brief Установить бит.
     * @param n Номер бита.
     */
    void SetBit(const int n);

    /**
     * @brief Очистить бит.
     * @param n Номер бита.
     */
    void ClrBit(const int n);

    /**
     * @brief Получить значение бита.
     * @param n Номер бита.
     * @return true если бит установлен, иначе false.
     */
    bool GetBit(const int n) const;

    /**
     * @brief Оператор сравнения.
     * @param bf Битовое поле для сравнения.
     * @return true если поля равны.
     */
    bool operator==(const TBitField& bf) const noexcept;

    /**
     * @brief Оператор неравенства.
     * @param bf Битовое поле для сравнения.
     * @return true если поля не равны.
     */
    bool operator!=(const TBitField& bf) const noexcept;

    /**
     * @brief Оператор присваивания.
     * @param bf Битовое поле для присваивания.
     * @return Ссылка на текущее битовое поле.
     */
    TBitField& operator=(const TBitField& bf);

    /**
     * @brief Оператор "или".
     * @param bf Второе битовое поле.
     * @return Результат операции.
     */
    TBitField operator|(const TBitField& bf) const;

    /**
     * @brief Оператор "и".
     * @param bf Второе битовое поле.
     * @return Результат операции.
     */
    TBitField operator&(const TBitField& bf) const;

    /**
     * @brief Оператор отрицания.
     * @return Результат операции.
     */
    TBitField operator~(void) const noexcept;

    /**
     * @brief Перегрузка оператора ввода.
     */
    friend istream& operator>>(istream& istr, TBitField& bf);

    /**
     * @brief Перегрузка оператора вывода.
     */
    friend ostream& operator<<(ostream& ostr, const TBitField& bf);
};

#endif // __BITFIELD_H__
