// vote_ease.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <termios.h> // For hiding password input

#define MAX_C 10

typedef struct Candidate {
    char name[50];
    int votes;
    char party[50];
} Candidate;

Candidate allCandidates[MAX_C];
int candidateCount = 0;
char electionType[100] = "No Election Type Set";
const char *adminPassword = "123";

void fillCandidate(int);
void displayWelcomeScreen();
void displayMainMenu();
void displayAdminPanel();
void displayVoterMenu();
void displayPostVoteMenu();
void displayVotingInterface();
void displayResults();
void saveResultsToFile();
void saveCandidatesToFile();
void handleFileError(FILE *, const char *);
void printHeader(const char *, const char *);
void printFooter(const char *);
void setColor(int);
int getValidInteger(int, int);
void clearInputBuffer();
int isAlpha(const char *);
void resetCandidates();
void getPasswordMasked(char *);

int main() {
    // Candidates will reset every time the program is launched
    resetCandidates();
    displayWelcomeScreen();

    while (1) {
        displayMainMenu();
        int choice = getValidInteger(1, 3);

        switch (choice) {
            case 1:
                displayVoterMenu();
                break;
            case 2:
                displayAdminPanel();
                break;
            case 3:
                printf("Exiting the program. Goodbye!\n");
                exit(0);
        }
    }
    return 0;
}

void resetCandidates() {
    candidateCount = 0;
    memset(allCandidates, 0, sizeof(allCandidates));
    strcpy(electionType, "No Election Type Set");
}

void displayWelcomeScreen() {
    system("clear");
    printHeader("WELCOME TO VOTE EASE!", "blue");
    printf("Press Enter to proceed...\n");
    getchar();
}

void displayMainMenu() {
    system("clear");
    printHeader("MAIN MENU", "blue");
    printf("1. Voter\n");
    printf("2. Admin\n");
    printf("3. Exit\n");
    printFooter("red");
    printf("Enter your choice: ");
}

void displayAdminPanel() {
    system("clear");
    printHeader("ADMIN LOGIN", "blue");

    char inputPassword[20];
    printf("Enter password: ");
    getPasswordMasked(inputPassword);

    if (strcmp(inputPassword, adminPassword) == 0) {
        printf("\nAccess granted.\n");
        sleep(1);

        while (1) {
            system("clear");
            printHeader("ADMIN MENU", "blue");
            printf("1. Add Candidates\n");
            printf("2. View Results\n");
            printf("3. Save Results\n");
            printf("4. Exit to Main Menu\n");
            printFooter("blue");
            printf("Enter your choice: ");
            int adminChoice = getValidInteger(1, 4);

            switch (adminChoice) {
                case 1: {
                    printf("Enter election type: ");
                    fgets(electionType, sizeof(electionType), stdin);
                    electionType[strcspn(electionType, "\n")] = 0;

                    printf("Enter number of candidates (1-%d): ", MAX_C);
                    candidateCount = getValidInteger(1, MAX_C);
                    for (int i = 0; i < candidateCount; i++) {
                        fillCandidate(i);
                    }
                    saveCandidatesToFile();
                    printf("Candidate details saved!\n");
                    sleep(2);
                    break;
                }
                case 2:
                    displayResults();
                    break;
                case 3:
                    saveResultsToFile();
                    sleep(2);
                    break;
                case 4:
                    return;
            }
        }
    } else {
        printf("\nIncorrect password! Returning to main menu...\n");
        sleep(2);
    }
}

void fillCandidate(int cNum) {
    printf("Enter the party for candidate %d (letters only): ", cNum + 1);
    char party[50];
    while (1) {
        fgets(party, sizeof(party), stdin);
        party[strcspn(party, "\n")] = 0;
        if (isAlpha(party)) {
            strcpy(allCandidates[cNum].party, party);
            break;
        } else {
            printf("Invalid input! Only letters allowed: ");
        }
    }

    printf("Enter the name of candidate %d: ", cNum + 1);
    fgets(allCandidates[cNum].name, sizeof(allCandidates[cNum].name), stdin);
    allCandidates[cNum].name[strcspn(allCandidates[cNum].name, "\n")] = 0;
    allCandidates[cNum].votes = 0;
}

void displayVoterMenu() {
    system("clear");
    printHeader("VOTER MENU", "yellow");

    if (candidateCount == 0) {
        printf("No active candidates found. Voting is currently unavailable.\n");
        printFooter("yellow");
        printf("Press Enter to return to the main menu...");
        getchar();
        return;
    }

    printf("Election Type: %s\n\n", electionType);
    for (int j = 0; j < candidateCount; j++) {
        printf("%d. %s (%s)\n", j + 1, allCandidates[j].name, allCandidates[j].party);
    }

    printFooter("yellow");
    printf("Enter your vote (1-%d): ", candidateCount);
    int choice = getValidInteger(1, candidateCount);

    allCandidates[choice - 1].votes++;
    saveCandidatesToFile();
    saveResultsToFile(); // Automatically save results after voting
    printf("Thank you for voting!\n");
    sleep(2);
}

void displayResults() {
    system("clear");
    printHeader("VOTING RESULTS", "yellow");

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
        printf("%-20s %-20s %-10d\n", allCandidates[i].name, allCandidates[i].party, allCandidates[i].votes);
    }

    printFooter("yellow");
    printf("Press Enter to continue...");
    getchar();
}

void saveResultsToFile() {
    FILE *file = fopen("voting_results.txt", "w");
    if (!file) {
        handleFileError(file, "Error saving results.");
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

void saveCandidatesToFile() {
    // Saving to a dummy file but will not be loaded on restart
    FILE *file = fopen("candidates.dat", "wb");
    if (!file) {
        handleFileError(file, "Could not save candidates.");
        return;
    }
    fwrite(&candidateCount, sizeof(int), 1, file);
    fwrite(allCandidates, sizeof(Candidate), candidateCount, file);
    fclose(file);
}

void handleFileError(FILE *file, const char *message) {
    if (file) fclose(file);
    perror(message);
    exit(1);
}

void printHeader(const char *title, const char *color) {
    if (strcmp(color, "blue") == 0) setColor(34);
    else if (strcmp(color, "yellow") == 0) setColor(33);
    else if (strcmp(color, "red") == 0) setColor(31);
    printf("========================================\n");
    printf(" %s\n", title);
    printf("========================================\n");
    setColor(0);
}

void printFooter(const char *color) {
    if (strcmp(color, "red") == 0) setColor(31);
    else if (strcmp(color, "blue") == 0) setColor(34);
    else if (strcmp(color, "yellow") == 0) setColor(33);
    printf("========================================\n");
    setColor(0);
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
    while (getchar() != '\n');
}

int isAlpha(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i])) return 0;
    }
    return 1;
}

void getPasswordMasked(char *password) {
    struct termios oldt, newt;
    int i = 0;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while ((ch = getchar()) != '\n' && i < 19) {
        if (ch == 127 || ch == 8) { // Handle backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
