#ifndef SINGLETON_H
#define SINGLETON_H

// Singleton template class
template <class T> 
class Singleton 
{ 
	public: 
		static T& GetInstance();
	protected: 
		Singleton(){} 
}; 

template <typename T>
T& Singleton<T>::GetInstance()
{
	static T* s_pInstance=NULL;
	if (s_pInstance==NULL)
		s_pInstance = new T;
	return *s_pInstance;
}

#endif
// Usage:
//     class X : public Singleton<X>
//		{	...
//		};
//
// Macro call:
//		#define X_Instance X::GetInstance()
//
// Comments:
//		Remember to use private constructors and assignment(=)
//		in derived classes