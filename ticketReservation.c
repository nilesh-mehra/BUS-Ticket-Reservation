#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Vehicle {
    int number;
    char source[50];
    char destination[50];
    int totalSeats;
    int availableSeats;
    float fare;
};

struct User {
    char username[50];
    char password[50];
};

void displayMainMenu();
void displayUserMenu();
int loginUser(struct User users[], int n, char uname[], char pass[]);
void displayAllVehicles(struct Vehicle v[], int n, const char *type);
void bookTicket(struct Vehicle v[], int n, const char *type);
void cancelTicket(struct Vehicle v[], int n, const char *type);

int main() {
    struct User users[4] = {
        {"user1","123"},{"user2","123"},{"user3","123"},{"user4","123"}
    };
    int numUsers = 4;

    struct Vehicle buses[8] = {
        {1,"ISBT","Rishikesh",43,43,80.0},
        {2,"Clement Town","Haridwar",35,35,120.0},
        {3,"Dehradun","Mussoorie",30,30,70.0},
        {4,"ISBT","Paonta",50,50,150.0},
        {5,"Dehradun","Vikasnagar",25,25,60.0},
        {6,"Shubhash Nagar","ClockTower",20,20,50.0},
        {7,"Dehradun","Saharanpur",38,38,130.0},
        {8,"ISBT","Doiwala",32,32,55.0}
    };
    
    struct Vehicle trains[8] = {
        {101,"Dehradun","Rishikesh",200,200,90.0},
        {102,"Dehradun","Haridwar",180,180,100.0},
        {103,"Dehradun","Cantt",150,150,70.0},
        {104,"Dehradun","Saharanpur",120,120,120.0},
        {105,"Dehradun","ClockTower",100,100,60.0},
        {106,"Dehradun","Bhauwala",180,180,140.0},
        {107,"Dehradun","MussoorieRd",80,80,50.0},
        {108,"Dehradun","Doiwala",150,150,75.0}
    };
    
    int numBuses = 8, numTrains = 8;
    int loggedIn = -1;
    char currentUser[50];

    while(1) {
        if(loggedIn == -1) {
            displayMainMenu();
            int ch; 
            scanf("%d", &ch);

            if(ch == 1) {
                char uname[50], pass[50];
                printf("Username: "); scanf("%s", uname);
                printf("Password: "); scanf("%s", pass);

                loggedIn = loginUser(users, numUsers, uname, pass);
                if(loggedIn != -1) {
                    strcpy(currentUser, uname);
                    printf("\n***** Welcome, %s! *****\n", currentUser);
                } else {
                    printf("\n*** Login failed! Try again. ***\n");
                }
            } 
            else if(ch == 2) { 
                printf("\nGoodbye!\n"); 
                break; 
            }
            else printf("\n*** Invalid choice! ***\n");
        } else {
            displayUserMenu();
            int ch; 
            scanf("%d", &ch);

            switch(ch){
                case 1: {
                    int type; 
                    printf("\n1. Bus\n2. Train\nChoice: "); 
                    scanf("%d", &type);
                    if(type == 1) bookTicket(buses, numBuses, "Bus");
                    else if(type == 2) bookTicket(trains, numTrains, "Train");
                    else printf("\n*** No Vehicle to select. ***\n");
                    break;
                }
                case 2: {
                    int type;
                    printf("\n1. Bus\n2. Train\nChoice: "); 
                    scanf("%d", &type);
                    if(type == 1) cancelTicket(buses, numBuses, "Bus");
                    else if(type == 2) cancelTicket(trains, numTrains, "Train");
                    else printf("\n*** Invalid type. ***\n");
                    break;
                }
                case 3:
                    displayAllVehicles(buses, numBuses, "Bus");
                    displayAllVehicles(trains, numTrains, "Train");
                    break;
                case 4: 
                    loggedIn = -1; 
                    printf("\n*** Logged out successfully! ***\n"); 
                    break;
                default: 
                    printf("\n*** Invalid choice! ***\n");
            }
        }
    }
    return 0;
}

void displayMainMenu() {
    printf("\n========== Main Menu ==========\n");
    printf("1. Login\n2. Exit\nEnter choice: ");
}

void displayUserMenu() {
    printf("\n========== User Menu ==========\n");
    printf("1. Book Ticket\n2. Cancel Ticket\n3. Check Status\n4. Logout\nEnter choice: ");
}

int loginUser(struct User users[], int n, char uname[], char pass[]) {
    for(int i = 0; i < n; i++)
        if(strcmp(users[i].username, uname) == 0 && strcmp(users[i].password, pass) == 0)
            return i;
    return -1;
}

void displayAllVehicles(struct Vehicle v[], int n, const char *type) {
    printf("\n============== %s List ==============\n", type);
    printf("---------------------------------------------------------------\n");
    printf("No  | From         | To           | Fare   | Total | Available\n");
    printf("---------------------------------------------------------------\n");
    for(int i = 0; i < n; i++) {
        printf("%3d | %-12s | %-12s | %6.2f | %5d | %9d\n",
               v[i].number, v[i].source, v[i].destination,
               v[i].fare, v[i].totalSeats, v[i].availableSeats);
    }
    printf("---------------------------------------------------------------\n");
}

void bookTicket(struct Vehicle v[], int n, const char *type) {
    displayAllVehicles(v, n, type);
    int num, seats;
    printf("Enter %s number to book: ", type); scanf("%d", &num);
    int idx = -1;
    for(int i = 0; i < n; i++){ if(v[i].number == num){ idx = i; break; } }
    if(idx == -1){ printf("\n*** Invalid number. ***\n"); return; }

    printf("Enter seats to book: "); scanf("%d", &seats);
    if(seats > v[idx].availableSeats){ 
        printf("\n*** Only %d seats available. ***\n", v[idx].availableSeats); 
        return; 
    }

    v[idx].availableSeats -= seats;
    printf("\n*** %s ticket booked successfully! ***\n", type);
}

void cancelTicket(struct Vehicle v[], int n, const char *type) {
    displayAllVehicles(v, n, type);
    int num, seats;
    printf("Enter %s number to cancel: ", type); scanf("%d", &num);
    int idx = -1;
    for(int i = 0; i < n; i++){ if(v[i].number == num){ idx = i; break; } }
    if(idx == -1){ printf("\n*** Invalid number. ***\n"); return; }

    printf("Enter seats to cancel: "); scanf("%d", &seats);
    int bookedSeats = v[idx].totalSeats - v[idx].availableSeats;
    if(seats > bookedSeats){ 
        printf("\n*** Cannot cancel more than booked (%d seats). ***\n", bookedSeats); 
        return; 
    }

    v[idx].availableSeats += seats;
    printf("\n*** %s ticket cancelled successfully! ***\n", type);
}
