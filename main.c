#include "./PJ_RPI/PJ_RPI.h"
#include <stdio.h>
#include <mysql.h>

void finish_with_error(MYSQL *con)
{
    printf("Finished with error. \n");
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

typedef struct{
    int gpio_number;
    int state;
    int prev_state;
} Rpi_pin;

Rpi_pin pin_list[] = {
    [0].gpio_number = 27,
    [0].state = 0,
    [0].prev_state = 0,

    [1].gpio_number = 22,
    [1].state = 0,
    [1].prev_state = 0
};

char querry[80];

int main(int argc, char **argv)
{
    int number_of_pins = sizeof(pin_list) / sizeof(pin_list[0]);

	if(map_peripheral(&gpio) == -1) 
	{
       	 	printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        	return -1;
    }

    MYSQL *con = mysql_init(NULL);
    printf("MySQL client version: %s\n", mysql_get_client_info());
    if (con == NULL) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }
    if (mysql_real_connect(con, "localhost", "PI", "raspberry", 
                "logs", 0, NULL, 0) == NULL) 
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

	while(1)
	{

        // Loop trough all pins in struct array
        for(int i = 0; i < number_of_pins; i++){

            int current_state;
            // "Convert" integer values to boolean
            if(GPIO_READ(pin_list[i].gpio_number))
                current_state = 1;
            else
                current_state = 0;

            // Debug purposes :)
            //printf("%d: curr state=%d, prev state=%d\n", i, current_state, pin_list[i].prev_state);

            // When button state changes => previous state is not equal to current state
            if( current_state != pin_list[i].prev_state){

                // Set up querry via sprintf
                sprintf(querry, "INSERT INTO `IO`(`gpio_number`, `state`, `date`) VALUES (%d,%d,now())", pin_list[i].gpio_number, current_state);
                printf("%s\n", querry);

                // Insert change into IO table
                if (mysql_query(con, querry))
                    finish_with_error(con);
            }

            // Set current state to previous state
            pin_list[i].prev_state = current_state;
        }

	}

	return 0;	

}