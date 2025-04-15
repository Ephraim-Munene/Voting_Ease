#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100

// Structure to store candidate details
typedef struct Candidate {
    char name[50];
    char party[50];
    int votes;
} Candidate;

// Function to retrieve a candidate's record from the file
void retrieveCandidateRecord(const char *filename, const char *searchKey) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;

    printf("\nSearching for candidate: %s...\n", searchKey);

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        Candidate candidate;

        // Parse the line (assuming format like: "Candidate: Name, Party: PartyName, Votes: 123")
        if (sscanf(line, "Candidate: %49[^,], Party: %49[^,], Votes: %d",
                   candidate.name, candidate.party, &candidate.votes) == 3) {

            // Check if the candidate's name matches the search key
            if (strcmp(candidate.name, searchKey) == 0) {
                printf("\n📋 Record found:\n");
                printf("Name  : %s\n", candidate.name);
                printf("Party : %s\n", candidate.party);
                printf("Votes : %d\n", candidate.votes);
                found = 1;
                break; // Exit the loop once the record is found
            }
        }
    }

    if (!found) {
        printf("\n⚠️ No record found for candidate: %s\n", searchKey);
    }

    fclose(file);
}

void adminMenu() {
    const char *filename = "voting_results.txt";
    char searchKey[50];

    while (1) {
        printf("\n===== RETRIEVE CANDIDATE RECORD =====\n");
        printf("Enter the candidate's name (or 'exit' to return): ");
        if (fgets(searchKey, sizeof(searchKey), stdin)) {
            // Remove the newline character from the input
            searchKey[strcspn(searchKey, "\n")] = '\0';

            // Check if the admin wants to exit
            if (strcmp(searchKey, "exit") == 0) {
                printf("Returning to the main menu...\n");
                break;
            }

            // Retrieve candidate record
            retrieveCandidateRecord(filename, searchKey);
        } else {
            printf("Error reading input.\n");
        }
    }
}

int main() {
    // Main menu for the program (simplified for demonstration)
    printf("Welcome to the Voting System\n");
    adminMenu();  // Admin menu to retrieve records
    return 0;
}
