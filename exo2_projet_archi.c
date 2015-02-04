#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define T_MAX 256
#define lines_max 100


// "Le programme est le simulateur qui r�cup�re de programme �crit en langage machine et ex�cute instruction apr�s instruction"
// on prend PC=0 et SP=0


char* T[lines_max]; // tableau d'instructions.
int mem[5000];// tab representant la memoire dans laquelle est stock�e la pile, ainsi que les variables.
int PC=0; // "pointe" sur une cellule du tableau d'instructions. adresse de la prochaine execution a executer.
int SP=0; // num de la 1�re case libre de la pile(ds mem)

push(char*s) //OK
{
    int a=strtol(s,NULL,16); // convertit en nbre hexadecimal la chaine de caract�res.
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
    case 0: // et logique bit � bit
        SP--;
        mem[SP-1]=mem[SP-1]&mem[SP];
        break;
    case 1: // ou logique bit � bit
        SP--;
        mem[SP-1]=mem[SP-1]|mem[SP];
        break;
    case 2: // ou-exclusif logique bit � bit
        SP--;
        mem[SP-1]=mem[SP-1]^mem[SP];
        break;
    case 3: // non logique bit � bit
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
    case 8: // division enti�re
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
}

read(char*s) //OK
{
    int a=strtol(s,NULL,16);
    scanf("rentrez une valeur svp : %d",&mem[a]);
}

halt(int*p)
{
   *p==0;
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

code_test(char*s,int*p) // tests du code assembleur(2 premi�res lettres de l'instruction)
{
    char *buf=strtok(s," "); // si s=0D 000003E8 --> s= 000003E8 et buf=0D
    int cod=strtol(buf,NULL,16);
    switch(cod)
    {
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
        halt(p);
        break;
    }
}


int main()
{
    FILE* fichier=NULL;
    srand(time(NULL)); // nombre aleatoire.
    fichier=fopen("out.txt","r+");
    if(fichier!=NULL){
        Fill_tab_instructions(fichier);
        fclose(fichier);
        int*p;
        *p=1;
        while(T[PC]!=NULL && *p==1){ // on regarde chaque instruction du tableau
            code_test(T[PC],p);
        }
        printf("end of program");
    }
    else printf("impossible d'ouvrir le fichier");
    return 0;
}
