#include "astDef.h"

void ast_r1(tNode* prog);
void ast_r2(tNode* othFunc);
void ast_r3(tNode* othFunc);
void ast_r4(tNode* mainF);
void ast_r5(tNode* function_tnode);
void ast_r6(tNode* input_par_tnode);
void ast_r7(tNode* output_par_tnode);
void ast_r8(tNode* output_par_tnode);
void ast_r9(tNode* parameter_list_tnode);
void ast_r10(tNode* dataType_tnode);
void ast_r11(tNode* dataType_tnode);
void ast_r12(tNode* primitiveDatatype_tnode);
void ast_r13(tNode* primitiveDatatype_tnode);
void ast_r14(tNode* constructedDatatype_tnode);
void ast_r15(tNode* constructedDatatype_tnode);
void ast_r16(tNode* constructedDatatype_tnode);
void ast_r17(tNode* remaining_list_tnode);
void ast_r18(tNode* remaining_list_tnode);
void ast_r19(tNode* stmts_tnode);
void ast_r20(tNode* typeDefinitions_node);
void ast_r21(tNode* typedefn);
void ast_r22(tNode* typeDefinition_node);
void ast_r23(tNode* typeDefinition_node);
void ast_r24(tNode* actualOrRedefined_node);
void ast_r25(tNode* actualOrRedefined_node);
void ast_r26(tNode* fieldDefinitions_node);
void ast_r27(tNode* fieldDefinition_node);
void ast_r28(tNode* moreFields_node);
void ast_r29(tNode* moreFields_node);
void ast_r30(tNode* fieldType_node);
void ast_r31(tNode* fieldType_node);
void ast_r32(tNode* declarations_node);
void ast_r33(tNode* declarations_node);
void ast_r34(tNode* declaration_node);
void ast_r35(tNode* global_or_not_node);
void ast_r36(tNode* global_or_not_node);
void ast_r37(tNode* otherStmts_node);
void ast_r38(tNode* otherStmts_node);
void ast_r39(tNode* stmt_tNode);
void ast_r40(tNode* stmt_tNode);
void ast_r41(tNode* stmt_tNode);
void ast_r42(tNode* stmt_tNode);
void ast_r43(tNode* stmt_tNode);
void ast_r44(tNode* assignmentStmt_tNode);
void ast_r45(tNode* oneExpansion_tNode);
void ast_r46(tNode* moreExpansions_tNode);
void ast_r47(tNode* moreExpansions_tNode);
void ast_r48(tNode* option_single_constructed_tNode);
void ast_r49(tNode* option_single_constructed_tNode);
void ast_r50(tNode* singleOrRecId_tNode);
void ast_r51(tNode* funCallStmt_tNode);
void ast_r52(tNode* outputParameters_tNode);
void ast_r53(tNode* outputParameters_tNode);
void ast_r54(tNode* inputParameters_tNode);
void ast_r55(tNode* iterativeStmt_tNode);
void ast_r56(tNode* conditionalStmt_tNode);
void ast_r57(tNode* elsePart_tNode);
void ast_r58(tNode* elsePart_tNode);
void ast_r59(tNode* expPrime_tNode);
void ast_r60(tNode* expPrime_tnode) ;
void ast_r61(tNode* term_tnode) ;
void ast_r62(tNode* termPrime_tnode) ;
void ast_r63(tNode* termPrime_tnode) ;
void ast_r64(tNode* factor_tnode) ;
void ast_r65(tNode* factor_tnode) ;
void ast_r66(tNode* highPrecedenceOperators_tnode) ;
void ast_r67(tNode* highPrecedenceOperators_tnode) ;
void ast_r68(tNode* lowPrecedenceOperators_tnode) ;
void ast_r69(tNode* lowPrecedenceOperators_tnode) ;
void ast_r70(tNode* ioStmt_tnode) ;
void ast_r71(tNode* ioStmt_tnode) ;
void ast_r72(tNode* arithmeticExpression_tnode) ;
void ast_r73(tNode* booleanExpression_tnode) ;
void ast_r74(tNode* booleanExpression_tnode) ;
void ast_r75(tNode* booleanExpression_tnode) ;
void ast_r76(tNode* var_tnode) ;
void ast_r77(tNode* var_node);
void ast_r78(tNode* var_node);
void ast_r79(tNode* logicalOp_node);
void ast_r80(tNode* logicalOp_node);
void ast_r81(tNode* relationalOp_node);
void ast_r82(tNode* relationalOp_node);
void ast_r83(tNode* relationalOp_node);
void ast_r84(tNode* relationalOp_node);
void ast_r85(tNode* relationalOp_node);
void ast_r86(tNode* relationalOp_node);
void ast_r87(tNode* returnStmt_node);
void ast_r88(tNode* optionalReturn_node);
void ast_r89(tNode* optionalReturn_node);
void ast_r90(tNode* idList_node);
void ast_r91(tNode* more_ids_node);
void ast_r92(tNode* more_ids_node);
void ast_r93(tNode* definetypestmt_node);
void ast_r94(tNode* A_node);
void ast_r95(tNode* A_node);
void ast_create(tNode* root);
ast_node* makeNode(CONSTRUCT c, ast_node** a, int no_children, struct node_info* n_info);
struct ast_node *return_child(tNode *node, int n);