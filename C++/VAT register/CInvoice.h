#ifndef UL5_CINVOICE_H
#define UL5_CINVOICE_H

#include "CDate.h"
#include "usefulFunctions.h"

/* This class represents one invoice */
class CInvoice
{
public:
    CInvoice ( const CDate & date, string seller, string buyer, unsigned int amount, double vat )
            : mDate ( date ), mSeller ( move ( seller ) ), mBuyer ( move ( buyer ) ), mAmount ( amount ), mVat ( vat ), mOrder ( 0 )
    {
    }

    //---------------------------------------------------------------------------------------------
    CInvoice ( CDate && date, string && seller, string && buyer, unsigned int amount, double vat )
            : mDate ( date ), mSeller ( move ( seller ) ), mBuyer ( move ( buyer ) ), mAmount ( amount ),
              mVat ( vat ), mOrder ( 0 )
    {
    }

    //---------------------------------------------------------------------------------------------
    CInvoice ( const CInvoice & inv )
            : mDate ( inv . mDate ), mSeller ( inv . mSeller ), mBuyer ( inv . mBuyer ),
              mAmount ( inv . mAmount ), mVat ( inv . mVat ), mOrder ( inv . mOrder )
    {
    }

    //---------------------------------------------------------------------------------------------
    CInvoice ( CInvoice && inv ) noexcept
            : mDate ( inv . mDate ), mSeller ( move ( inv . mSeller ) ), mBuyer ( move ( inv . mBuyer ) ),
              mAmount ( inv . mAmount ), mVat ( inv . mVat ), mOrder ( inv . mOrder )
    {
    }

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] CDate getDate ( ) const;

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] string getBuyer ( ) const;

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] string getSeller ( ) const;

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] unsigned int getAmount ( ) const;

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] double getVat ( ) const;

    //---------------------------------------------------------------------------------------------
    [[nodiscard]] unsigned long long getOrder ( ) const;

    //---------------------------------------------------------------------------------------------
    void setBuyersName ( const string & str );

    //---------------------------------------------------------------------------------------------
    void setSellersName ( const string & str );

    //---------------------------------------------------------------------------------------------
    void setBuyersName ( string && str );

    //---------------------------------------------------------------------------------------------
    void setSellersName ( string && str );

    //---------------------------------------------------------------------------------------------
    void setOrder ( unsigned long long order );

    //---------------------------------------------------------------------------------------------
    bool operator == ( const CInvoice & inv ) const;

    //---------------------------------------------------------------------------------------------
    bool operator > ( const CInvoice & inv ) const;

    //---------------------------------------------------------------------------------------------
    bool operator < ( const CInvoice & inv ) const;

    //---------------------------------------------------------------------------------------------
    bool operator != ( const CInvoice & inv ) const;

private:
    CDate mDate;
    string mSeller;
    string mBuyer;
    unsigned int mAmount;
    double mVat;
    unsigned long long mOrder;
};


#endif //UL5_CINVOICE_H
