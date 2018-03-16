#pragma once
#ifndef _BIT_TYPE_H
#define _BIT_TYPE_H
#ifndef _STD 
#define _STD ::std::
#endif
#include <functional>
#include <type_traits>

template<typename _Ty> struct BIT_type
{

	using Func = typename std::tr1::function < _Ty(const _Ty&, const _Ty&)>;

	using Inverse_Funciton = typename std::tr1::function<_Ty(const _Ty&)>;

	using modify_func = typename std::tr1::function<_Ty(const _Ty&)>;

};
#endif // !_BIT_TYPE_H
