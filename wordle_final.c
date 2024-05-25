#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define WORDL 5
#define MAXW 1000
#define MAXG 6
#define GREEN "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET "\e[0;39m"
//colour codes taken from a github repository.

typedef struct scorenode
{
    int score;
    struct scorenode * next;
}skore; //linked list to maintain score.

skore* scorelist = NULL;

void addscore(int score) 
{
    skore* newnode = (skore*)malloc(sizeof(skore));
    newnode->score = score;
    newnode->next = scorelist;
    scorelist = newnode;
}
void displayscores()
{
    if (scorelist == NULL) 
    {
        printf("\nNo scores to display.\n");
        return;
    }
    printf("\nPrevious Score: \n");
    skore* current = scorelist;
    int z=1;
    while (current != NULL) 
    {
        printf("Score %d : %d \n",z, current->score);
        current = current->next;
        z++;
    }
}
int load(char words[], int *word_count) 
{
    FILE *fp = fopen("word.txt", "r");
    if (fp == NULL) 
    {
        return -1;
    }
    *word_count = 0;
    char buffer[WORDL + 1];
    while (fscanf(fp, "%s", buffer) != EOF) 
    {
        if (strlen(buffer) == WORDL) 
        {
            strcpy(words + (*word_count * (WORDL + 1)), buffer);
            (*word_count)++;
        }
    }
    fclose(fp);
    return 0;
}
int select(char words[], int word_count, char *answer) 
{
    if (word_count <= 0) 
    {
        return -1;
    }
    srand(time(NULL));
    int random_index = rand() % word_count;
    strcpy(answer, words + random_index * (WORDL + 1));
    return 0;
}
void display_guess(const char *guess, const char *answer, char *result) 
{
    int charcount[26] = {0};
    int charcount1[26] = {0};
    for (int i = 0; i < WORDL; i++) 
    {
        charcount[(answer[i]) - 'a']++;
    }
    for (int i = 0; i < WORDL; i++) 
    {
        if (guess[i] == answer[i]) 
        {
            result[i] = answer[i];
            printf(GREEN "%c" RESET, answer[i]);
            charcount[(answer[i]) - 'a']--;
        } else 
        {
            result[i] = '-';
            printf(RED "%c" RESET, guess[i]);
        }
    }
    for (int i = 0; i < WORDL; i++) 
    {
        if (result[i] == '-') 
        {
            charcount1[(guess[i]) - 'a']++;
            if (charcount1[(guess[i]) - 'a'] <= charcount[(guess[i]) - 'a']) 
            {
                result[i] = '*';
                printf(YELLOW "%c" RESET, guess[i]);
            }
        }
    }

    result[WORDL] = '\0';
    printf("\n");
}

int main() 
{
    char words[MAXW * (WORDL + 1)];
    int word_count = 0;
    char answer[WORDL + 1];
    char guess[WORDL + 1];
    char result[WORDL + 1];
    if (load(words, &word_count) != 0) 
    {
        printf("\nError: Could not load words from file.\n");
        return 1;
    }

    int choice;
    do {
        printf("\nWelcome to Wordle!\n");
        printf("1. Play Game\n");
        printf("2. View Scores\n");
        printf("3. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) 
        {
        case 1:
        printf("\nPlease use LOWERCASE for the letters entered.\n\n");
        if (select(words, word_count, answer) != 0) 
        {
        printf("Error: Could not select a word.\n");
        break;
        }
        printf("You have %d guesses to find the 5-letter word.\n", MAXG);
        int tries = 0;
        for (int i = 0; i < MAXG; i++) 
        {
        printf("Guess %d: ", i + 1);
        scanf("%s", guess);
        if (strlen(guess) != WORDL) 
        {
        printf("Invalid input. Please enter a 5-letter word.\n");
        i--; 
        continue;
        }
        display_guess(guess, answer, result);
        tries++;
        if (strcmp(guess, answer) == 0)
        {
        printf("Congratulations! You guessed the word in %d tries.\n", tries);
        addscore(tries); 
        break;
        }
        }
        if (strcmp(guess, answer) != 0) 
        {
        printf("You ran out of guesses. The word was: %s\n", answer);
        }
        break;

        case 2:
        displayscores(); 
        break;

        case 3:
        printf("Exiting Wordle...\n");
        break;

        default:
        printf("Invalid choice. Please try again.\n");
        break;
        }
    } 
    while(choice != 3);
    skore* current = scorelist;
    while (current != NULL) 
    {
        skore* temp = current;
        current = current->next;
        free(temp);
    }
    return 0;
}