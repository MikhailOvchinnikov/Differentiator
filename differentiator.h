#pragma once
#include <stdio.h>

typedef struct Node Node;
typedef struct Stack Stack;


enum ErrorCode {
	ERRFILE = 1,
	ERRPTR,
	ERRREADFILE,
	ERRALLOC
};


enum VarType
{
	NUM = 1,
	VAR,
	OP,
	SIN,
	COS
};


/// <summary>
/// Allocate memory for node of the tree
/// </summary>
/// <returns>Pointer to allocated memory or nullptr if error occured</returns>
Node* CreateNode();


/// <summary>
/// Read all nodes of the tree and write them to the log file
/// </summary>
/// <param name="node">Transmitted root of the tree</param>
/// <returns>0</returns>
int ReadTree(Node* node);


/// <summary>
/// Reade data from the string and write them to the node
/// </summary>
/// <param name="node">Transmitted root of the tree</param>
/// <param name="text">Data from a file</param>
int ParseExp(Node* node, char* text);


/// <summary>
/// Detect digit string
/// </summary>
/// <param name="str">Transmitted string</param>
/// <returns>0 if str is not digit, else 1</returns>
int IsDigit(char* str);


/// <summary>
/// Differentiate node and write a result to a new tree
/// </summary>
/// <param name="node">Transmitted root of a tree</param>
/// <param name="target_node">Transmitted target root of a tree for writing result</param>
/// <returns>-1 if an error occured, else 0 </returns>
int DiffFunc(Node* node, Node* target_node);


/// <summary>
/// Copy to the new tree all roots's lower nodes
/// </summary>
/// <param name="target_node">Transmitted target root of a tree</param>
/// <param name="node">Transmitted root of a tree</param>
/// <returns>-1 if an error occured, else 0 </returns>
int CpyTree(Node* target_node, Node* node);


/// <summary>
/// Write tree to the file
/// </summary>
/// <param name="node">Transmitted root of a tree</param>
/// <param name="file">Transmitted pointer to a file</param>
/// <returns>-1 if an error occured, else 0 </returns>
int WriteTree(Node* node, FILE* file);


/// <summary>
/// Write to the file a data in dot format
/// </summary>
/// <param name="node">Transmitted root of the tree</param>
/// <param name="file">File for writing data</param>
/// <returns>Number of writed node</returns>
int DrawTree(Node* node, FILE* file);


/// <summary>
/// Write branches between nodes of grafic tree
/// </summary>
/// <param name="node">Transmitted root of the tree</param>
/// <param name="file">ransmitted pointer to a file</param>
/// <param name="stack">Transmitted pointer to a stack</param>
/// <returns>-1 if an error occured, else 0</returns>
int DrawWays(Node* node, FILE* file, Stack* stack);


/// <summary>
/// Write tree in the dot format for png view
/// </summary>
/// <param name="node">Transmitted root of the tree</param>
/// <returns>-1 if an error occured, else 0</returns>
int Dump(Node* node);