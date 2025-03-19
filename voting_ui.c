#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> // For Windows-specific functions like text coloring
#include <windows.h> // For Windows API functions like GetStdHandle

#define MAX_C 11

typedef struct Candidate {
    char name[50];
    int votes;
    char symbol;
} Candidate;

Candidate allCandidates[MAX_C];
int candidateCount = 0;
char symbols[10] = { '!', '@', '#', '$', '%', '^', '&', '*', '~', '+' };
int symbolTaken[11];

void fillCandidate(int);
void displayMainMenu();
void displayVotingInterface();
void displayResults();
void saveResultsToFile();
void handleFileError(FILE *file, const char *message);
void printHeader(const char *title);
void printFooter();

// Function to set text color (Windows-specific)
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

int main() {
    for (int i = 0; i < 11; i++) {
        symbolTaken[i] = 0;
    }

    printf("Enter the number of candidates: ");
    scanf("%d", &candidateCount);
    if (candidateCount >= MAX_C) {
        printf("Number of candidates cannot be greater than 10.\n Terminating the program\n\n");
        return 0;
    }

    for (int i = 0; i < candidateCount; i++) {
        fillCandidate(i);
    }

    while (1) {
        displayMainMenu();
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayVotingInterface();
                break;
            case 2:
                displayResults();
                break;
            case 3:
                saveResultsToFile();
                printf("Results saved to voting_results.txt\n");
                break;
            case 4:
                printf("Exiting the program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

void fillCandidate(int cNum) {
    printf("Available Symbols: \n");
    for (int j = 0; j < 10; j++) {
        if (symbolTaken[j] == 1)
            continue;
        printf("%d  %c\n", j + 1, symbols[j]);
    }

    int num = 0;

    printf("\nEnter the symbol number of candidate %d: ", cNum + 1);
    scanf("%d", &num);

    if (num <= 0 || num > 10 || symbolTaken[num - 1] == 1) {
        printf("This Symbol is not available. Please choose from the available symbols\n");
        fillCandidate(cNum);
    } else {
        symbolTaken[num - 1] = 1;
        allCandidates[cNum].symbol = symbols[num - 1];
        printf("Enter the name of candidate %d: ", cNum + 1);
        scanf("%s", allCandidates[cNum].name);

        allCandidates[cNum].votes = 0;
    }
}

void displayMainMenu() {
    system("cls"); // Clear the screen (Windows-specific)
    printHeader("MAIN MENU");
    printf("1. Vote for a Candidate\n");
    printf("2. View Results\n");
    printf("3. Save Results to File\n");
    printf("4. Exit\n");
    printFooter();
    printf("Enter your choice: ");
}

void displayVotingInterface() {
    system("cls");
    printHeader("VOTING INTERFACE");

    for (int j = 0; j < candidateCount; j++) {
        printf("%d. %s\t\t(%c)\n", j + 1, allCandidates[j].name, allCandidates[j].symbol);
    }

    printFooter();
    printf("Enter your choice (1-%d): ", candidateCount);
    int choice;
    scanf("%d", &choice);

    if (choice >= 1 && choice <= candidateCount) {
        allCandidates[choice - 1].votes++;
        printf("Thank you for voting!\n");
    } else {
        printf("Invalid choice! Please try again.\n");
    }
    getch(); // Wait for user input before returning to the main menu
}

void displayResults() {
    system("cls");
    printHeader("VOTING RESULTS");

    printf("%-20s %-10s %-10s\n", "Candidate", "Symbol", "Votes");
    printf("----------------------------------------\n");
    for (int i = 0; i < candidateCount; i++) {
        printf("%-20s %-10c %-10d\n", allCandidates[i].name, allCandidates[i].symbol, allCandidates[i].votes);
    }

    printFooter();
    getch(); // Wait for user input before returning to the main menu
}

void saveResultsToFile() {
    FILE *file = fopen("voting_results.txt", "w");
    if (!file) {
        handleFileError(file, "Failed to open file for writing.");
        return;
    }

    fprintf(file, "-----VOTING RESULTS-----\n");
    for (int i = 0; i < candidateCount; i++) {
        fprintf(file, "Candidate: %s, Symbol: %c, Votes: %d\n",
                allCandidates[i].name, allCandidates[i].symbol, allCandidates[i].votes);
    }

    fclose(file);
}

void handleFileError(FILE *file, const char *message) {
    if (file) {
        fclose(file);
    }
    perror(message);
    exit(1);
}

void printHeader(const char *title) {
    setColor(14); // Yellow text
    printf("========================================\n");
    printf("          %s\n", title);
    printf("========================================\n");
    setColor(7); // Reset to default color
}

void printFooter() {
    setColor(14); // Yellow text
    printf("========================================\n");
    setColor(7); // Reset to default color
}