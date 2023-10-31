#ifndef  HW4_Header
#define _HW4_Header

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN_OF_PROD_NAME 51

typedef struct Product //struct of a product (dish).
{
	char* product_name;
	int quantity;
	int price;
	struct Product* next;
}Product, * P_Product;

typedef struct L_prod //manager struct of list of products. 
{
	struct Product* head;
	struct Product* tail;
}L_prod, * P_L_prod;

void Error_Msg(char* msg); //prints error message and quit the program.
P_L_prod* Create_tables(int tables_size); //creates the array of the tables in the restaurant and an empty list of product for each one of them.
int CreateProducts(P_L_prod Kitchen, FILE* pf, P_L_prod tables[], int tables_size); //creates the list of products in the kitchen.
P_Product Is_exist_product(P_L_prod Kitchen, char* temp_name); //check if a product is already exist in a list by its name.
void AddAsLast(P_Product new_node, P_L_prod Kitchen); //append a new node to the end of a list.
int AddItems(char* prod_name, int num, P_L_prod Kitchen); //add amount of products to an existing product in the kitchen.
int OrderItem(P_L_prod tables[], int tables_size, int TableNumber, char* ProductName, int Quantity, P_L_prod Kitchen); //add products from the kitchen to a specific table.
int RemoveItem(P_L_prod tables[], int tables_size, int TableNumber, char* ProductName, int Quantity); //return products from a specific table.
int RemoveTable(P_L_prod tables[], int tables_size, int TableNumber); //gets a bill of a table.
void DeleteList(P_L_prod Kitchen); //delete list of product and release allocated memory.
void Delete_Tables(P_L_prod tables[], int tables_size); //delete array of tables and release allocated memory.

#endif
