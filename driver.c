// #include"lexer.h"
// #include "lookupTable.h"

/* 
Tushar Garg - 2019A7PS0104P
Vikas Balani - 2019A7PS0054P
Ruchir Jain - 2019A7PS0067P
Usneek Singh - 2019A7PS0127P
Abhijith S Raj - 2019A7PS0055P 
*/

#include "ast.h"
#include <stdio.h>
#include <time.h>

extern NODE lookUpTable;

char *terminal__map[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RUID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "DOLLAR",
    "ERROR",
    "eps"}; // DOLLAR corresponds to end of input and not the dollar sign

int main(int argc, char **argv)
{
    if(argc != 3) {
        printf("Too many/few arguments provided.");
        return -1;
    }
    printf("\n\033[0;35m****IMPLEMENTATION DETAILS****\n1. FIRST and FOLLOW set automated\n2. Both lexical and syntax analysis modules implemented\n3. In-order traversal of parse tree can be printed in the provided file.\n4. The option 2 cannot be run more than once per execution of the driver.\n5. For error recovery, we are checking the lookahead in the first sets of the non-terminal on the stack and the next nonterminal on the stack and if it doesn't match with either of them then we skip the lookahead and keep searching until we find a match.\nIn case the lookahead is in the first set of the next non-terminal on the stack after the top we simply pop the top element of the stack and continue parsing\033[0m");
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();

    FILE *fp;
    fp = fopen("./grammar.txt", "r");

    grammar *gr = load_grammar(fp);
    assign_rule_no(gr);
    FnF *fnf = ComputeFirstAndFollowSets(gr);

    parseTable *pt = parseTableCreate(gr, fnf);

    // the lexer is initialized inside parseInputSourceCode in parser.c
    Tree *tr = parseInputSourceCode(argv[1], pt);

    end_time = clock();
    total_CPU_time = (double)(end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

    // printf("total_CPU_time: %f" , total_CPU_time);
    // printf("\ntotal_CPU_time_in_seconds: %f" , total_CPU_time_in_seconds);

    FILE *fp1 = fopen(argv[1], "r");
    FILE *fpt = fopen(argv[2], "w");
    // removeComments(argv[1]);
    char menu[] = "\n\033[0;32mOptions Available\n0. Exit\n1. Remove Comments\n2. Print token list generated by lexer\n3. Printing the parse tree to specified file\n4. Print time taken by stage 1\n5. Print grammar\n6. Print first and follow sets\n7. Print Postorder traver\033[0m";
    printf("%s\n", menu);
    
    while (1)
    {
        int option;
        printf("\nEnter option: ");
        scanf("%d", &option);
        if (option == 0)
        {   
            printf("\n*END OF PROGRAM*");
            // printf("\n OR SO I THINK");
            break;
        }
        switch (option)
        {
        case 1:
            removeComments(argv[1]);
            break;
        case 2:
            fp1 = initialize_lexer(argv[1]);
            printf("\n%4s | %15s | %22s", "lNum", "tokenType", "lexeme");
            while (1)
            {
                Token tk = getNextToken(&(fp1));
                if (tk == NULL){
                    break;
                }
                    
                printf("\n%4d | %15s | %22s", tk->lineNo, terminal__map[tk->type], tk->lexeme);
            }
            break;
        case 3:
            fprintf(fpt, "\n %20s %25s %7s %20s %25s %10s %10s\n", "lexeme", "CurrentNodeSymbol", "lineNum", "value (0 if not num)", "parentNodeSymbol", "isLeafNode", "Rule No");
            fflush(fpt);
            printParseTree(tr->root, argv[2]);
            printf("\nSuccessfully printed parse tree to %s" , argv[2]);
            break;
        case 4:
            printf("total_CPU_time: %f", total_CPU_time);
            printf("\ntotal_CPU_time_in_seconds: %f", total_CPU_time_in_seconds);
            break;
        case 5:
            print_grammar(gr);
        break;
        case 6:
            printf("\n******************FIRST SETS******************\n");
            printFirstOrFollowSets(fnf->firstSet);
            printf("\n\n\n******************FOLLOW SETS******************\n");
            printFirstOrFollowSets(fnf->followSet);
        break;
        case 7:
            ast_create(tr->root);
        break;
        default:
            printf("\nIncorrect option.");
            break;
        }
    }
}