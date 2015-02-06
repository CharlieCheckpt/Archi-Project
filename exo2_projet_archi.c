#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define T_MAX 256
#define lines_max 100


// "Le programme est le simulateur qui récupère de programme écrit en langage machine et exécute instruction après instruction"
// on prend PC=0 et SP=0


char* T[lines_max]; // tableau d'instructions.
int mem[5000];// tab representant la memoire dans laquelle est stockée la pile, ainsi que les variables.
int PC=0; // "pointe" sur une cellule du tableau d'instructions. adresse de la prochaine execution a executer.
int SP=0; // num de la 1ère case libre de la pile(ds mem)

push(char*s) //OK
{
    int a=strtol(s,NULL,16); // convertit en nbre hexadecimal la chaine de caractères.
    mem[SP++]=a; // incrementation apres
    PC++;
}

iPush() // OK
{
    int x=mem[SP-1];
    mem[SP-1]=mem[x];
    PC++;
}

push_val(char*s) //OK
{
    int a=strtol(s,NULL,16);
    mem[SP++]=a;
    PC++;
}

pop(char*s) //OK
{
    int a=strtol(s,NULL,16);
    mem[a]=mem[--SP];//decrementation avant
    PC++;
}

iPop() //OK
{
    mem[mem[SP-1]]=mem[SP-2];
    SP-=2;
    PC++;
}

dup() //OK
{
    mem[SP]=mem[SP-1];
    SP++;
    PC++;
}

op(char*s) // ?
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

jmp(char*s) //OK
{
    int a=strtol(s,NULL,16);
    PC+=a;
}

jpz(char*s) //OK
{
    int a=strtol(s,NULL,16);
    int x=mem[--SP];
    if(x==0) PC+=a;
}

call(char*s) // ?
{
    int a=strtol(s,NULL,16);
    mem[SP++]=&T[PC];
    PC+=a;
}

ret()// ?
{
    SP--;
    PC++;
}

rnd(char*s) // ?
{
    int a=strtol(s,NULL,16);
    int nb_alea;
    nb_alea = (rand() % a); // nbre aleatoire entre 0 et a-1;
    mem[SP++]=nb_alea;
    PC++;
}

write(char*s) //OK
{
    int a=strtol(s,NULL,16);
    printf("%d\n",mem[a]);
    PC++;
}

read(char*s) //OK
{
    int a=strtol(s,NULL,16);
    scanf("rentrez une valeur svp : %d",&mem[a]);
    PC++;
}

halt(int*p)
{
   *p=0;
}

Fill_tab_instructions(FILE*fichier) // remplissage du tab d'instructions
{
    int i=0;
    char chaine[T_MAX];
    do{
        T[i]=chaine;
        printf("%s\n",T[i]);
        i++;
    }while(fgets(chaine,T_MAX,fichier)!=NULL);
}

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
    }
}


int main()
{
    FILE* fichier=NULL;
    srand(time(NULL)); // nombre aleatoire. sert pour la fonction
    fichier=fopen("out.txt","r+");
    int*p;
    *p=1;
    if(fichier!=NULL){
        Fill_tab_instructions(fichier);
        fclose(fichier);
        printf("tab filled\n");
        while(*p==1){ // on regarde chaque instruction du tableau
            code_test(T[PC],p); // peut-on utiliser strtok avec T[PC] ?? c'est peut etre le bug.
        }
        printf("end of program");
    }
    else printf("impossible d'ouvrir le fichier");
    return 0;
}
