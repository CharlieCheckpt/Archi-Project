#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define T_MAX 256
#define INPUT_FILE "assembleur_test.txt"

//Label
typedef struct{
	char label[T_MAX];
} Label;

//Intructions to int
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
		printf("Invalid function");
		exit(1);
	}
	return i;
}

//Argument type
int argtype(char* str){
	/*
		0 = pas d'argument
		1 = un nombre
		2 = une chaîne
	 */
	int i;
	if (!strcmp(str, "push")) {
		i=1;
	}else if (!strcmp(str, "iPush")){
		i=0;
	}else if (!strcmp(str, "push#")){
		i=1;
	}else if (!strcmp(str, "pop")){
		i=1;
	}else if (!strcmp(str, "iPop")){
		i=0;
	}else if (!strcmp(str, "dup")){
		i=0;
	}else if (!strcmp(str, "op")){
		i=1;
	}else if (!strcmp(str, "jmp")){
		i=2;
	}else if (!strcmp(str, "jpz")){
		i=2;
	}else if (!strcmp(str, "call")){
		i=2;
	}else if (!strcmp(str, "ret")){
		i=0;
	}else if (!strcmp(str, "rnd")){
		i=1;
	}else if (!strcmp(str, "write")){
		i=1;
	}else if (!strcmp(str, "read")){
		i=1;
	}else if (!strcmp(str, "halt")){
		i=0;
	}else{
		printf("Invalid argument");
		exit(1);
	}
	return i;
}

//Lines counter
int countLines(FILE* file){
	int line_number = 0;
	char str[T_MAX];
	if (file!=NULL){
		while(fgets(str,T_MAX,file)!='\0') {
			//printf("line nb = %d\n", line_number);
			line_number++;
		}
	}
	return line_number;
}

//Label table creator
Label* createLabelTab(int line_count){
	Label* label_tab = malloc(sizeof(Label)*line_count);
	return label_tab;
}

//Label table filling
/*
	fgets(str,T_MAX,file) lit T_MAX-1 caractères de fichier file et le met dans str

	strstr(str, ":") renvoie un pointeur dans str qui pointe vers une chaine commencant par ":"
		char str[T_MAX] = "Mon chat est blanc"
		strstr(str, "chat") renvoie "chat est blanc" (pointeur vers le premier char)
		strstr(str, "mignon") renvoie NULL

	strtok(str, " \t\r\n:") renvoie un pointeur dans str vers une chaine finissant par un espace, tab, CR ou LF suivi de ":"
		char str[T_MAX] = "Je suis Charlie"
		mot = strtok(str, "suis") met "Je " dans mot
 */
void fillLabelTab(FILE* file, Label* label_tab){
	char str[T_MAX];
	char *word;
	int i = -1;
	if (file!=NULL){
		while(fgets(str,T_MAX,file)) {
			i++;
			if (strstr(str, ":") != NULL) {
				word = strtok(str, " \t\r\n:");
				printf("mot (%d) = %s\n", i, word);

				label_tab[i].label = "hi";
			}
		}
	}
}

//Find a label in the table and returns the line #
/*
	On ecrit sizeof(label_tab)/ sizeof(Label) car sizeof(label_tab) renvoie la taille du tableau en bits (et non en nombre de "cases")
 */
int findLabel(char* str, Label* label_tab){
	int i;
	printf("%d", sizeof(label_tab));
	for(i=0; i< sizeof(label_tab)/ sizeof(Label); i++){
		if(strcmp(label_tab[i].label, str) == 0)
			return i;
	}
	return -1;
}

int main(){
    FILE* file=NULL;
	FILE* file2=NULL;

    file=fopen(INPUT_FILE,"r+");

	//totalline = nombre de lignes
	int totalline = countLines(file);

	//creation du tableau
	Label* label_tab = createLabelTab(totalline);
	fclose(file);

	//remplissage du tableau des etiquettes
	file=fopen(INPUT_FILE,"r+");
	fillLabelTab(file, label_tab);
	fclose(file);

	int i;
	for(i=0; i< 5; i++){
		printf("Dans la case %d, il y a letiquette %s\n", i, label_tab[i].label);
	}
	
	file=fopen(INPUT_FILE,"r+");

	//creation fichier d'exportation
	file2=fopen("out.txt","w");
	
	char str[T_MAX];
	char *word; //mot a transformer en nombre avec la fonction instrucstions
	char *arg; //argument de type chaine de caracteres
	char *useless; //pour ignorer les etiquettes lors de la traduction
	int argint; //argument de type entier
	int line_n = 0;

	if (file!=NULL){
		while(fgets(str,T_MAX,file)) {
			line_n++;

			if (strstr(str, ":") != NULL) {
				useless = strtok(str, " \t\r\n:"); //met l'etiquette de cote
				word = strtok(NULL, " \t\r\n:");
			} else {
				word = strtok(str, " \t\r\n:");
			}

			arg = strtok(NULL, " \t\r\n:");


			if (argtype(word) == 0) {
				if(arg != NULL) exit(1);
				argint = 0;
			} else if (argtype(word) == 1) {
				argint = atoi(arg);
			} else if (argtype(word) == 2) {
				if (findLabel(arg, label_tab) > line_n) {
					printf("val = %d\n", findLabel(arg, label_tab));
					argint = findLabel(arg, label_tab) - line_n +1;
				} else {
					printf("val = %d, %s\n", findLabel(arg, label_tab), arg);
					argint = line_n + 1 - findLabel(arg, label_tab);
				}
			}

			printf("word = %s\n",word);
			printf("argu = %s\n", arg);
			//fonction qui transforme str en int s'il le faut
			printf("code = %02X + arg = %08X \n\n",instructions(word), argint);
			//fprintf(file2,"%02X%08X \n\n",instructions(word), argint);
		}
	}
	

    return 0;

}