#ifndef NUMBER_HPP
#define NUMBER_HPP
namespace dev
{
    template<typename T> class Integer
    {
    private:
        T _number;
    public:
        Integer();
        template<typename InType> Integer(InType in) : _number((T) in) {}
    };
}
#endif // NUMBER_HPP
