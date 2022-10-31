#include "structures.h"

// Description de chaque fonction dans fonctions.c

void viderBuffer();
int strInput (char *chaine, int len);
int nbInput (int vMin, int vMax);
void initTab (int **tableau, int tabSize, int mines);
void display(int **tableau, int tabSize);
xy chkCase(char *chaine, int tabSize);
int lookAround(int **tableau, int **tabDisplay, int x, int y, int tabSize);
void discover(int **tableau, int **tabDisplay, int x, int y, int tabSize);
int gameWin(int **tableau, int **tabDisplay, int tabSize);
