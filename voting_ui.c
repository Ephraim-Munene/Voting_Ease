
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD
#include <termios.h>
#include <unistd.h>  // For sleep function
#include <ctype.h>   // For isalpha function


#define MAX_C 10  // Maximum number of candidates
#define MAX_PASSWORD_LENGTH 50  // Max length of password

typedef struct Candidate {
    char name[50];
    int votes;
    char party[50];  // Now stores the party name instead of symbol
=======
#include <ctype.h> // For checking if characters are alphabetic
#include <unistd.h> // For sleep function

#define MAX_C 10 // Maximum number of candidates

typedef struct Candidate {
char name[50];
int votes;
char party[50]; // Now stores the party name instead of symbol
>>>>>>> 9fd968dd6e7e484e2465b9f23743f202e9f4322b
} Candidate;

Candidate allCandidates[MAX_C];
int candidateCount = 0;
<<<<<<< HEAD
char electionType[100] = "No Election Type Set";  // Default election type
=======
char electionType[100] = "No Election Type Set"; // Default election type
>>>>>>> 9fd968dd6e7e484e2465b9f23743f202e9f4322b

// Admin Password
const char *adminPassword = "123";

// Function Declarations
void fillCandidate(int);
void displayWelcomeScreen();
void displayMainMenu();
<<<<<<< HEAD
void displayAdminMenu();
void displayVoterMenu();
void displayPostVoteMenu();  // Function prototype added here
=======
void displaySecondMenu();
void displayAdminMenu();
void displayVoterMenu();
void displayPostVoteMenu(); // Function prototype added here
>>>>>>> 9fd968dd6e7e484e2465b9f23743f202e9f4322b
void setPassword();
void displayVotingInterface();
void displayResults();
void saveResultsToFile();
void handleFileError(FILE *file, const char *message);
void printHeader(const char *title, const char *color);
void printFooter(const char *color);
void setColor(int color);
int getValidInteger(int min, int max);
void showProgressBar(int progress, int total);
void loadingScreen(const char* action);
void displayNoCandidatesMessage();
void clearInputBuffer();
void adminSetupComplete();
<<<<<<< HEAD
int isAlpha(const char *str);  // Helper function to check if input contains only letters
void getPasswordWithAsterisks(char *password, int maxLength); // New function for password masking

int main() {
    // Welcome screen and first interaction
    displayWelcomeScreen();

    // Main menu loop
    while (1) {
        displayMainMenu();
        int choice = getValidInteger(1, 3);

        switch (choice) {
            case 1:  // Voter
                displayVoterMenu();
                break;
            case 2:  // Admin
                displayAdminMenu();
                break;
            case 3:  // Exit
                printf("Exiting the program. Goodbye!\n");
                exit(0);
        }
    }
    return 0;
}

void displayWelcomeScreen() {
    system("clear");  // Clear the screen (Linux-specific)
    printHeader("WELCOME TO VOTE EASE!", "blue");
    printf("Press Enter to proceed...\n");
    getchar();  // Wait for the user to press Enter
}

void displayMainMenu() {
    system("clear");  // Clear the screen (Linux-specific)
    printHeader("MAIN MENU", "blue");
    printf("1. Voter\n");
    printf("2. Admin\n");
    printf("3. Exit\n");
    printFooter("red");
    printf("Enter your choice: ");
}

void displayAdminMenu() {
    system("clear");
    printHeader("ADMIN LOGIN", "blue");

    char inputPassword[MAX_PASSWORD_LENGTH];
    printf("Enter password: ");
    getPasswordWithAsterisks(inputPassword, MAX_PASSWORD_LENGTH);  // Password input with masking

    if (strcmp(inputPassword, adminPassword) == 0) {
        printf("Password correct! Access granted.\n");
        getchar(); // Clear the newline character from input buffer
        // Prompt for election type
        printf("Enter the election type (e.g., 'Delegates election'): ");
        fgets(electionType, sizeof(electionType), stdin);
        electionType[strcspn(electionType, "\n")] = 0;  // Remove newline character
        printf("Election type set to: %s\n", electionType);

        // Admin can now enter candidate details
        while (1) {
            printf("Enter number of candidates (1-10): ");
            candidateCount = getValidInteger(1, MAX_C);
            for (int i = 0; i < candidateCount; i++) {
                fillCandidate(i);
            }
            printf("Candidate details have been set.\n");
            adminSetupComplete();  // Return to second menu after admin setup
            return;
        }
    } else {
        printf("Incorrect password! Returning to second menu...\n");
        sleep(2);  // Pause to show the message before returning
        return;
    }
}

void adminSetupComplete() {
    printf("Admin setup complete. Returning to the main menu...\n");
    sleep(2);  // Pause to show the message before returning
}

void fillCandidate(int cNum) {
    printf("Enter the party for candidate %d (letters only): ", cNum + 1);
    char party[50];
    while (1) {
        fgets(party, sizeof(party), stdin);
        party[strcspn(party, "\n")] = 0;  // Remove newline character
        if (isAlpha(party)) {
            strcpy(allCandidates[cNum].party, party);
            break;
        } else {
            printf("Invalid party name! Please enter alphabetic characters only: ");
        }
    }

    printf("Enter the name of candidate %d: ", cNum + 1);
    fgets(allCandidates[cNum].name, sizeof(allCandidates[cNum].name), stdin);
    // Remove the newline character properly
    allCandidates[cNum].name[strcspn(allCandidates[cNum].name, "\n")] = 0;
    
    allCandidates[cNum].votes = 0;
}

void displayVoterMenu() {
    system("clear");
    printHeader("VOTER MENU", "yellow");

    if (candidateCount == 0) {
        displayNoCandidatesMessage();
        return;
    }

    printf("Election Type: %s\n\n", electionType);

    for (int j = 0; j < candidateCount; j++) {
        // Ensure the candidate's name is printed correctly
        printf("%d. %s (%s)\n", j + 1, allCandidates[j].name, allCandidates[j].party);
    }

    printFooter("yellow");
    printf("Enter your choice (1-%d): ", candidateCount);
    int choice = getValidInteger(1, candidateCount);

    // Voting
    allCandidates[choice - 1].votes++;
    printf("Thank you for voting!\n");

    // Display options after voting
    while (1) {
        displayPostVoteMenu();
        int resultChoice = getValidInteger(1, 3);
        switch (resultChoice) {
            case 1:
                displayResults();
                break;
            case 2:
                saveResultsToFile();
                break;
            case 3:
                return;
        }
    }
}

void displayPostVoteMenu() {
    system("clear");
    printHeader("POST VOTE MENU", "yellow");
    printf("1. View Results\n");
    printf("2. Save Results\n");
    printf("3. Exit\n");
    printFooter("yellow");
    printf("Enter your choice: ");
}

void displayResults() {
    system("clear");
    printHeader("VOTING RESULTS", "yellow");

    // Sort candidates by votes (Descending)
    for (int i = 0; i < candidateCount - 1; i++) {
        for (int j = 0; j < candidateCount - i - 1; j++) {
            if (allCandidates[j].votes < allCandidates[j + 1].votes) {
                Candidate temp = allCandidates[j];
                allCandidates[j] = allCandidates[j + 1];
                allCandidates[j + 1] = temp;
            }
        }
    }

    printf("%-20s %-20s %-10s\n", "Candidate", "Party", "Votes");
    printf("---------------------------------------------\n");
    for (int i = 0; i < candidateCount; i++) {
        // Ensure the candidate name displays correctly
        printf("%-20s %-20s %-10d\n", allCandidates[i].name, allCandidates[i].party, allCandidates[i].votes);
    }
    printFooter("yellow");
    printf("Press Enter to continue...");
    while (getchar() != '\n');  // Clear buffer
}

void saveResultsToFile() {
    FILE *file = fopen("voting_results.txt", "w");
    if (!file) {
        handleFileError(file, "Failed to open file for writing.");
        return;
    }

    fprintf(file, "-----VOTING RESULTS-----\n");
    for (int i = 0; i < candidateCount; i++) {
        fprintf(file, "Candidate: %s, Party: %s, Votes: %d\n",
                allCandidates[i].name, allCandidates[i].party, allCandidates[i].votes);
    }

    fclose(file);
    printf("Results saved to voting_results.txt\n");
}

void handleFileError(FILE *file, const char *message) {
    if (file) {
        fclose(file);
    }
    perror(message);
    exit(1);
}

void displayNoCandidatesMessage() {
    system("clear");
    printHeader("VOTER MENU", "yellow");
    printf("There are no current active candidates.\n");
    printFooter("yellow");
    printf("Press Enter to return to the main menu...");
    getchar();  // Wait for the user to press Enter
}

void printHeader(const char *title, const char *color) {
    if (strcmp(color, "blue") == 0) {
        setColor(34);  // Blue text
    } else if (strcmp(color, "yellow") == 0) {
        setColor(33);  // Yellow text
    } else if (strcmp(color, "red") == 0) {
        setColor(31);  // Red text
    }
    printf("========================================\n");
    printf("          %s\n", title);
    printf("========================================\n");
    setColor(0);  // Reset to default color
}

void printFooter(const char *color) {
    if (strcmp(color, "red") == 0) {
        setColor(31);  // Red text
    }
    printf("========================================\n");
    setColor(0);  // Reset to default color
}

void setColor(int color) {
    printf("\033[1;%dm", color);
}

int getValidInteger(int min, int max) {
    int num;
    char input[100];

    while (1) {
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &num) == 1 && num >= min && num <= max) {
            return num;
        }
        printf("Invalid input! Please enter a number between %d and %d: ", min, max);
    }
}

void clearInputBuffer() {
    while (getchar() != '\n');  // Clear the input buffer
}

int isAlpha(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i])) {
            return 0;  // Non-alphabetic character found
        }
    }
    return 1;  // All characters are alphabetic
}

void getPasswordWithAsterisks(char *password, int maxLength) {
    struct termios oldt, newt;
    int ch, i = 0;

    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;  // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read the password
    while ((ch = getchar()) != '\n' && ch != EOF && i < maxLength - 1) {
        if (ch == 127) {  // Backspace handling
            if (i > 0) {
                i--;
                printf("\b \b");  // Move cursor back, overwrite with space, and move cursor back again
            }
        } else {
            password[i++] = ch;
            printf("*");  // Print asterisk for each character
        }
    }
    password[i] = '\0';  // Null-terminate the string

    // Restore the terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
=======
int isAlpha(const char *str); // Helper function to check if input contains only letters

int main() {
// Welcome screen and first interaction
displayWelcomeScreen();

// Main menu loop
while (1) {
displayMainMenu();
int choice = getValidInteger(1, 3);

switch (choice) {
case 1: // Voter
displayVoterMenu();
break;
case 2: // Admin
displayAdminMenu();
break;
case 3: // Exit
printf("Exiting the program. Goodbye!\n");
exit(0);
}
}
return 0;
}

void displayWelcomeScreen() {
system("clear"); // Clear the screen (Linux-specific)
printHeader("WELCOME TO VOTE EASE!", "blue");
printf("Press Enter to proceed...\n");
getchar(); // Wait for the user to press Enter
}

void displayMainMenu() {
system("clear"); // Clear the screen (Linux-specific)
printHeader("MAIN MENU", "blue");
printf("1. Voter\n");
printf("2. Admin\n");
printf("3. Exit\n");
printFooter("red");
printf("Enter your choice: ");
}

void displayAdminMenu() {
system("clear");
printHeader("ADMIN LOGIN", "blue");

char inputPassword[20];
printf("Enter password: ");
scanf("%s", inputPassword);
clearInputBuffer(); // Clear the input buffer after reading password

if (strcmp(inputPassword, adminPassword) == 0) {
printf("Password correct! Access granted.\n");
getchar(); // Clear the newline character from input buffer
// Prompt for election type
printf("Enter the election type (e.g., 'Delegates election'): ");
fgets(electionType, sizeof(electionType), stdin);
electionType[strcspn(electionType, "\n")] = 0; // Remove newline character
printf("Election type set to: %s\n", electionType);

// Admin can now enter candidate details
while (1) {
printf("Enter number of candidates (1-10): ");
candidateCount = getValidInteger(1, MAX_C);
for (int i = 0; i < candidateCount; i++) {
fillCandidate(i);
}
printf("Candidate details have been set.\n");
adminSetupComplete(); // Return to second menu after admin setup
return;
}
} else {
printf("Incorrect password! Returning to second menu...\n");
sleep(2); // Pause to show the message before returning
return;
}
}

void adminSetupComplete() {
printf("Admin setup complete. Returning to the main menu...\n");
sleep(2); // Pause to show the message before returning
}

void fillCandidate(int cNum) {
printf("Enter the party for candidate %d (letters only): ", cNum + 1);
char party[50];
while (1) {
fgets(party, sizeof(party), stdin);
party[strcspn(party, "\n")] = 0; // Remove newline character
if (isAlpha(party)) {
strcpy(allCandidates[cNum].party, party);
break;
} else {
printf("Invalid party name! Please enter alphabetic characters only: ");
}
}

printf("Enter the name of candidate %d: ", cNum + 1);
fgets(allCandidates[cNum].name, sizeof(allCandidates[cNum].name), stdin);
// Remove the newline character properly
allCandidates[cNum].name[strcspn(allCandidates[cNum].name, "\n")] = 0;

 
allCandidates[cNum].votes = 0;
}

void displayVoterMenu() {
system("clear");
printHeader("VOTER MENU", "yellow");

if (candidateCount == 0) {
displayNoCandidatesMessage();
return;
}

printf("Election Type: %s\n\n", electionType);

for (int j = 0; j < candidateCount; j++) {
// Ensure the candidate's name is printed correctly
printf("%d. %s (%s)\n", j + 1, allCandidates[j].name, allCandidates[j].party);
}

printFooter("yellow");
printf("Enter your choice (1-%d): ", candidateCount);
int choice = getValidInteger(1, candidateCount);

// Voting
allCandidates[choice - 1].votes++;
printf("Thank you for voting!\n");

// Display options after voting
while (1) {
displayPostVoteMenu();
int resultChoice = getValidInteger(1, 3);
switch (resultChoice) {
case 1:
displayResults();
break;
case 2:
saveResultsToFile();
break;
case 3:
return;
}
}
}

void displayPostVoteMenu() {
system("clear");
printHeader("POST VOTE MENU", "yellow");
printf("1. View Results\n");
printf("2. Save Results\n");
printf("3. Exit\n");
printFooter("yellow");
printf("Enter your choice: ");
}

void displayResults() {
system("clear");
printHeader("VOTING RESULTS", "yellow");

// Sort candidates by votes (Descending)
for (int i = 0; i < candidateCount - 1; i++) {
for (int j = 0; j < candidateCount - i - 1; j++) {
if (allCandidates[j].votes < allCandidates[j + 1].votes) {
Candidate temp = allCandidates[j];
allCandidates[j] = allCandidates[j + 1];
allCandidates[j + 1] = temp;
}
}
}

printf("%-20s %-20s %-10s\n", "Candidate", "Party", "Votes");
printf("---------------------------------------------\n");
for (int i = 0; i < candidateCount; i++) {
// Ensure the candidate name displays correctly
printf("%-20s %-20s %-10d\n", allCandidates[i].name, allCandidates[i].party, allCandidates[i].votes);
}
printFooter("yellow");
printf("Press Enter to continue...");
while (getchar() != '\n'); // Clear buffer
}

void saveResultsToFile() {
FILE *file = fopen("voting_results.txt", "w");
if (!file) {
handleFileError(file, "Failed to open file for writing.");
return;
}

fprintf(file, "-----VOTING RESULTS-----\n");
for (int i = 0; i < candidateCount; i++) {
fprintf(file, "Candidate: %s, Party: %s, Votes: %d\n",
allCandidates[i].name, allCandidates[i].party, allCandidates[i].votes);
}

fclose(file);
printf("Results saved to voting_results.txt\n");
}

void handleFileError(FILE *file, const char *message) {
if (file) {
fclose(file);
}
perror(message);
exit(1);
}

void displayNoCandidatesMessage() {
system("clear");
printHeader("VOTER MENU", "yellow");
printf("There are no current active candidates.\n");
printFooter("yellow");
printf("Press Enter to return to the main menu...");
getchar(); // Wait for the user to press Enter
}

void printHeader(const char *title, const char *color) {
if (strcmp(color, "blue") == 0) {
setColor(34); // Blue text
} else if (strcmp(color, "yellow") == 0) {
setColor(33); // Yellow text
} else if (strcmp(color, "red") == 0) {
setColor(31); // Red text
}
printf("========================================\n");
printf(" %s\n", title);
printf("========================================\n");
setColor(0); // Reset to default color
}

void printFooter(const char *color) {
if (strcmp(color, "red") == 0) {
setColor(31); // Red text
}
printf("========================================\n");
setColor(0); // Reset to default color
}

void setColor(int color) {
printf("\033[1;%dm", color);
}

int getValidInteger(int min, int max) {
int num;
char input[100];

while (1) {
fgets(input, sizeof(input), stdin);
if (sscanf(input, "%d", &num) == 1 && num >= min && num <= max) {
return num;
}
printf("Invalid input! Please enter a number between %d and %d: ", min, max);
}
}

void clearInputBuffer() {
while (getchar() != '\n'); // Clear the input buffer
}

int isAlpha(const char *str) {
for (int i = 0; str[i]; i++) {
if (!isalpha(str[i])) {
return 0; // Non-alphabetic character found
}
}
return 1; // All characters are alphabetic
>>>>>>> 9fd968dd6e7e484e2465b9f23743f202e9f4322b
}
