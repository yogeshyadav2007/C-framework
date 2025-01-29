//sll

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.c"

#define DATA_FILE "framework.dat"

FILE *fp_menu;
FILE *fp_fields;
FILE *fp_data;

char status[2];
int record_counter = 0;

struct details *head = NULL;
struct details *new_node = NULL;

void show_menu()
{
	char menu[MAX];
	while(fgets(menu, MAX, fp_menu) != NULL)
	{
		printf("%s", menu);
	}
}

void read_field_names() 
{
	while (fgets(field_names[fields_count], MAX, fp_fields) != NULL) 
	{
		field_names[fields_count][strcspn(field_names[fields_count], "\n")] = 0;
		fields_count++;
	}
	FILE *fp_status = fopen("status.cfg", "r");
	for(int counter = 0; counter < 2; counter++)
	{
		fread(&status[counter], sizeof(char), 1, fp_status);
	}
}

void save_list_into_file(char mode)
{
	if(mode == 'a')
	{
		fp_data = fopen(DATA_FILE, "a");
		fwrite(&new_node->fields, sizeof(struct domain), 1, fp_data);
		fclose(fp_data);
	}
	else
	{
		fp_data = fopen(DATA_FILE, "w");
		struct details *temp = head;
		while(temp != NULL)
		{
			fwrite(&temp->fields, sizeof(struct domain), 1, fp_data);
			temp = temp->next;
		}
		fclose(fp_data);
	}
}

void load_records()
{
	fp_data = fopen(DATA_FILE, "r");
	struct details *new_node;
	struct details *current = NULL;
	while((new_node = (struct details *)malloc(sizeof(struct details))) != NULL && fread(&new_node->fields, sizeof(struct domain), 1, fp_data) == 1)
	{
		new_node->next = NULL;
		if(head == NULL)
		{
			head = new_node;
		}
		else
		{
			current->next = new_node;
		}
		current = new_node;
		record_counter++;
	}
	fclose(fp_data);
}

void save_data();
void show_data();
void update_data();
void find_match();
void delete_data();
void sort();
void sort_by(int choice);
void generate_id(char *id);

int main()
{
	int choice;
	fp_menu = fopen("menu.cfg", "r");
	fp_fields = fopen("fields.cfg", "r");
	load_records();
	read_field_names();
	while (1) 
	{
		show_menu();
		scanf("%d", &choice);
		switch (choice) 
		{
			case 1: save_data();
					break;
			case 2: show_data();
					break;
			case 3: update_data();
					break;
			case 4: find_match();
					break;
			case 5: sort();
					break;
			case 6: exit(0);
			default: printf("Invalid choice. Please try again.\n");
		}
		rewind(fp_menu);
	}
	fclose(fp_menu);
	fclose(fp_fields);
	return 0;
}

void save_data() 
{
	char **field_pointer;
	new_node = (struct details *)malloc(sizeof(struct details));
	field_pointer = get_address(new_node);
	generate_id(field_pointer[0]);
	for (int counter = 1; counter < fields_count - 1; counter++) 
	{
		printf("Enter %s: ", field_names[counter]);
		if(counter == count - 1)
		{
			scanf("%f", (float *)field_pointer[counter]);
		}
		else
		{
			fflush(stdin);
			fgets(field_pointer[counter], MAX, stdin);
			field_pointer[counter][strcspn(field_pointer[counter], "\n")] = 0;
		}
	}
	*(char*)field_pointer[fields_count - 1] = status[0];
	new_node->next = NULL;
	if (head == NULL) 
	{
		head = new_node;
	} 
	else 
	{
		struct details *temp = head;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new_node;
	}
	save_list_into_file('a');
	record_counter++;
	printf("Details saved successfully.\n");
}

void show_data()
{
	char **field_pointer;
	if (head == NULL) 
	{
		printf("The list is empty.\n");
		return;
	}
	for(int counter = 0; counter < fields_count; counter++)
	{
		printf("| %-25s", field_names[counter]);
	}
	printf("\n");
	struct details* temp = head;
	while (temp != NULL) 
	{
		field_pointer = get_address(temp);
		for(int counter = 0; counter < fields_count; counter++)
		{
			if(counter == 2)
			{
				printf("| %-25.2f", *(float*)field_pointer[counter]);
			}
			else if (counter == fields_count - 1)
			{
				printf("| %-25c ", *(char*)field_pointer[counter]);
			}
			else
			{
				printf("| %-25s", (char*)field_pointer[counter]);
			}
		}
		printf("\n");
		temp = temp->next;
	}
}

void update_data() 
{
	char **field_pointer;
	char update_id[15];
	int choice;
	if (head == NULL) 
	{
		printf("The list is empty. Nothing to update.\n");
		return;
	}
	printf("Enter %s to update: ", field_names[0]);
	scanf("%s", update_id);
	struct details *temp = head;
	while (temp != NULL && strcmp((char*)temp, update_id) != 0) 
	{
		temp = temp->next;
	}
	if (temp == NULL) 
	{
		printf("Details with %s: %s not found.\n", field_names[0], update_id);
		return;
	}
	int user_choice;
	printf("How many fields do you want to update? ");
	scanf("%d", &user_choice);
	field_pointer = get_address(temp);
	for(int iterator = 0; iterator < user_choice; iterator++)
	{
		printf("Which data would you like to update?\n");
		for (int counter = 1; counter < fields_count; counter++) 
		{
			printf("%d. %s\n", counter, field_names[counter]);
		}
		printf("Enter your choice: ");
		scanf("%d", &choice);
		if (choice < 1 || choice >= fields_count) 
		{
			printf("Invalid choice. No updates made.\n");
			return;
		}
		if(choice == fields_count - 1)
		{
			if(*field_pointer[fields_count - 1] == status[1])
			{
				*field_pointer[fields_count - 1] = status[0];
			}
			else
			{
				*field_pointer[fields_count - 1] = status[1];
			}
		}
		else
		{
			printf("Enter new %s: ", field_names[choice]);
			if (choice == count - 1) 
			{
				scanf("%f", (float *)field_pointer[choice]);
			} 
			else
			{
				fflush(stdin);
				fgets(field_pointer[choice], MAX, stdin);
				field_pointer[choice][strcspn(field_pointer[choice], "\n")] = 0;
			}
		}
	}
	save_list_into_file('w');
	printf("Details updated successfully.\n");
}

void find_match()
{
	char del_id[MAX];
	struct details *temp = head, *prev = NULL;
	printf("Enter the %s of the record you want to delete: ", field_names[0]);
	fflush(stdin);
	fgets(del_id, MAX, stdin);
	del_id[strcspn(del_id, "\n")] = 0;
	while (temp != NULL && strcmp((char*)temp, del_id) != 0)
	{
		prev = temp;
		temp = temp->next;
	}
	delete_data(temp);
	save_list_into_file('w');
	printf("Details deleted successfully.\n");
}

void delete_data(struct details *match)
{
	struct details **indirect = &head;
	while(*indirect != match)
	{
		indirect = &((*indirect)->next);
	}
	*indirect = (*indirect)->next;
	record_counter--;
	free(match);
}

void sort_by(int choice)
{
	int count = 0;
	char **temp_pointer, **front_pointer;
	struct details *temp = head;
	struct details *prev = NULL;
	struct details *front = NULL;
	while(temp != NULL)
	{
		count++;
		temp = temp->next;
	}
	while(count != 0)
	{
		temp = head;
		prev = NULL;
		for (int counter = 0; counter < count - 1; counter++)
		{
			int swap = 0;
			front = temp->next;
			temp_pointer = get_address(temp);
			front_pointer = get_address(front);
			if (choice == 2)
			{
				if (*(float *)temp_pointer[choice] < *(float *)front_pointer[choice])
				{
					swap = 1;
				}
			}
			else
			{
				if (strcasecmp(temp_pointer[choice], front_pointer[choice]) > 0)
				{
					swap = 1;
				}
			}
			if (swap)
			{
				temp->next = front->next;
				front->next = temp;
				if (prev == NULL)
				{
					head = front;
				}
				else
				{
					prev->next = front;
				}

				prev = front;
			}
			else
			{
				prev = temp;
				temp = front;
			}
			free(temp_pointer);
			free(front_pointer);
		}
			count--;
	}
}

void sort()
{
	int choice;
	printf("Which field would you like to sort by?\n");
	for (int counter = 1; counter < fields_count; counter++) 
	{
		printf("%d. %s\n", counter, field_names[counter]);
	}
	printf("Enter your choice: ");
	scanf("%d", &choice);
	sort_by(choice);
	show_data();
	sort_by(0);
}

void generate_id(char *id)
{
	static char *previous_id = NULL;
	if(record_counter == 0)
	{
		strcpy(id, "SBIN0000001");
		return;
	}
	if(previous_id == NULL)
	{
		struct details *temp = head;
		while(temp->next != NULL)
		{
			temp = temp->next;
		}
		strcpy(id, (char*)temp);
	}
	int len = strlen(id);
	id[len - 1] = (char)((int)id[len - 1] + 1);
	previous_id = id;
}