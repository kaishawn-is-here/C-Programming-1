#include <stdio.h>
#include <string.h>

//Function to check if either one conditions are met to cause unsafe reading
int isUnsafe(double temperature, double current){
    return temperature >= 80.0 || current >= 8.0;
}

//Function to check if both conditions are met for safe reading for reset
int isSafeForReset(double temperature, double current){
    return temperature <= 70.0 && current <= 6.0;
}

int main()
{
    //Declare variables and arrays
    int N;

    double temperature[200];
    double current[200];
    char command[200][10];

    //Read N
    scanf("%d", &N);

    //Read temperature, current, command for each N line
    for (int i = 0; i < N; i++){
        scanf("%lf %lf %9s", &temperature[i], &current[i], command[i]);
    }

    //Adding tripped state to check whether if it's active
    //Adding consecutive count for both safe and unsafe readings
    //Adding a count for trips and resets
    int tripped = 0, safecount = 0, unsafecount = 0;
    int trips = 0, resets = 0;

    for (int i = 0; i < N; i++){
        int unsafe = isUnsafe(temperature[i], current[i]);
        int safe = isSafeForReset(temperature[i], current[i]);
        int isReset = (strcmp(command[i], "RESET") == 0);

        //Add towards consecutive unsafe reading count when conditions are met
        if (unsafe){
            unsafecount++;
            safecount = 0;
        }

        //Add towards consecutive safe reading count when conditions are met
        else if (safe){
            safecount++;
            unsafecount = 0;
        }

        //For values neither unsafe nor safe for reset, restarts both consecutive count
        //For example, temperature above 70 but below 80 or current above 6 but below 8
        else{
            safecount = 0;
            unsafecount = 0;
        }

        const char *status;

        if (tripped){
            //Successfully resets after meeting conditions
            if (safecount >= 2 && isReset){
                tripped = 0;
                resets++;
                unsafecount = 0;
                safecount = 0;
                status = "NORMAL";
            }

            //Remains tripped state if condtions not yet fulfilled
            else{
                status = "TRIPPED";
            }
        }

        else{
            //Third consecutive unsafe reading causes a trip
            if (unsafe && unsafecount >= 3){
                tripped = 1;
                trips++;
                status = "TRIPPED";
            }

            //Unsafe reading causing warning sign
            else if (unsafe){
                status = "WARNING";
            }

            //Not unsafe, normal operation
            else{
                status = "NORMAL";
            }
        }

        //Outputs every reading and its status line by line
        printf("%d %s\n", i+1, status);
    }

    //Prints number of trips and resets at the end
    printf("TRIPS %d RESETS %d\n", trips, resets);
    return 0;
}
