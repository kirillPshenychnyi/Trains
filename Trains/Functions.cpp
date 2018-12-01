#include "Trains.h"
#include <iostream>
#include <string.h>
#include <cassert>

Train * Create (int size) {
	Train * trains = new Train[size]; 

	return trains;

}



void clear (Train * trains) {

	delete [] trains;

}


void input_data(Train * trains ,int size) {

	std :: cout << "Enter town , destanation and departure month , day and time " << std :: endl;
	for (int i = 0 ; i < size; i++) {
		trains[i].time = new Time;
		std :: cin >> trains[i].m_town;
		std :: cin >> trains[i].m_dest;
		std :: cin >> trains[i].time ->m_month;
		std :: cin >> trains[i].time ->m_day;
		std :: cin >> trains[i].time ->m_hours;
		std :: cin >> trains[i].time ->m_minutes;
		trains[i].m_avaliable = true;
	}
}

void output(const Train * trains , int size) {
	for (int i = 0 ; i < size; i++) 
		if (trains[i].m_avaliable) {
			std :: cout << trains[i].m_town << '-';
			std :: cout << trains[i].m_dest << std :: endl;
			std :: cout << MONTH_NAMES [ trains[i].time -> m_month - 1 ] << ' ' << trains[i].time -> m_day << ' ' << trains[i].time ->m_hours << ' ' << trains[i].time -> m_minutes << std :: endl ;
			}

}


bool IsLastDayOfTheMonth (Time * time) {
	return time ->m_day == DAYS_IN_MONTHS[time -> m_month - 1];

}


void swap (Train * a , Train * b) {
	Train temp = * a;
	* a = * b;
	* b = temp;


}

void sort (Train * p_trains , int size) {
	for (int i = 0 ; i < size - 1; i++) {
		int lowIndex = i;
		for (int j = i + 1; j < size; j++)
			if(p_trains[j].time ->m_month < p_trains[i].time -> m_month)
				lowIndex = j;
			else if (p_trains[j].time ->m_month == p_trains[i].time -> m_month)
				if (p_trains[j].time ->m_day < p_trains[i].time -> m_day)
					lowIndex = j;
				else if (p_trains[j].time ->m_day == p_trains[i].time -> m_day)
					if (p_trains[j].time ->m_hours < p_trains[i].time -> m_hours)
						lowIndex = j;	
					else if (p_trains[j].time ->m_hours == p_trains[i].time -> m_hours)
						if (p_trains[j].time -> m_minutes < p_trains[i].time -> m_minutes)		
						lowIndex = j;
		swap (p_trains + lowIndex , p_trains + i);
				
			}
	}

void delay_all (Train * trains , int size) {
	std :: cout << "Enter on which time you would like to delay the trains in format month/day/hour/minutes" << std :: endl;
	int month , day , hour , minutes;
	std :: cin >> month >> day >> hour >> minutes;
	

	for (int i = 0 ; i < size ; i++) {
		if (trains[i].time -> m_minutes + minutes >= 60) 
		{
			int delta = trains[i].time -> m_minutes + minutes + 60;
			trains[i].time ->m_hours++;
			trains[i].time ->m_minutes = delta;
		}
		else trains[i].time -> m_minutes += minutes;

		if (trains[i].time -> m_hours + hour >= 24) {
			trains[i].time -> m_day ++;
			trains[i].time -> m_hours = hour;
		}
		else trains ->time ->m_hours += hour;
		if ( trains[i].time -> m_day + day > DAYS_IN_MONTHS[ trains[i].time ->m_month - 1 ]) {
			int delta = trains[i].time -> m_day + day - DAYS_IN_MONTHS[ trains[i].time ->m_month - 1 ];
			trains[i].time -> m_month ++;
			trains[i].time ->m_day = delta;
		}
		else trains[i].time -> m_day += day ;
	}
}


void delay_train (Train * trains , int size) {
	std :: cout << "Enter the departure town and destanation and delay time day/hours/minutes " << std :: endl;

	int hour , minute , day;
	int minIndex = 0;
	int maxIndex = size;
	char departure[20];
	char dest[20];


	std :: cin >> departure;
	std :: cin >> dest;
	std :: cin >> day >> hour >> minute;
	Train  target;
	for (int i =0 ; i < size; i++)
		if (!strcmp (departure , trains[i].m_town) && !strcmp (dest , trains[i].m_dest) ) 
			 target = trains[i];
			 
		
	
	
 if (target.time -> m_minutes + minute >= 60) 
		{
			int delta = target.time -> m_minutes + minute - 60;
			target.time ->m_hours++;
			target.time ->m_minutes = delta;
		}
		else target.time -> m_minutes += minute;

		if (target.time -> m_hours + hour >= 24) {
			target.time -> m_day ++;
			target.time -> m_hours = hour;
		}
		else target.time ->m_hours += hour;
		if ( target .time -> m_day + day > DAYS_IN_MONTHS[ target.time ->m_month - 1 ]) {
			int delta = target.time -> m_day + day - DAYS_IN_MONTHS[ target.time ->m_month - 1 ];
			target.time -> m_month ++;
			target.time -> m_day = delta;
		}
		else target.time -> m_day += day ;
		}

	

void deny (Train * trains ,  int size ) {
	std :: cout << "Enter the departure town and destanation of the cancelled train " << std :: endl;
	char departure[20];
	char dest[20];
	std :: cin >> departure;
	std :: cin >> dest;
	for (int i =0 ; i < size; i++)
		if (!strcmp (departure , trains[i].m_town) && !strcmp (dest , trains[i].m_dest) ) {
			 trains -> m_avaliable = 0;
		     return;
		}
}


void closest ( const Train * trains ,  int size ) 
{

	std :: cout << "Enter the wished date - month and day " << std :: endl;
	
	int month , day;

	std :: cin >> month >> day;

	Train day_before;
	for (int i = 0 ; i < size ; i++)
		if (trains[i].time -> m_month == month && trains[i].time -> m_day <= day )
			day_before = trains[i];

	std :: cout << "The closest train to the entered date is " <<
		day_before.m_town << ' ' << day_before.m_dest;
	std :: cout << MONTH_NAMES [ day_before.time -> m_month - 1 ] << ' ' << day_before.time -> m_day << ' ' << day_before.time ->m_hours << ' ' << day_before.time -> m_minutes 
	<< std :: endl ;

}


void findOnDate (const Train * trains , int size) {
	std :: cout << "Enter the wished date - month and day " << std :: endl;
	int month , day;
	Train day_before;
	Train day_after;
	Train target_day;
	std :: cin >> month >> day;
	
	int index = 0;
	bool init = 0;
	for (int i = 0 ; i < size; i++)
		if (trains[i].time -> m_month == month && trains[i].time -> m_day <= day ) {
			day_before = trains[i];
			init = 1 ;
			index = i;
		}
		if (! init) {
			for (int i = 0 ; i < size; i++)
				if (trains[i].time -> m_month == month) 
			day_before = trains[i];
					}	
		
		day_after = trains[index+ 1 ];
		int delta1 = day - day_before.time -> m_day;
		int delta2 = day - day_after.time ->  m_day;
		if (delta1 == delta2 ) {
			if (day_before.time ->m_hours > day_after.time ->m_hours)
				target_day = day_before;
			else target_day = day_after;
		}
		else 
		std :: cout << delta1 << delta2 << std :: endl;
		delta1 < delta2 ? target_day = day_before : target_day = day_after;
		std :: cout << "The closest train to the entered date is" << ' '  << target_day.m_town << '-' << target_day.m_dest << ' ';
		std :: cout << MONTH_NAMES [ target_day.time -> m_month - 1 ] << ' ' << target_day.time -> m_day << ' ' << target_day.time ->m_hours << ' ' << target_day.time -> m_minutes 
		<< std :: endl ;


}


void percentage_deny (Train * trains , int size) 
{
	double * repeats = new double [size]; 
	memset(repeats , 0 , sizeof(double) * size);
	for (int i = 0 ; i < size - 1; i++) {
		for (int j = i + 1; j < size; j++)
			if (!strcmp (trains[i].m_dest , trains[j].m_dest) )
				repeats[i]++;
	}
	
	
	for (int i = 0 ; i < size; i++ )
		if (repeats[i] / size > 0.1) {
			char * dest = trains[i].m_dest;
			double delta_perc = repeats[i] / size * 100 - 10;
			delta_perc =( delta_perc * size ) / 10; 
			
			for (int j = 0 ; j < size ; j ++)
				if (!strcmp(trains[j].m_dest , dest  ) && delta_perc > 1 ) 
				{
					trains[i].m_avaliable = false;
					delta_perc--;
				}
		}
				
}