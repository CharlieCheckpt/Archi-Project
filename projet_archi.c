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

int instructions(char* str){
	int i;
	if (!strcmp(str, "push")) {
    	i=0;
	}else if (!strcmp(str, "iPush")){
    	i=1;
	}else if (!strcmp(str, "push#")){
    	i=2;
	}else if (!strcmp(str, "pop")){
    	i=3;
	}else if (!strcmp(str, "iPop")){
    	i=4;
	}else if (!strcmp(str, "dup")){
    	i=5;
	}else if (!strcmp(str, "op")){
    	i=6;
	}else if (!strcmp(str, "jmp")){
    	i=7;
	}else if (!strcmp(str, "jpz")){
    	i=8;
	}else if (!strcmp(str, "call")){
    	i=9;
	}else if (!strcmp(str, "ret")){
    	i=10;
	}else if (!strcmp(str, "rnd")){
    	i=11;
	}else if (!strcmp(str, "write")){
    	i=12;
	}else if (!strcmp(str, "read")){
    	i=13;
	}else if (!strcmp(str, "halt")){
    	i=99;
	}else{
		printf("Fonction non définie");
	}
	printf("%02X",i);
//c'est sale mais ça marche :D
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


//test
