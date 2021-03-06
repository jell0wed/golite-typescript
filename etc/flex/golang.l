%{
#include <iostream>
#include <stdbool.h>
#include <golite/bison.h>
#include <golite/utils.h>
#include <bison/golang_bison.h>
#include <golite/literal.h>
#include <sstream>

#define YY_DECL extern "C" int yylex()

// Function prototype
int fToken(int token);
std::stringstream ss;
%}

%option yylineno

%x C_COMMENT
%x C_STRING
%x C_STRING_ESC

%%
chan            { golite::Utils::log_token("tCHAN"); return fToken(tCHAN); }
const           { golite::Utils::log_token("tCONST"); return fToken(tCONST); }
defer           { golite::Utils::log_token("tDEFER"); return fToken(tDEFER); }
fallthrough     { golite::Utils::log_token("tFALLTHROUGH"); return fToken(tFALLTHROUGH); }
interface       { golite::Utils::log_token("tINTERFACE"); return fToken(tINTERFACE); }
map             { golite::Utils::log_token("tMAP"); return fToken(tMAP); }
range           { golite::Utils::log_token("tRANGE"); return fToken(tRANGE); }
select          { golite::Utils::log_token("tSELECT"); return fToken(tSELECT); }
"<-"            { golite::Utils::log_token("tARROW"); return fToken(tARROW); }
"..."           { golite::Utils::log_token("tELLIPSIS"); return fToken(tELLIPSIS); }
go              { golite::Utils::log_token("tGO"); return fToken(tGO); }
goto            { golite::Utils::log_token("tGOTO"); return fToken(tGOTO); }

case            { golite::Utils::log_token("tCASE"); return fToken(tCASE); }
default         { golite::Utils::log_token("tDEFAULT"); return fToken(tDEFAULT); }
else            { golite::Utils::log_token("tELSE"); return fToken(tELSE); }
for             { golite::Utils::log_token("tFOR"); yylval.g_line.line = yylineno; return fToken(tFOR); }
func            { golite::Utils::log_token("tFUNC"); return fToken(tFUNC); }
if              { golite::Utils::log_token("tIF"); yylval.g_line.line = yylineno; return fToken(tIF); }
import          { golite::Utils::log_token("tIMPORT"); return fToken(tIMPORT); }
package         { golite::Utils::log_token("tPACKAGE"); return fToken(tPACKAGE); }
return          { golite::Utils::log_token("tRETURN"); yylval.g_line.line = yylineno; return fToken(tRETURN); }
struct          { golite::Utils::log_token("tSTRUCT"); return fToken(tSTRUCT); }
switch          { golite::Utils::log_token("tSWITCH"); yylval.g_line.line = yylineno; return fToken(tSWITCH); }
type            { golite::Utils::log_token("tTYPE"); return fToken(tTYPE); }
var             { golite::Utils::log_token("tVAR"); return fToken(tVAR); }
print           { golite::Utils::log_token("tPRINT"); return fToken(tPRINT); }
println         { golite::Utils::log_token("tPRINTLN"); return fToken(tPRINTLN); }
append          { golite::Utils::log_token("tAPPEND"); return fToken(tAPPEND); }
break           { golite::Utils::log_token("tBREAK"); yylval.g_line.line = yylineno; return fToken(tBREAK); }
continue        { golite::Utils::log_token("tCONTINUE"); yylval.g_line.line = yylineno; return fToken(tCONTINUE); }

"+"             { golite::Utils::log_token("tPLUS"); return fToken(tPLUS); }
"-"             { golite::Utils::log_token("tMINUS"); return fToken(tMINUS); }
"*"             { golite::Utils::log_token("tMULTIPLY"); return fToken(tMULTIPLY); }
"/"             { golite::Utils::log_token("tDIVIDE"); return fToken(tDIVIDE); }
"%"             { golite::Utils::log_token("tMODULO"); return fToken(tMODULO); }
"&"             { golite::Utils::log_token("tBIT_AND"); return fToken(tBIT_AND); }
"|"             { golite::Utils::log_token("tBIT_OR"); return fToken(tBIT_OR); }
"^"             { golite::Utils::log_token("tBIT_XOR"); return fToken(tBIT_XOR); }
"<<"            { golite::Utils::log_token("tLEFT_SHIFT"); return fToken(tLEFT_SHIFT); }
">>"            { golite::Utils::log_token("tRIGHT_SHIFT"); return fToken(tRIGHT_SHIFT); }
"&^"            { golite::Utils::log_token("tBIT_CLEAR"); return fToken(tBIT_CLEAR); }
"+="            { golite::Utils::log_token("tPLUS_EQUAL"); return fToken(tPLUS_EQUAL); }
"-="            { golite::Utils::log_token("tMINUS_EQUAL"); return fToken(tMINUS_EQUAL); }
"*="            { golite::Utils::log_token("tMULTIPLY_EQUAL"); return fToken(tMULTIPLY_EQUAL); }
"/="            { golite::Utils::log_token("tDIVIDE_EQUAL"); return fToken(tDIVIDE_EQUAL); }
"%="            { golite::Utils::log_token("tMODULO_EQUAL"); return fToken(tMODULO_EQUAL); }
"&="            { golite::Utils::log_token("tBIT_AND_EQUAL"); return fToken(tBIT_AND_EQUAL); }
"|="            { golite::Utils::log_token("tBIT_OR_EQUAL"); return fToken(tBIT_OR_EQUAL); }
"^="            { golite::Utils::log_token("tBIT_XOR_EQUAL"); return fToken(tBIT_XOR_EQUAL); }
"<<="           { golite::Utils::log_token("tLEFT_SHIFT_EQUAL"); return fToken(tLEFT_SHIFT_EQUAL); }
">>="           { golite::Utils::log_token("tRIGHT_SHIFT_EQUAL"); return fToken(tRIGHT_SHIFT_EQUAL); }
"&^="           { golite::Utils::log_token("tBIT_CLEAR_EQUAL"); return fToken(tBIT_CLEAR_EQUAL); }
"&&"            { golite::Utils::log_token("tAND"); return fToken(tAND); }
"||"            { golite::Utils::log_token("tOR"); return fToken(tOR); }
"++"            { golite::Utils::log_token("tINC"); return fToken(tINC); }
"--"            { golite::Utils::log_token("tDEC"); return fToken(tDEC); }
"=="            { golite::Utils::log_token("tIS_EQUAL"); return fToken(tIS_EQUAL); }
"<"             { golite::Utils::log_token("tLESS_THAN"); return fToken(tLESS_THAN); }
">"             { golite::Utils::log_token("tGREATER_THAN"); return fToken(tGREATER_THAN); }
"="             { golite::Utils::log_token("tEQUAL"); return fToken(tEQUAL); }
"!"             { golite::Utils::log_token("tNOT"); return fToken(tNOT); }
"!="            { golite::Utils::log_token("tIS_NOT_EQUAL"); return fToken(tIS_NOT_EQUAL); }
"<="            { golite::Utils::log_token("tLESS_THAN_EQUAL"); return fToken(tLESS_THAN_EQUAL); }
">="            { golite::Utils::log_token("tGREATER_THAN_EQUAL"); return fToken(tGREATER_THAN_EQUAL); }
":="            { golite::Utils::log_token("tDECLARATION"); return fToken(tDECLARATION); }
"("             { golite::Utils::log_token("tLEFT_PAR"); yylval.g_line.line = yylineno; return fToken(tLEFT_PAR); }
"["             { golite::Utils::log_token("tLEFT_SQUARE"); return fToken(tLEFT_SQUARE); }
"{"             { golite::Utils::log_token("tLEFT_CURL"); yylval.g_line.line = yylineno; return fToken(tLEFT_CURL); }
","             { golite::Utils::log_token("tCOMMA"); return fToken(tCOMMA); }
"."             { golite::Utils::log_token("tDOT"); return fToken(tDOT); }
")"             { golite::Utils::log_token("tRIGHT_PAR"); return fToken(tRIGHT_PAR); }
"]"             { golite::Utils::log_token("tRIGHT_SQUARE"); return fToken(tRIGHT_SQUARE); }
"}"             { golite::Utils::log_token("tRIGHT_CURL"); return fToken(tRIGHT_CURL); }
";"             { golite::Utils::log_token("tSEMICOLON"); return fToken(tSEMICOLON); }
":"             { golite::Utils::log_token("tCOLON"); yylval.g_line.line = yylineno; return fToken(tCOLON); }

[ \t\r] {
    // White-space
}

\n {
    int token = fToken(tNEWLINE);
    if(token == tSEMICOLON) {
        golite::Utils::log_token("tSEMICOLON");
        return token;
    }
}

<<EOF>> {
    // Read EOF without closing comment
    if(YY_START == C_COMMENT) {
        golite::Utils::error_message("Block comment not closed before reaching end of file", yylineno-1);
    } else if(YY_START == C_STRING) {
        golite::Utils::error_message("Missing closing string quote before end of file", yylineno);
    } else {

        // Always add a new line
        static bool add_newline = true;
        if(add_newline) {
            add_newline = false;
            unput('\n');
        } else {
            return 0;
        }
    }
}

"//".* {
    // Inline comment
}

0+|([1-9][0-9]*) {
    // Integer
    golite::Utils::log_token("tINT", yytext);
    try {
        yylval.g_expression = new golite::Literal<int>(atoi(yytext), yylineno);
    } catch(...) {
        yylval.g_expression = new golite::Literal<int>(-1, yylineno);
    }
    return fToken(tINT);
}

0[1-7][0-7]* {
    // Octal
    golite::Utils::log_token("tINT", yytext);
    try {
        yylval.g_expression = new golite::Literal<int>(std::stoi(yytext, 0, 8), yylineno);
    } catch(...) {
        yylval.g_expression = new golite::Literal<int>(-1, yylineno);
    }
    return fToken(tINT);
}

0[xX][0-9a-fA-F]+ {
    // Hex
    golite::Utils::log_token("tINT", yytext);
    try {
        yylval.g_expression = new golite::Literal<int>(std::stoi(yytext, 0, 16), yylineno);
    } catch(...) {
        yylval.g_expression = new golite::Literal<int>(-1, yylineno);
    }
    return fToken(tINT);
}

([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*) {
    // Float
    golite::Utils::log_token("tFLOAT", yytext);
    yylval.g_expression = new golite::Literal<float>(atof(yytext), yylineno);
    return fToken(tFLOAT);
}

'((\\[abfnrtv\\\'])|[^\\'])' {
    // Rune/Character
    golite::Utils::log_token("tRUNE", yytext);
    golite::Literal<char*>* rune = new golite::Literal<char*>(strdup(yytext), yylineno);
    rune->f_rune_ = true;
    yylval.g_expression = rune;
    return fToken(tRUNE);
}

`([^`]|\n)*` {
    // Raw String
    golite::Utils::log_token("tSTRING", yytext);
    golite::Literal<char*>* raw_string = new golite::Literal<char*>(strdup(yytext), yylineno);
    raw_string->f_raw_ = true;
    yylval.g_expression = raw_string;
    return fToken(tSTRING);
}

<INITIAL>\" {
    ss.str("");
    ss.clear();
    ss << "\"";
    BEGIN(C_STRING);
}
<C_STRING>[^\\"\n]* { ss << yytext; }
<C_STRING>\\ { BEGIN(C_STRING_ESC); }
<C_STRING>\" {
    ss << "\"";
    BEGIN(INITIAL);
    golite::Utils::log_token("tSTRING", ss.str().c_str());
    yylval.g_expression = new golite::Literal<char*>(strdup(ss.str().c_str()), yylineno);
    return fToken(tSTRING);
}
<C_STRING>\n {
    golite::Utils::error_message("Missing closing string quote", yylineno-1);
}
<C_STRING_ESC>[abfnrtv"\\] {
    ss << "\\" << yytext;
    BEGIN(C_STRING);
}
<C_STRING_ESC>\n {
    golite::Utils::error_message("Missing escape character", yylineno);
}
<C_STRING_ESC>. {
    std::stringstream msg;
    msg << "Unsupported escape character '\\" << yytext << "'";
    golite::Utils::error_message(msg.str().c_str(), yylineno);
}

[a-zA-Z_][a-zA-Z0-9_]* {
    // Identifier
    golite::Utils::log_token("tIDENTIFIER", yytext);
    yylval.g_identifier = new golite::Identifier(strdup(yytext), yylineno);
    return fToken(tIDENTIFIER);
}

"/*"            { BEGIN(C_COMMENT); /*Go to C_COMMENT state*/ }
<C_COMMENT>.    { /* Comment can contain any char */ }
<C_COMMENT>\n   { /* Comment can contains new lines */ }
<C_COMMENT>"*/" { BEGIN(INITIAL); /* Go back to initial state */ }

. {
    // Everything else
    golite::Utils::support_error(yytext, yylineno);
}
%%

// Auto-inject semicolon tokens
int last_token = 0;
int fToken(int token) {
    if(token == tNEWLINE) {
        switch(last_token) {
            case tIDENTIFIER:
            case tINT:
            case tFLOAT:
            case tRUNE:
            case tSTRING:
            case tBREAK:
            case tCONTINUE:
            case tFALLTHROUGH:
            case tRETURN:
            case tINC:
            case tDEC:
            case tRIGHT_PAR:
            case tRIGHT_SQUARE:
            case tRIGHT_CURL:
                // Put another new line
                unput('\n');

                // Update last token
                last_token = tSEMICOLON;
                return tSEMICOLON;
            default:
                last_token = token;
                return tNEWLINE;
        }
    } else {
        last_token = token;
        return token;
    }
}