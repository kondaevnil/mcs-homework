#pragma once

#include "matrix.hpp"

class shared_ptr {
public:
	/**
	 * Получает объект по сырому указателю во владение.
	 * Передача того же указателя во владение кому либо ещё — неопределённое поведение.
	 */
    explicit shared_ptr(Matrix *obj = nullptr);
	/**
	 * Строит копию умного указателя, разделяя владение с оригиналом.
	 */
    shared_ptr(const shared_ptr &other) noexcept;
	/**
	 * Присваивает умному указателю новое значение, корректно меняя владение с одного объекта на другой.
	 */
    shared_ptr & operator=(shared_ptr other);
    ~shared_ptr();

	/**
	 * Возвращает сырой указатель для несовместимого кода.
	 */
    [[nodiscard]] Matrix * ptr() const noexcept;
	/**
	 * Проверка на nullptr.
	 */
    [[nodiscard]] bool isNull() const noexcept;
	/**
	 * Присваивает умному указателю новый для владения.
	 */
    void reset(Matrix *obj = nullptr);

    Matrix * operator->() const;
    Matrix & operator*() const;

private:
    class Storage {
    public:
        explicit Storage(Matrix* mtx);
        ~Storage();

        void incr();
        void decr();

        [[nodiscard]] int getCounter() const;
        [[nodiscard]] Matrix* getObject() const;

    private:
        Matrix *data_;
        int ref_count_;
    };

private:
    Storage *storage_;
};
