#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define T_MAX 256

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

int argtype(char* str){
	/*
		0 = pas d'arg
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

typedef struct{
	char label[T_MAX];
} Label;

typedef struct{
	char Label[T_MAX];
	char inst[T_MAX];
	char arg[T_MAX];
} Line;

int countLines(FILE* file){
	int line_number = 0;
	char str[T_MAX];
	if (file!=NULL){
		while(fgets(str,T_MAX,file)!='\0') {
			printf("line nb = %d\n", line_number);
			line_number++;
		}
	}
	return line_number;
}

Label* createLabelTab(FILE* file, int line_count){
	Label* label_tab = malloc(sizeof(Label)*line_count);
	return label_tab;
}

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

			}
		}
	}
}

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
    FILE*file=NULL;
	FILE*file2=NULL;
    file=fopen("assembleur_test.txt","r+");
	int cl = countLines(file);

	Label* label_tab = createLabelTab(file, cl);

	fclose(file);
	file=fopen("assembleur_test.txt","r+");

	fillLabelTab(file, label_tab);

	fclose(file);
	
	file=fopen("assembleur_test.txt","r+");
	file2=fopen("out.txt","w");
	
	char str[T_MAX];
	char *word;
	char *arg;
	char *useless;
	int argint;
	int line_n = 0;
	if (file!=NULL){
		while(fgets(str,T_MAX,file)) {
			line_n++;
			//fscanf(file, "%s", &word);

			if (strstr(str, ":") != NULL) {
				useless = strtok(str, " \t\r\n:");
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
			fprintf(file2,"%02X%08X \n\n",instructions(word), argint);
		}
	}
	

    return 0;

}


//test2
