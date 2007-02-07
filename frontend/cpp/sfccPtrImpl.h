

template <> void sfccPtr<Tc, Te>::release() 
{ 
   // printf("+++ sfccPtr<Tc, Te>::release(): %p-%p\n",this,enc);
   if (enc) {
      int rc=enc->decRefCount();
      if (rc <= 0) {
         enc->releaseEnc();
      }
      delete enc;
      enc = 0;
   }
}

template <> sfccPtr<Tc, Te>::sfccPtr(Te* p) 
{
   // printf("+++ sfccPtr<Tc, Te>::sfccPtr(Te* p): %p %p\n",this,p);
   if (p) {
      enc=new Tc(p);
      enc->incRefCount();
   }
   else enc=NULL;
}
   
template <> sfccPtr<Tc, Te>::sfccPtr(Tc* p) 
{
   // printf("+++ sfccPtr<Tc, Te>::sfccPtr(Tc* p): %p %p\n",this,p);
   if (p) {
      enc=p;
      enc->incRefCount();
   }
   else enc=NULL;
}

template <> sfccPtr<Tc, Te>::sfccPtr(sfccPtr* p)
{
   // printf("+++ sfccPtr<Tc, Te>::sfccPtr(sfccPtr* p): %p %p\n",this,p);
   if (p) {
      enc=p->enc;
      p->enc->incRefCount();
   }
   else enc=NULL;
}

template <> sfccPtr<Tc, Te>::~sfccPtr() 
{
   //  printf("+++ sfccPtr<Tc, Te>::~sfccPtr(): %p\n",this);
   release();
}

template <> sfccPtr<Tc, Te>::sfccPtr(const sfccPtr& r) 
{ 
   // printf("+++ sfccPtr<Tc, Te>::sfccPtr(const sfccPtr& r): %p %p\n",this,&r);
   enc=NULL;
   if (r.enc) enc=r.enc;
   if (enc) enc->incRefCount();
}
   
template <> sfccPtr<Tc, Te>& sfccPtr<Tc, Te>::operator=(const sfccPtr& r) 
{
   // printf("+++ sfccPtr<Tc, Te>::operator=(const sfccPtr& r: %p %p\n",this,&r);
   if (this != &r) {
      release();
      enc=r.enc;
      if (enc) enc->incRefCount();
   }
   return *this;
}
   
template <> Tc& sfccPtr<Tc, Te>::operator *() const 
{ 
   return *enc;
}
template <> Tc* sfccPtr<Tc, Te>::operator->() const 
{
   return enc;
}

