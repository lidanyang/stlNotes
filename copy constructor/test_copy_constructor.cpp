#include<iostream>

template<typename X>
class T {
public:
    T() = default;
    template<typename Y>
    T(T<Y> &t) // this can not be copy constructor
    {
        std::cout << "T(const T<Y> &t)" << std::endl;
    }
    T(T&) // T(const T&) compile error!!!
    {
        std::cout << "T(const T&)" << std::endl;
    }
};

class B {};
class D : public B {};

T<B> Fun(T<B> t)
{
    return t;
}

int main()
{
    T<D> t {};
    // user-defined conversion: ‘T<X>::T(T<Y>&) [with Y = D; X = B]
    Fun(t);
}