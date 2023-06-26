#include "CVATRegister.h"


/* This structure is created for creating hash for CInvoice instance */
size_t CVATRegister :: CInvoiceHash :: operator () ( const CInvoice & inv ) const
    {
        hash < string > strH;
        hash < unsigned int > unIntH;
        hash < double > doubleH;
        CDate date = inv . getDate ( );
        size_t yearH = date . getYear ( );
        size_t monthH = date . getMonth ( );
        size_t dayHash = date . getDay ( );

        size_t dateH = yearH;
        dateH ^= ( monthH << 4 );
        dateH ^= ( dayHash << 8 );
        string seller, buyer;
        replaceOrAdd ( inv . getSeller(), seller );
        replaceOrAdd ( inv . getBuyer(), buyer );
        size_t sellerH = strH ( seller );
        size_t buyerH = strH ( buyer );
        size_t amountH = unIntH ( inv . getAmount ( ) );
        size_t vatH = doubleH ( inv . getVat ( ) );

        size_t resultH = dateH;
        resultH ^= ( sellerH << 1 );
        resultH ^= ( buyerH << 2 );
        resultH ^= ( amountH << 3 );
        resultH ^= ( vatH << 4 );

        return resultH;
    }

//---------------------------------------------------------------------------------------------
bool CVATRegister :: registerCompany ( const string & name )
{
    string name_copy;
    replaceOrAdd ( name, name_copy );
    if ( mCompaniesByNames . find ( name_copy ) != mCompaniesByNames . end ( ) )
        return false;
    mCompaniesByNames . emplace ( move ( name_copy ), name );
    return true;
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: canAddInvoice ( CInvoice & inv, bool issued )
{
    string seller, buyer;
    replaceOrAdd ( inv . getSeller(), seller );
    replaceOrAdd ( inv . getBuyer(), buyer );
    if ( mCompaniesByNames . find ( seller ) == mCompaniesByNames . end ( )
         || mCompaniesByNames . find ( buyer ) == mCompaniesByNames . end ( )
         || seller == buyer )
        return false;
    inv . setOrder ( mOrderInv );
    mOrderInv ++;
    if ( issued )
    {
        if ( ! mCompaniesInvoicesIssued[ seller ] . insert ( inv ) . second )
        {
            mOrderInv --;
            return false;
        }
        if ( mCompaniesInvoicesAccepted[ buyer ] . find ( inv ) !=
             mCompaniesInvoicesAccepted[ buyer ] . end ( ) )
        {
            mSavedUnmatched[ seller ] . erase ( inv );
            mSavedUnmatched[ buyer ] . erase ( inv );
        }
        else
        {
            inv . setBuyersName ( mCompaniesByNames[ buyer ] );
            inv . setSellersName ( mCompaniesByNames[ seller ] );
            mSavedUnmatched[ seller ] . insert ( inv );
            mSavedUnmatched[ buyer ] . insert ( inv );
        }
        return true;
    }
    if ( ! mCompaniesInvoicesAccepted[ buyer ] . insert ( inv ) . second )
    {
        mOrderInv --;
        return false;
    }
    if ( mCompaniesInvoicesIssued[ seller ] . find ( inv ) != mCompaniesInvoicesIssued[ seller ] . end ( ) )
    {
        mSavedUnmatched[ seller ] . erase ( inv );
        mSavedUnmatched[ buyer ] . erase ( inv );
    }
    else
    {
        inv . setBuyersName ( mCompaniesByNames[ buyer ] );
        inv . setSellersName ( mCompaniesByNames[ seller ] );
        mSavedUnmatched[ seller ] . insert ( inv );
        mSavedUnmatched[ buyer ] . insert ( inv );
    }
    return true;
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: addIssued ( const CInvoice & x )
{
    CInvoice inv = x;
    return canAddInvoice ( inv, true );
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: addIssued ( CInvoice && x ) noexcept
{
    return canAddInvoice ( x, true );
}


//---------------------------------------------------------------------------------------------
bool CVATRegister :: addAccepted ( const CInvoice & x )
{
    CInvoice inv = x;
    return canAddInvoice ( inv, false );
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: addAccepted ( CInvoice && x ) noexcept
{
    return canAddInvoice ( x, false );
}

//---------------------------------------------------------------------------------------------
void CVATRegister :: cleanAfterDelete ( CInvoice & inv, const string & seller, const string & buyer, bool unmatched )
{
    if ( unmatched )
    {
        inv . setBuyersName ( mCompaniesByNames[ buyer ] );
        inv . setSellersName ( mCompaniesByNames[ seller ] );
        mSavedUnmatched[ buyer ] . insert ( inv );
        mSavedUnmatched[ seller ] . insert ( inv );
    }
    else
    {
        mSavedUnmatched[ buyer ] . erase ( inv );
        mSavedUnmatched[ seller ] . erase ( inv );
    }
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: deleteInvoice ( CInvoice & inv, bool issued ) noexcept
{
    string seller, buyer;
    replaceOrAdd ( inv . getSeller(), seller );
    replaceOrAdd ( inv . getBuyer(), buyer );
    if ( issued )
    {
        if ( mCompaniesInvoicesIssued[ seller ] . erase ( inv ) )
        {
            if ( mCompaniesInvoicesAccepted[ buyer ] . find ( inv ) !=
                 mCompaniesInvoicesAccepted[ buyer ] . end ( ) )
                cleanAfterDelete ( inv, seller, buyer, true );
            else
                cleanAfterDelete ( inv, seller, buyer, false );
            return true;
        }
        return false;
    }

    if ( mCompaniesInvoicesAccepted[ buyer ] . erase ( inv ) )
    {
        if ( mCompaniesInvoicesIssued[ seller ] . find ( inv ) !=
             mCompaniesInvoicesIssued[ seller ] . end ( ) )
            cleanAfterDelete ( inv, seller, buyer, true );
        else
            cleanAfterDelete ( inv, seller, buyer, false );
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: delIssued ( const CInvoice & x )
{
    CInvoice inv = x;
    return deleteInvoice ( inv, true );
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: delIssued ( CInvoice && x ) noexcept
{
    return deleteInvoice ( x, true );
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: delAccepted ( const CInvoice & x )
{
    CInvoice inv = x;
    return deleteInvoice ( inv, false );
}

//---------------------------------------------------------------------------------------------
bool CVATRegister :: delAccepted ( CInvoice && x ) noexcept
{
    return deleteInvoice ( x, false );
}

/* This method is responsible for creating lower case version of the given string */
void myTolower ( string & str )
{
    for ( size_t i = 0 ; i < str . length ( ) ; i ++ )
        str[ i ] = ( char ) tolower ( str[ i ] );
}

//---------------------------------------------------------------------------------------------
int CVATRegister :: cmpBySeller ( const CInvoice & inv1, const CInvoice & inv2 )
{
    string str1 = inv1 . getSeller(), str2 = inv2 . getSeller();
    myTolower ( str1 );
    myTolower ( str2 );
    if ( str1 < str2 )
        return - 1;
    if ( str1 > str2 )
        return 1;
    return 0;
}

//---------------------------------------------------------------------------------------------
int CVATRegister :: cmpByBuyer ( const CInvoice & inv1, const CInvoice & inv2 )
{
    string str1 = inv1 . getBuyer(), str2 = inv2 . getBuyer();
    myTolower ( str1 );
    myTolower ( str2 );
    if ( str1 < str2 )
        return - 1;
    if ( str1 > str2 )
        return 1;
    return 0;
}

//---------------------------------------------------------------------------------------------
int CVATRegister :: cmpByAmount ( const CInvoice & inv1, const CInvoice & inv2 )
{
    unsigned int amount1 = inv1 . getAmount ( );
    unsigned int amount2 = inv2 . getAmount ( );
    if ( amount1 < amount2 )
        return - 1;
    if ( amount1 > amount2 )
        return 1;
    return 0;
}

//---------------------------------------------------------------------------------------------
int CVATRegister :: cmpByVat ( const CInvoice & inv1, const CInvoice & inv2 )
{
    double vat1 = inv1 . getVat ( );
    double vat2 = inv2 . getVat ( );
    if ( vat1 < vat2 )
        return - 1;
    if ( vat1 > vat2 )
        return 1;
    return 0;
}

//---------------------------------------------------------------------------------------------
int CVATRegister :: decideWhichFunctionToCallAndReturnValue ( const CSortOpt & sortBy, size_t index, const CInvoice & inv1,
                                                              const CInvoice & inv2 )
{
    if ( sortBy . mSortingOptions[ index ] . first == 0 )
        return inv1 . getDate ( ) . compare ( inv2 . getDate ( ) );
    if ( sortBy . mSortingOptions[ index ] . first == 1 )
        return cmpByBuyer ( inv1, inv2 );
    if ( sortBy . mSortingOptions[ index ] . first == 2 )
        return cmpBySeller ( inv1, inv2 );
    if ( sortBy . mSortingOptions[ index ] . first == 3 )
        return cmpByAmount ( inv1, inv2 );
    return cmpByVat ( inv1, inv2 );
}

//---------------------------------------------------------------------------------------------
list < CInvoice > CVATRegister :: unmatched ( const string & company, const CSortOpt & sortBy )
{
    list < CInvoice > listOfUnmatched;
    unordered_set < CInvoice, CInvoiceHash > allUnmatchedInvoices;

    auto cmpFunction = [ sortBy ] ( const CInvoice & inv1, const CInvoice & inv2 )
    {
        int value = 0;
        size_t i;
        for ( i = 0 ; i < sortBy . mSortingOptions . size ( ) ; i ++ )
        {
            value = decideWhichFunctionToCallAndReturnValue ( sortBy, i, inv1, inv2 );
            if ( value == 0 )
                continue;
            if ( sortBy . mSortingOptions[ i ] . second )
                return value < 0;
            return value > 0;
        }
        return inv1 . getOrder ( ) < inv2 . getOrder ( );
    };

    string name;
    replaceOrAdd ( company, name );
    string seller;
    string buyer;
    copy ( mSavedUnmatched[ name ] . begin ( ), mSavedUnmatched[ name ] . end ( ),
           back_inserter ( listOfUnmatched ) );
    listOfUnmatched . sort ( cmpFunction );
    return listOfUnmatched;
}
