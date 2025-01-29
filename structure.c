#define MAX 30

struct domain
{
	char account_number[15];
	char name[20];
	float balance;
	char mobile_number[15];
	char aadhar_number[13];
	char email[20];
	char account_type[10];
	char status;
};

struct details
{
	struct domain fields;
	struct details *next;
};

int count = 3;
char **addressor;
char field_names[8][MAX];
int fields_count = 0;

char **get_address(struct details *locator)
{
	addressor = malloc(fields_count * sizeof(char*));
	addressor[0] = locator->fields.account_number;
	addressor[1] = locator->fields.name;
	addressor[2] = (char*)&locator->fields.balance;
	addressor[3] = locator->fields.mobile_number;
	addressor[4] = locator->fields.aadhar_number;
	addressor[5] = locator->fields.email;
	addressor[6] = locator->fields.account_type;
	addressor[7] = &locator->fields.status;

	return addressor;
}