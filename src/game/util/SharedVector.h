#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>

    template <typename T>
    class SharedVector
    {
    public:
		SharedVector();
		SharedVector(SharedVector &&) {}
        ~SharedVector();

        T& front();
        void pop_front();

        void push_back(const T& item);
        void push_back(T&& item);

        int size();
        bool empty();
		void clear();
		void reserve(uint16_t count);

    private:
        std::vector<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    }; 

    template <typename T>
	SharedVector<T>::SharedVector(){}

    template <typename T>
	SharedVector<T>::~SharedVector(){}

    template <typename T>
    T& SharedVector<T>::front()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        return queue_.front();
    }

    template <typename T>
    void SharedVector<T>::pop_front()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty())
        {
            cond_.wait(mlock);
        }
        queue_.pop_front();
    }     

	template <typename T>
	void SharedVector<T>::reserve(uint16_t count)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.reserve(count);
		mlock.unlock();
	}

    template <typename T>
    void SharedVector<T>::push_back(const T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push_back(item);
        mlock.unlock();     // unlock before notificiation to minimize mutex con
        cond_.notify_one(); // notify one waiting thread

    }

    template <typename T>
    void SharedVector<T>::push_back(T&& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push_back(std::move(item));
        mlock.unlock();     // unlock before notificiation to minimize mutex con
        cond_.notify_one(); // notify one waiting thread

    }

    template <typename T>
    int SharedVector<T>::size()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        int size = queue_.size();
        mlock.unlock();
        return size;
    }

	template<typename T>
	bool SharedVector<T>::empty()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		bool empty = queue_.empty();
		mlock.unlock();
		return empty;
	}

	template <typename T>
	void SharedVector<T>::clear()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.clear();
		mlock.unlock();

	}