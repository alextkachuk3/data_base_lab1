#pragma once

#define name_lenght 20

typedef struct
{
	int ID;
	int flight_number;
	int ID_owner;
	int next_ticket_pos;
}ticket;

typedef struct
{
	int ID;
	int country_code;
	char name[name_lenght];
	int count_of_tickets;
	int head_ticket_pos;
}passenger;

typedef struct
{
	int ID;
	int pos;
}passenger_ind;

typedef struct
{
	int ID;
	int pos;
}ticket_ind;

typedef struct
{
	int count_passenger;
	int count_ticket;
}count_info;