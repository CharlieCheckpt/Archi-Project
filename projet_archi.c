#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define T_MAX 256

void find_etiquette(char*ch){
    int i=0;
    while(ch[i]!='\0'){
        if(ch[i]==':'){
            ch[i]='\0';
            //printf("%d\n",i);
        } i++;
    }
}


int main(){
    FILE*fichier=NULL;
    fichier=fopen("assembleur_test.txt","r+");
    char chaine[T_MAX];
    int line = 0;

    if (fichier!=NULL){
        fgets(chaine,T_MAX,fichier);
        do{
            printf("%d   %s\n",line,chaine);
            line ++;

            char word[T_MAX];
            while(fscanf(fichier,"%s",&word)!=EOF){
                //parcourir tab banque de données
            }

        }while(fgets(chaine,T_MAX,fichier)!='\0');
    }
    return 0;
}


//super programme
