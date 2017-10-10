#ifndef MY_SMART_PTR_TO_REPLACE_BOOST_PTR_20140030203902
#define MY_SMART_PTR_TO_REPLACE_BOOST_PTR_20140030203902

template <typename T>
class SmartPtr 
{
public:
    SmartPtr(T *p = 0);
    SmartPtr(const SmartPtr& src);
    SmartPtr& operator= (const SmartPtr& rhs) ;
    bool operator== (T*rhs) ;
    bool operator!= (T*rhs) ;
    bool operator== (const SmartPtr& rhs) ;
    bool operator!= (const SmartPtr& rhs) ;
    T*get() const;
    void reset();
    
    T *operator->() ;
    const T *operator->() const ;
    T &operator*() ;
    const T &operator*() const ;
    ~SmartPtr() ;
    
private:
    void decrUse() ;
    T *ptr;
    unsigned int *pUse;
};
#endif