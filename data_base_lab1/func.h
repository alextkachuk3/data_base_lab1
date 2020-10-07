#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

#define Ticket "T.fl"
#define Ticket_ind "T.ind"
#define Passenger "P.fl"
#define Passenger_ind "P.ind"
#define CountInfo "CI.bin"

int get_count_of_masters()
{
    FILE* COUNT_INFO;
    count_info ci;
    fopen_s(&COUNT_INFO, CountInfo, "rb");
    fread(&ci, sizeof(count_info), 1, COUNT_INFO);
    fclose(COUNT_INFO);
    return ci.count_passanger;
}

int get_count_of_slaves()
{
    FILE* COUNT_INFO;
    count_info ci;
    fopen_s(&COUNT_INFO, CountInfo, "rb");
    fread(&ci, sizeof(count_info), 1, COUNT_INFO);
    fclose(COUNT_INFO);
    return ci.count_ticket;
}

count_info get_count()
{
    FILE* COUNT_FILE;
    count_info ci;
    fopen_s(&COUNT_FILE, CountInfo, "rb");
    fread(&ci, sizeof(count_info), 1, COUNT_FILE);
    fclose(COUNT_FILE);
    return ci;
}

void sort_pass_ind()
{
    FILE* PASS_ind;
    fopen_s(&PASS_ind, Passenger_ind, "r+b");
    int count = get_count_of_masters();
    passenger_ind* pass_ind = (passenger_ind*)malloc(count*sizeof(passenger_ind));
    fread(pass_ind, sizeof(passenger_ind), count, PASS_ind);
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count - 1 ; j++)
        {
            if (pass_ind[j].ID > pass_ind[j+1].ID)
            {
                passenger_ind temp;
                temp.ID = pass_ind[j].ID;
                temp.pos = pass_ind[j].pos;
                pass_ind[j].ID = pass_ind[j+1].ID;
                pass_ind[j].pos = pass_ind[j + 1].pos;
                pass_ind[j+1].ID = temp.ID;
                pass_ind[j + 1].pos = temp.pos;
            }
        }
    }
    fseek(PASS_ind, 0L, SEEK_SET);
    fwrite(pass_ind, sizeof(passenger_ind), count, PASS_ind);
    fclose(PASS_ind);
}

void sort_tick_ind()
{
    FILE* TICK_ind;
    fopen_s(&TICK_ind, Ticket_ind, "r+b");
    int count = get_count_of_slaves();
    ticket_ind* tick_ind = (ticket_ind*)malloc(count * sizeof(ticket_ind));
    fread(tick_ind, sizeof(ticket_ind), count, TICK_ind);
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count - 1; j++)
        {
            if (tick_ind[j].ID > tick_ind[j + 1].ID)
            {
                passenger_ind temp;
                temp.ID = tick_ind[j].ID;
                temp.pos = tick_ind[j].pos;
                tick_ind[j].ID = tick_ind[j + 1].ID;
                tick_ind[j].pos = tick_ind[j + 1].pos;
                tick_ind[j + 1].ID = temp.ID;
                tick_ind[j + 1].pos = temp.pos;
            }
        }
    }
    fseek(TICK_ind, 0L, SEEK_SET);
    fwrite(tick_ind, sizeof(tick_ind), count, TICK_ind);
    fclose(TICK_ind);
}

int get_m_pos(int index)
{
    FILE* PASS_IND;
    fopen_s(&PASS_IND, Passenger_ind, "rb");
    passenger_ind pass_ind;
    int count_m = get_count_of_masters();
    for (int i = 0; i < count_m; i++)
    {
        fread(&pass_ind, sizeof(passenger_ind), 1, PASS_IND);
        if (pass_ind.ID == index)
        {
            fclose(PASS_IND);
            return pass_ind.pos;
        }
    }
    fclose(PASS_IND);
    return -1;
}

int get_s_pos(int index)
{
    FILE* TICK_IND;
    fopen_s(&TICK_IND, Ticket_ind, "rb");
    ticket_ind tick_ind;
    int count_s = get_count_of_slaves();
    for (int i = 0; i < count_s; i++)
    {
        fread(&tick_ind, sizeof(tick_ind), 1, TICK_IND);
        if (tick_ind.ID == index)
        {
            fclose(TICK_IND);
            return tick_ind.pos;
        }
    }
    fclose(TICK_IND);
    return -1;
}

passenger get_m(int index)
{
    FILE* PASS;
    fopen_s(&PASS, Passenger, "rb");
    fseek(PASS, get_m_pos(index), SEEK_CUR);
    passenger result;
    fread(&result, sizeof(passenger), 1, PASS);
    fclose(PASS);
    return result;
}

ticket get_s(int index)
{
    FILE* TICK;
    fopen_s(&TICK, Ticket, "rb");
    fseek(TICK, get_s_pos(index), SEEK_CUR);
    ticket result;
    fread(&result, sizeof(ticket), 1, TICK);
    fclose(TICK);
    return result;
}

void del_s(int index)
{
    FILE* COUNT_FILE;
    fopen_s(&COUNT_FILE, CountInfo, "r+b");
    count_info ci;
    fread(&ci, sizeof(count_info), 1, COUNT_FILE);
    fclose(COUNT_FILE);

    FILE* TICK_ind;
    fopen_s(&TICK_ind, Ticket_ind, "r+b");
    int tick_pos;

    ticket_ind tick_ind;

    for (int i = 0; i < ci.count_ticket; i++)
    {
        fread(&tick_ind, sizeof(tick_ind), 1, TICK_ind);
        if (tick_ind.ID == index)
        {
            break;
        }
    }

    int tick_ind_pos = ftell(TICK_ind) - sizeof(ticket_ind);
   
    FILE* TICK;
    fopen_s(&TICK, Ticket, "r+b");
    fseek(TICK, tick_pos, SEEK_SET);
    ticket del_tick;
    fread(&del_tick, sizeof(ticket), 1, TICK);
    FILE* PASS;
    fopen_s(&PASS, Passenger, "r+b");
    int pass_pos = get_m_pos(del_tick.ID_owner);
    passenger pass;
    fseek(PASS, pass_pos, SEEK_SET);
    fread(&pass, sizeof(passenger), 1, PASS);


    char q = '0';

    int is_last_ticket = tick_pos + sizeof(ticket) / sizeof(ticket) == ci.count_ticket;

    if (ci.count_ticket == 1)
    {
        fseek(TICK, 0, SEEK_SET);
        fwrite(&q, 1, sizeof(ticket), TICK);
        pass.head_ticket_pos = 0;
        pass.count_of_tickets = 0;
        //fwrite(&q, 1, sizeof(ticket_ind), TICK_ind);
    }
    else if (pass.head_ticket_pos==tick_pos && !del_tick.next_ticket_pos)
    {
        pass.count_of_tickets = 0;
        pass.head_ticket_pos = 0;
        if (is_last_ticket)
        {
            fseek(TICK, tick_pos - sizeof(ticket), SEEK_SET);
            fwrite(&q, 1, sizeof(ticket), TICK);
        }
        else
        {

        }
    }
    else if (pass.head_ticket_pos == tick_pos && del_tick.next_ticket_pos)
    {

    }
    else if(!del_tick.next_ticket_pos)
    {

    }
    else
    {

    }
    

    ci.count_ticket--;
    fopen_s(&COUNT_FILE, CountInfo, "r+b");
    fseek(PASS, pass_pos, SEEK_SET);
    fwrite(&pass, sizeof(passenger), 1, PASS);
    fseek(COUNT_FILE, 0, SEEK_SET);
    fwrite(&ci, sizeof(count_info), 1, COUNT_FILE);
    fclose(PASS);
    fclose(TICK);



    fclose(TICK_ind);
    fclose(COUNT_FILE);
}

void del_m(int ID)
{
    
}

void update_m(int index, int new_country_code)
{
    int pass_pos = get_m_pos(index);
    passenger edit_pass;
    FILE* PASS;
    fopen_s(&PASS, Passenger, "r+b");
    fseek(PASS, pass_pos, SEEK_SET);
    fread(&edit_pass, sizeof(passenger), 1, PASS);
    edit_pass.country_code = new_country_code;
    fseek(PASS, pass_pos, SEEK_SET);
    fwrite(&edit_pass, sizeof(passenger), 1, PASS);
    fclose(PASS);
}

void update_s(int index, int new_fligth_number)
{
    int tick_pos = get_s_pos(index);
    ticket edit_tick;
    FILE* TICK;
    fopen_s(&TICK, Ticket, "r+b");
    fseek(TICK, tick_pos, SEEK_SET);
    fread(&edit_tick, sizeof(passenger), 1, TICK);
    edit_tick.flight_number = new_fligth_number;
    fseek(TICK, tick_pos, SEEK_SET);
    fwrite(&edit_tick, sizeof(passenger), 1, TICK);
    fclose(TICK);
}

void insert_m(int ID, int country_code, char name[name_lenght])
{
    FILE* COUNT_FILE;
    fopen_s(&COUNT_FILE, CountInfo, "r+b");
    count_info ci;
    fread(&ci, sizeof(count_info), 1, COUNT_FILE);


    passenger new_passenger;
    new_passenger.ID = ID;
    new_passenger.country_code = country_code;
    strcpy_s(new_passenger.name, name_lenght ,name);
    new_passenger.count_of_tickets = 0;
    new_passenger.head_ticket_pos = 0;
    FILE* PASS;
    fopen_s(&PASS, Passenger, "r+b");
    fseek(PASS, ci.count_passanger*sizeof(passenger), SEEK_SET);
    int pos = ftell(PASS);
    fwrite(&new_passenger, sizeof(passenger), 1, PASS);
    fclose(PASS);

    passenger_ind new_passenger_ind = { ID, pos };
    FILE* PASS_IND;
    fopen_s(&PASS_IND, Passenger_ind, "r+b");
    fseek(PASS_IND, ci.count_passanger * sizeof(passenger_ind), SEEK_SET);
    fwrite(&new_passenger_ind, sizeof(passenger_ind), 1, PASS_IND);
    fclose(PASS_IND);


    ci.count_passanger++;
    fseek(COUNT_FILE, 0, SEEK_SET);
    fwrite(&ci, sizeof(count_info), 1, COUNT_FILE);
    fclose(COUNT_FILE);

    sort_pass_ind();
}

void insert_s(int ID, int fligt_number, int owner_id)
{
    int pass_poss = get_m_pos(owner_id);
    if (pass_poss == -1)
    {
        system("cls");
        printf("Wrong ID of owner\n");
        return;
    }


    FILE* COUNT_FILE;
    fopen_s(&COUNT_FILE, CountInfo, "r+b");
    count_info ci;
    fread(&ci, sizeof(count_info), 1, COUNT_FILE);


    ticket new_tick = { ID, fligt_number, owner_id, 0 };
    FILE* TICK;
    fopen_s(&TICK, Ticket, "r+b");
    fseek(TICK, ci.count_ticket*sizeof(ticket), SEEK_SET);
    int pos_tick = ftell(TICK);
    int size = sizeof(ticket);
    fwrite(&new_tick, sizeof(ticket), 1, TICK);

    FILE* TICK_ind;
    fopen_s(&TICK_ind, Ticket_ind, "r+b");
    fseek(TICK_ind, ci.count_ticket * sizeof(ticket_ind), SEEK_SET);
    ticket_ind new_ticket_ind = { ID, pos_tick };
    fwrite(&new_ticket_ind, sizeof(ticket_ind), 1, TICK_ind);
    fclose(TICK_ind);

    FILE* PASS;
    fopen_s(&PASS, Passenger, "r+b");
    fseek(PASS, pass_poss, SEEK_CUR);
    size = sizeof(passenger);
    passenger r_pass;
    fread(&r_pass, sizeof(passenger), 1, PASS);

    if (!r_pass.count_of_tickets)
    {
        r_pass.count_of_tickets++;
        r_pass.head_ticket_pos = pos_tick;
        fseek(PASS, pass_poss, SEEK_SET);
        fwrite(&r_pass, sizeof(passenger), 1, PASS);
    }
    else
    {
        r_pass.count_of_tickets++;
        fseek(PASS, pass_poss, SEEK_SET);
        fwrite(&r_pass, sizeof(passenger), 1, PASS);
        fseek(TICK, r_pass.head_ticket_pos, SEEK_SET);

        ticket current_ticket;
        fread(&current_ticket, sizeof(ticket), 1, TICK);
        for (int i = 0; i < r_pass.count_of_tickets - 2; i++)
        {
            fseek(TICK, current_ticket.next_ticket_pos, SEEK_SET);
            fread(&current_ticket, sizeof(ticket), 1, TICK);
        }
        current_ticket.next_ticket_pos = pos_tick;
        fseek(TICK, get_s_pos(current_ticket.ID), SEEK_SET);
        fwrite(&current_ticket, sizeof(ticket), 1, TICK);
    }

    fclose(TICK);
    fclose(PASS);

    
    ci.count_ticket++;
    fseek(COUNT_FILE, 0, SEEK_SET);
    fwrite(&ci, sizeof(count_info), 1, COUNT_FILE);
    fclose(COUNT_FILE);

    sort_tick_ind();
}

int get_count_of_master_slaves(int index)
{
    passenger pass = get_m(index);
    return pass.count_of_tickets;
}

void ut_m()
{
    count_info ci = get_count();

    printf("P.fl\n");
    FILE* PASS;
    fopen_s(&PASS, Passenger, "rb");
    passenger pass;
    for (int i = 0; i < ci.count_passanger; i++)
    {
        fread(&pass, sizeof(passenger), 1, PASS);
        printf("%-5d %-5d %10s %-5d %-5d \n", pass.ID, pass.country_code, pass.name, pass.count_of_tickets, pass.head_ticket_pos);
    }
    fclose(PASS);

    printf("P.ind\n");
    FILE* PASS_ind;
    fopen_s(&PASS_ind, Passenger_ind, "rb");
    passenger_ind pass_ind;
    for (int i = 0; i < ci.count_passanger; i++)
    {
        fread(&pass_ind, sizeof(passenger_ind), 1, PASS_ind);
        printf("%-5d %-5d\n", pass_ind.ID, pass_ind.pos);
    }
    fclose(PASS_ind);
}

void ut_s()
{
    count_info ci = get_count();

    printf("T.fl\n");
    FILE* TICK;
    fopen_s(&TICK, Ticket, "rb");
    ticket tick;
    for (int i = 0; i < ci.count_ticket; i++)
    {
        fread(&tick, sizeof(ticket), 1, TICK);
        printf("%-5d %-5d %-5d %-5d \n", tick.ID, tick.flight_number, tick.ID_owner, tick.next_ticket_pos);
    }
    fclose(TICK);

    printf("T.ind\n");
    FILE* TICK_ind;
    fopen_s(&TICK_ind, Ticket_ind, "rb");
    ticket_ind tick_ind;
    for (int i = 0; i < ci.count_ticket; i++)
    {
        fread(&tick_ind, sizeof(ticket_ind), 1, TICK_ind);
        printf("%-5d %-5d\n", tick_ind.ID, tick_ind.pos);
    }
    fclose(TICK_ind);    
}