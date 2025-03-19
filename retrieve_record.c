#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100

// Structure to store candidate details
typedef struct Candidate {
    char name[50];
    char symbol;
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

    printf("Searching for candidate: %s\n", searchKey);

    // Read the file line by line
    while (fgets(line, sizeof(line), file)) {
        Candidate candidate;
        // Parse the line to extract candidate details
        if (sscanf(line, "Candidate: %49[^,], Symbol: %c, Votes: %d",
                   candidate.name, &candidate.symbol, &candidate.votes) == 3) {
            // Check if the candidate's name matches the search key
            if (strcmp(candidate.name, searchKey) == 0) {
                printf("Record found:\n");
                printf("Name: %s\n", candidate.name);
                printf("Symbol: %c\n", candidate.symbol);
                printf("Votes: %d\n", candidate.votes);
                found = 1;
                break; // Exit the loop once the record is found
            }
        }
    }

    if (!found) {
        printf("No record found for candidate: %s\n", searchKey);
    }

    fclose(file);
}

int main() {
    const char *filename = "voting_results.txt";
    char searchKey[50];

    // Ask the user for the candidate's name to search
    printf("Enter the candidate's name to retrieve their record: ");
    if (fgets(searchKey, sizeof(searchKey), stdin)) {
        // Remove the newline character from the input
        searchKey[strcspn(searchKey, "\n")] = '\0';
        retrieveCandidateRecord(filename, searchKey);
    } else {
        printf("Error reading input.\n");
    }

    return 0;
}