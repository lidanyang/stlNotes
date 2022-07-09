namespace std
{
    template <typename _Tp, _Tp __v>
    struct integral_constant
    {
        static constexpr _Tp value = __v;
        typedef _Tp value_type;
        typedef integral_constant<_Tp, __v> type;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };
    /// The type used as a compile-time boolean with true value.
    typedef integral_constant<bool, true> true_type;

    /// The type used as a compile-time boolean with false value.
    typedef integral_constant<bool, false> false_type;
    template <typename, typename>
    struct is_same
        : public false_type
    {
    };

    template <typename _Tp>
    struct is_same<_Tp, _Tp>
        : public true_type
    {
    };

    template <typename _Tp, typename _Up>
    inline constexpr bool is_same_v = std::is_same<_Tp, _Up>::value;

    template <typename...>
    using __void_t = void;
    // Given Template<T, ...> and U return Template<U, ...>, otherwise invalid.
    template <typename _Tp, typename _Up>
    struct __replace_first_arg
    {
    };

    template <template <typename, typename...> class _Template, typename _Up,
              typename _Tp, typename... _Types>
    struct __replace_first_arg<_Template<_Tp, _Types...>, _Up>
    {
        using type = _Template<_Up, _Types...>;
    };

    template <typename _Tp, typename _Up>
    using __replace_first_arg_t = typename __replace_first_arg<_Tp, _Up>::type;

    struct __allocator_traits_base
    {
        template <typename _Tp, typename _Up, typename = void>
        struct __rebind : __replace_first_arg<_Tp, _Up>
        {
        };

        template <typename _Tp, typename _Up>
        struct __rebind<_Tp, _Up,
                        __void_t<typename _Tp::template rebind<_Up>::other>>
        {
            using type = typename _Tp::template rebind<_Up>::other;
        };
    };
    template <typename _Alloc, typename _Up>
    using __alloc_rebind = typename __allocator_traits_base::template __rebind<_Alloc, _Up>::type;

    template <typename _Alloc>
    struct allocator_traits : __allocator_traits_base
    {
        template <typename _Tp>
        using rebind_alloc = __alloc_rebind<_Alloc, _Tp>;
    };
#define __SIZE_TYPE__ long unsigned int
#define __PTRDIFF_TYPE__ long int
    typedef __SIZE_TYPE__ size_t;
    typedef __PTRDIFF_TYPE__ ptrdiff_t;
    template <typename _Tp>
    struct allocator
    {
        typedef _Tp value_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef _Tp *pointer;
        typedef const _Tp *const_pointer;
        typedef _Tp &reference;
        typedef const _Tp &const_reference;
        template <typename _Tp1>
        struct rebind
        {
            typedef allocator<int> other;
        };
    };
    template <typename _Tp>
    struct allocator_traits<allocator<_Tp>>
    {
        template <typename _Up>
        using rebind_alloc = allocator<_Up>;
    };
}

namespace __gnu_cxx
{
    template <typename _Alloc, typename = typename _Alloc::value_type>
    struct __alloc_traits
        : std::allocator_traits<_Alloc>
    {

        typedef std::allocator_traits<_Alloc> _Base_type;
        template <typename _Tp>
        struct rebind
        {
            typedef typename _Base_type::template rebind_alloc<_Tp> other;
        };
    };

}
template <typename _CharT, typename _Traits, typename _Alloc>
class basic_string
{
public:
    typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_CharT>::other _Char_alloc_type;
    typedef __gnu_cxx::__alloc_traits<_Char_alloc_type> _Alloc_traits;
};

template <typename T>
class char_traits
{
};

template <typename _Tp>
struct MyAllocatorWithoutRebind
{
    typedef _Tp value_type;
};

template <typename _Tp>
struct MyAllocatorWithRebind
{
    typedef _Tp value_type;
    template<typename Up>
    struct rebind {
        using other = MyAllocatorWithRebind<int>;
    };
};

struct MyAllocatorWithRebindNonTemp
{
    typedef int value_type;
    template<typename Up>
    struct rebind {
        using other = MyAllocatorWithRebindNonTemp;
    };
};
static_assert(std::is_same_v<basic_string<char, char_traits<char>, MyAllocatorWithoutRebind<int>>::_Char_alloc_type, MyAllocatorWithoutRebind<char>>);
static_assert(std::is_same_v<basic_string<char, char_traits<char>, MyAllocatorWithRebind<int>>::_Char_alloc_type, MyAllocatorWithRebind<int>>);
static_assert(std::is_same_v<basic_string<char, char_traits<char>, MyAllocatorWithRebindNonTemp>::_Char_alloc_type, MyAllocatorWithRebindNonTemp>);
int main()
{
    return 0;
}