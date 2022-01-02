
//Header file from the Stack project and it is at C:\Users\DELL\source\repos\Stack_dinamic\Stack_dinamic\stack.h
#include "..\..\Stack_dinamic\Stack_dinamic\stack.h"
#include "differentiator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const unsigned int SIZE_FIELD = 100;

#define ValidPtr(ptr) if(!ptr) {errno = ErrorCode::ERRPTR; return -1;}
#define LastNode(ptr) if(!ptr) {return 0;}


#define parseFunc(node, code)\
node->var_type = code;\
newNode(node->right);\
newNode(node->left);\
ParseExp(node->right, text);


#define newNode(node) node = CreateNode();

#define fillField(node, value, code)\
strncpy(node->field, value, SIZE_FIELD);\
node->var_type = code;

#define fillNode(node_ptr, str, code) \
newNode(node_ptr);\
strncpy(node_ptr->field, str, SIZE_FIELD);\
node_ptr->var_type = code;

#define fillDifNode(t_node, node)\
newNode(t_node);\
DiffFunc(node, t_node);

#define fillCpyNode(t_node, node)\
newNode(t_node);\
CpyTree(t_node, node);


typedef struct Node {
    char field[SIZE_FIELD] = {};
    int var_type = 0;
    Node* left;
    Node* right;
} Node;


Node* CreateNode() {
    Node* node = (Node*)calloc(1, sizeof(Node));
    if (node == NULL) {
        //errno = ErrorCode::ERRPTR;
        return nullptr;
    }
    node->left = NULL;
    node->right = NULL;
    return node;
}


int ReadTree(Node* node) {
    LastNode(node);
    printf("field %s, type %d\n", node->field, node->var_type);
    ReadTree(node->left);
    ReadTree(node->right);
    return 0;
}


int ParseExp(Node* node, char* text)
{
    ValidPtr(text);
    ValidPtr(node);
    static int i = 1;
    char temp_str[SIZE_FIELD] = {};
    int ind = 0;
    if (text[i] == '(') {
        newNode(node->left);
        i++;
        ParseExp(node->left, text);
        ParseExp(node, text);
        newNode(node->right);
        i++;
        ParseExp(node->right, text);
    }
    else {
        ind = i;
        while (text[i] != ')' && text[i] != '(') {
            temp_str[i - ind] = text[i];
            i++;
        }
        temp_str[i - ind] = '\0';
        if (!strcmp(temp_str, "*") || !strcmp(temp_str, "+") || !strcmp(temp_str, "-") || !strcmp(temp_str, "^"))
            node->var_type = VarType::OP;
        else if (IsDigit(temp_str))
            node->var_type = VarType::NUM;
        else if (!strcmp(temp_str, "x"))
            node->var_type = VarType::VAR;
        else if (!strcmp(temp_str, "sin")) {
            i++;
            parseFunc(node, VarType::SIN);
        }
        else if (!strcmp(temp_str, "cos")) {
            i++;
            parseFunc(node, VarType::COS);
        }

        strncpy(node->field, temp_str, SIZE_FIELD);
        while (text[i] == ')')
            i++;
    }
    return 0;
}

int IsDigit(char* str) {
    int i = 0;
    if (str[0] == '-')
        i++;
    for (i; i < strlen(str); i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int DiffFunc(Node* node, Node* target_node)
{
    LastNode(node);
    ValidPtr(target_node);
    if (node->var_type == VarType::NUM) {
        fillField(target_node, "0", VarType::NUM)
    }
    else if (node->var_type == VarType::VAR) {
        fillField(target_node, "1", VarType::NUM)
    }
    else if (node->var_type == VarType::SIN) {
        fillField(target_node, "*", VarType::OP)

        fillNode(target_node->left, "cos", VarType::COS);
        Node* node_l = target_node->left;
        newNode(node_l->left);
        fillCpyNode(node_l->right, node->right);

        fillDifNode(target_node->right, node->right);
    }
    else if (node->var_type == VarType::COS) {
        fillField(target_node, "*", VarType::OP)

        fillNode(target_node->left, "*", VarType::OP);
        Node* node_l = target_node->left;
        fillNode(node_l->right, "-1", VarType::NUM);

        fillNode(node_l->left, "sin", VarType::SIN);
        Node* node_ll = node_l->left;
        newNode(node_ll->left);
        fillCpyNode(node_ll->right, node->right);

        fillDifNode(target_node->right, node->right);
    }
    else if (node->var_type == VarType::OP){
        if (!strcmp(node->field, "*")) {
            fillField(target_node, "+", VarType::OP)

            fillNode(target_node->left, "*", VarType::OP);
            Node* node_l = target_node->left;
            fillDifNode(node_l->left, node->left);
            fillCpyNode(node_l->right, node->right);

            fillNode(target_node->right, "*", VarType::OP);
            Node* node_r = target_node->right;
            fillDifNode(node_r->left, node->right);
            fillCpyNode(node_r->right, node->left);

        }
        else if (!strcmp(node->field, "+")) {
            fillField(target_node, "+", VarType::OP)

            fillDifNode(target_node->left, node->left);
            fillDifNode(target_node->right, node->right);
        }
        else if (!strcmp(node->field, "-")) {
            fillField(target_node, "-", VarType::OP)

            fillDifNode(target_node->left, node->left);
            fillDifNode(target_node->right, node->right);
        }
        else if (!strcmp(node->field, "^")) {
            fillField(target_node, "*", VarType::OP)

            fillNode(target_node->left, "*", VarType::OP);
            Node* node_l = target_node->left;
            fillCpyNode(node_l->left, node->right);
            fillNode(node_l->right, "^", VarType::OP);
            Node* node_lr = node_l->right;
            fillCpyNode(node_lr->left, node->left);
            fillNode(node_lr->right, "-", VarType::OP);
            Node* node_lrr = node_lr->right;
            fillCpyNode(node_lrr->left, node->right);
            fillNode(node_lrr->right, "1", VarType::NUM);

            fillDifNode(target_node->right, node->left);
        }
    }
    return 0;
}


int CpyTree(Node* target_node, Node* node){
    ValidPtr(node);
    ValidPtr(target_node);
    strncpy(target_node->field, node->field, SIZE_FIELD);
    target_node->var_type = node->var_type;
    if (node->left) {
        fillCpyNode(target_node->left, node->left);
    }
    if (node->right) {
        fillCpyNode(target_node->right, node->right);
    }
    return 0;
}


int WriteTree(Node* node, FILE* file) {
    ValidPtr(file);
    LastNode(node);
    fprintf(file, "(");
    if (node->var_type == VarType::OP || node->var_type == VarType::NUM || node->var_type == VarType::VAR) {
        WriteTree(node->left, file);
    }
    printf("val %s type %d\n", node->field, node->var_type);
    fprintf(file, "%s", node->field);
    WriteTree(node->right, file);
    fprintf(file, ")");
    return 0;
}


int DrawTree(Node* node, FILE* file) {
    ValidPtr(file);
    static int i = 0;
    if (node->left == NULL && node->right == NULL) {
        fprintf(file, "\t\"box %d\" [shape=plaintext, label=<\n<table>\n\t", i);
        fprintf(file, "<tr><td colspan='2' port='field'>%s</td></tr>\n\t",
            node->field);
        fprintf(file, "</table>\n\t>]; \n");
        i++;
        return 0;
    }
    fprintf(file, "\t\"box %d\" [shape=plaintext, label=<\n<table>\n\t", i);
    fprintf(file, "<tr><td colspan='2' port='field'>%s</td></tr>\n\t",
        node->field);
    fprintf(file, "</table>\n\t>]; \n");
    i++;
    DrawTree(node->left, file);
    DrawTree(node->right, file);
    return i;
}


int DrawWays(Node* node, FILE* file, Stack* stack) {
    ValidPtr(file);
    ValidPtr(stack);
    if (node->left == NULL && node->right == NULL)
        return 0;
    static int i = 0;
    fprintf(file, "\t\"box %d\":field->\"box %d\":field;\n", i, i + 1);
    Push(stack, i);
    i++;
    DrawWays(node->left, file, stack);
    float ind = 0;
    Pop(stack, &ind);
    fprintf(file, "\t\"box %d\":field->\"box %d\":field;\n", (int)ind, i + 1);
    i++;
    DrawWays(node->right, file, stack);
    return 0;
}


int Dump(Node* node) {
    ValidPtr(node);
    FILE* file = fopen("diff_graf.txt", "w");

    fprintf(file, "digraph List {\n");
    fprintf(file, "rankdir=TB;\n");

    DrawTree(node, file);

    Stack* stack = CreateStack("stack");
    DrawWays(node, file, stack);
    ClearStk(stack);
    fprintf(file, "}\n");
    fclose(file);
    return 0;
}