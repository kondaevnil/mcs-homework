#include "shared_ptr.hpp"

bool shared_ptr::isNull() const noexcept
{
    return storage_ == nullptr;
}

Matrix * shared_ptr::ptr() const noexcept
{
    if (storage_ == nullptr)
        return nullptr;

    return storage_->getObject();
}

void shared_ptr::reset(Matrix *obj)
{
    auto tmp_storage = new Storage(obj);

    if (storage_ == nullptr)
    {
        storage_ = tmp_storage;
        return;
    }

    if (storage_->getCounter() == 1)
    {
        delete storage_;
        storage_ = tmp_storage;
        return;
    }

    storage_->decr();
    storage_ = tmp_storage;
}

shared_ptr::shared_ptr(Matrix *obj)
{
    if (obj == nullptr)
    {
        storage_ = nullptr;
        return;
    }

    storage_ = new Storage(obj);
}

shared_ptr::shared_ptr(const shared_ptr &other) noexcept
{
    other.storage_->incr();
    storage_ = other.storage_;
}

shared_ptr &shared_ptr::operator=(shared_ptr other)
{
    if (other.storage_ == storage_)
        return *this;

    if (storage_ != nullptr)
    {
        if (storage_->getCounter() == 1)
            delete storage_;
        else
            storage_->decr();
    }

    storage_ = other.storage_;
    storage_->incr();

    return *this;
}

shared_ptr::~shared_ptr()
{
    if (storage_ != nullptr)
    {
        if (storage_->getCounter() == 1)
            delete storage_;
        else
            storage_->decr();
    }
}

Matrix * shared_ptr::operator->() const
{
    return ptr();
}

Matrix & shared_ptr::operator*() const
{
    return *ptr();
}

void shared_ptr::Storage::decr()
{
    ref_count_--;
}

void shared_ptr::Storage::incr()
{
    ref_count_++;
}

int shared_ptr::Storage::getCounter() const
{
    return ref_count_;
}

Matrix *shared_ptr::Storage::getObject() const
{
    return data_;
}

shared_ptr::Storage::Storage(Matrix *mtx) : data_(mtx), ref_count_(1)
{
}

shared_ptr::Storage::~Storage()
{
    delete data_;
}

