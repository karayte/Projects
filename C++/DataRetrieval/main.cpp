#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <filesystem>
#include <httplib.h>
#include <unordered_set>
#include <string>

using namespace std;
namespace fs = filesystem;

class CTerms
{
public:
    void initialization ( );

    double termFrequency ( const string & term, const string & doc );

    double inverseDocFrequency ( const string & term );

    double maxInverseDocFrequency ( const string & term, double inverse_doc_frequency );

    void getWeightForTerms ( );

    static bool inputGrammarTest ( bool & was_term, int & brackets, const string & s, bool & was_not );

    vector < string > ReadInput ( const string & request );

    double processBooleanQuery ( size_t & i, const vector < string > & query, const string & file_name );

    vector < pair < double, string>> requestCalculation ( vector < string > & request );

    map < string, string > doc_description;

private:
    map < string, vector < string>> doc_with_terms;
    map < string, set < string>> term_to_docs;
    set < string > terms;
    map < pair < string, string >, double > term_weight;
    int g_doc_count = 0;
    set < string > documents;
    unordered_set < string > g_stop_words {
            "a", "an", "and", "are", "as", "at", "be", "but", "by", "for", "if", "in",
            "into", "is", "it", "no", "not", "of", "on", "or", "such", "that", "the", "their",
            "then", "there", "these", "they", "this", "to", "was", "will", "with"
    };

};

void normalizeTerm ( string & term )
{
    string copy;
    for ( auto i : term )
    {
        if ( ! isalpha ( i ) && i != '-' )
            continue;
        copy += tolower ( i );
    }
    term = copy;
}

void CTerms::initialization ( )
{
    string directory_path = "documents/";
    for ( const auto & entry : fs::directory_iterator ( directory_path ) )
    {
        string term;
        if ( entry . is_regular_file ( ) )
        {
            string file_path = entry . path ( ) . string ( );
            ifstream file ( file_path );
            vector < string > all_terms_in_file;
            string description;
            size_t num_of_sentences = 0;
            if ( file . is_open ( ) )
            {
                g_doc_count ++;
                while ( file >> term )
                {

                    if ( term == "." || term == "," || term == "-" || term == "\n" )
                        continue;
                    if ( num_of_sentences < 2 )
                        description += term + ' ';
                    if ( term . find ( '.' ) != string::npos )
                        num_of_sentences ++;
                    normalizeTerm ( term );
                    if ( term . size ( ) > 1 && g_stop_words . find ( term ) == g_stop_words . end ( ) )
                    {
                        all_terms_in_file . push_back ( term );
                        terms . insert ( term );
                        term_to_docs[ term ] . insert ( file_path );
                    }
                }
                doc_with_terms[ file_path ] = all_terms_in_file;
                doc_description[ file_path ] = description;
                description . clear ( );
                all_terms_in_file . clear ( );
                sort ( doc_with_terms[ file_path ] . begin ( ), doc_with_terms[ file_path ] . end ( ) );
                documents . insert ( file_path );
                file . close ( );
            }
            else
                throw logic_error("File is not opened");
        }
    }
}

double CTerms::termFrequency ( const string & term, const string & doc )
{
    double count_of_terms = ( double ) count ( doc_with_terms[ doc ] . begin ( ), doc_with_terms[ doc ] . end ( ),
                                               term );
    double retval = ( double ) ( count_of_terms / ( double ) doc_with_terms[ doc ] . size ( ) );
    return retval;
}

double CTerms::inverseDocFrequency ( const string & term )
{
    double size = ( double ) term_to_docs[ term ] . size ( );
    double retval = log2 ( ( ( double ) g_doc_count ) / size );
    return retval;
}

double CTerms::maxInverseDocFrequency ( const string & term, double inverse_doc_frequency )
{
    double max_idf = inverse_doc_frequency;
    for ( const string & term1 : terms )
    {
        double x = inverseDocFrequency ( term1 );
        if ( max_idf < x ) max_idf = x;
    }
    return max_idf;
}

void CTerms::getWeightForTerms ( )
{
    for ( const auto & term : terms )
    {
        for ( const auto & doc : documents )
        {
            double term_frequency = termFrequency ( term, doc );
            if ( term_frequency == 0 )
            {
                term_weight[ make_pair ( term, doc ) ] = 0;
                continue;
            }
            double inverse_doc_frequency = inverseDocFrequency ( term );
            double max_inverse_doc_frequency = maxInverseDocFrequency ( term, inverse_doc_frequency );
            term_weight[ make_pair ( term, doc ) ] = term_frequency * inverse_doc_frequency / max_inverse_doc_frequency;
        }
    }
}

bool CTerms::inputGrammarTest ( bool & was_term, int & brackets, const string & s, bool & was_not )
{
    if ( s == "(" )
    {
        if ( was_term ) return false;
        ++ brackets;
        was_not = false;
    }
    else if ( s == ")" )
    {
        if ( brackets - 1 < 0 || ! ( was_term ) ) return false;
        -- brackets;
        was_not = false;
    }
    else if ( s == "AND" || s == "OR" )
    {
        if ( ! ( was_term ) || was_not ) return false;
        was_term = false;
        was_not = false;
    }
    else if ( s == "NOT" )
    {
        if ( was_term || was_not )
            return false;
        was_not = true;
    }
    else
    {
        if ( was_term ) return false;
        was_term = true;
        was_not = false;
    }
    return true;
}

vector < string > CTerms::ReadInput ( const string & request )
{
    vector < string > arr;
    bool was_term = false;
    bool was_not = false;
    int brackets = 0;
    istringstream request_by_tokens ( request );
    string token;
    while ( request_by_tokens >> token )
    {
        if ( ! inputGrammarTest ( was_term, brackets, token, was_not ) )
        {
            throw exception ( );
        }
        if ( token != "AND" && token != "OR" && token != "NOT" && token != "(" && token != ")" )
            normalizeTerm ( token );
        arr . push_back ( token );
    }
    return arr;
}

double relevant_and ( vector < double > & weight )
{
    double sum = 0;
    double count = 0;
    for ( double & w : weight )
    {
        sum += ( 1 - w ) * ( 1 - w );
        count ++;
    }
    double res = sum / count;
    if ( res <= 0.0 )
        return 0;
    sum = 1 - sqrt ( res );
    return sum;
}

double relevant_or ( vector < double > & weight )
{
    double sum = 0;
    double count = 0;
    for ( double & w : weight )
    {
        sum += w * w;
        count ++;
    }
    sum /= count;
    if ( sum > 0.0 )
        return sqrt ( sum );
    return 0;
}

double CTerms::processBooleanQuery ( size_t & i, const vector < string > & query, const string & file_name )
{
    vector < double > vector_and;
    vector < double > vector_or;
    bool prev_and = false;
    bool was_not = false;
    if ( query . size ( ) <= 2 )
    {
        double weight;
        if ( terms . find ( query[ query . size ( ) - 1 ] ) != terms . end ( ) )
            weight = term_weight . find ( make_pair ( query[ query . size ( ) - 1 ], file_name ) ) -> second;
        else
            weight = 0.0;
        if ( query[ 0 ] == "NOT" )
            weight = 1 - weight;
        vector_or . push_back ( weight );
        return relevant_or ( vector_or );
    }
    while ( i < query . size ( ) )
    {
        string token = query[ i ];
        if ( token == "AND" )
        {
            prev_and = true;
            was_not = false;
        }
        else if ( token == "OR" )
        {
            if ( ! vector_and . empty ( ) )
            {
                double result = relevant_and ( vector_and );
                vector_or . push_back ( result );
                vector_and . clear ( );
            }
            prev_and = false;
            was_not = false;
        }
        else if ( token == "(" )
        {
            ++ i;
            double tw = processBooleanQuery ( i, query, file_name );
            if ( was_not )
                tw = 1 - tw;
            if ( i + 1 < query . size ( ) && query[ i + 1 ] == "OR" && ! prev_and ||
                 i == query . size ( ) - 1 && ! prev_and )
                vector_or . push_back ( tw );

            if ( prev_and || ( i + 1 < query . size ( ) && query[ i + 1 ] == "AND" ) )
                vector_and . push_back ( tw );
            was_not = false;
        }
        else if ( token == ")" )
        {
            ++ i;
            break;
        }
        else if ( token == "NOT" )
        {
            was_not = true;
        }
        else
        {
            auto it = term_weight . find ( make_pair ( token, file_name ) );
            double tw;
            if ( it == term_weight . end ( ) )
                tw = 0;
            else
                tw = it -> second;
            if ( was_not )
                tw = 1 - tw;
            if ( i + 1 < query . size ( ) && query[ i + 1 ] == "OR" && ! prev_and ||
                 i == query . size ( ) - 1 && ! prev_and || query[ i + 1 ] == ")" && ! prev_and )
                vector_or . push_back ( tw );

            if ( prev_and || ( i + 1 < query . size ( ) && query[ i + 1 ] == "AND" ) )
                vector_and . push_back ( tw );
            was_not = false;
        }
        i ++;
    }
    if ( ! vector_and . empty ( ) )
    {
        double result = relevant_and ( vector_and );
        vector_or . push_back ( result );
        vector_and . clear ( );
    }
    double relevance = 0.0;
    if ( ! vector_or . empty ( ) )
    {
        relevance = relevant_or ( vector_or );
    }
    return relevance;
}

vector < pair < double, string>> CTerms::requestCalculation ( vector < string > & request )
{
    size_t i = 0;
    vector < pair < double, string>> values;
    for ( auto & doc : documents )
    {
        double value = processBooleanQuery ( i, request, doc );
        i = 0;
        if ( value == 0 ) continue;
        values . emplace_back ( value, doc );
    }
    sort ( values . rbegin ( ), values . rend ( ) );
    return values;
}

int main ( )
{
    CTerms term;
    term . initialization ( );
    term . getWeightForTerms ( );
    httplib::Server server;
    server . Get ( "/", [ ] ( const httplib::Request &, httplib::Response & res )
    {
        std::string html_content = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Boolean Model Search</title>
            <style>
                body {
                    font-family: Arial, sans-serif;
                    background-color: rgb(255,280,165);
                }
                .container {
                    border: 1px solid #ccc;
                    border-radius: 5px;
                    padding: 20px;
                    max-width: 500px;
                    margin: 0 auto;
                    margin-bottom: 15px;
                }
                label {
                    display: block;
                    margin-bottom: 5px;
                }
                #main-heading {
                    font-size: 36px;
                }
            </style>
        </head>
        <body>
            <div class="container">
                <h1 id="main-heading">Welcome to Boolean Model Search</h1>
                <form action="/submit" method="post">
                    <label for="input_text">Enter terms you would like to search for:</label>
                    <input type="text" id="input_text" name="input_text">
                    <input type="submit" value="Search" id="submit-button">
                </form>
            </div>
            <div id="result-1" class="container" style="display: none; border-color: red; background-color: #ffe6e6; height: 150px; overflow-y: scroll;">
                <h2>Text 1</h2>
                <p>Your first text content goes here.</p>
            </div>
            <div id="result-2" class="container" style="display: none; border-color: green; background-color: #e6e6ff; height: 150px; overflow-y: scroll;">
                <h2>Text 2</h2>
                <p>Your second text content goes here.</p>
            </div>
            <div id="result-3" class="container" style="display: none; border-color: blue; background-color: #e6ffe6; height: 150px; overflow-y: scroll;">
                <h2>Text 3</h2>
                <p>Your third text content goes here.</p>
            </div>
        </body>
        </html>
    )";

        res . set_content ( html_content, "text/html" );
    } );
    server . Post ( "/submit", [ &term ] ( const httplib::Request & req, httplib::Response & res )
    {
        string input_text = req . get_param_value ( "input_text" );
        vector < pair < double, string>> result;
        try
        {
            vector < string > request = term . ReadInput ( input_text );
            result = term . requestCalculation ( request );
        }
        catch ( ... )
        {
        }

        string output = R"(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Results</title>
            <style>
                .container {
                    display: inline-block;
                    border: 1px solid black;
                    padding: 10px;
                    margin: 10px;
                    width: 600px;
                    height: 200px; /* Уменьшаем высоту рамок в 2 раза */
                    overflow: auto;
                }

                .container:nth-child(odd) {
                    background-color: lightblue;
                }

                .container:nth-child(even) {
                    background-color: lightgreen;
                }

                .title {
                    font-weight: bold;
                    font-size: 1.5em;
                }

                h1 {
                    text-align: center;
                }

                .content {
                    font-size: 1.2em;
                }

                /* Выравнивание рамок по центру */
                #content {
                    display: flex;
                    flex-wrap: wrap;
                    justify-content: center;
                }
            </style>

        </head>
        <body>
            <h1>Top of relevant documents</h1>
            <div id="content">
    )";
        if ( result . empty ( ) )
            output = "Wrong input syntax. Please, write another one.\n";
        else
        {
            for ( size_t i = 0 ; i < min ( result . size ( ), size_t ( 10 ) ) ; ++ i )
            {
                output += R"(<div class="container">)";
                output += "<span class='title'>Document " + result[ i ] . second + " with relevancy ";
                output += to_string ( result[ i ] . first * 100 ) + "%" + "</span><br>";
                output += "<span class='title'>A few words about text you can see below:</span> <br>";
                output += "<br>";
                output += "<span class='content'>" + term . doc_description[ result[ i ] . second ] + "</span>";
                output += "</div>";
            }
        }
        output += R"(
            </div>
        </body>
        </html>
    )";

        res . set_content ( output, "text/html" );
    } );

    std::cout << "Starting server at http://localhost:8080" << std::endl;
    server . listen ( "localhost", 8080 );

}
