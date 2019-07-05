1:if 与 switch 的括号中间允许进行变量初始化
2：initializer_list 辅助类完成一致初始化操作
3：结构化绑定-----使用tuple可以实现多返回值功能
4:类型推导:auto（不允许用于函数形参但可用于推导返回值类型，不能用于数组类型推导）与decltype
5:控制流 if constexpr() 让代码在编译时完成分支判断，更加提升代码效率
6：区间for迭代--- for(i : s);
7:外部模板--->扩充了原来强制编译器特定位置实例化模板的语法，是我们能够显式的通知编译器何时进行模板的实例化
    template class std::vector<bool>;          // 强行实例化
    extern template class std::vector<double>; // 不在该当前编译文件中实例化模板
8:类型别名模板，统一使用using 
9：默认模板参数--->可以为模板指定默认参数
    template<typename T = int, typename U = int>
    auto add(T x, U y) -> decltype(x+y) {
    return x+y;
    }
10:变长模板参数---->允许任意个数、任意类别的模板参数，同时也不需要在定义时将参数的个数固定。
    template<typename... Ts> class Magic;
    template<typename... Args> void printf(const std::string &str, Args... args);
    模板参数展开--->递归方式展开、初始化列表展开
11:折叠表达式--->会将...参数按照一定操作进行展开并运算
    template<typename ... T>
    auto sum(T ... t) {
        return (t + ...);
    }
    int main() {
        std::cout << sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << std::endl;
    }
12:委托构造：
    C++11 引入了委托构造的概念，这使得构造函数可以在同一个类中一个构造函数调用另一个构造函数，从而达到简化代码的目的：

    class Base {
    public:
        int value1;
        int value2;
        Base() {
            value1 = 1;
        }
        Base(int value) : Base() { // 委托 Base() 构造函数
            value2 = value;
        }
    };

    int main() {
        Base b(2);
        std::cout << b.value1 << std::endl;
        std::cout << b.value2 << std::endl;
    }

13：继承构造：
    在传统 C++ 中，构造函数如果需要继承是需要将参数一一传递的，这将导致效率低下。C++11 利用关键字 using 引入了继承构造函数的概念：

    class Base {
    public:
        int value1;
        int value2;
        Base() {
            value1 = 1;
        }
        Base(int value) : Base() { // 委托 Base() 构造函数
            value2 = value;
        }
    };
    class Subclass : public Base {
    public:
        using Base::Base; // 继承构造
    };
    int main() {
        Subclass s(3);
        std::cout << s.value1 << std::endl;
        std::cout << s.value2 << std::endl;
    }

14.显式虚函数重载  
    override：
    当重载虚函数时，引入 override 关键字将显式的告知编译器进行重载，编译器将检查基函数是否存在这样的虚函数，否则将无法通过编译
    struct Base {
        virtual void foo(int);
    };
    struct SubClass: Base {
        virtual void foo(int) override; // 合法
        virtual void foo(float) override; // 非法, 父类没有此虚函数
    };
    final：
    final 则是为了防止类被继续继承以及终止虚函数继续重载引入的。
    struct Base {
        virtual void foo() final;
    };
    struct SubClass1 final: Base {
    }; // 合法

    struct SubClass2 : SubClass1 {
    }; // 非法, SubClass1 已 final

    struct SubClass3: Base {
        void foo(); // 非法, foo 已 final
    };

15:显示禁用默认函数 =delete， 显示采用默认函数=default
    class Magic {
        public:
        Magic() = default; // 显式声明使用编译器生成的构造
        Magic& operator=(const Magic&) = delete; // 显式声明拒绝编译器生成构造
        Magic(int magic_number);
    }
16: 强枚举类型 enumaration class
    在传统 C++中，枚举类型并非类型安全，枚举类型会被视作整数，则会让两种完全不同的枚举类型可以进行直接的比较（虽然编译器给出了检查，但并非所有），甚至同一个命名空间中的不同枚举类型的枚举值名字不能相同，这通常不是我们希望看到的结果。
    C++11 引入了枚举类（enumaration class），并使用 enum class 的语法进行声明：
    enum class new_enum : unsigned int {
        value1,
        value2,
        value3 = 100,
        value4 = 100
    };

17:Lambda表达式
    [捕获列表](参数列表) mutable(可选) 异常属性 -> 返回类型 {
    // 函数体
    }
    捕获分为值捕获，引用捕获，隐式捕获以及表达式捕获
    表达式捕获(C++14)

    上面提到的值捕获、引用捕获都是已经在外层作用域声明的变量，因此这些捕获方式捕获的均为左值，而不能捕获右值。

    C++14 给与了我们方便，允许捕获的成员用任意的表达式进行初始化，这就允许了右值的捕获，被声明的捕获变量类型会根据表达式进行判断，判断方式与使用 auto 本质上是相同的：

    #include <iostream>
    #include <utility>

    int main() {
        auto important = std::make_unique<int>(1);
        auto add = [v1 = 1, v2 = std::move(important)](int x, int y) -> int {
            return x+y+v1+(*v2);
        };
        std::cout << add(3,4) << std::endl;
        return 0;
    }
    在上面的代码中，important 是一个独占指针，是不能够被捕获到的，这时候我们需要将其转移为右值，在表达式中初始化。

18:泛型lambda：lambda不是普通的函数，其不允许被模板化，同时函数中的形参不能为auto的规则对其无效
   因此lambda的泛型不能用模板而如此使用：
    auto add = [](auto x, auto y) {
        return x+y;
    };

    add(1, 2);
    add(1.1, 2.2);

19:函数对象包装器(函数的容器)：
    function bind placeholder

20:右值与右值引用

21：std:array :与vector不同，他不会自动扩容，与数组不同，他提供了额外的方法利于对数组进行更加高效的操作，同时更好的使用std算法库

22:std::forward_list：单向链表 std::list双向链表

23:有序关联容器  std::map/std::multimap/std::set/set::multiset 内部通过红黑树进行实现，插入和搜索的平均复杂度为o(log(size))

24:无序关联容器std::unordered_map/std::unordered_multimap 和 std::unordered_set/std::unordered_multiset 内部通过hash实现，复杂度为o(1)，在不关心容器内部顺序的时候用这个能够大幅度改善效率

25:元组std::tuple--->用于存放不同类型的各种数据

26:std::is_same 用于判定两个类别是否相同

27:std::shared_ptr,std::unique_ptr 不可复制，但是可以被move,std::weak_ptr弱引用不会增加shared_ptr的引用计数，也不存在*和->运算符，他存在就是为了检查shared_ptr是否存在。

28：正则表达式(regex expression)

29:线程相关std::thread,std::mutex，std::lock_guard, std::unique_lock -->如果使用了条件变量std::condition_variable::wait则必须使用此作为参数
   异步返回操作结果:std::future, std::packaged_task

//std::future 则是提供了一个访问异步操作结果的途径，这句话很不好理解。为了理解这个特性，我们需要先理解一下在 C++11之前的多线程行为。
//
//试想，如果我们的主线程 A 希望新开辟一个线程 B 去执行某个我们预期的任务，并返回我一个结果。而这时候，线程 A 可能正在忙其他的事情，无暇顾及 B 的结果，所以我们会很自然的希望能够在某个特定的时间获得线程 B 的结果。
//
//在 C++11 的 std::future 被引入之前，通常的做法是：创建一个线程A，在线程A里启动任务 B，当准备完毕后发送一个事件，并将结果保存在全局变量中。而主函数线程 A 里正在做其他的事情，当需要结果的时候，调用一个线程等待函数来获得执行的结果。
//
//而 C++11 提供的 std::future 简化了这个流程，可以用来获取异步任务的结果。自然地，我们很容易能够想象到把它作为一种简单的线程同步手段。
//
//此外，std::packaged_task 可以用来封装任何可以调用的目标，从而用于实现异步的调用。例如：
//
//#include <iostream>
//#include <future>
//#include <thread>
//
//int main()
//{
//    // 将一个返回值为7的 lambda 表达式封装到 task 中
//    // std::packaged_task 的模板参数为要封装函数的类型
//    std::packaged_task<int()> task([](){return 7;});
//    // 获得 task 的 future
//    std::future<int> result = task.get_future(); // 在一个线程中执行 task
//    std::thread(std::move(task)).detach(); std::cout << "Waiting...";
//    result.wait();
//    // 输出执行结果
//    std::cout << "Done!" << std:: endl << "Result is " << result.get() << '\n';
//}
//在封装好要调用的目标后，可以使用 get_future() 来获得一个 std::future 对象，以便之后实施线程同步。
std::condition_variable

30:noexcept 表明不产生异常

31:原始字符串字面量R（）省去了添加转义字符的负担
// #include <iostream>
// #include <string>

// int main() {
//     std::string str = R"(C:\What\The\Fxxk)";
//     std::cout << str << std::endl;
//     return 0;
// }

32:自定义字面量
C++11 引进了自定义字面量的能力，通过重载双引号后缀运算符实现：

//// 字符串字面量自定义必须设置如下的参数列表
//std::string operator"" _wow1(const char *wow1, size_t len) {
//return std::string(wow1)+"woooooooooow, amazing";
//}
//
//std::string operator"" _wow2 (unsigned long long i) {
//return std::to_string(i)+"woooooooooow, amazing";
//}
//
//int main() {
//auto str = "abc"_wow1;
//auto num = 1_wow2;
//std::cout << str << std::endl;
//std::cout << num << std::endl;
//return 0;
//}
//自定义字面量支持四种字面量：
//
//整型字面量：重载时必须使用 unsigned long long、const char *、模板字面量算符参数，在上面的代码中使用的是前者；
//浮点型字面量：重载时必须使用 long double、const char *、模板字面量算符；
//字符串字面量：必须使用 (const char *, size_t) 形式的参数表；
//字符字面量：参数只能是 char, wchar_t, char16_t, char32_t 这几种类型。






