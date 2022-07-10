# stlNotes
record my learning notes of stl

## 2022/7/10

1. 实验了basic_string的allocator中的rebind的机制:
    - 传给basic_string的allocator中必须有value_type这个类型成员
    - allocator及其traits的命名空间布局
      ```c++
      namespace std
      {
          
          /** bits/allocator_traits.h **/
          // allocator_traits的基类
          struct __allocator_traits_base;
          
          template <typename _Alloc>
          struct allocator_traits : __allocator_traits_base;
          
          // specialization for std::allocator
          template <typename _Tp>
          struct allocator_traits<allocator<_Tp>>;
          
          /** bits/allocator.h **/
          template <typename _Tp>
          struct allocator : public __allocator_base<_Tp>;
          
          /** bits/c++allocator.h **/
          template<typename _Tp>
          using __allocator_base = __gnu_cxx::new_allocator<_Tp>;
          
          /** bits/basic_string.h **/
          template<typename _CharT, typename _Traits, typename _Alloc>
          class basic_string
          {
            typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
        rebind<_CharT>::other _Char_alloc_type;
            typedef __gnu_cxx::__alloc_traits<_Char_alloc_type> _Alloc_traits;
          };
      }
      
      // __gun_cxx: GNU extensions for public use
      namespace __gnu_cxx
      {
          /** ext/alloc_traits.h **/
          template <typename _Alloc, typename = typename _Alloc::value_type>
          struct __alloc_traits
              : std::allocator_traits<_Alloc>
      }
      ```
    - allocator和rebind
    
    1. 如果用的是默认的std::allocator，那么string里的_Char_alloc_type即`std::allocator<_CharT>`，推导过程：
        - `typename __gnu_cxx::__alloc_traits<_Alloc>::template rebind<_CharT>::other`
        - `typename _Base_type::template rebind_alloc<_CharT>, _Base_type = std::allocator_traits<_Alloc>`
        - `allocator_traits<allocator<_CharT>>:rebind_alloc<_CharT>;`
        - `allocator<_CharT>`


    2. 如果用的是自定义的allocator，假设为MyAllocator，分两种情况：
        - 有自定义的rebind模板类
        
          此时_Char_alloc_type为MyAllocator::rebind<_CharT>，自定义的allocator可以非模板类；
        - 没有自定义的rebind
        
          此时_Char_alloc_type为MyAllocator<_CharT>，此时自定义的allocator必须为模板类，
          因为此时是通过__replace_first_arg_t将MyAllocator的第一个模板参数直接替换为_CharT得到rebind后的类型的；

2. _If_sv在string中的作用

    [Why is SFINAE for one of the std::basic_string constructors so restrictive?](https://stackoverflow.com/questions/70591571/why-is-sfinae-for-one-of-the-stdbasic-string-constructors-so-restrictive)

    [LWG 2758. std::string{}.assign("ABCDE", 0, 1) is ambiguous](https://cplusplus.github.io/LWG/lwg-defects.html#2758)

    [LWG 2946. LWG 2758's resolution missed further corrections](https://cplusplus.github.io/LWG/issue2946)

    主要用处是为了解决歧义，在C++17引入string_view之后，曾经有这么两个构造函数
    ```c++
    basic_string& assign(const  basic_string& str, size_type pos, size_type n = npos);

    basic_string& assign(basic_string_view<charT, traits> sv, size_type pos, size_type n = npos);
    ```
    此时如果传入const char *或者char *，两个函数在编译器看来匹配度一样，就出现了歧义，编译失败。因此为了
    解决这种歧义，可以对所有形式(const char *, char *, string,...)的调用都写一个assign函数，但是这样太麻烦。因此引入了模板，并通过对模板参数添加_If_sv的constraints，消除歧义。
