%{
#include "ast.h"
#include "parser.tab.h"
#include "visitor.h"
#define YY_DECL extern "C" int yylex()

extern union node yylval;
int lineno = 0;

char * translateString(char *, int);
%}

%option noyywrap


CHAR (\.|[^\"])


%%

\+                              { /*cout<<"plus"<<endl;*/ return (PLUS); }
\-                              { /*cout<<"minus"<<endl;*/ return (MINUS); }
\*                              { /*cout<<"mult"<<endl;*/ return (MULT); }
\/                              { /*cout<<"div"<<endl;*/ return (DIV); }
\%                              { /*cout<<"div"<<endl;*/ return (MOD); }

\=                              { /*cout<<"eql"<<endl;*/ return (EQL); }
\-\=                            { /*cout<<"meql"<<endl;*/ return (MEQL); }
\+\=                            { /*cout<<"peql"<<endl;*/ return (PEQL); }

\&\&                            { /*cout<<"and"<<endl;*/ return (AND); }
\|\|                            { /*cout<<"or"<<endl;*/ return (OR);}

\=\=                            { /*cout<<"eqleql"<<endl;*/ return (EQLEQL);}
\!\=                            { /*cout<<"neql"<<endl;*/ return (NEQL);}
\<                              { /*cout<<"lt"<<endl;*/ return (LT); }
\>                              { /*cout<<"gt"<<endl;*/ return (GT); }
\<\=                            { /*cout<<"leq"<<endl;*/ return (LEQ); }
\>\=                            { /*cout<<"geq"<<endl;*/ return (GEQ);}
\{                              { /*cout<<"ocur"<<endl;*/ return (OCUR); }
\}                              { /*cout<<"ccur"<<endl;*/ return (CCUR); }
\(                              { /*cout<<"opar"<<endl;*/ return (OPAR); }
\)                              { /*cout<<"cpar"<<endl;*/ return (CPAR); }
"["                             { /*cout<<"osqu"<<endl;*/ return (OSQU); }
\]                              { /*cout<<"csqu"<<endl;*/ return (CSQU); }

int                             { /*cout<<"int"<<endl;*/ return (INT); }
boolean                         { /*cout<<"boolean"<<endl;*/ return (BOOLEAN); }
void                            { /*cout<<"void"<<endl;*/ return (VOID); }

class                           { /*cout<<"class"<<endl;*/ return (CLASS); }
if                              { /*cout<<"if\n"<<endl;*/ return (IF); }
else                            { /*cout<<"else\n"<<endl;*/ return (ELSE); }
for                             { /*cout<<"for\n"<<endl;*/ return (FOR); }
break                           { /*cout<<"break\n"<<endl;*/ return (BREAK); }
continue                        { /*cout<<"continue\n"<<endl;*/ return (CONTINUE); }
return                          { /*cout<<"return\n"<<endl;*/ return (RETURN); }
printf                         { /*cout<<"printf"<<endl;*/ return (PRNT); }
true                            { /*cout<<"true\n"<<endl;*/ return (TRUE); }
false                           { /*cout<<"false\n"<<endl;*/ return (FALSE); }

0x[0-9a-fA-F][0-9a-fA-F]*       { /*cout<<"hex"<<endl;*/ return (INT_VAL); }
[0-9]+                          { /*cout<<"decimal\n"<<endl;*/ yylval.ival = atoi(yytext); return (INT_VAL); }
\"{CHAR}*\"                     { /*cout<<"string \n"<<endl;*/ yylval.sval = translateString(yytext + 1, strlen(yytext) - 2); return (STR); }
\'{CHAR}?\'                     { /*cout<<"char\n"<<endl;*/ yylval.cval = yytext[1]; return (CHAR); }
[a-zA-Z_][a-zA-Z0-9_]*          { /*cout<<"id\n"<<endl;*/ yylval.sval = strdup(yytext); return (ID); }

!                               { /*cout<<"excla\n"<<endl;*/ return (NOT); }
;                               { /*cout<<"semicolon\n"<<endl;*/ return (SCOLON); }
,                               { /*cout<<"comma\n"<<endl; */ return (COMMA); }
\t
\n                              { /*cout<<lineno<<endl;*/ lineno++; }
" "
.                       {printf("Error here\n");}     {return (MINUS);}
%%
char* translateString(char * string, int size) {
    char * ch = string;
    char * newString = (char *) malloc(size + 1);
    char * temp = newString;
    int i=0;
    while (i < size) {
          if(*ch == '\\'){
            i+=1;
            ch+=1;
            if(*ch=='a')
              *temp = '\a';
            else if(*ch=='\b')
              *temp = 'b';
            else if(*ch=='\f')
              *temp = 'f';
            else if(*ch=='n')
              *temp = '\n';
            else if(*ch=='r')
              *temp = '\r';
            else if(*ch=='t')
              *temp = '\t';
            else if(*ch=='v')
              *temp = '\v';
            else if(*ch=='\\')
              *temp = '\\';
            else if(*ch=='\'')
              *temp = '\'';
            else if(*ch=='\"')
              *temp = '\"';
            else if(*ch=='?')
              *temp = '\?';
          }
          else{
            *temp = *ch;
          }
          ch++;
          temp++;
          i++;
        
  }
  *temp = '\0';
  return newString;
}
