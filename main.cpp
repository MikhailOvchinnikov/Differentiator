#include "differentiator.h"

//Header from the SortOnegin project, that src located at C:\Users\DELL\source\repos\Sort_Onegin\Sort_Onegin
#include "..\..\Sort_Onegin\Sort_Onegin\sort_text.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE* file = fopen("expression.txt", "rb");
    if (file == NULL)
        errno = ErrorCode::ERRFILE;

    int syms = CountSymbols(file);
    char* text = (char*)calloc((syms + 1), sizeof(char));
    if (text == NULL)
        errno = ErrorCode::ERRALLOC;
    GetParametersFile(file, text, syms);
    fclose(file);
    Node* eqtn = CreateNode();
    ParseExp(eqtn, text);
    ReadTree(eqtn);
    //Dump(eqtn);

    Node* dif_eqtn = CreateNode();
    DiffFunc(eqtn, dif_eqtn);
    FILE* file2 = fopen("target_file.txt", "w");
    if (file2 == NULL)
        errno = ErrorCode::ERRFILE;
    WriteTree(dif_eqtn, file2);
    fclose(file2);
    Dump(dif_eqtn);
}
