// * ************************************************************************ //
// * * Callback header                                                    * * //
// * *   : defines template functor and adaptor for producing callback    * * //
// * *     objects                                                        * * //
// * *                                                                    * * //
// * * Disclaimer: This document represents no significant intellectual   * * //
// * * property and my be used freely. The author(s) takes no             * * //
// * * responsibility for any loss or damage arising from the use of      * * //
// * * the computer code contained herein. Where this document is         * * //
// * * submitted as part of an assessment task this header must remain    * * //
// * * intact and the student must make clear indication which parts      * * //
// * * have been added by themselves.                                     * * //
// * *                                                                    * * //
// * ********************************************************************** * //
// * ********************************************************************** * //
// * * Callback.h                                                         * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 26/03/11     Last Modified: 31/03/11                      * * //
// * ********************************************************************** * //
#ifndef CALLBACK_H
#define CALLBACK_H

template <
	typename A,
	typename R
>
class BFunctor1
{
	public:
		virtual R operator()(A) = 0;
};

template <
	typename A,
	typename R
>
class Functor1 : public BFunctor1<A,R>
{
	public:
		typedef R (*_func)(A);

		Functor1(_func _f) : fpFunc(_f) {}
		R operator()(A a) { return (*fpFunc)(a); }

	private:
		_func	fpFunc;
};

template <
	typename T,
	typename A,
	typename R
>
class Adaptor1 : public BFunctor1<A,R>
{
	public:
		typedef typename T	_type;
		typedef R (_type::*_memfunc)(A);

		Adaptor1(_type * _obj, _memfunc _fnc) : pObj(_obj), fpFunc(_fnc) {}
		R operator()(A a) { return (pObj->*fpFunc)(a); }

	private:
		_type		*pObj;
		_memfunc	fpFunc;
};



#endif