#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define T_MAX 256
#define LINES_MAX 100


// "Le programme est le simulateur qui recupere de programme ecrit en langage machine et execute instruction apres instruction"
// on prend PC=0 et SP=0

typedef struct {
    char instruction[20];
} Line;


Line insTab[LINES_MAX]; // tableau d'instructions
int line_counter = 0;
long mem[5000]; // tab representant la memoire dans laquelle est stockee la pile, ainsi que les variables
int PC = 0; // "pointe" sur une cellule du tableau d'instructions. adresse de la prochaine execution a executer
int SP = 0; // num de la 1ere case libre de la pile(dans mem)
int p = 1;

void push(char *s){
    long a = strtol(s, NULL, 16); // convertit en nbre hexadecimal la chaine de caracteres.
    mem[SP++] = a; // incrementation apres
    PC++;
}

void iPush(){
    long x = mem[SP - 1];
    mem[SP - 1] = mem[x];
    PC++;
}

void push_val(char *s){
    long a = strtol(s, NULL, 16);
    mem[SP++] = a;
    PC++;
}

void pop(char *s){
    long a = strtol(s, NULL, 16);
    mem[a] = mem[--SP];//decrementation avant
    PC++;
}

void iPop(){
    mem[mem[SP - 1]] = mem[SP - 2];
    SP -= 2;
    PC++;
}

void dup(){
    mem[SP] = mem[SP - 1];
    SP++;
    PC++;
}

void op(char *s){
    long a = strtol(s, NULL, 16);
    switch (a) {
        case 0: // et logique bit a bit
            SP--;
            mem[SP - 1] = mem[SP - 1] & mem[SP];
            break;
        case 1: // ou logique bit a bit
            SP--;
            mem[SP - 1] = mem[SP - 1] | mem[SP];
            break;
        case 2: // ou-exclusif logique bit a bit
            SP--;
            mem[SP - 1] = mem[SP - 1] ^ mem[SP];
            break;
        case 3: // non logique bit a bit
            mem[SP - 1] = ~mem[SP - 1];
            break;
        case 4: // inverse la valeur au sommet de la pile
            mem[SP - 1] = -mem[SP - 1];
            break;
        case 5: // additionne les deux valeurs au sommet
            SP--;
            mem[SP - 1] = mem[SP - 1] + mem[SP];
            break;
        case 6: // soustraction
            SP--;
            mem[SP - 1] = mem[SP - 1] - mem[SP];
            break;
        case 7: // multiplication
            SP--;
            mem[SP - 1] = mem[SP - 1] * mem[SP];
            break;
        case 8: // division entiere
            SP--;
            mem[SP - 1] = mem[SP - 1] / mem[SP];
            break;
        case 9: // modulo
            SP--;
            mem[SP - 1] = mem[SP - 1] % mem[SP];
            break;
        case 10: // test d'egalite
            SP--;
            if (mem[SP - 1] == mem[SP]) mem[SP - 1] = 0;
            else mem[SP - 1] = 1;
            break;
        case 11: //  test d'inegalite
            SP--;
            if (mem[SP - 1] != mem[SP]) mem[SP - 1] = 0;
            else mem[SP - 1] = 1;
            break;
        case 12: // test >
            SP--;
            if (mem[SP - 1] > mem[SP]) mem[SP - 1] = 0;
            else mem[SP - 1] = 1;
            break;
        case 13: // test >=
            SP--;
            if (mem[SP - 1] >= mem[SP]) mem[SP - 1] = 0;
            else mem[SP - 1] = 1;
            break;
        case 14: // test <
            SP--;
            if (mem[SP - 1] < mem[SP]) mem[SP - 1] = 0;
            else mem[SP - 1] = 1;
            break;
        case 15: // test <=
            SP--;
            if (mem[SP - 1] <= mem[SP]) mem[SP - 1] = 0;
            else mem[SP - 1] = 1;
            break;
        default:
            exit(1);
    }
    PC++;
}

void jmp(char *s){
    //PROBLEME STRTOL ET NEGATIFS !!!!!!!!!!!!
    long a = strtol(s, NULL, 16);
    PC += a;
}

void jpz(char *s){
    long a = strtol(s, NULL, 16);
    long x = mem[--SP];
    PC++;
    if (x == 0) PC += a;
}

void call(char *s){
    long a = strtol(s, NULL, 16);
    mem[SP++] = (long) &insTab[PC].instruction;
    PC += a;
}

void ret(){
    SP--;
    PC++;
}

void rnd(char *s){
    long a = strtol(s, NULL, 16);
    long nb_alea;
    nb_alea = (rand() % a); // nbre aleatoire entre 0 et a-1;
    mem[SP++] = nb_alea;
    PC++;
}

void write(char *s){
    long a = strtol(s, NULL, 16);
    printf("%ld\n", mem[a]);
    PC++;
}

void read(char *s){
    long a = strtol(s, NULL, 16);
    printf("rentrez une valeur svp : ");
    scanf("%ld", &mem[a]);
    PC++;
}

void halt() {
    p = 0;
}

void Fill_tab_instructions(FILE *fichier){ // remplissage du tab d'instructions
    int i = 0;
    char chaine[T_MAX];
    while (fgets(chaine, T_MAX, fichier) != NULL){
        line_counter++;
        strcpy(insTab[i].instruction, chaine);
        //printf("!DEBUT! dans insTab[%d], il y a : %s", i, chaine);
        //T[i] = chaine;
        //printf("dans T[%d], il y a : %s", i, insTab[i].instruction);
        i++;
        //printf("!FIN! dans chaine[%d], il y a : %s\n", i, chaine);
        //fgets(chaine, T_MAX, fichier);
        //printf("OK");
    }
    printf("Fill tab ok\n");
    printf("line counter : %d\n", line_counter);
}

void code_test(char *s){ // tests du code assembleur(2 premieres lettres de l'instruction)
    char *instBuffer = strtok(s, " "); // si s=0D 000003E8 --> s= 000003E8 et buf=0D
    s = strtok(NULL, "\n");
    //debug
    printf("instruction buffer : %s et s = %s\n", instBuffer, s);

    //long cod = strtol(instBuffer, NULL, 16);
    switch (strtol(instBuffer, NULL, 16)) {
        case 0:
            push(s);
            break;
        case 1:
            iPush();
            break;
        case 2:
            push_val(s);
            break;
        case 3:
            pop(s);
            break;
        case 4:
            iPop();
            break;
        case 5:
            dup();
            break;
        case 6:
            op(s);
            break;
        case 7:
            jmp(s);
            break;
        case 8:
            jpz(s);
            break;
        case 9:
            call(s);
            break;
        case 10:
            ret();
            break;
        case 11:
            rnd(s);
            break;
        case 12:
            write(s);
            break;
        case 13:
            read(s);
            break;
        case 99:
            halt();
            break;
        default:
            exit(1);
    }
}


int main() {
    FILE* file = NULL;
    srand((unsigned int) time(NULL)); // nombre aleatoire
    file = fopen("out.txt", "r+");
    if (file != NULL){
        Fill_tab_instructions(file);
        //printf("test1");
        fclose(file);
        //exit(1);

        while (PC < line_counter && p == 1) { // on regarde chaque instruction du tableau
            if (insTab[PC+2].instruction == '\0') exit(1);
            printf("t(%d) = %s", PC, insTab[PC].instruction);
            code_test(insTab[PC].instruction);
            //printf("p = %d\n", p);
        }
        printf("end of program");
        exit(1);
    }
    else printf("impossible d'ouvrir le fichier");
    return 0;
}