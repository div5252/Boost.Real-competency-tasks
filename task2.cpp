/* Task2: Explain how could you provide a "Small String Optimization"-like approach for Real numbers representation at compile time. */

template <typename T, int N>
class small_vector : public small_vector_impl<T>
{
    private:
        char buffer[sizeof(T) * N];

    public:
        small_vector() : small_vector_impl((T *)buffer, (T *)buffer, N) {}

        // other vector functions
}

template <typename T>
class small_vector_impl
{
    private:
        T *_begin, *_end;
        size_t capacity;

    public:
        small_vector_impl(T *_begin, T *_end, size_t capacity);
        
        iterator begin()
        {
            return _begin;
        }
        iterator end()
        {
            return _end;
        }

        // other vector functions
}
