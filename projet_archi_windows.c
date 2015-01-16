#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define T_MAX 256
#define INPUT_FILE "assembleur_test.txt"

//Global variable
int line_n = 0;

//Label
typedef struct{
	char label[T_MAX];
} Label;

//Intructions to int & test
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
		printf("ERROR: command not found: %s at line %d\n", str, line_n);
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
	}
	return i;
}

//Lines counter
int countLines(FILE* file){
	int line_number = 0;
	char str[T_MAX];
	while(fgets(str,T_MAX,file)!='\0') {
		//printf("line nb = %d\n", line_number);
		line_number++;
	}
	return line_number;
}

//Label table creator
Label* createLabelTab(int line_count){
	Label* label_tab = malloc(sizeof(Label)*line_count);
	if (label_tab == NULL){
		printf("ERROR: out of memory\n");
		exit(1);
	}
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
	while(fgets(str,T_MAX,file)) {
		i++;
		if (strstr(str, ":") != NULL) {
			word = strtok(str, " \t\r\n:");

			//printf("mot (%d) = %s\n", i, word);
			strcpy(label_tab[i].label, word);
		}
	}
}

//Find a label in the table and returns the line #
/*
	sizeof(label_tab) = taille du tableau d'etiquettes
 */
int findLabel(char* str, Label* label_tab){
	int i;
	for(i=0; i<= sizeof(label_tab)*2; i++){
		if(strcmp(label_tab[i].label, str) == 0) {
			return i;
		}
	}
	printf("ERROR: label not found: %s at line %d\n", str, line_n);
	exit(1);
}

//Int test
/*
	Renvoie 1 si str est un entier, 0 sinon
	atoi convertit une chaine en int
 */
int is_int(char const * str) {
	char buf[256];
	sprintf(buf, "%d", atoi(str));
	return (strcmp(buf, str) == 0);
}

int main(){
    FILE* file=NULL;
	FILE* file2=NULL;

	file=fopen(INPUT_FILE,"r+");
	if (file==NULL){
		printf("ERROR: unexistant file\n");
		exit(1);
	}

	//totalline = nombre de lignes
	int totalline = countLines(file);

	//creation du tableau
	Label* label_tab = createLabelTab(totalline);
	fclose(file);

	//remplissage du tableau des etiquettes
	file=fopen(INPUT_FILE,"r+");
	fillLabelTab(file, label_tab);
	fclose(file);

	//creation fichier a exporter
	file=fopen(INPUT_FILE,"r+");
	file2=fopen("out.txt","w");

	char str[T_MAX];
	char *word; //mot a transformer en nombre avec la fonction instrucstions
	char *arg; //argument de type chaine de caracteres
	char *useless; //pour ignorer les etiquettes lors de la traduction
	int argint; //argument de type entier

	while(fgets(str,T_MAX,file)) {
		line_n++;

		//Evite l'etiquette a la traduction et met l'instruction dans word
		if (strstr(str, ":") != NULL) {
			useless = strtok(str, " \t\r\n:"); //met l'etiquette de cote
			word = strtok(NULL, " \t\r\n"); //prend la suite (jusqu'a " \t\r\n") et le met dans word
		} else {
			word = strtok(str, " \t\r\n");
		}

		//Teste l'existence de l'instruction
		int functiontest = instructions(word);

		//Met l'argument dans arg
		arg = strtok(NULL, " \t\r\n:");

		//Teste les types d'arguments attendus et le change en entier
		//On attend aucun argument
		if (argtype(word) == 0) {
			if(arg != NULL) {
				printf("ERROR: too many arguments to function ‘%s’ at line %d\n", word, line_n);
				exit(1);
			}
			argint = 0;
		}
		//On attend un nombre
		else if (argtype(word) == 1) {
			if (arg == NULL) {
				printf("ERROR: too few arguments to function ‘%s’ at line %d\n", word, line_n);
				exit(1);
			} else if (is_int(arg) == 0) {
				printf("ERROR: expected ‘int’ argument to function ‘%s’ at line %d\n", word, line_n);
				exit(1);
			} else if (strtok(NULL, " \t\r\n") != NULL) {
				printf("ERROR: too many arguments to function ‘%s’ at line %d\n", word, line_n);
				exit(1);
			}
			argint = atoi(arg);

		}
		//On attend une etiquette
		else if (argtype(word) == 2) {
			if (arg == NULL) {
				printf("ERROR: too few arguments to function ‘%s’ at line %d\n", word, line_n);
				exit(1);
			} else if (is_int(arg) == 1) {
				printf("ERROR: expected ‘char *’ but argument is of type ‘int’ at line %d\n", line_n);
				exit(1);
			} else if (strtok(NULL, " \t\r\n") != NULL) {
				printf("ERROR: too many arguments to function ‘%s’ at line %d\n", word, line_n);
				exit(1);
			}
			argint = findLabel(arg, label_tab) - line_n; //difference entre la position actuelle et de l'etiquette
		}

		//printf de verification
		//printf("instruction : %s // argument : %s\n", word, arg);
		//printf("code intruction : %02X // arg = %08X \n\n",instructions(word), argint);

		//ecriture sur le fichier de sortie file2
		fprintf(file2,"%02X %08X\n",instructions(word), argint);
	}
	fclose(file);
	fclose(file2);

	free(label_tab); //Liberation de la memoire
	printf("It's OKAY !! :D");
    return 0;
}

/*
Copyright © 2014, Philippe NGUYEN & Charlie Saillard
*/
