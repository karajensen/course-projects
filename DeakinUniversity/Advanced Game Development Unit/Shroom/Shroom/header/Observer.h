// * ************************************************************************ //
// * * Observer header                                                    * * //
// * *   : defines pure virtual interface for Observer pattern objects    * * //
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
// * * Observer.h                                                         * * //
// * * Author: Tim Wilkin                                                 * * //
// * * Created: 26/03/11     Last Modified: 31/03/11                      * * //
// * ********************************************************************** * //

#ifndef OBSERVER_H
#define OBSERVER_H


class Observer
{
	public:
		virtual void notify() = 0;
		virtual ~Observer() = 0;
};
inline Observer::~Observer(){}


class Observable
{
	public:
		virtual void attach(Observer*) = 0;
		virtual ~Observable() = 0;
};
inline Observable::~Observable(){}


#endif