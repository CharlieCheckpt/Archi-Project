#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define T_MAX 256
#define LINES_MAX 100


// "Le programme est le simulateur qui recupere de programme ecrit en langage machine et execute instruction apres instruction"
// on prend PC=0 et SP=0


char *T[LINES_MAX]; // tableau d'instructions.
long mem[5000];
// tab representant la memoire dans laquelle est stockee la pile, ainsi que les variables.
int PC = 0; // "pointe" sur une cellule du tableau d'instructions. adresse de la prochaine execution a executer.
int SP = 0; // num de la 1ere case libre de la pile(dans mem)
int p = 1;

push(char *s) //OK
{
    long a = strtol(s, NULL, 16); // convertit en nbre hexadecimal la chaine de caracteres.
    mem[SP++] = a; // incrementation apres
    PC++;
}

iPush() // OK
{
    long x = mem[SP - 1];
    mem[SP - 1] = mem[x];
    PC++;
}

push_val(char *s) //OK
{
    long a = strtol(s, NULL, 16);
    mem[SP++] = a;
    PC++;
}

pop(char *s) //OK
{
    long a = strtol(s, NULL, 16);
    mem[a] = mem[--SP];//decrementation avant
    PC++;
}

iPop() //OK
{
    mem[mem[SP - 1]] = mem[SP - 2];
    SP -= 2;
    PC++;
}

dup() //OK
{
    mem[SP] = mem[SP - 1];
    SP++;
    PC++;
}

op(char *s) // ?
{
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

jmp(char *s) //OK
{
    long a = strtol(s, NULL, 16);
    PC += a;
}

jpz(char *s) //OK
{
    long a = strtol(s, NULL, 16);
    long x = mem[--SP];
    if (x == 0) PC += a;
}

call(char *s) // ?
{
    long a = strtol(s, NULL, 16);
    mem[SP++] = (long) &T[PC];
    PC += a;
}

ret()// ?
{
    SP--;
    PC++;
}

rnd(char *s) // ?
{
    long a = strtol(s, NULL, 16);
    long nb_alea;
    nb_alea = (rand() % a); // nbre aleatoire entre 0 et a-1;
    mem[SP++] = nb_alea;
    PC++;
}

write(char *s) //OK
{
    long a = strtol(s, NULL, 16);
    printf("%l\n", mem[a]);
}

read(char *s) //OK
{
    long a = strtol(s, NULL, 16);
    printf("rentrez une valeur svp : ");
    scanf("%l", &mem[a]);
}

halt() {
    p = 0;
}

void Fill_tab_instructions(FILE *fichier) // remplissage du tab d'instructions
{
    int i = 0;
    char chaine[T_MAX];
    do {
        //strcpy(T[i], chaine);
        printf("!DEBUT! dans chaine[%d], il y a : %s", i, chaine);
        T[i] = chaine;
        printf("dans T[%d], il y a : %s", i, T[i]);
        i++;
        printf("!FIN! dans chaine[%d], il y a : %s\n", i, chaine);
        fgets(chaine, T_MAX, fichier);
        printf("OK");
    } while (i < 5);
    printf("Fill tab ok");
}

<<<<<<< HEAD
code_test(char *s,int*p) // tests du code assembleur(2 premières lettres de l'instruction)
{
    char instruction[T_MAX]; // tente de contouner le pb de strtok qui ne veut pas de const char* ??
    strcpy(instruction,s);
    printf("je suis dans la fonction code_test");
    char*buf[T_MAX]=strtok(instruction," "); // si s=0D 000003E8 --> buf=0D
    char*arg=strtok(NULL," "); //arg=000003E8
    int cod=strtol(buf,NULL,16);
    switch(cod)
    {
    case 0:
        push(arg);
        break;
    case 1:
        iPush();
        break;
    case 2:
        push_val(arg);
        break;
    case 3:
        pop(arg);
        break;
    case 4:
        iPop();
        break;
    case 5:
        dup();
        break;
    case 6:
        op(arg);
        break;
    case 7:
        jmp(arg);
        break;
    case 8:
        jpz(arg);
        break;
    case 9:
        call(arg);
        break;
    case 10:
        ret();
        break;
    case 11:
        rnd(arg);
        break;
    case 12:
        write(arg);
        break;
    case 13:
        read(arg);
        break;
    case 99:
        halt(p);
        break;
    default:
        printf("problem");
        break;
=======
code_test(char *s) // tests du code assembleur(2 premieres lettres de l'instruction)
{
    char *buf = strtok(s, " "); // si s=0D 000003E8 --> s= 000003E8 et buf=0D
    long cod = strtol(buf, NULL, 16);
    switch (cod) {
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
>>>>>>> origin/master
    }
}


<<<<<<< HEAD
int main()
{
    FILE* fichier=NULL;
    srand(time(NULL)); // nombre aleatoire. sert pour la fonction
    fichier=fopen("out.txt","r+");
    int*p;
    *p=1;
    if(fichier!=NULL){
=======
int main() {
    FILE *fichier = NULL;
    srand((unsigned int) time(NULL)); // nombre aleatoire.
    fichier = fopen("out.txt", "r+");
    if (fichier != NULL) {
>>>>>>> origin/master
        Fill_tab_instructions(fichier);
        printf("test1");
        fclose(fichier);
<<<<<<< HEAD
        printf("tab filled\n");
        while(*p==1){ // on regarde chaque instruction du tableau
            code_test(T[PC],p); // peut-on utiliser strtok avec T[PC] ?? c'est peut etre le bug.
=======

        while (T[PC] != NULL && p == 1) { // on regarde chaque instruction du tableau
            printf("t(pc) = %s", T[PC]);
            code_test(T[PC]);
            printf("p = %d", p);
>>>>>>> origin/master
        }
        printf("end of program");
    }
    else printf("impossible d'ouvrir le fichier");
    return 0;
}
