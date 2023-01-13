#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int COLUMNS_LENGTH = 6;

void use_file(char* mode, void (*handler)()) {
    FILE* file = fopen("transfers.csv", mode);
    if (file == NULL) 
    {
        printf("Fehler: Datei konnte nicht geöffnet werden.\n");
        exit(1);
    }
    
    (*handler)(file);
    
    if (fclose(file) != 0)
    {
        printf("Fehler: Datei konnte nicht geschlossen werden.\n");
        exit(1);
    }
}

void find_account(FILE* file) 
{
    char account[64];
    printf("Kontonummer oder Name: ");
    scanf("%s", account);
    
    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char row[COLUMNS_LENGTH][64];
        
        char* token = strtok(line, "\t");
        if (token == NULL) continue;
        strcpy(row[0], token);
        
        for (int i = 1; i < COLUMNS_LENGTH && token != NULL; i++)
        {
            token = strtok(NULL, "\t");
            strcpy(row[i], token);
        }
        
        const int COLUMNS_TO_COMPARE = 4;
        for (int i = 0; i < COLUMNS_TO_COMPARE; i++)
        {
            if (strcmp(account, row[i]) == 0)
            {
                printf("Von: %s (%s)\n", row[0], row[1]);
                printf("An: %s (%s)\n", row[2], row[3]);
                printf("Betrag: %s EUR für %s", row[4], row[5]);
                break;
            }
        }
    }
}

void create_account(FILE* file) 
{
    char row[COLUMNS_LENGTH][64];
    
    printf("Quelle - Name: ");
    scanf("%s", row[0]);
    
    printf("Quelle - Kontonummer: ");
    scanf("%s", row[1]);
    
    printf("Ziel - Name: ");
    scanf("%s", row[2]);
    
    printf("Ziel - Kontonummer: ");
    scanf("%s", row[3]);
    
    printf("Betrag: ");
    scanf("%s", row[4]);
    
    printf("Verwendungszweck: ");
    scanf("%s", row[5]);
    
    for (int i = 0; i < COLUMNS_LENGTH - 1; i++)
    {
        fprintf(file, "%s\t", row[i]);
    }
    
    fprintf(file, "%s\n", row[COLUMNS_LENGTH - 1]);
}

int main() 
{
    unsigned char option;
    while (1) 
    {
        printf("Überweisungen\n");
        printf("1 - Suchen\n");
        printf("2 - Neu\n");
        printf("0 - Beenden\n");
        printf("Eingabe: ");
        scanf("%hhu", &option);
        
        switch (option) 
        {
            case 0:
                return 0;
            case 1:
                use_file("r", find_account);
                break;
            case 2:
                use_file("a", create_account);
                break;
            default:
                printf("Ungültige Eingabe. Bitte wiederholen.\n");
                break;
        }
    }
    
    return 0;
}