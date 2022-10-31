#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "fonctions.h"

void viderBuffer()                                      // Super classique
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

int nbInput (int vMin, int vMax)                        // Input classique limité aux nombres avec valeur min et max
{
    int choixOk = 0, number = 0;
    char temp[3] = "";
    char* positionEntree = NULL;
    while (!choixOk)
    {
        if (fgets(temp, 3, stdin))
        {
            number = strtol(temp, NULL, 10);                // Cette chaîne est un chiffre !
            if (number < vMin || number > vMax)             // Si ça ne correspond pas aux valeurs demandées, C'EST NON.
            {
                printf("Choix invalide, recommencez : ");
            }
            else
            {
                choixOk = 1;
            }

            positionEntree = strchr (temp, '\n');         // On cherche entrée
            if (positionEntree == NULL)                   // Si y'a pas, c'est dans le buffer donc on le vide
            {
                viderBuffer();
            }
        }
        else
        {
            printf("C PT.");
            exit(1);
        }
    }
    return number;
}

int strInput (char *chaine, int len)                    // Du classique
{
    char* positionEntree = NULL;
    if (fgets(chaine, len, stdin))
    {
        positionEntree = strchr (chaine, '\n');         // On cherche entrée
        if (positionEntree != NULL)
        {
            *positionEntree = '\0';                     // Si y'a, on replace la fin de chaîne
        }
        else                                            // Si y'a pas, c'est dans le buffer donc on le vide
        {
            viderBuffer();
        }
        return 1;   // Pas d'erreur
    }
    else
    {
        viderBuffer();
        return 0;   // C'est pas bon
    }
}

xy chkCase(char *chaine, int tabSize)                   // On convertir l'input en coordonnées x/y
{
    int lettre = 0, chiffre = 0;
    xy retValue = {99, 99};                             // Valeurs par défaut pour signaler une erreur (0,0 étant valide)

    lettre = toupper(chaine[0]);                        // Lettre en majuscule en premiere position
    chiffre = strtol(chaine+1, NULL, 10);               // On extrait le chiffre en 2nde position


    if (lettre-65 < 0 || lettre-65 > tabSize-1 || chiffre < 1 || chiffre > tabSize);    // Si ça ne correspond pas à ce qui est dans le tableau, on renvoie les valeurs par défaut
    else                                                                                // Sinon on renvoie les valeurs rentrées
    {
        retValue.x = chiffre-1;
        retValue.y = lettre-65;
    }
    return retValue;
}

void initTab(int **tableau, int tabSize, int mines)     // On remplit un tableau à 0, puis avec des éventuelles mines
{
    int i = 0, j = 0, k = 0, m = 0;
    for (i = 0; i < tabSize; i++)
    {
        for (j = 0; j < tabSize; j++)
        {
            tableau[i][j] = 0;
        }
    }
    if (mines)
    {
        srand(time( NULL ));
        for (i = 0; i < mines; i++)
        {
            j = rand() % (tabSize*tabSize - 1);
            k = j/tabSize;
            m = j%tabSize;
            if (tableau[k][m] != 1)
            {
                tableau[k][m] = 1;
            }
            else
            {
                i--;
            }
        }
    }
}

void display(int **tableau, int tabSize)                // On affiche le tableau, tout connement
{
    int i = 0, j = 0;

    printf("     ");
    for (j = 0; j < tabSize; j++)               // Affichage des lettres en haut
    {
        printf(" %c |", j+65);
    }
    for (i = 0; i < tabSize; i++)
    {
        printf("\n    ");
        for (j = 0; j < tabSize; j++)           // Trait horizontal
        {
            printf(" - -");
        }
        printf("\n");
        if (i<9)                                // On remplace les dizaines par un espace si < 10
        {
            printf(" ");
        }
        printf(" %d |", i+1);                   // Affichage des chiffres sur le côté
        for (j = 0; j < tabSize; j++)
        {
            printf(" %c |", tableau[i][j]+32);  // Affichage de la case (+32 pour convertir "0" en espace)
        }
    }
    printf("\n");
}

int lookAround(int **tableau, int **tabDisplay, int x, int y, int tabSize)  // On check les 8 cases autour d'une case de départ et on retourne le nombre de mines trouvées
{
    int i = 0, j = 0, flag = 0;
    for (i = -1; i < 2; i++)                                // On vérifie -1, 0 et +1 de la colonne de départ
    {
        for (j = -1; j < 2; j++)                            // On vérifie -1, 0 et +1 de la rangée de départ
        {
            if (!(!i && !j) &&                              // On exclut la case de départ
                x+i >= 0 && y+j >= 0 &&                     // et les cases hors-champ
                x+i < tabSize && y+j < tabSize)
            {
                if (tableau[x+i][y+j] == 1)                 // Si on trouve une mine, on ajoute 1 à la valeur de la case
                    flag++;
            }
        }
    }
    tabDisplay[x][y] = flag+16;                             // On update la case à afficher avec le nombre de mines trouvées autour d'elle
    return flag;                                            // On return la valeur trouvée pour faciliter le travail de discover()
}

void discover(int **tableau, int **tabDisplay, int x, int y, int tabSize)   // On appelle lookAround() sur chaque case que celle-ci retourne à 0
{
    int i = 0, j = 0;
    if(lookAround(tableau, tabDisplay, x, y, tabSize) == 0) // Si on trouve une case à zéro, on lance le balayage
    {
        for (i = -1; i < 2; i++)                            // On vérifie -1, 0 et +1 de la colonne de départ
        {
            for (j = -1; j < 2; j++)                        // On vérifie -1, 0 et +1 de la rangée de départ
            {
                if (!(!i && !j) &&                          // On exclut la case de départ
                    x+i >= 0 && y+j >= 0 &&                 // les cases hors-champ
                    x+i < tabSize && y+j < tabSize &&
                    tabDisplay[x+i][y+j] == 0)             // et les cases déjà révélées
                {
                    if (lookAround(tableau, tabDisplay, x+i, y+j, tabSize) == 0)
                    {
                        discover(tableau, tabDisplay, x+i, y+j, tabSize);
                    }
                }
            }
        }
    }
}

int gameWin(int **tableau, int **tabDisplay, int tabSize)
{
    int i = 0, j = 0;
    for (i = 0; i < tabSize; i++)
    {
        for (j = 0; j < tabSize; j++)
        {
            if (tableau[i][j] == 0 && tabDisplay[i][j] == 0) // Si on trouve une case sans mine et non découverte, alors ce n'est pas fini.
            {
                return 0;
            }
        }
    }
    for (i = 0; i < tabSize; i++)
    {
        for (j = 0; j < tabSize; j++)
        {
            if (tableau[i][j] == 1) // Si on trouve une case avec mine, on l'affiche désormais car la partie est gagnée.
            {
                tabDisplay[i][j] = 'X'-32;
            }
        }
    }
    return 1;
}
