#include "HW4_Header.h"

/*  name: Error_Msg
input: a string.
output: none.
description: the function gets a string and print it an then exit the program.*/
void Error_Msg(char* msg)
{
	printf("\n%s", msg);
	exit(1);
}

/*  name: Create_tables
input: an integer.
output: a pointer to an array of lists of product (double pointer).
description: the function gets a size (amount of table in the restaurant) and creates an array of tables.
for each tables it set an empty list of product. the func returns the array created.*/
P_L_prod* Create_tables(int tables_size)
{
	int i;
	P_L_prod* tables = (P_L_prod*)malloc(sizeof(P_L_prod) * tables_size); //allocates the array of tables (lists).
	if (tables == NULL) //there is nothing to release yet.
		Error_Msg("allocation error.\n");
	for (i = 0; i < tables_size; i++) //for each table
	{
		tables[i] = (P_L_prod)malloc(sizeof(L_prod)); //allocates a list manager struct.
		if (tables[i] == NULL) { //checks the allocation.
			Delete_Tables(tables, i - 1); //release the allocated memory until that point.
			Error_Msg("allocation error.\n"); //quits.
		}
		tables[i]->head = NULL; //reset the lists in each table.
		tables[i]->tail = NULL; //reset the lists in each table.
	}
	return tables; //returns the array.
}

/*  name: CreateProducts
input: a pointer to list of products, a pointer to file, a pointer to array of lists, and the size of is (integer).
output: an integer {0\1}.
description: the function gets pointer to a list of products in the kitchen. the func reads the products from the file and creates the list.
the func gest the tables array only for a case of an error so it could release it before quitting the program.*/
int CreateProducts(P_L_prod Kitchen, FILE* pf, P_L_prod tables[], int tables_size)
{
	char flag; //indicates if all details valid.
	//resets the list.
	Kitchen->head = NULL;
	Kitchen->tail = NULL;
	P_Product temp; //helping node.
	char temp_name[MAX_LEN_OF_PROD_NAME] = ""; //helping string
	temp = (P_Product)malloc(sizeof(Product)); //allocates memory of the first product.
	if (temp == NULL) { //checks the allocation.
		Delete_Tables(tables, tables_size); //release the tables array.
		Error_Msg("allocation error.\n");
	}
	//reads product for the file (each line is a product) until the end of the file.
	while (fscanf(pf, "%s %d %d", temp_name, &temp->quantity, &temp->price) != EOF)
	{
		flag = 1; //starts from 'valid' position each time.
		if (Is_exist_product(Kitchen, &temp_name) != NULL) { //checks if the item is already exist in the list.
			printf("the product '%s' is already exist.\n", temp_name);
			flag = 0;
		}
		if (temp->quantity <= 0) { //checks if the quantity is a positive number.
			printf("the product quantity must be a positive number.\n");
			flag = 0;
		}
		if (temp->price <= 0) { //checks if the price is a positive number.
			printf("the product price must be a positive number.\n");
			flag = 0;
		}
		if (flag == 1) { //if every thing was ok.
			AddAsLast(temp, Kitchen); //links the product to the end of the list.
			temp->product_name = (char*)malloc(strlen(temp_name) + 1); //allocates memory for the name of the products.
			if (temp->product_name == NULL) { //checks the allocation.
				DeleteList(Kitchen);
				Delete_Tables(tables, tables_size);
				Error_Msg("allocation error.\n");
			}
			strcpy(temp->product_name, temp_name); //copy the string (name) to the allocated memory in the node.
		}
		temp = (P_Product)malloc(sizeof(Product)); //allocates memory for another node (product).
		if (temp == NULL) { //checks the allocation.
			DeleteList(Kitchen);
			Delete_Tables(tables, tables_size);
			Error_Msg("allocation error.\n");
		}
	}
	return 1; //returns 1 if all seceded.
}

/*  name: Is_exist_product
input: a pointer to list of products (kitchen), a pointer to a string (name of product).
output: a pointer of a product or NULL value.
description: the function gets pointer to a list of products in the kitchen, and a name of a product.
the func checks if the product exist in the list. if it does it returns its address. else it returns NULL.*/
P_Product Is_exist_product(P_L_prod Kitchen, char* temp_name)
{
	P_Product temp = Kitchen->head; //starts from the first node in the list.
	while (temp != NULL) { //run over the list.
		if (strcmp(temp->product_name, temp_name) == 0) //compare the names of the products.
			return temp;
		temp = temp->next;
	}
	return NULL; //if it is not exist yet.
}

/*  name: AddAsLast
input: a pointer to a new node (product), and a pointer to list (manager struct of kitchen).
output: none.
description: the function gets a linked list (struct which include the head, and the tail), and a new node.
the func, append the new node to the end (tail) of the linked list.*/
void AddAsLast(P_Product new_node, P_L_prod Kitchen)
{
	new_node->next = NULL; //make sure the new node leads to nothing (NULL).
	if (Kitchen->tail != NULL) //if the list is NOT empty, it should link the last node to that new one.
		Kitchen->tail->next = new_node;
	else //if the list is empty, it should set the head of the list.
		Kitchen->head = new_node;
	Kitchen->tail = new_node; //set the tail to be the new last node.
}
/*  name: AddItems
input: a pointer to list of products (kitchen), a pointer to a string (name of product), an integer (quantity).
output: a integer {0\1}.
description: the function gets pointer to a list of products in the kitchen, and a name of a product, and quantity to add.
the func checks if the product exist in the list. if it does it add the 'num' to its quantity. else it prints a message and returns 0.*/
int AddItems(char* prod_name, int num, P_L_prod Kitchen)
{
	P_Product temp = Is_exist_product(Kitchen, prod_name); //search for the wanted product.
	if (temp == NULL) { //if it is not exist.
		printf("Item '%s' is NOT exist.  ", prod_name);
		return 0;
	}
	if (num <= 0) { //checks if the num to add is positive.
		printf("the amount of product to add must be a positive integer.  ");
		return 0;
	}
	temp->quantity += num; //add the quantity.
	return 1; //returns 1 if all seceded.
}

/*  name: OrderItem
input: a pointer to list of products (kitchen), a pointer to array of lists (tables), and the size of is (integer),
	   a number of specific table, a name of product and a quantity.
output: an integer {0\1} (indicates of success).
description: the function gets a specific number of table (0 - num of tables minus one), and a name of a product to order, and a quantity.
if the product is available in the kitchen, it orders it to the table. it means add the products to the list of product of the table,
and miss it from the kitchen list of products.*/
int OrderItem(P_L_prod tables[], int tables_size, int TableNumber, char* ProductName, int Quantity, P_L_prod Kitchen)
{
	P_Product temp_on_table, temp_in_kitchen, new_item;
	if (TableNumber < 0 || TableNumber > tables_size) { //check if the TableNumber is valid.
		printf("Table number %d is NOT exist.  ", TableNumber);
		return 0;
	}
	if (Quantity < 0) { //checks if the quantity is a positive number.
		printf("%d is NOT a valid amount of '%s' to order.  ", Quantity, ProductName);
		return 0;
	}
	temp_on_table = Is_exist_product(tables[TableNumber], ProductName); //search for the specific product in the table.
	temp_in_kitchen = Is_exist_product(Kitchen, ProductName); //search for the specific product in the kitchen.
	if (temp_in_kitchen == NULL) {//if the product is not exist in the kitchen.
		printf("We don't have '%s', sorry.  ", ProductName);
		return 0;
	}
	if (Quantity > temp_in_kitchen->quantity) { //if there are no enough of this product left in the kitchen.
		printf("There is NOT enough quantity of '%s' in the kitchen to add %d to the table.  ", ProductName, Quantity);
		return 0;
	}
	temp_in_kitchen->quantity -= Quantity; //miss the amount ordered from the kitchen.
	if (temp_on_table != NULL) { //if already ordered that kind of product to the table.
		temp_on_table->quantity += Quantity;
		return 1;
	}
	new_item = (P_Product)malloc(sizeof(Product)); //allocates memory for that new product in the table.
	if (new_item == NULL) { //checks the allocation.
		DeleteList(Kitchen);
		Delete_Tables(tables, tables_size);
		Error_Msg("allocation error.\n");
	}
	AddAsLast(new_item, tables[TableNumber]); //links the new product to the end of the list.
	new_item->product_name = (char*)malloc(strlen(temp_in_kitchen->product_name) + 1); //allocate memory for the name of the product.
	if (new_item->product_name == NULL) { //checks the allocation.
		DeleteList(Kitchen);
		Delete_Tables(tables, tables_size);
		Error_Msg("allocation error.\n");
	}
	strcpy(new_item->product_name, temp_in_kitchen->product_name); //copy the name into the allocated memory.
	new_item->price = temp_in_kitchen->price; //set the price of the product ordered.
	new_item->quantity = Quantity; //set the quantity of the product ordered.
	return 1;
}

/*  name: RemoveItem
input: a pointer to array of lists (tables), and the size of is (integer), a number of specific table, a name of product and a quantity.
output: an integer {0\1} (indicates of success).
description: the function gets a specific number of table (0 - num of tables minus one), and a name of a product to remove, and a quantity.
the func remove the products from the table to the garbage and release the allocated memory of them if necessary.*/
int RemoveItem(P_L_prod tables[], int tables_size, int TableNumber, char* ProductName, int Quantity)
{
	P_Product temp;
	if (TableNumber < 0 || TableNumber > tables_size) { //check if the TableNumber is valid
		printf("Table number %d is NOT exist.  ", TableNumber);
		return 0;
	}
	temp = Is_exist_product(tables[TableNumber], ProductName); //search for the specific product in the table.
	if (temp == NULL) { //if it doesnt exist.
		printf("Cant remove '%s' because you didnt ordered any of those to table %d.  ", ProductName, TableNumber);
		return 0;
	}
	if (Quantity <= 0) { //checks if the quantity is a positive number.
		printf("Cant remove %d items. (must be a positive number of items).  ", Quantity);
		return 0;
	}
	if (Quantity > temp->quantity) { //checks if there is enough to remove.
		printf("You ordered only %d of '%s' to table %d. cant remove %d of those.  ", temp->quantity, ProductName, TableNumber, Quantity);
		return 0;
	}
	temp->quantity -= Quantity; //removes.
	return 1;
}

/*  name: RemoveItem
input: a pointer to array of lists (tables), and the size of is (integer), a number of specific table.
output: an integer {0\1} (indicates of success).
description: the function gets a specific number of table to remove (0 - num of tables minus one).
the func prints the order of the table and the final bill with and without tip.
the func also release the allocated memory of the list of product for that table.*/
int RemoveTable(P_L_prod tables[], int tables_size, int TableNumber)
{
	P_Product temp;
	float sum = 0; //the bill.
	if (TableNumber < 0 || TableNumber > tables_size) { //check if the TableNumber is valid
		printf("Table number %d is NOT exist.  ", TableNumber);
		return 0;
	}
	if (tables[TableNumber]->head == NULL) { //if the table has no ordered products.
		printf("Table number %d is not ordered yet.  ", TableNumber);
		return 0;
	}
	printf("The order of table %d :\n", TableNumber);
	while (tables[TableNumber]->head != NULL) //run over the products ordered to the table.
	{
		temp = tables[TableNumber]->head;
		if (temp->quantity > 0) //prints each products that hasnt removed back to the kitchen.
			printf("%s %d %d ---> ", temp->product_name, temp->quantity, temp->price);
		sum += temp->quantity * temp->price; //sum the bill.
		tables[TableNumber]->head = tables[TableNumber]->head->next;
		//release the memory allocated.
		free(temp->product_name);
		free(temp);
	}
	tables[TableNumber]->tail = NULL; //rests also the tail of the list.
	//prints the final bill of the table.
	printf("\nThe final bill is: %g $\n", sum);
	printf("The final bill is (with 10%% tip): %g $\n", (sum * 1.1));
	return 1;
}

/*  name: DeleteList
input: a pointer to list (manager structs).
output: none.
description: the function gets list and removes it (release the memory allocated for its nodes) and set the head and tail to be NULL.*/
void DeleteList(P_L_prod Kitchen)
{
	P_Product temp; //temporary node.
	while (Kitchen->head != NULL) { //for each node in the list it release the first each time.
		temp = Kitchen->head;
		Kitchen->head = (Kitchen->head)->next;
		free(temp->product_name);
		free(temp); //release the memory.
	}
	Kitchen->tail = NULL; //set the tail to be NULL.
}

/*  name: Delete_Tables
input: a pointer to an array of lists (tables), and the size of it (integer).
output: none.
description: the function gets an array of lists (tables) and removes it (release the memory allocated for its nodes) and set the head and tail to be NULL.*/
void Delete_Tables(P_L_prod tables[], int tables_size)
{
	int i;
	P_Product temp;
	for (i = 0; i < tables_size; i++) { //for each table in the array.
		while (tables[i]->head != NULL) { //run over the list of each table
			temp = tables[i]->head;
			tables[i]->head = (tables[i]->head)->next;
			free(temp->product_name); //release the memory allocated for the name.
			free(temp); //release the node itself.
			free(tables[i]); //release the manager struct.
		}
	}
	free(tables); //release the array itself.
}
