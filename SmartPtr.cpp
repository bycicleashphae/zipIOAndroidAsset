#include "SmartPtr.h"

#include "IfcFileStream.h"
#include "IMemoryStream.h"
#include "IfcFile.h"

#include "IfcStream.h"
template <typename T>
SmartPtr<T>::SmartPtr(T *p): ptr(p), pUse(new unsigned int(1)) {
}

template <typename T>
SmartPtr<T>::SmartPtr(const SmartPtr& src): ptr(src.ptr), pUse(src.pUse) {
	if (pUse){
		++*pUse;
	}
}

template <typename T>
SmartPtr<T>& SmartPtr<T>::operator= (const SmartPtr& rhs) {
	// self-assigning is also 
	if (rhs.pUse){
		++*rhs.pUse;
	}

	decrUse();
	ptr = rhs.ptr;
	pUse = rhs.pUse;

	return *this;
}

template <typename T> T* SmartPtr<T>::get() const
{
    return ptr;
}

template <typename T>
void SmartPtr<T>::reset()
{
    SmartPtr<T> t;
    (*this) = t;
}

template <typename T>
 bool SmartPtr<T>::operator== (T* rhs)
 {
    return ptr == rhs;
 }

 template <typename T>
 bool SmartPtr<T>::operator!= (T* rhs)
 {
    return ptr != rhs;
 }

 template <typename T>
 bool SmartPtr<T>::operator== (const SmartPtr<T>& rhs)
 {
    return ptr == rhs.get();
 }

 template <typename T>
 bool SmartPtr<T>::operator!= (const SmartPtr<T>& rhs)
 {
   return ptr != rhs.get();
 }

 template <typename T>
 T *SmartPtr<T>::operator->() {
    if (ptr)
        return ptr;
    return NULL;
 }

 template <typename T>
 const T *SmartPtr<T>::operator->() const {
    if (ptr)
        return ptr;
     return NULL;
}

template <typename T>
T &SmartPtr<T>::operator*() {
    if (ptr)
        return *ptr;
    int *i = NULL;
    *(i + 100) = 1;
     return *ptr;
}

template <typename T>
const T &SmartPtr<T>::operator*() const {
    if (ptr)
        return *ptr;
    int *i = NULL;
    *(i + 100) = 1;
     return *ptr;
}

template <typename T>
SmartPtr<T>::~SmartPtr() {
    decrUse();
}

template <typename T>
void SmartPtr<T>::decrUse() {
	if (pUse && --*pUse == 0) {
        delete ptr;
        delete pUse;
    }
}


template class SmartPtr<IfcFileStream>;
template class SmartPtr<IMemoryStream>;
template class SmartPtr<IfcFile>;
template class SmartPtr<IfcStream>;

  