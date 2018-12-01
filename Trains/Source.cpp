#include "Trains.h"
#include <iostream>
int main () {

	std :: cout << "Enter the initial size of trains" << std :: endl;
	int size;
	std :: cin >> size;
	Train * trains; 
	
	trains  = Create (size);
	input_data (trains , size);

	sort(trains ,size );
	std :: cout << "Commands : 1 - to find on date , 2 - to find on date(the closest) , 3 - to delay all trains , 4 - to delay the wished train , 5 - to deny train , 6 - percentage deny   , 7 - out ,  any key to exit " << std :: endl;
	while (true) {
		int choose;
		std :: cin >> choose ; 
		switch (choose) {
		case 1 : findOnDate(trains , size);
			break;
		case 2: closest(trains , size);
			break;
		case 3 : delay_all(trains , size);
			break;
		case 4 : delay_train (trains , size);
			break;
		case 5 : deny (trains , size);
			break;
		case 6 : percentage_deny(trains, size);
			break;
		case 7 : output(trains , size);
			break;
		default : clear(trains);


		}



	}
	
	
	return 0 ; 
}