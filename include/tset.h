// ---------------------------------------------------------------------------
// Множество - реализация через битовое поле
// ---------------------------------------------------------------------------

#ifndef __TSET_H__
#define __TSET_H__

#include "tbitfield.h"
#include <vector>

/**
 * @brief Класс TSet - множество, реализованное через битовое поле.
 */
class TSet
{
private:
	int maxPower;         ///< максимальная мощность множества
	TBitField bitField;   ///< битовое поле для хранения характеристического вектора
public:
	/**
	 * @brief Конструктор множества.
	 * @param maxPower Максимальная мощность множества.
	 */
	TSet(size_t maxPower);

	/**
	 * @brief Конструктор копирования.
	 * @param other Копируемое множество.
	 */
	TSet(const TSet& other);

	/**
	 * @brief Конструктор преобразования из битового поля.
	 * @param bitField Битовое поле.
	 */
	TSet(const TBitField& bitField);

	/**
	 * @brief Преобразование к битовому полю.
	 */
	operator TBitField();

	/**
	 * @brief Получить максимальную мощность множества.
	 * @return Максимальная мощность.
	 */
	int GetMaxPower() const noexcept;

	/**
	 * @brief Включить элемент в множество.
	 * @param elem Элемент.
	 */
	void InsElem(int elem);

	/**
	 * @brief Удалить элемент из множества.
	 * @param elem Элемент.
	 */
	void DelElem(int elem);

	/**
	 * @brief Проверить наличие элемента в множестве.
	 * @param elem Элемент.
	 * @return 1 если элемент присутствует, 0 иначе.
	 */
	int IsMember(int elem) const;

	/**
	 * @brief Операторы сравнения.
	 */
	int operator==(const TSet& other) const;
	int operator!=(const TSet& other) const;

	/**
	 * @brief Оператор присваивания.
	 */
	TSet& operator=(const TSet& other);

	/**
	 * @brief Объединение с элементом.
	 * @param elem Элемент.
	 */
	TSet operator+(int elem);

	/**
	 * @brief Разность с элементом.
	 * @param elem Элемент.
	 */
	TSet operator-(int elem);

	/**
	 * @brief Объединение с другим множеством.
	 * @param other Другое множество.
	 */
	TSet operator+(const TSet& other);

	/**
	 * @brief Пересечение с другим множеством.
	 * @param other Другое множество.
	 */
	TSet operator*(const TSet& other);

	/**
	 * @brief Дополнение множества.
	 */
	TSet operator~();

	/**
	 * @brief Перегрузка ввода множества.
	 */
	friend std::istream& operator>>(std::istream& istr, TSet& set);

	/**
	 * @brief Перегрузка вывода множества.
	 */
	friend std::ostream& operator<<(std::ostream& ostr, const TSet& set);
};
#endif // __TSET_H__
