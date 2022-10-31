#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "fonctions.h"

int main()
{
    int temp = 0, mines = 0, tabSize = 0, i = 0, choixOk = 0, gameOver = 0;
    char input[5] = "";
    xy nbCase = {0,0};
    int **tableau = NULL, **tabDisplay = NULL;
reset:
    printf("Bienvenue sur Demineur !\nChoisissez une difficulte : \n1. Facile\n2. Moyen\n3. Difficile\n");

    temp = nbInput(1, 3);                                                           // Nombre en entr?e, de 1 ? 3
    mines = 9 + temp*3;                                                             // 12, 15 et 18 : pourcentage de mines dans le tableau.

    printf("Choisissez la taille de cote du champ de mines (6 a 20): ");
    tabSize = nbInput (6, 20);

    tableau = malloc(sizeof(int*) * tabSize);
    tabDisplay = malloc(sizeof(int*) * tabSize);
    for (i = 0; i < tabSize; i++)
    {
        tableau[i] = malloc(sizeof(int) * tabSize);
        tabDisplay[i] = malloc(sizeof(int) * tabSize);
    }

    initTab(tabDisplay, tabSize, 0);
    initTab(tableau, tabSize, mines);

    display(tableau, tabSize);
    while (!gameOver)
    {
        while (!choixOk)
        {
            printf("\nCase a decouvrir : ");
            if(!strInput(input, 5))
            {
                printf("CPT.");
                exit(1);
            }
            nbCase = chkCase(input, tabSize);
            if (nbCase.x == 99 || nbCase.y == 99 || tabDisplay[nbCase.x][nbCase.y] != 0) // Si la valeur retourn?e est celle par d?faut (99) ou que la case est d?j? r?v?l?e, c'est NON.
            {
                printf("PAS BON. TOI REFAIRE CHOIX.\n");
            }
            else
            {
                choixOk = 1;
            }
        }
        choixOk = 0;

        if (tableau[nbCase.x][nbCase.y] == 1) // Si on tombe sur une mine
        {
            printf("\nBOOM BITCH. U DED. lol\n\n");
            display(tableau, tabSize);
            gameOver = 1;
        }
        else
        {
            discover(tableau, tabDisplay, nbCase.x, nbCase.y, tabSize);
            if (gameWin(tableau, tabDisplay, tabSize))
            {
                printf("\nMwais, pas mal. Un coup de chance, probablement.\n\n");
                gameOver = 1;
            }
            else
            {
                printf("\nTu restes en vie. Pour l'instant.\n\n");
            }

            display(tabDisplay, tabSize);
        }
    }

    // Fin de programme (on free, on quitte)
    for (i = 0; i < tabSize; i++)
    {
        free(tableau[i]);
        free(tabDisplay[i]);
    }
    free(tableau);
    free(tabDisplay);

    printf("\nEntrez 1 pour rejouer : ");
    if(nbInput(0,1))
    {
        printf("\n\n\n");
        gameOver = 0;
        goto reset;
    }

    return 0;
}
