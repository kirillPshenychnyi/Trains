#ifndef TRAINS_HPP
#define TRAINS_HPP


static const char * MONTH_NAMES [] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static const int DAYS_IN_MONTHS[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


struct Time 
{
	int m_month;
	int m_day;
	int m_hours;
	int m_minutes;
};

struct Train {
	Time * time;
	char m_town[20];
	char m_dest[20];
	bool m_avaliable;
};

bool IsLastDayOfTheMonth (Time * time);

void input_data (Train * trains , int size);

void output (const Train * trains , int size);

Train * Create (int size);

void clear (Train * _trains);

void delay_train (Train * trains , int size);

void delay_all (Train * trains , int size);

void deny (Train * train ,  int size );

void findOnDate ( const Train * trains ,  int size );

void percentage_deny (Train * trains , int size);

void closest ( const Train * trains , int size );

void sort (Train * trains , int size);

void swap (Train * a , Train * b );
#endif