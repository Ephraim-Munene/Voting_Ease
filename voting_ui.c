#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_C 10  // Maximum number of candidates

typedef struct Candidate {
    char name[50];
    int votes;
    char symbol;
} Candidate;

Candidate allCandidates[MAX_C];
int candidateCount = 0;
char symbols[10] = { '!', '@', '#', '$', '%', '^', '&', '*', '~', '+' };
int symbolTaken[10];

void fillCandidate(int);
void displayMainMenu();
void displayVotingInterface();
void displayResults();
void saveResultsToFile();
void handleFileError(FILE *file, const char *message);
void printHeader(const char *title);
void printFooter();

// Function to set text color (Linux-specific using ANSI escape codes)
void setColor(int color) {
    printf("\033[1;%dm", color);
}

// Function to get a valid integer input
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

// Main function
int main() {
    // Initialize symbol tracking array
    for (int i = 0; i < 10; i++) {
        symbolTaken[i] = 0;
    }

    // Get the number of candidates with validation
    printf("Enter the number of candidates (1-10): ");
    candidateCount = getValidInteger(1, MAX_C);

    // Fill candidate details
    for (int i = 0; i < candidateCount; i++) {
        fillCandidate(i);
    }

    // Main menu loop
    while (1) {
        displayMainMenu();
        int choice = getValidInteger(1, 4);

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
        }
    }

    return 0;
}

// Function to fill candidate details
void fillCandidate(int cNum) {
    printf("Available Symbols: \n");
    for (int j = 0; j < 10; j++) {
        if (symbolTaken[j] == 1)
            continue;
        printf("%d  %c\n", j + 1, symbols[j]);
    }

    int num;
    while (1) {
        printf("\nEnter the symbol number of candidate %d: ", cNum + 1);
        num = getValidInteger(1, 10);

        if (symbolTaken[num - 1] == 0) {
            symbolTaken[num - 1] = 1;
            allCandidates[cNum].symbol = symbols[num - 1];
            break;
        } else {
            printf("This symbol is already taken. Please choose another.\n");
        }
    }

    printf("Enter the name of candidate %d: ", cNum + 1);
    fgets(allCandidates[cNum].name, sizeof(allCandidates[cNum].name), stdin);
    allCandidates[cNum].name[strcspn(allCandidates[cNum].name, "\n")] = 0; // Remove newline character

    allCandidates[cNum].votes = 0;
}

// Function to display the main menu
void displayMainMenu() {
    system("clear"); // Clear the screen (Linux-specific)
    printHeader("MAIN MENU");
    printf("1. Vote for a Candidate\n");
    printf("2. View Results\n");
    printf("3. Save Results to File\n");
    printf("4. Exit\n");
    printFooter();
    printf("Enter your choice: ");
}

// Function to handle voting
void displayVotingInterface() {
    system("clear");
    printHeader("VOTING INTERFACE");

    for (int j = 0; j < candidateCount; j++) {
        printf("%d. %s\t\t(%c)\n", j + 1, allCandidates[j].name, allCandidates[j].symbol);
    }

    printFooter();
    printf("Enter your choice (1-%d): ", candidateCount);
    int choice = getValidInteger(1, candidateCount);

    allCandidates[choice - 1].votes++;
    printf("Thank you for voting!\n");

    printf("Press Enter to continue...");
    while (getchar() != '\n'); // Clear buffer
}

// Function to display results
void displayResults() {
    system("clear");
    printHeader("VOTING RESULTS");

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

    printf("%-20s %-10s %-10s\n", "Candidate", "Symbol", "Votes");
    printf("----------------------------------------\n");
    for (int i = 0; i < candidateCount; i++) {
        printf("%-20s %-10c %-10d\n", allCandidates[i].name, allCandidates[i].symbol, allCandidates[i].votes);
    }

    printFooter();
    printf("Press Enter to continue...");
    while (getchar() != '\n'); // Clear buffer
}

// Function to save results to a file
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

// Function to handle file errors
void handleFileError(FILE *file, const char *message) {
    if (file) {
        fclose(file);
    }
    perror(message);
    exit(1);
}

// Function to print headers
void printHeader(const char *title) {
    setColor(33); // Yellow text
    printf("========================================\n");
    printf("          %s\n", title);
    printf("========================================\n");
    setColor(0); // Reset to default color
}

// Function to print footers
void printFooter() {
    setColor(33); // Yellow text
    printf("========================================\n");
    setColor(0); // Reset to default color
}
