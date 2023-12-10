/*
** EPITECH PROJECT, 2021
** Babel
** File description:
** Template for circular buffer-desu
*/

#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <mutex>
#include <memory>
#include <array>
#include <optional>

template <class T, long int lenght>
class Circular_buffer {
    public:
        explicit Circular_buffer();
        void put(T data);
		bool swap_head_value(T &new_data);
		std::optional<T> get();
		bool swap_tail_value(T &new_data);
        void reset();
        void act_full();
        bool empty() const;
        bool full() const;
        long capacity() const;
        long size() const;

    private:
        std::mutex circular_buffer_protector;
        std::array<T, lenght> buffer;
        long head = 0;
        long tail = 0;
        const long max_size = lenght;
        bool is_full = false;
};


template <class T, long int lenght>
Circular_buffer<T, lenght>::Circular_buffer()
{}

template <class T, long int lenght>
void Circular_buffer<T, lenght>::put(T data)
{
	std::lock_guard<std::mutex> lock(circular_buffer_protector);

	buffer[head] = data;
	if (is_full)
		tail = ((tail + 1) % max_size);
	head = ((head + 1) % max_size);
	is_full = (head == tail);
}

template <class T, long int lenght>
bool Circular_buffer<T, lenght>::swap_head_value(T &new_data)
{
	std::lock_guard<std::mutex> lock(circular_buffer_protector);

	if (!is_full) {
		std::swap(buffer[head], new_data);
		head = ((head + 1) % max_size);
		is_full = (head == tail);
		return true;
	}
	return false;

	// std::swap(buffer[head], new_data);
	// if (is_full)
	// 	tail = ((tail + 1) % max_size);
	// head = ((head + 1) % max_size);
	// is_full = (head == tail);
}

template <class T, long int lenght>
std::optional<T> Circular_buffer<T, lenght>::get()
{
	std::lock_guard<std::mutex> lock(circular_buffer_protector);

	if(empty())
		return std::nullopt;
	auto rsl = buffer[tail];
	is_full = false;
	tail = ((tail + 1) % max_size);
	return rsl;
}

template <class T, long int lenght>
bool Circular_buffer<T, lenght>::swap_tail_value(T &new_data)
{
	std::lock_guard<std::mutex> lock(circular_buffer_protector);

	if(!empty()) {
		std::swap(buffer[tail], new_data);
		is_full = false;
		tail = ((tail + 1) % max_size);
		return true;
	}
	return false;
}

template <class T, long int lenght>
void Circular_buffer<T, lenght>::reset()
{
	std::lock_guard<std::mutex> lock(circular_buffer_protector);

	head = tail;
	is_full = false;
}

template <class T, long int lenght>
void Circular_buffer<T, lenght>::act_full()
{
	std::lock_guard<std::mutex> lock(circular_buffer_protector);

	head = 0;
	tail = 0;
	is_full = true;
}

template <class T, long int lenght>
bool Circular_buffer<T, lenght>::empty() const // return true if empty
{
	return ((head == tail) && (is_full == false));
}

template <class T, long int lenght>
bool Circular_buffer<T, lenght>::full() const
{
	return is_full;
}

template <class T, long int lenght>
long Circular_buffer<T, lenght>::capacity() const
{
	return max_size;
}

template <class T, long int lenght>
long Circular_buffer<T, lenght>::size() const
{
	if (is_full == false) {
		if(head >= tail)
			return (head - tail);
		else
			return (max_size + (head - tail));
    }
    return max_size;
}

#endif /* !CIRCULAR_BUFFER_HPP */
