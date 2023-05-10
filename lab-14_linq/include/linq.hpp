#ifndef LINQ_H_
#define LINQ_H_

#include <utility>
#include <vector>

namespace linq
{
    namespace impl
    {
        template<typename T, typename Iter>
        class range_enumerator;
        template<typename T>
        class drop_enumerator;
        template<typename T>
        class take_enumerator;
        template<typename T, typename U, typename F>
        class select_enumerator;
        template<typename T, typename F>
        class until_enumerator;
        template<typename T, typename F>
        class where_enumerator;

        template<typename T>
        class enumerator
        {
        public:
            virtual const T & operator*() = 0; // Получает текущий элемент.
            virtual void operator++() = 0;  // Переход к следующему элементу
            virtual operator bool() const = 0;  // Возвращает true, если есть текущий элемент

            auto drop(int count)
            {
                return drop_enumerator<T>(*this, count);
            }

            auto take(int count)
            {
                return take_enumerator<T>(*this, count);
            }

            template<typename U = T, typename F>
            auto select(F func)
            {
                return select_enumerator<U, T, F>(*this, func);
            }

            template<typename F>
            auto until(F func)
            {
                return until_enumerator<T, F>(*this, func);
            }

            template<typename F>
            auto where(F func)
            {
                return where_enumerator<T, F>(*this, func);
            }

            auto until_eq(T value)
            {
                const auto func = [&value](T a){ return a != value; };
                return until_enumerator<T, decltype(func)>(*this, func);
            }

            auto where_neq(T value)
            {
                const auto func = [&value](T a){ return a != value; };
                return where_enumerator<T, decltype(func)>(*this, func);
            }

            std::vector<T> to_vector()
            {
                std::vector<T> v;

                while (operator bool())
                {
                    v.push_back(std::move(operator*()));
                    operator++();
                }

                return v;
            }

            template<typename Iter>
            void copy_to(Iter it)
            {
                while (operator bool())
                {
                    *it = operator*();
                    ++it;
                }
            }
        };

        template<typename T, typename Iter>
        class range_enumerator : public enumerator<T>
        {
        public:
            range_enumerator(Iter begin, Iter end) : begin_(begin), end_(end)
            {
            }

            operator bool() const override
            {
                return begin_ != end_;
            }

            void operator++() override
            {
                ++begin_;
            }

            const T & operator*() override
            {
                return *begin_;
            }

        private:
          Iter begin_, end_;
        };

        template<typename T>
        class drop_enumerator : public enumerator<T>
        {
        public:
            drop_enumerator(enumerator<T> &parent, int count) : parent_(parent)
            {
                for (std::size_t i = 0; i < count && parent_.operator bool(); i++)
                {
                    ++parent_;
                }
            }

            operator bool() const override
            {
                return parent_.operator bool();
            }

            void operator++() override
            {
                ++parent_;
            }

            const T & operator*() override
            {
                return *parent_;
            }

        private:
            enumerator<T> &parent_;
        };

        template<typename T>
        class take_enumerator : public enumerator<T>
        {
        public:
            take_enumerator(enumerator<T> &parent, int count) : parent_(parent), count_(count)
            {
            }

            operator bool() const override
            {
                return parent_.operator bool() && count_ > 0;
            }

            void operator++() override
            {
                if (count_ > 0)
                {
                    ++parent_;
                    count_--;
                }
            }

            const T & operator*() override
            {
                return *parent_;
            }

        private:
            enumerator<T> &parent_;
            int count_;
        };

        template<typename T, typename U, typename F>
        class select_enumerator : public enumerator<T>
        {
        public:
            select_enumerator(enumerator<U> &parent, F func) : parent_(parent), func_(func), next_(true)
            {
            }

            operator bool() const override
            {
                return parent_.operator bool();
            }

            void operator++() override
            {
                ++parent_;
                next_ = true;
            }

            const T & operator*() override
            {
                if (next_)
                {
                    tmp_ = func_(*parent_);
                    next_ = false;
                }
                return tmp_;
            }

        private:
            enumerator<U> &parent_;
            T tmp_;
            bool next_;
            F func_;
        };

        template<typename T, typename F>
        class until_enumerator : public enumerator<T>
        {
        public:
            until_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(predicate), end_(false)
            {
            }

            operator bool() const override
            {
                return parent_.operator bool() && !end_;
            }

            void operator++() override
            {
                if (end_ || predicate_(*parent_))
                {
                    end_ = true;
                    return;
                }
                ++parent_;
            }

            const T & operator*() override
            {
                return *parent_;
            }

        private:
            enumerator<T> &parent_;
            F predicate_;
            bool end_;
        };

        template<typename T, typename F>
        class where_enumerator : public enumerator<T>
        {
        public:
            where_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(predicate)
            {
            }

            operator bool() const override
            {
                return parent_.operator bool();
            }

            void operator++() override
            {
                while (parent_.operator bool() && !predicate_(*parent_))
                    ++parent_;
            }

            const T & operator*() override
            {
                return *parent_;
            }

        private:
            enumerator<T> &parent_;
            F predicate_;
        };

    } // namespace impl

    template<typename T>
    auto from(T begin, T end)
    {
      return impl::range_enumerator<typename std::iterator_traits<decltype(begin)>::value_type, T>(begin, end);
    }
} // namespace linq

#endif
