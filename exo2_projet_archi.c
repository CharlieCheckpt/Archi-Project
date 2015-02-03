#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define T_MAX 256
#define lines_max 100

 // "Le programme est le simulateur qui r�cup�re de programme �crit en langage machine et ex�cute instruction apr�s instruction"
 // on prend PC=0 et SP=0

char* T[lines_max]; // tableau contenant les insctructions
int mem[5000];//m�moire dans laquelle est stock�e la pile, ainsi que les variables.
int PC=0; // pointe sur un cellule du tableau d'instructions. adresse de la prochaine execution a executer.
int SP=0; // num de la 1�re case libre de la pile(ds mem)

push(char*s)
{
    int a=strtol(&s[3],NULL,16); // convertit en nbre hexadecimal ce qu'il y a apr�s l'espace
    mem[SP++]=a; // incrementation apres
    PC++;
}

iPush() // pas fini
{
    int a=mem[SP-1];
    SP--;
}

push_i(char*s)
{
    int a=strtol(&s[3],NULL,16);
    T[SP++]=a;
    PC++;
}

pop(char*s)
{
    int a=strtol(&s[3],NULL,16);
    mem[a]=mem[--SP];//decrementation avant
    PC++;
}

iPop()
{

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

code_test(char*s) // tests du code assembleur(2 premi�res lettres de l'instruction)
{
    if(s[0]=='0'){
        switch(s[1])
        {
        case '0':
            push(s);
            break;
        case '1':
            iPush();
            break;
        case '2':
            push_val(s);
            break;
        case '3':
            pop(s);
            break;
        case '4':
            iPop();
            break;
        case '5':
            dup();
            break;
        case '6':
            op(s);
            break;
        case '7':
            jmp(s);
            break;
        case '8':
            jpz(s);
            break;
        case '9':
            call(s);
            break;
        }
    }
    else{
        switch(s[0])
        {
        case 'A':
            ret();
            break;
        case 'B':
            rnd(s);
            break;
        case 'C':
            write(s);
            break;
        case 'D':
            read(s);
            break;
        default:
            halt();
    }
    }
}

int main()
{
    FILE* fichier=NULL;
    char str[T_MAX];
    fichier=fopen("out.txt","r+");
    if(fichier!=NULL){
        Fill_tab_instructions(fichier);
        fclose(fichier);
        int j=0;
        while(T[PC]!=NULL){ // on regarde chaque instruction du tableau
            code_test(T[PC]);
        }
    }
    else printf("impossible d'ouvrir le fichier");
    return 0;
}
