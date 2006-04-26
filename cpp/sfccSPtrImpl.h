

template <> void sfccSPtr<T>::release() 
{ 
   if (enc) {
      if (--enc->refCount == 0) {
        delete enc;
      }
      enc = 0;
   }
}

template <> sfccSPtr<T>::sfccSPtr() 
{
      enc= new T;
      enc->refCount=1;
}

template <> sfccSPtr<T>::sfccSPtr(sfccSPtr* p)
{
   if (p) {
      enc=p->enc;
      p->enc->refCount++;
   }
}

template <> sfccSPtr<T>::~sfccSPtr() {
   release();
}

template <> sfccSPtr<T>::sfccSPtr(const sfccSPtr& r) 
{ 
   enc=r.enc;
   if (enc) enc->refCount++;
}
   
template <> sfccSPtr<T>& sfccSPtr<T>::operator=(const sfccSPtr& r) 
{
   if (this != &r) {
      release();
      enc=r.enc;
      if (enc) enc->refCount++;
   }
   return *this;
}
   
template <> T& sfccSPtr<T>::operator *() const 
{ 
   return *enc;
}

template <> T* sfccSPtr<T>::operator->() const 
{
   return enc;
}

