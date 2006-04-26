

#ifndef sfccPtr_H
#define sfccPtr_H

template <class Tc, class Te> class  sfccPtr { 
  friend class CimData;
  friend class CimClient;
  friend class CimcInstanceEnumeration;
  friend class CimcObjectPathEnumeration;
  friend class CimcClassEnumeration;
  friend class CimcClass;
  friend class CimcInstance;
  friend class CimcObjectPath;
  friend class CimcIterator;
  friend class CimcDateTime;
  private:
   Tc* enc;
   void release();
   sfccPtr(Te* p);
   sfccPtr(Tc* p);
  public:
   explicit sfccPtr(sfccPtr* p);
   explicit sfccPtr() { enc=NULL; }
  ~sfccPtr();
   sfccPtr(const sfccPtr& r);
   sfccPtr& operator=(const sfccPtr& r);
   Tc& operator *() const;
   Tc* operator->() const;
};

#endif 
