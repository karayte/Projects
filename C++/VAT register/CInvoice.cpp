#include "CInvoice.h"


 CDate CInvoice :: getDate ( ) const
{
    return mDate;
}

//---------------------------------------------------------------------------------------------
string CInvoice :: getBuyer ( ) const
{
    return mBuyer;
}

//---------------------------------------------------------------------------------------------
string CInvoice :: getSeller ( ) const
{
    return mSeller;
}

//---------------------------------------------------------------------------------------------
unsigned int CInvoice :: getAmount ( ) const
{
    return mAmount;
}

//---------------------------------------------------------------------------------------------
double CInvoice :: getVat ( ) const
{
    return mVat;
}

//---------------------------------------------------------------------------------------------
unsigned long long CInvoice :: getOrder ( ) const
{
    return mOrder;
}

//---------------------------------------------------------------------------------------------
void CInvoice :: setBuyersName ( const string & str )
{
    mBuyer = str;
}

//---------------------------------------------------------------------------------------------
void CInvoice :: setSellersName ( const string & str )
{
    mSeller = str;
}

//---------------------------------------------------------------------------------------------
void CInvoice :: setBuyersName ( string && str )
{
    mBuyer = move ( str );
}

//---------------------------------------------------------------------------------------------
void CInvoice :: setSellersName ( string && str )
{
    mSeller = move ( str );
}

//---------------------------------------------------------------------------------------------
void CInvoice :: setOrder ( unsigned long long order )
{
    mOrder = order;
}

//---------------------------------------------------------------------------------------------
bool CInvoice :: operator == ( const CInvoice & inv ) const
{
    string sellerStrThis, sellerStrInv, buyerStrThis, buyerStrInv;
    replaceOrAdd ( mSeller, sellerStrThis );
    replaceOrAdd ( inv . mSeller, sellerStrInv );
    replaceOrAdd ( mBuyer, buyerStrThis );
    replaceOrAdd ( inv . mBuyer, buyerStrInv );
    return sellerStrThis == sellerStrInv && buyerStrThis == buyerStrInv &&
           mDate . compare ( inv . mDate ) == 0 && mAmount == inv . mAmount && mVat == inv . mVat;
}

//---------------------------------------------------------------------------------------------
bool CInvoice :: operator > ( const CInvoice & inv ) const
{
    return mSeller < inv . getSeller ( );
}

//---------------------------------------------------------------------------------------------
bool CInvoice :: operator < ( const CInvoice & inv ) const
{
    return ! ( * this == inv || * this > inv );
}

//---------------------------------------------------------------------------------------------
bool CInvoice :: operator != ( const CInvoice & inv ) const
{
    return ! ( * this == inv );
}