#ifndef SINGLETON_HPP
#define SINGLETON_HPP


template<typename T>
class Singleton
{
protected:
    Singleton(){};
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
public:
    static T& Instance()
    {
        static T _ins;
        return _ins;
    }
};

#endif // SINGLETON_HPP
