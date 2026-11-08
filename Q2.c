#include <stdio.h>
#include <string.h>
#include <math.h>

//Function to calculate the required motor torque
double requiredTorque(double load_torque, double G, double efficiency){
    return fabs(load_torque) / (G * efficiency);
}

//Function to calculate the required motor speed
double requiredSpeed(double load_speed, double G){
    return G * fabs(load_speed);
}

//Function to calculate the estimated motor current
double motorCurrent(double Tm, double Kt, double I0){
    return I0 + Tm / Kt;
}

//Function to calculate the electrical energy per stage
double stageEnergy(double V, double I, double duration){
    return V * I * duration;
}

//Function to check each candidate across all stages: returns 1 if feasible, 0 if not
//Stores the total cycle energy through total_energy
int evaluation(double load_torque[], double load_speed[], double duration[], int S, double G, double efficiency, double Tmax, double nmax, double Kt,
               double I0, double V, double Imax, double *total_energy){
    int feasibility = 1;
    *total_energy = 0.0;

    for (int j = 0; j < S; j++){
        double Tm = requiredTorque(load_torque[j], G, efficiency);
        double nm = requiredSpeed(load_speed[j], G);
        double I = motorCurrent(Tm, Kt, I0);

        //Any limit exceeded in any stage makes the candidate infeasible (equality is allowed)
        if (Tm > Tmax || nm > nmax || I > Imax){
            feasibility = 0;
        }

        //Adds up the energy from all stages
        *total_energy += stageEnergy(V, I, duration[j]);
    }

    return feasibility;
}

int main()
{
    //Declare variables and arrays
    int S, M;
    double V, Imax;
    double G, efficiency, Tmax, nmax, Kt, I0;

    double load_torque[100], load_speed[100], duration[100];
    char id[50][13];
    double energy[50];
    int feasible[50];

    double best_energy = 0.0;
    char bestID[13];
    int found = 0;

    //Read S, M, V, Imax
    scanf("%d %d", &S, &M);
    scanf("%lf %lf", &V, &Imax);

    //Read load torque, load speed, duration for each stage
    for (int i = 0; i < S; i++){
        scanf("%lf %lf %lf", &load_torque[i], &load_speed[i], &duration[i]);
    }

    // Read ID, G, efficiency, Tmax, nmax, Kt, I0 for each candidate
    for (int i = 0; i < M; i++){
        scanf("%12s %lf %lf %lf %lf %lf %lf", id[i], &G, &efficiency, &Tmax, &nmax, &Kt, &I0);

        //Evaluate current candidate
        //Checks feasibility and total energy
        feasible[i] = evaluation(load_torque, load_speed, duration, S, G, efficiency, Tmax, nmax, Kt, I0, V, Imax, &energy[i]);
    }

    //Outputs the candidates line by line
    for (int i = 0; i < M; i++){
        if (feasible[i]){
            printf("%s FEASIBLE %.2f\n", id[i], energy[i]);

            //Update best if lower energy, or equal energy with an alphabetically earlier ID
            if (!found || energy[i] < best_energy || (energy[i] == best_energy && strcmp(id[i], bestID) < 0)){
                found = 1;
                best_energy = energy[i];
                strcpy(bestID, id[i]);
            }
        }

        //Outputs the candidates for those that are infeasible
        else{
            printf("%s INFEASIBLE\n", id[i]);
        }
    }

    //Prints when there is feasible candidates
    if (found){
        printf("BEST %s %.2f\n", bestID, best_energy);
    }

    //Prints when no feasible candidates are found
    else{
        printf("BEST NONE\n");
    }

    return 0;
}
