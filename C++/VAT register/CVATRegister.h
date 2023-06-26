#ifndef UL5_CVATREGISTER_H
#define UL5_CVATREGISTER_H

#include "CInvoice.h"
#include "CSortOpt.h"

#include <unordered_set>
#include <list>

/* Issued and received invoices are recorded by the CVATRegister class. Companies must register
 * issued and received invoices. The register matches pairs of corresponding invoices and
 * is able to find unpaired invoices. The invoice can be added to the register as well as
 * canceled/deleted (e.g. in case of incorrectly entered data) */
class CVATRegister
{
public:
    CVATRegister ( )
            : mOrderInv ( 0 )
    {
    }

    /* the method will add the given company to the register. The given name is the official name
     * of the company, this name will be used in exports from the register. The return value is a
     * success (true)/failure (false) indicator. Consider it a failure if a company with the same
     * name already exists in the register. Names are the same, if their lower case versions with
     * reduced spaces are the same. */
    bool registerCompany ( const string & name );

    /* This method checks if the Invoice can be added/registered. It cannot be added in case:
     * 1. The seller or the buyer are not registered or is the same company
     * 2. The same invoice has been already issued */
    bool canAddInvoice ( CInvoice & inv, bool issued );

    /* This method adds the invoice to the register. This method is called by the company
     * that issued the invoice. The return value is a success (true)/failure (false) flag. */
    bool addIssued ( const CInvoice & x );

    bool addIssued ( CInvoice && x ) noexcept;

    /* This method adds the invoice to the register, this method is called by the company
     * that accepted the invoice (buyer). The method have the same restrictions as addIssued. */
    bool addAccepted ( const CInvoice & x );

    //---------------------------------------------------------------------------------------------
    bool addAccepted ( CInvoice && x ) noexcept;

    /* This method is responsible for "cleaning" after the invoice has been deleted. So, if
     * 1. The invoice had a pair, it will be moved to unpaired invoices
     * 2. The invoice did not have a pair, it will be removed */
    void cleanAfterDelete ( CInvoice & inv, const string & seller, const string & buyer, bool unmatched );

    /* This method is responsible for erasing accepted or issued invoice.
     * If the invoice exists, it will be deleted and the returning value is True,
     * otherwise it is False */
    bool deleteInvoice ( CInvoice & inv, bool issued ) noexcept;

    /* This method removes the invoice from the register. This method is called by the company
     * that issued the invoice and previously registered it. The return value is a success
     * (true)/failure (false) flag. It is considered an error if the identical invoice was not
     * previously registered using the addIssued method */
    bool delIssued ( const CInvoice & x );

    //---------------------------------------------------------------------------------------------
    bool delIssued ( CInvoice && x ) noexcept;

    /* This method removes the invoice from the register. This method is called by the company
     * that accepted the invoice and previously registered it. The return value is a success
     * (true)/failure (false) flag. It is considered an error if the identical invoice was not
     * previously registered using the addAccepted method. */
    bool delAccepted ( const CInvoice & x );

    //---------------------------------------------------------------------------------------------
    bool delAccepted ( CInvoice && x ) noexcept;

    /* Here go methods for invoice comparison for sorting purposes */
    static int cmpBySeller ( const CInvoice & inv1, const CInvoice & inv2 );

    //---------------------------------------------------------------------------------------------
    static int cmpByBuyer ( const CInvoice & inv1, const CInvoice & inv2 );

    //---------------------------------------------------------------------------------------------
    static int cmpByAmount ( const CInvoice & inv1, const CInvoice & inv2 );

    //---------------------------------------------------------------------------------------------
    static int cmpByVat ( const CInvoice & inv1, const CInvoice & inv2 );

    /* This function is responsible for making the decision whether one invoice must go
     * before or after the second one during sorting process */
    static int decideWhichFunctionToCallAndReturnValue ( const CSortOpt & sortBy, size_t index, const CInvoice & inv1,
                                                         const CInvoice & inv2 );

    /* This method finds all invoices that relate to the specified company and were not matched
     * ( they were registered only with addIssued or only with addAccepted ).
     * The method returns a list of these invoices, the invoices will be sorted according to the
     * criteria given by sortOpt. Invoices returned by this method will have the "official" name
     * instead of the company name, i.e. the name that was entered when the company was registered
     * using the registerCompany method. This official name will also be used for sorting. */
    list < CInvoice > unmatched ( const string & company, const CSortOpt & sortBy );

private:
    struct CInvoiceHash
    {
        size_t operator () ( const CInvoice & inv ) const;
    };
    unordered_map < string, unordered_set < CInvoice, CInvoiceHash > > mCompaniesInvoicesIssued;
    unordered_map < string, unordered_set < CInvoice, CInvoiceHash > > mCompaniesInvoicesAccepted;
    unordered_map < string, unordered_set < CInvoice, CInvoiceHash > > mSavedUnmatched;
    unordered_map < string, string > mCompaniesByNames;
    unsigned long long mOrderInv;
};


#endif //UL5_CVATREGISTER_H
