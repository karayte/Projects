#include <cassert>

#include "CVATRegister.h"
#include "CInvoice.h"

/* The challenge is to implement the classes that implement the VAT management.
 * We assume that companies transfer invoices to each other. The company that
 * issues the invoice must deduct VAT from the invoiced getAmount to the state.
 * Similarly, a company that receives (and pays) an invoice can request a VAT
 * refund from the state (under certain circumstances). VAT refunds are a weak
 * spot that can be used for tax reduction and tax fraud. The challenge is
 * therefore to implement a core system that will make such tax cuts more difficult. */


bool equalLists ( const list < CInvoice > & a, const list < CInvoice > & b )
{
    if ( a . size ( ) != b . size ( ) )
        return false;

    return equal ( a . begin ( ), a . end ( ), b . begin ( ) );
}

int main ( )
{
    CVATRegister r;
    assert ( r . registerCompany ( "first Company" ) );
    assert ( r . registerCompany ( "Second     Company" ) );
    assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
    assert ( r . registerCompany ( "Third Company, Ltd." ) );
    assert ( ! r . registerCompany ( "Third Company, Ltd." ) );
    assert ( ! r . registerCompany ( " Third  Company,  Ltd.  " ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( r . addIssued (
            CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
    assert ( r . addIssued (
            CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
    assert ( ! r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( ! r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
    assert ( ! r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company",
                                          CSortOpt ( ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey (
                                                  CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt ( ) . addKey ( CSortOpt::BY_DATE, true ) . addKey (
                                  CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200,
                                                     30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt ( ) . addKey ( CSortOpt::BY_VAT, true ) . addKey (
                                  CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER,
                                                                                                              true ) . addKey (
                                  CSortOpt::BY_BUYER, true ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt ( ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "second company", CSortOpt ( ) . addKey ( CSortOpt::BY_DATE, false ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company",
                                                     400, 34.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "last company", CSortOpt ( ) . addKey ( CSortOpt::BY_VAT, true ) ),
                          list < CInvoice >
                                  {
                                  } ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addAccepted (
            CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company",
                                          CSortOpt ( ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey (
                                                  CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     32.000000 )
                                  } ) );
    assert ( ! r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( ! r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
    assert ( ! r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
    assert ( ! r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
    assert ( ! r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company",
                                          CSortOpt ( ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey (
                                                  CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     32.000000 )
                                  } ) );
    assert ( r . delAccepted (
            CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company",
                                          CSortOpt ( ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey (
                                                  CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     32.000000 )
                                  } ) );
    assert ( r . delIssued (
            CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company",
                                          CSortOpt ( ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey (
                                                  CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list < CInvoice >
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300,
                                                     32.000000 )
                                  } ) );
    return 0;
}
