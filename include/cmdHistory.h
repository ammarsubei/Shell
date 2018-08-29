#pragma once


typedef struct NODE
{
	char* Command;
	struct NODE* Next;
	struct NODE* Previous;
} NODE;

typedef struct LL
{
	NODE* Head;
	NODE* Tail;
	int Count;
} LL;

LL* LL_init(LL* list);

void push_node(LL* list, NODE* newNode);

NODE* create_node(char* text);