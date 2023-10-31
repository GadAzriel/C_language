#include "HW4_Header.h"

int main()
{
	FILE* pf, * inst; //pf will be 'Manot.txt' and inst will be 'Instructions.txt'.
	L_prod L; //the struct of the kitchen
	P_L_prod Kitchen = &L;
	P_L_prod* tables; //will be the array of the tables.
	int tables_size; //the number of tables.
	int ch; //the command read from the instructions files (1-5)
	char temp_name[MAX_LEN_OF_PROD_NAME];
	int temp_quantity, temp_tab_num;
	if ((inst = fopen("Instructions.txt", "r")) == NULL) //opens 'Instructions' file.
		Error_Msg("error with the 'Instructions' input file.\n");
	if ((pf = fopen("Manot.txt", "r")) == NULL) //opens 'Manot' file.
		Error_Msg("error with the 'Manot' input file.\n");
	fscanf(inst, "%d", &tables_size); //read the amount of tables in the restaurant.
	fscanf(inst, "%d", &ch); //read from the file the wanted command. (should be '1').
	tables = Create_tables(tables_size); //creates the array of the tables (including empty list for each tables).
	if (ch != 1) {//if the first command is not '1' as it should be. it releases allocated memory and quit the program.
		printf("the first action must be '1' !\n");
		Delete_Tables(tables, tables_size);
		exit(1);
	}
	if (CreateProducts(Kitchen, pf, tables, tables_size) == 1) //creates the list of the kitchen products.
		printf("The kitchen was created successfully.\n");
	//loop - do the wanted actions until the end of the instruction file or an error.
	while (fscanf(inst, "%d", &ch) != EOF) //read another command
	{
		if (ch == 2) {
			fscanf(inst, "%s %d", temp_name, &temp_quantity); //reads the necessary details for using function '2'.
			if (AddItems(&temp_name, temp_quantity, Kitchen) == 1) //do the action the check if succeeded.
				printf("%d %s were added to the kitchen.\n", temp_quantity, temp_name);
			else
				printf("Adding Items failed.\n");
		}
		else if (ch == 3) {
			fscanf(inst, "%d %s %d", &temp_tab_num, temp_name, &temp_quantity); //reads the necessary details for using function '3'.
			if (OrderItem(tables, tables_size, temp_tab_num, &temp_name, temp_quantity, Kitchen) == 1) //do the action the check if succeeded.
				printf("%d %s were added to the table number %d .\n", temp_quantity, temp_name, temp_tab_num);
			else
				printf("Order of items '%s' for table %d failed.\n", temp_name, temp_tab_num);
		}
		else if (ch == 4) {
			fscanf(inst, "%d %s %d", &temp_tab_num, temp_name, &temp_quantity); //reads the necessary details for using function '4'.
			if (RemoveItem(tables, tables_size, temp_tab_num, &temp_name, temp_quantity) == 1) //do the action the check if succeeded.
				printf("%d '%s' were returned successfully to the kitchen from table number %d.\n", temp_quantity, temp_name, temp_tab_num);
			else
				printf("Items removing from table %d failed.\n", temp_tab_num);
		}
		else if (ch == 5) {
			fscanf(inst, "%d", &temp_tab_num); //reads the necessary details for using function '5'.
			if (RemoveTable(tables, tables_size, temp_tab_num) == 1) //do the action the check if succeeded.
				printf("Table %d closed successfully.\n", temp_tab_num);
		}
		else //if the wanted action is not 1-5.
			printf("Invalid action.\n");
	}
	DeleteList(Kitchen); //release the allocation of the kitchen list.
	Delete_Tables(tables, tables_size); //release the allocation of the tables array and lists.
	fclose(pf); //closes the file.
	fclose(inst); //closes the file.
	return 0;
}
