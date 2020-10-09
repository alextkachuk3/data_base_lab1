#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main()
{
	FILE* CI;
	fopen_s(&CI, CountInfo, "r+b");
	fseek(CI, 0L, SEEK_END);
	if (!ftell(CI))
	{
		count_info ci = { 0, 0 };
		fwrite(&ci, sizeof(count_info), 1, CI);
	}
	fclose(CI);
	int op = 0;
	while (1)
	{
		printf("Select operation\n1.get-m\n2.get-s\n3.del-m\n4.del-s\n5.update-m\n6.update-s\n7.insert-m\n8.insert-s\n9.ut-m\n10.ut-s\n11.get-masters-count\n12.get-slaves-count\n13.get-count-master-slaves\n14.exit\n");
		scanf_s("%d", &op);
		switch (op)
		{
		case 1:
		{
			system("cls");
			printf_s("Enter index:");
			int num;
			scanf_s("%d", &num);
			passenger pass = get_m(num);
			printf_s("%-5d %-5d %-20s %-5d %-5d\n", pass.ID, pass.country_code, pass.name, pass.count_of_tickets, pass.head_ticket_pos);
			break;
		}
		case 2:
		{
			system("cls");
			printf_s("Enter index:");
			int num;
			scanf_s("%d", &num);
			ticket tick = get_s(num);
			printf_s("%-5d %-5d %-5d %-5d\n", tick.ID, tick.flight_number, tick.ID_owner, tick.next_ticket_pos);
			break;
		}
		case 3:
		{
			system("cls");
			printf("Enter index\n");
			int index;
			scanf_s("%d", &index);
			del_m(index);
			break;
		}
		case 4:
		{
			system("cls");
			printf("Enter index\n");
			int index;
			scanf_s("%d", &index);
			del_s(index);
			break;
		}
		case 5:
		{
			system("cls");
			int num, new_country_code;
			printf_s("Enter index:");
			scanf_s("%d", &num);
			printf_s("Enter new country code:");
			scanf_s("%d", &new_country_code);
			update_m(num, new_country_code);
			break;
		}
		case 6:
		{
			system("cls");
			int num, new_fl_num;
			printf_s("Enter index:");
			scanf_s("%d", &num);
			printf_s("Enter new flight number:");
			scanf_s("%d", &new_fl_num);
			update_s(num, new_fl_num);
			break;
			break;
		}
		case 7:
		{
			system("cls");
			int ID = 1, country_code = 1;
			char name[20];
			printf_s("Enter ID:");
			scanf_s("%d", &ID);
			printf_s("Enter country code:");
			scanf_s("%d", &country_code);
			printf_s("Enter name:");
			fseek(stdin, 0, SEEK_END);
			gets(name);
			insert_m(ID, country_code, name);
			break;
		}
		case 8:
		{
			system("cls");
			int ID = 1, fligth_number = 1, id_owner = 1;
			printf_s("Enter ID of ticket:");
			scanf_s("%d", &ID);
			printf_s("Enter fligth number:");
			scanf_s("%d", &fligth_number);
			printf_s("Enter ID of owner:");
			scanf_s("%d", &id_owner);
			insert_s(ID, fligth_number, id_owner);
			break;
		}
		case 9:
		{
			system("cls");
			ut_m();
			break;
		}
		case 10:
		{
			system("cls");
			ut_s();
			break;
		}
		case 11:
		{
			system("cls");
			printf_s("%d\n", get_count_of_masters());
			break;
		}
		case 12:
		{
			system("cls");
			printf_s("%d\n", get_count_of_slaves());
			break;
		}
		case 13:
		{
			system("cls");
			int index;
			printf("Enter index\n");
			scanf_s("%d", &index);
			printf_s("%d\n", (get_count_of_master_slaves(index)));
			break;
		}
		case 14: return 0;
		default:
		{
			system("cls");
			printf_s("Wrong operation!\n");
			break;
		}
		}
	}
	return 0;
}