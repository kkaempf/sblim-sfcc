

#ifndef sfccSPtr_H
#define sfccSPtr_H

template <class T> class  sfccSPtr { 
  friend class CimcIterator;
  friend class CimcClass;
  friend class CimcInstance;
  private:
   T* enc;
   void release();
   sfccSPtr();
  public:
   explicit sfccSPtr(sfccSPtr* p);
   ~sfccSPtr();
   sfccSPtr(const sfccSPtr& r);
   sfccSPtr& operator=(const sfccSPtr& r);
   T& operator *() const;
   T* operator->() const;
};

#endif 
