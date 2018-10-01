#ifndef REFLECTION_HPP
#define REFLECTION_HPP

#include "Interfaces/IReflected.hpp"

template<class T>
class FuncWrap {
public:
	FuncWrap() : func_map() {}

	typedef void*(__cdecl T::*fn)(...);

	std::map < std::string, fn> func_map;

	template<typename ...Args>
	void call(std::string fn, T* t, Args ... args) {
		(*t.*func_map[fn])(args ...);
	}

	void add(std::string func, fn fnc) {
		func_map.insert({ func, fnc });
	}
	template<typename Ty>
	fn convert(Ty t) {
		return (fn)(Ty)t;
	}
};

template<class T>
class Reflected : public IReflected , public T{

public:
	
	template<typename ...Args>
	Reflected(Args ... args) : T(args) {
		T::register_functions(wrap);
	}

	void call(std::string fn, std::string arg) {
		//wrap->call(fn, this, )
	}

	FuncWrap<T> wrap;

};


#endif

#if 0

template<class T>
class FuncWrap
{
public:
    FuncWrap() : func_map() {}

    typedef void*(__cdecl T::*fn)(...);
    
    std::map < std::string, fn> func_map;
    
    template<typename ...Args>
    void call(std::string fn, T* t, Args ... args)
    {
        (*t.*func_map[fn])(args ...);
    }

    void add(std::string func, fn fnc)
    {
        func_map.insert({ func, fnc });
    }
    template<typename Ty>
    fn convert(Ty t)
    {
        return (fn)(Ty)t;
    }
};


#define STR(x)  #x
//reflect and reflect_no_arg needs the following arguments an existing instance of FuncWrap<cls>
//The class cls, and the function in cls, eg cls::Test
//reflect takes an additional parameter that defines the arguments for that function.
//returns void, inserts the function as a string with parameteres eg. [cls = Test, fnc = Test::foo, ... = int]
//generated string will be "Test::foo(int)"
//for reflect_no_arg "Test::foo()"
#define reflect(fnmap, cls, fnc, ...) fnmap.func_map.insert({""STR(fnc)"("STR(__VA_ARGS__)")", fnmap.convert<void(__cdecl cls::*)(__VA_ARGS__)>(&fnc)} )
#define reflect_no_arg(fnmap, cls, fnc) fnmap.func_map.insert({""STR(fnc)"()", fnmap.convert(&fnc)} )

class Test
{
    void f() {
        std::cout << "test::f()" << std::endl; 
    }

    void __cdecl bar(int q) {
        std::cout << "test::bar(" << std::to_string(q)<< ")\n";
    }

    void __cdecl bar(int q, double d) {
        std::cout << "test::bar(" << std::to_string(q) << "," << std::to_string(d) << ")\n";
    }

    template<typename T>
    void __cdecl typenamed(T t) {
        std::cout << "test::typenamed(" << std::to_string(t) << ")\n";
    }

public:

    static void register_functions(FuncWrap<Test> & const fnwrap)
    {

        reflect_no_arg(fnwrap, Test, Test::f);
        reflect(fnwrap, Test, Test::bar, int);
        fnwrap.func_map.insert({ "f" ,  fnwrap.convert(&Test::f) });
        fnwrap.func_map.insert({ "bar(int)" ,  fnwrap.convert<void(__cdecl Test::*)(int)>(&Test::bar) });
        fnwrap.func_map.insert({ "bar(int,double)" ,  fnwrap.convert<void(__cdecl Test::*)(int, double)>(&Test::bar) });
        fnwrap.func_map.insert({ "typenamed<int>" ,  fnwrap.convert(&Test::typenamed<int>) });
        fnwrap.func_map.insert({ "typenamed<double>" ,  fnwrap.convert(&Test::typenamed<double>) });
    }

    Test() {}
};


int main()
{

    FuncWrap<Test> twrap;

    Test::register_functions(twrap);

    Test t;

    void* vt = &t;

    twrap.call("f", &t);
    twrap.call("Test::f()", &t);
    twrap.call("bar(int)", &t, 54);
    twrap.call("bar(int,double)", &t, 54, 55.0);
    twrap.call("typenamed<double>", &t, 123.0);
    twrap.call("typenamed<int>", &t, 567);

    return 0;
}

#endif