#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define T_MAX 256
#define LINES_MAX 100


// "Le programme est le simulateur qui recupere de programme ecrit en langage machine et execute instruction apres instruction"
// on prend PC=0 et SP=0


//tableaux d'instructions
char* T_type[LINES_MAX]; // tableau contenant les types des instructions.
char* T_arg[LINES_MAX]; // tableaux contenant les arguments des instructions.


int mem[5000];
// tab representant la memoire dans laquelle est stockee la pile, ainsi que les variables.
int PC = 0; // "pointe" sur une cellule du tableau d'instructions. adresse de la prochaine execution a executer.
int SP = 0; // num de la 1ere case libre de la pile(dans mem)
int p = 1;

void push(char*s) //OK
{
    int a=strtol(s,NULL,16); // convertit en nbre hexadecimal la chaine de caractères.
    mem[SP++]=a; // incrementation apres
    PC++;
}

void iPush(void) // OK
{
    int x=mem[SP-1];
    mem[SP-1]=mem[x];
    PC++;
}

void push_val(char*s) //OK
{
    int a=strtol(s,NULL,16);
    mem[SP++]=a;
    PC++;
}

void pop(char*s) //OK
{
    int a=strtol(s,NULL,16);
    mem[a]=mem[--SP];//decrementation avant
    PC++;
}

void iPop(void) //OK
{
    mem[mem[SP-1]]=mem[SP-2];
    SP-=2;
    PC++;
}

void dup(void) //OK
{
    mem[SP]=mem[SP-1];
    SP++;
    PC++;
}

void op(char*s) // ?
{
    int a=strtol(s,NULL,16);
    switch(a){
    case 0: // et logique bit à bit
        SP--;
        mem[SP-1]=mem[SP-1]&mem[SP];
        break;
    case 1: // ou logique bit à bit
        SP--;
        mem[SP-1]=mem[SP-1]|mem[SP];
        break;
    case 2: // ou-exclusif logique bit à bit
        SP--;
        mem[SP-1]=mem[SP-1]^mem[SP];
        break;
    case 3: // non logique bit à bit
        mem[SP-1]=~mem[SP-1];
        break;
    case 4: // inverse la valeur au sommet de la pile
        mem[SP-1]=-mem[SP-1];
        break;
    case 5: // additionne les deux valeurs au sommet
        SP--;
        mem[SP-1]=mem[SP-1]+mem[SP];
        break;
    case 6: // soustraction
        SP--;
        mem[SP-1]=mem[SP-1]-mem[SP];
        break;
    case 7: // multiplication
        SP--;
        mem[SP-1]=mem[SP-1]*mem[SP];
        break;
    case 8: // division entière
        SP--;
        mem[SP-1]=mem[SP-1]/mem[SP];
        break;
    case 9: // modulo
        SP--;
        mem[SP-1]=mem[SP-1]%mem[SP];
        break;
    case 10: // test d'egalite
        SP--;
        if(mem[SP-1]==mem[SP]) mem[SP-1]=0;
        else mem[SP-1]=1;
        break;
    case 11: //  test d'inegalite
        SP--;
        if(mem[SP-1]!=mem[SP]) mem[SP-1]=0;
        else mem[SP-1]=1;
        break;
    case 12: // test >
        SP--;
        if(mem[SP-1]>mem[SP]) mem[SP-1]=0;
        else mem[SP-1]=1;
        break;
    case 13: // test >=
        SP--;
        if(mem[SP-1]>=mem[SP]) mem[SP-1]=0;
        else mem[SP-1]=1;
        break;
    case 14: // test <
        SP--;
        if(mem[SP-1]<mem[SP]) mem[SP-1]=0;
        else mem[SP-1]=1;
        break;
    case 15: // test <=
        SP--;
        if(mem[SP-1]<=mem[SP]) mem[SP-1]=0;
        else mem[SP-1]=1;
        break;
    }
    PC++;
}

void jmp(char*s) //OK
{
    int a=strtol(s,NULL,16);
    PC+=a;
}

void jpz(char*s) //OK
{
    int a=strtol(s,NULL,16);
    int x=mem[--SP];
    if(x==0) PC+=a;
}

void call(char*s) // ?
{
    int a=strtol(s,NULL,16);
    mem[SP++]=&T_type[PC];
    PC+=a;
}

void ret(void)// ?
{
    SP--;
    PC++;
}

void rnd(char*s) // ?
{
    int a=strtol(s,NULL,16);
    int nb_alea;
    nb_alea = (rand() % a); // nbre aleatoire entre 0 et a-1;
    mem[SP++]=nb_alea;
    PC++;
}

void write(char*s) //OK
{
    int a=strtol(s,NULL,16);
    printf("%d\n",mem[a]);
    PC++;
}

void read(char*s) //OK
{
    int a=strtol(s,NULL,16);
    printf("rentrez une valeur svp: ");
    scanf("%d",&mem[a]);
    PC++;
}

void halt(int j)
{
   j=0;
}

void Fill_tab_instructions(FILE *fichier) // remplissage du tab d'instructions
{
    printf("on rentre ds Fill_tab_instructions\n");
    int i = 0;
    char chaine1[T_MAX],chaine2[T_MAX];
    while(fscanf(fichier, "%s %s", &chaine1, &chaine2)!=EOF){
        T_type[i]=chaine1;
        T_arg[i]=chaine2;
        printf(" T_type[%d] vaut : %s et T_arg[%d] vaut : %s\n",i,T_type[i],i,T_arg[i]);
        i++;
    }
    fclose(fichier);
    printf("tab filled\n\n");
}

void code_test(char *s1,char*s2) // tests du code assembleur(2 premieres lettres de l'instruction)
{
    printf("je suis dans la fonction code_test\n");
    char s3[T_MAX];
    s3[0]=s1[0];
    s3[1]=s1[1];
    s3[2]='\0';
    printf("s3 vaut : %s\n",s3);
    int cod = strtol(s3,NULL,16);
    printf("cod vaut : %d\n",cod);

    switch (cod) {
        case 0:
            push(s1);
            break;
        case 1:
            iPush();
            break;
        case 2:
            push_val(s1);
            break;
        case 3:
            pop(s1);
            break;
        case 4:
            iPop();
            break;
        case 5:
            dup();
            break;
        case 6:
            op(s1);
            break;
        case 7:
            jmp(s1);
            break;
        case 8:
            jpz(s1);
            break;
        case 9:
            call(s1);
            break;
        case 10:
            ret();
            break;
        case 11:
            rnd(s1);
            break;
        case 12:
            write(s1);
            break;
        case 13:
            read(s1);
            break;
        case 99:
            halt(p);
            break;
        default:
            exit(1);
    }
}


int main() {
    FILE *fichier = NULL;
    srand((unsigned int) time(NULL)); // nombre aleatoire. // pq unsigned int?
    fichier = fopen("out.txt", "r+");
    if (fichier != NULL) {
        Fill_tab_instructions(fichier);
        //printf("test1");
        //fclose(fichier);
        code_test(T_type[0],T_arg[0]);

      /* while (p == 1) { // on regarde chaque instruction du tableau
            // printf("t(pc) = %s", T[PC]);
            code_test(T_type[PC],T_arg[PC]);
            //printf("p = %d", p);
        }*/
        printf("end of program\n");
    }
    else printf("impossible d'ouvrir le fichier");
    return 0;
}
