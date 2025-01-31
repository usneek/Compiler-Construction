

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "symbolTable.h"
extern char* LabelMap[];
extern char* TerminalMap[];
extern char* NonTerminalMap[];

#define SLOTS 8


symbolTable* newSymbolTable(int no_slots){
	
	symbolTable* table =  (symbolTable *)malloc(sizeof(symbolTable));
    
	table->no_slots = no_slots;
	table->currentOffset =0;
	table->list = (symbolTableSlot **)malloc(no_slots*sizeof(symbolTableSlot*));
	
	for(int i=0;i<no_slots;i++){
		table->list[i] = (symbolTableSlot*)malloc(sizeof(symbolTableSlot));
		table->list[i]->head = NULL;
	}

	return table;
}

int hashFunction(char* key, int no_of_slots){
	int sum = 0;
    int n=strlen(key);
	for(int i=0;i<n;i++){
        int temp=key[i]-'0';
		sum = sum *10 +temp;
		sum = sum%no_of_slots;
	}

	int hash = sum%no_of_slots;
	return hash;
}

void addSymbol(symbolTable* symbolTable, char* key, SymbolTableRecord* symbol){
    //Keep the current offset stored in symbol table, assign that offset to any symbol added and update the offset
    int offset = symbolTable->currentOffset;
    symbol->offset = offset;
    symbolTable->currentOffset = symbolTable->currentOffset + symbol->width;

	int hash =  hashFunction(key,symbolTable->no_slots);
	symbol->next = symbolTable->list[hash]->head;
	symbolTable->list[hash]->head = symbol;

	symbolTable->list[hash]->noOfRecords++;
}

SymbolTableRecord* getSymbolInfo(char* key, symbolTable* symbolTable){
	int hash = hashFunction(key, symbolTable->no_slots);

	SymbolTableRecord* temp = symbolTable->list[hash]->head;
	while(temp!=NULL){
		if(strcmp(temp->lexeme,key)==0)
			return temp;
		temp = temp->next;
		
	}
	return NULL;
}

typeInfo* add_function_par(typeInfo * typeInfoList, Type type, char *ruid){
    typeInfo * typeInfoNode = (typeInfo *) malloc(sizeof(typeInfo));
    typeInfoNode->type = type;
    typeInfoNode->type_ruid = ruid;
    typeInfoNode->next = NULL;
    typeInfo * iterator = typeInfoList;
    if(iterator==NULL) {
        return typeInfoNode;
    } 
    while(iterator->next!=NULL){
        iterator=iterator->next;
    }
    iterator->next = typeInfoNode;
    return typeInfoList;
}

void add_record_field (SymbolTableRecord * record, struct record_field * field){
    struct record_field * temp = record->recordFields;
    if(temp == NULL){
        record->recordFields = field;
    }
    else{
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = field;
    }
}
int traverseNode(ast_node * current, symbolTable* global){
    if(current->construct== typeDefinition_){
        struct constructed_type_struct * info = (struct constructed_type_struct *)current->ninf;
        SymbolTableRecord * temp = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        if(info->union_or_record == TK_RECORD){
            temp->type = RECORD;
            temp->lexeme = info->ruid;
            temp->line_no = info->lineNum;
            temp->recordFields = NULL;

            int offset =0; //for calculting the offset of fields relative to the base of the record definition
            //typedefinitions -> fielddefiniitions->fielddefinition
            //child == **fieldDefinition**
            ast_node * child = current->firstChild->firstChild; 
            while(child != NULL){ 
                //for each field definition create a record field * and attach it to linked list
                struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                struct field_id_struct * info2 = (struct field_id_struct *)child->ninf;
                field->lexeme = info2->fieldid;
                field->line_no = info2->lineNum;

                if(child->firstChild->construct == primitiveDatatype_){
                    struct ast_node * primitive = child->firstChild; //primitive datatype node
                    struct primitive_type_struct * info3 = (struct primitive_type_struct *)primitive->ninf;
                    if(info3->int_or_real == TK_INT){
                        field->type = INT;
                        field->width = 4 ;
                        field->offset = offset;
                        offset += field->width;
                        add_record_field(temp,field);
                    }
                    else {
                        field->type = REAL;
                        field->width = 8 ;
                        field->offset = offset;
                        offset += field->width;
                        add_record_field(temp,field);
                    }   
                }
                else if (child->firstChild->construct == fieldtype_){
                    struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                    struct ast_node * constructed = child->firstChild;
                    struct constructed_type_struct * info2 = ( struct constructed_type_struct *)constructed->ninf;
                    field->lexeme = info2->ruid;
                    field->line_no = info->lineNum;
                    SymbolTableRecord* record_ruid = getSymbolInfo(info2->ruid,global);
                    if(record_ruid == NULL)
                     if(record_ruid==NULL) {
                        printf("Line NO: %d, Error: The ruid used in the field declaration has not been defined till now. %s",info2->lineNum,info2->ruid);
                        exit(0);
                    }
                    SymbolTableRecord* record_original = getSymbolInfo(record_ruid->type_ruid, global);
                   if(record_original==NULL){
                       printf("LINENO: %d, Error: There is no record corresponding to the ruid %s",info2->lineNum,record_ruid->type_ruid);
                   }
                    if (record_original->type == RECORD){
                        field->type = RECORD;
                        field->width = record_original->width;
                        field->offset = offset;
                        offset += field->width;
                    }
                    else if(record_original->type == UNION){
                        field->type = UNION;
                        field->width = record_original->width;
                        field->offset=offset;
                        offset = field->width + offset;
                    }
                    add_record_field(temp,field);
                }
                child = child->nextSib;
            }
            temp->width = offset;
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,global);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(global,temp->lexeme,temp);
        }
        else if (info->union_or_record = TK_UNION){
            temp->type = UNION;
            temp->lexeme = info->ruid;
            temp->line_no = info->lineNum;
            temp->recordFields = NULL;

            int offset =0; //for calculting the offset of fields relative to the base of the record definition

            //child == **fieldDefinition**
            ast_node * child = current->firstChild->firstChild; 
            while(child != NULL){ 
                //for each field definition create a record field * and attach it to linked list
                struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                struct field_id_struct * info2 = (struct field_id_struct *)child->ninf;
                field->lexeme = info2->fieldid;
                field->line_no = info2->lineNum;

                if(child->firstChild->construct == primitiveDatatype_){
                    struct ast_node * primitive = child->firstChild; //primitive datatype node
                    struct primitive_type_struct * info3 = (struct primitive_type_struct *)primitive->ninf;
                    if(info3->int_or_real == TK_INT){
                        field->type = INT;
                        field->width = 4 ;
                        field->offset = 0;
                        offset = (offset > field->width ? offset: field->width);
                        add_record_field(temp,field);
                    }
                    else {
                        field->type = REAL;
                        field->width = 8 ;
                        field->offset = 0;
                        offset = (offset > field->width ? offset: field->width);
                        add_record_field(temp,field);
                    }   
                }
                else if (child->firstChild->construct == fieldtype_){
                    struct record_field *field = (struct record_field *)malloc(sizeof(struct record_field));
                    struct ast_node * constructed = child->firstChild;
                    struct constructed_type_struct * info2 = (struct constructed_type_struct *)constructed->ninf;
                    field->lexeme = info2->ruid;
                    field->line_no = info->lineNum;
                    SymbolTableRecord* record_ruid = getSymbolInfo(info2->ruid,global);
                    if(record_ruid == NULL)
                     if(record_ruid==NULL) {
                        printf("Line NO: %d, Error: The ruid used in the field declaration has not been defined till now. %s",info2->lineNum,info2->ruid);
                        exit(0);
                    }
                    SymbolTableRecord* record_original = getSymbolInfo(record_ruid->type_ruid, global);
                   if(record_original==NULL){
                       printf("LINENO: %d, Error: There is no record corresponding to the ruid %s",info2->lineNum,record_ruid->type_ruid);
                   }
                    if (record_original->type == RECORD){
                        field->type = RECORD;
                        field->width = record_original->width;
                        field->offset = 0;
                        offset = (offset > field->width ? offset: field->width);
                    }
                    else if(record_original->type == UNION){
                        field->type = UNION;
                        field->width = record_original->width;
                        field->offset = 0;
                        offset = (offset > field->width ? offset: field->width);
                    }
                    add_record_field(temp,field);
                }
                child = child->nextSib;
            }
            temp->width = offset;
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,global);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(global,temp->lexeme,temp);
        }
    }

    else if (current->construct == definetypestmt_){
        struct defineType_struct *info = (struct defineType_struct *)current->ninf;
        struct constructed_type_struct *from = info->from;
        struct constructed_type_struct *to = info->to;
        SymbolTableRecord * entry = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        entry->lexeme = to->ruid;
        entry->type = RUID;
        entry->type_ruid = from->ruid;
        entry->line_no = to->lineNum;
        SymbolTableRecord * entry2 = getSymbolInfo(entry->lexeme,global);
            if(entry2 != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", entry->line_no, entry->lexeme, entry2->line_no);
                return 1;
            }
        addSymbol(global,entry->lexeme,entry);
    }
    ast_node * child = current->firstChild;
    while(child!=NULL){
        if(traverseNode(child,global)==1) return 1;
        child=child->nextSib;
    }
}
int traverseNodeFunction(ast_node * current, symbolTable* table, symbolTable* global, SymbolTableRecord * function){ //returns 0 if no error
    if(current->construct == input_par_ || current->construct == output_par_){
        ast_node * child = current->firstChild;
        while(child!=NULL){
            SymbolTableRecord * record = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
            Type type;
            int width;
            char * ruid = NULL;
            int lineNo;
            if(child->construct==primitiveDatatype_){
                struct primitive_type_struct * info = (struct primitive_type_struct *)child->ninf;
                if(info->int_or_real==TK_INT){
                    type = INT;
                    width = 4;
                }
                else {type = REAL; width = 8;}

                lineNo = info->lineNum;
            }
            else if (child->construct==constructedDatatype_){
                struct constructed_type_struct * info = (struct  constructed_type_struct*)child->ninf;
                ruid = info->ruid;
                if(info->union_or_record==TK_RECORD){
                    type = RECORD;
                }
                else type = UNION;

                lineNo = info->lineNum;
                SymbolTableRecord * entry = getSymbolInfo(ruid,global);
                if(entry==NULL){
                    printf("Line Num: %d, Error: Couldn't find definition for this constructed datatype.",lineNo);
                    return 1;
                }
                width = entry->width;
            }
            child = child->nextSib;
            struct id_struct * info = (struct  id_struct*)child->ninf;
            char * lexeme = info->lexID;
            record->type = type;
            record->lexeme = lexeme;
            record->line_no = lineNo;
            record->type_ruid = ruid;
            record->width = width;
            if(current->construct==input_par_){
                function->function_field->InputHead = add_function_par(function->function_field->InputHead,record->type,record->type_ruid);
            }else{
              function->function_field->OutputHead = add_function_par(function->function_field->OutputHead,record->type,record->type_ruid);   
            }
            SymbolTableRecord * entry = getSymbolInfo(record->lexeme,table);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", record->line_no, record->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(table,record->lexeme,record);
            child = child->nextSib;
        }
    }
    if(current->construct == declaration_){
        SymbolTableRecord * temp = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        struct id_struct * info = (struct id_struct *)current->ninf;
        temp->lexeme = info->lexID;
        temp->line_no = info->lineNum;
        ast_node * dataType = current->firstChild;
        if(dataType->construct == primitiveDatatype_){
            struct primitive_type_struct *info =(struct primitive_type_struct *) dataType->ninf;
            temp->line_no = info->lineNum;
            if(info->int_or_real==TK_INT){
                temp->type = INT;
                temp->type_ruid = NULL;
                temp->width = 4;
            }
            else if(info->int_or_real==TK_REAL){
                temp->type=REAL;
                temp->type_ruid = NULL;
                temp->width = 8;
            }
        }
        else if (dataType->construct==constructedDatatype_){
            struct constructed_type_struct *info = (struct constructed_type_struct *)dataType->ninf;
            if(info->union_or_record==TK_UNION){
                
                SymbolTableRecord * entry = getSymbolInfo(info->ruid,global);
                if(entry == NULL){
                    printf("Line No: %d, No type definition found corresponding to this type %s\n",info->lineNum,info->ruid);
                    return 1;
                }
                temp->type = UNION;
                temp->width = entry->width;
                temp->type_ruid = info->ruid;
                temp->line_no = info->lineNum;
            }
            else if (info->union_or_record==TK_RECORD){
                SymbolTableRecord * entry = getSymbolInfo(info->ruid,global);
                if(entry == NULL){
                    printf("Line No: %d, No type definition found corresponding to this type %s\n",info->lineNum,info->ruid);
                    return 1;
                }
                temp->type = RECORD;
                temp->type_ruid = info->ruid;
                temp->line_no = info->lineNum;
                temp->width = entry->width;
            }
            else if (info->union_or_record == -1){
                SymbolTableRecord * entry = getSymbolInfo(info->ruid,global);
                if(entry==NULL){
                    printf("Line No: %d, No type definition found corresponding to this type %s\n",info->lineNum,info->ruid);
                    return 1;
                }
                temp->type = entry->type;
                temp->type_ruid = entry->type_ruid;
                temp->line_no = info->lineNum;
                temp->width = entry->width;
            }
        }
        if(info->isGlobal){
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,global);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(global,temp->lexeme, temp);
        }
        else {
            SymbolTableRecord * entry = getSymbolInfo(temp->lexeme,table);
            if(entry != NULL){
                printf("Line NO: %d, Redeclaration of same identifier %s, previous declaration found on line %d", temp->line_no, temp->lexeme, entry->line_no);
                return 1;
            }
            addSymbol(table,temp->lexeme, temp);
            }
    }
    ast_node * child = current->firstChild;
    while(child!=NULL){
        if(traverseNodeFunction(child,table,global, function)==1) return 1;
        child=child->nextSib;
    }
    return 0;
}

symbolTable * populateSymbolTable(ast_node * root){
    symbolTable * globalTable = newSymbolTable(SLOTS);
    traverseNode(root,globalTable);
    ast_node * function = root->firstChild;
    while(function!=NULL){
        SymbolTableRecord * FunEntry = (SymbolTableRecord*) malloc(sizeof(SymbolTableRecord));
        struct func_struct *info = (struct func_struct *)function->ninf;
        FunEntry->lexeme = info->funID;
        FunEntry->line_no = info->lineNum;
        FunEntry->type = FUNCTION;
        symbolTable * funTable = newSymbolTable(SLOTS);
        FunEntry->functionTable = funTable;
        addSymbol(globalTable,FunEntry->lexeme,FunEntry);
        FunEntry->function_field = (struct function_field *)malloc(sizeof(struct function_field));
        FunEntry->function_field->InputHead = NULL;
        FunEntry->function_field->OutputHead=NULL;
        FunEntry->width =0;
        traverseNodeFunction(function,funTable,globalTable,FunEntry);
        function = function->nextSib;
    }
    return globalTable;
}
// printSymbolTable(symbolTable * table, char * name){
// printf("**************%s Symbol Table*****************",name);
// printf("Current Offset (Or Size): %d", table->currentOffset);
// for(int i=0;i<table->no_slots;i++){
//     SymbolTableRecord * entry = table->list[i];

// }
// }
// void printSymbolTable(symbolTable * global){

// }