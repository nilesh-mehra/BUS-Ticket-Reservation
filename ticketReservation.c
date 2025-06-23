#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Bus {
    int busNumber;
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

void displayMainMenu() {
    printf("\n=== Main Menu ===\n");
    printf("1. Login\n2. Exit\nEnter your choice: ");
}

void displayUserMenu() {
    printf("\n=== User Menu ===\n");
    printf("1. Book Ticket\n2. Cancel Ticket\n3. Check Bus Status\n4. Logout\nEnter your choice: ");
}

int loginUser(struct User users[], int n, char uname[], char pass[]) {
    for (int i = 0; i < n; i++) {
        if (strcmp(users[i].username, uname) == 0 && strcmp(users[i].password, pass) == 0)
            return i;
    }
    return -1;
}

void displayAllBuses(struct Bus buses[], int n) {
    printf("\n---------------------------------------------------------------------------\n");
    printf("Bus No | From          | To            | Fare   | Total | Available\n");
    printf("---------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%6d | %-13s | %-13s | %-6.2f | %5d | %9d\n", 
               buses[i].busNumber, buses[i].source, buses[i].destination, 
               buses[i].fare, buses[i].totalSeats, buses[i].availableSeats);
    }
    printf("---------------------------------------------------------------------------\n");
}

void saveBusesToFile(struct Bus buses[], int n) {
    FILE *fp = fopen("buses.txt", "wb");
    fwrite(buses, sizeof(struct Bus), n, fp);
    fclose(fp);
}

void loadBusesFromFile(struct Bus buses[], int n) {
    FILE *fp = fopen("buses.txt", "rb");
    if (fp != NULL) {
        fread(buses, sizeof(struct Bus), n, fp);
        fclose(fp);
    }
}

void bookTicket(struct Bus buses[], int n, const char *username) {
    displayAllBuses(buses, n);
    int busNumber, seats;
    printf("Enter Bus Number to book: ");
    scanf("%d", &busNumber);

    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (buses[i].busNumber == busNumber) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Invalid bus number.\n");
        return;
    }

    printf("Enter seats to book: ");
    scanf("%d", &seats);

    if (seats > buses[idx].availableSeats) {
        printf("Only %d seats available.\n", buses[idx].availableSeats);
        return;
    }

    buses[idx].availableSeats -= seats;
    saveBusesToFile(buses, n);

    char filename[100];
    sprintf(filename, "%s_bookings.txt", username);
    FILE *fp = fopen(filename, "a");
    fprintf(fp, "Booked %d seats in Bus %d from %s to %s\n", seats, busNumber, buses[idx].source, buses[idx].destination);
    fclose(fp);

    printf("Ticket booked successfully!\n");
}

void cancelTicket(struct Bus buses[], int n, const char *username) {
    displayAllBuses(buses, n);
    int busNumber, seats;
    printf("Enter Bus Number to cancel: ");
    scanf("%d", &busNumber);

    int idx = -1;
    for (int i = 0; i < n; i++) {
        if (buses[i].busNumber == busNumber) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Invalid bus number.\n");
        return;
    }

    printf("Enter number of seats to cancel: ");
    scanf("%d", &seats);

    int booked = buses[idx].totalSeats - buses[idx].availableSeats;
    if (seats > booked) {
        printf("Cannot cancel more than booked seats (%d).\n", booked);
        return;
    }

    buses[idx].availableSeats += seats;
    saveBusesToFile(buses, n);

    char filename[100];
    sprintf(filename, "%s_bookings.txt", username);
    FILE *fp = fopen(filename, "a");
    fprintf(fp, "Cancelled %d seats in Bus %d from %s to %s\n", seats, busNumber, buses[idx].source, buses[idx].destination);
    fclose(fp);

    printf("Ticket cancelled successfully!\n");
}

int main() {
    struct User users[4] = {
        {"user1", "one"}, {"user2", "two"},
        {"user3", "three"}, {"user4", "four"}
    };
    int numUsers = 4;

    struct Bus buses[16] = {
        {1, "Dehradun", "Haridwar", 50, 50, 120.0},
        {2, "Delhi", "Agra", 45, 45, 200.0},
        {3, "Mumbai", "Pune", 40, 40, 150.0},
        {4, "Bangalore", "Mysore", 35, 35, 120.0},
        {5, "Kolkata", "Siliguri", 50, 50, 350.0},
        {6, "Chennai", "Pondicherry", 40, 40, 180.0},
        {7, "Hyderabad", "Warangal", 30, 30, 130.0},
        {8, "Ahmedabad", "Rajkot", 45, 45, 140.0},
        {9, "Jaipur", "Udaipur", 40, 40, 250.0},
        {10, "Lucknow", "Kanpur", 50, 50, 100.0},
        {11, "Patna", "Bodh Gaya", 35, 35, 90.0},
        {12, "Chandigarh", "Shimla", 40, 40, 300.0},
        {13, "Dehradun", "Rishikesh", 40, 40, 90.0},
        {14, "Surat", "Vadodara", 45, 45, 110.0},
        {15, "Varanasi", "Allahabad", 30, 30, 130.0},
        {16, "Clement Town", "Clock Tower", 35, 35, 76.0}
    };
    int numBuses = 16;

    loadBusesFromFile(buses, numBuses); // Load saved state

    int loggedIn = -1;
    char currentUser[50];

    while (1) {
        if (loggedIn == -1) {
            displayMainMenu();
            int ch;
            scanf("%d", &ch);

            if (ch == 1) {
                char uname[50], pass[50];
                printf("Username: "); scanf("%s", uname);
                printf("Password: "); scanf("%s", pass);

                loggedIn = loginUser(users, numUsers, uname, pass);
                if (loggedIn == -1) {
                    printf("Login failed!\n");
                } else {
                    printf("Welcome, %s!\n", uname);
                    strcpy(currentUser, uname);
                }
            } else if (ch == 2) {
                printf("Goodbye!\n");
                break;
            } else {
                printf("Invalid choice!\n");
            }
        } else {
            displayUserMenu();
            int ch;
            scanf("%d", &ch);

            switch (ch) {
                case 1: bookTicket(buses, numBuses, currentUser); break;
                case 2: cancelTicket(buses, numBuses, currentUser); break;
                case 3: displayAllBuses(buses, numBuses); break;
                case 4: loggedIn = -1; printf("Logged out.\n"); break;
                default: printf("Invalid choice!\n");
            }
        }
    }

    return 0;
}
