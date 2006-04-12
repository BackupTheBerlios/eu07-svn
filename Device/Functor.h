#ifndef FUNCTOR_H
#define FUNCTOR_H 1

#include <string>
#include <vector>
#include <map>

/*!
 *
 * \class Functor
 * \brief Functions with a state packaged into abstract class
 *
 * \author Zbyszek "ShaXbee" Mandziejewicz
 * \sa http://www.newty.de/fpt/functor.html
 * \sa http://www.tutok.sk/fastgl/callback.html
 *
 */

class Functor
{

protected:
	std::string m_type;

public:
	Functor() { m_type = "Functor"; };
	virtual std::string getType() { return m_type; };

};

/*!
 *
 * \class Functor0
 * \brief Functor with no return value, without parameters
 *
 */

class Functor0 : public Functor
{

public:
	Functor0() { m_type = "Functor0"; };
	virtual void operator()(void) { };

};

/*!
 *
 * \class Functor1
 * \brief Functor with no return value, with single parameter
 *
 */

template <typename P1>
class Functor1 : public Functor
{

public:
	Functor1() { m_type = "Functor1"; };
	virtual void operator()(P1 param) { };

};

/*!
 *
 * \class Functor0R
 * \brief Functor with return value, without parameters
 *
 */

template <typename R>
class Functor0R : public Functor
{

public:
	Functor0R() { m_type = "Functor0R"; };
	virtual R operator()(void) { };

};

/*!
 *
 * \class Functor1R
 * \brief Functor with return value, with single parameter
 *
 */

template <typename R, typename P1>
class Functor1R: public Functor
{

public:
	Functor1R() { m_type = "Functor1R"; };
	virtual R operator()(P1 param) { };

};


template <class C>
class Functor0Translator : public Functor0
{

private:
	typedef void (C::*FuncPtr)();
	FuncPtr m_functionPtr;
	C* m_objectPtr;

public:
	Functor0Translator() { m_type = "Functor0"; };
	Functor0Translator(C* objectPtr, FuncPtr functionPtr) { m_type = "Functor0"; m_objectPtr = objectPtr; m_functionPtr = functionPtr; };
	virtual void operator()(void) { (*m_objectPtr.*m_functionPtr)(); };

};

template <class C, typename P1>
class Functor1Translator : public Functor1<P1>
{

private:
	typedef void (C::*FuncPtr)(const P1);
	FuncPtr m_functionPtr;
	C* m_objectPtr;

public:
	Functor1Translator() { m_type = "Functor1"; };
	Functor1Translator(C* objectPtr, FuncPtr functionPtr) { m_type = "Functor1"; m_objectPtr = objectPtr; m_functionPtr = functionPtr; };
	virtual void operator()(P1 param1) { (*m_objectPtr.*m_functionPtr)(param1); };

};

template <class C, typename R>
class Functor0RTranslator : public Functor0R<R>
{

private:
	typedef R (C::*FuncPtr)();
	FuncPtr m_functionPtr;
	C* m_objectPtr;

public:
	Functor0RTranslator() { m_type = "Functor0R"; };
	Functor0RTranslator(C* objectPtr, FuncPtr functionPtr) { m_type = "Functor0R"; m_objectPtr = objectPtr; m_functionPtr = functionPtr; };
	virtual R operator()(void) { return (*m_objectPtr.*m_functionPtr)(); };

};

template <class C, typename R, typename P1>
class Functor1RTranslator : public Functor1R<R, P1>
{

private:
	typedef R (C::*FuncPtr)(P1);
	FuncPtr m_functionPtr;
	C* m_objectPtr;

public:
	Functor1RTranslator() { m_type = "Functor1R"; };
	Functor1RTranslator(C* objectPtr, FuncPtr functionPtr) { m_type = "Functor1R"; m_objectPtr = objectPtr; m_functionPtr = functionPtr; };
	virtual R operator()(P1 param1) { return (*m_objectPtr.*m_functionPtr)(param1); };

};

#endif