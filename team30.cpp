#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* total, * temp;

int cursor = 0;
int line = 1;

int num_t[9][4] = { {1, 2, 3, -1},{-1, 4, 3, -1},{-1, -1, -1, 5},{-1, 3, 3, 5},{-1, -1, -1, 5},{-1, 6, 7, -1},{-1, 8, 7, -1},{-1, 8, 7, -1}, {-1, 8, 7, -1} };

int string_t[3][4] = { {1, -1, -1, -1},{2, 1, 1, 1},{-1, -1, -1, -1} };

int Id_t[2][3] = { {1,-1,1},{1,1,1} };

int ws_t[2][3] = { {1,1,1},{1,1,1} };


struct Token
{
	char* token_name;
	char* token_value;
};

struct Token* token;

int token_num = 0;


int isZero(char c);
int isNonZero(char c);
int isLetter(char c);
int isWS(char c);
int isNumber();
int isString();
int isID();
int isArith();
int isBit();
int isAssign();
int isComp();
int isTermi();
int isComma();
int isLbrac();
int isRbrac();
int isLparen();
int isRparen();
int isComma();
void tokenize(int size);






















int main(int argc, char** argv) {

	int size;

	FILE* fp;
	if (argc == 2) 
		fp = fopen(argv[1], "rb");
	
	else 
		fp = fopen("test.c", "rb");

	if (fp == NULL) 
		printf("Wrong File\n");
	
	else {
		fseek(fp, 0, SEEK_END);


		size = ftell(fp);

		total = (char*)malloc(sizeof(char) * size);

		temp = (char*)malloc(sizeof(char) * size);

		token = (struct Token*)malloc(sizeof(token) * size);

		for (int i = 0; i < size; i++) {
			token[i].token_name = (char*)malloc(sizeof(char) * 30);
			token[i].token_value = (char*)malloc(sizeof(char) * 30);
		}

		fseek(fp, 0, SEEK_SET);

		fread(total, sizeof(char) * size, 1, fp);

		fclose(fp);

		//

		int i = 0;


		while (cursor < size ) {
			tokenize(size);
		}

		for (int cnt = 0; cnt < token_num; cnt++) {
		   printf("%s\t\t%s\n", token[cnt].token_value, token[cnt].token_name);
		}
		if (cursor == size + 1)
		   printf("line : %d \tlexical error", line);

		   
		free(temp);
		free(total);
	}

	FILE* out;
	if (argc == 2) {
		out = fopen(strcat(strtok(argv[1],"."), ".out"), "w");
	}
	else {
		out = fopen("test.out", "w");
	}

	for (int j = 0; j < token_num; j++) {
		fprintf(out, "<%s,%s>\n", token[j].token_name, token[j].token_value);
	}

	if (cursor == size + 1)
		fprintf(out, "\nline : %d    -    lexical error", line);
	fclose(out);
	printf("\n\nFinish");

	return 0;
}




int isZero(char c) {

	if (c == '0')
		return 1;

	return 0;
}


int isNonZero(char c) {

	if (c - '0' > 0 && c - '0' <= 9)
		return 1;

	return 0;
}


int isLetter(char c) {

	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 1;

	return 0;
}


int isWS(char c) {

	int num = 0;

	if (c == '\t' || c == '\r' || c == ' ') {

		return 1;

	}
	else if (c == '\n') {
		line++;
		return 1;
	}


	return 0;
}

int isNumber() {

	int num = 0;

	int i = 0, j;
	int chkWS = 1;
	int first = cursor;
	int before;
	int chkZero = 1;
	int dotCnt = 0;

	while (1) {

		if (total[cursor] == '-') {

			while (isWS(total[cursor - chkWS])) {
				chkWS++;
			}
			if ((cursor == 0 || total[cursor - chkWS] == '*' || total[cursor - chkWS] == '/' || total[cursor - chkWS] == '+')) {
				j = 0;
			}
			else { break; }
		}

		else if (isZero(total[cursor]))
			j = 1;

		else if (isNonZero(total[cursor]))
			j = 2;

		else if (total[cursor] == '.') {
			dotCnt++;
			if (dotCnt < 2)
				j = 3;
			else {
				num--;
				cursor--;
				break;
			}
		}

		else if (total[cursor] == NULL || total[cursor] == '\r')
			break;

		else {
			break;
		}
		before = i;
		temp[num++] = total[cursor++];

		i = num_t[i][j];

		if (i == -1) {      //reject
			num--;
			cursor--;
			if (before == 2 || before == 3) {
				i = 2;
				break;
			}
			else if (before == 6 || before == 7) {
				i = 6;
				break;
			}
			return 0;

		}

	}

	temp[num] = '\0';

	if (num == 0)
		return 0;

	if (i == 2 || i == 3) {      //accept - Int

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "INTEGER");
		return 1;

	}

	else if (i == 6 || i == 7) {      //accept = Float

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "FLOAT");
		return 1;

	}
	else if (i == 8) {
		while (isZero(temp[--num])) {
			cursor--;
			temp[num] = '\0';
		}
		if (temp[num] == '.') {
			temp[num + 1] = '0';
			cursor++;
		}
		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "FLOAT");
		return 1;

	}
	cursor = first;
	return 0;

}


int isString() {

	int num = 0;
	int i = 0, j;
	int count = 0;
	int first = cursor;
	while (1) {

		if (total[cursor] == '"') {
			count++;
			if (count == 2) {
				i = 2;
				temp[num++] = total[cursor++];
				break;
			}
			j = 0;
		}

		else if (isLetter(total[cursor]) && count == 1)
			j = 1;

		else if ((isZero(total[cursor]) || isNonZero(total[cursor])) && count == 1)
			j = 2;

		else if (total[cursor] == ' ' && count == 1)
			j = 3;

		else break;

		temp[num++] = total[cursor++];

		i = string_t[i][j];

		if (i == -1) {

			//reject

			return 0;
		}

	}

	temp[num] = '\0';

	if (i == 2) {
		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "String");
		return 1;

	}
	cursor = first;
	return 0;
}


int isID() {

	int num = 0;
	int i = 0, j;
	int first = cursor;
	int before = 0;

	while (1) {

		if (isLetter(total[cursor]))
			j = 0;

		else if (isZero(total[cursor]) || isNonZero(total[cursor]))
			j = 1;

		else if (total[cursor] == '_')
			j = 2;

		else {
			break;
		}

		temp[num++] = total[cursor++];
		before = i;
		i = Id_t[i][j];

		if (i == -1) {
			break;
		}
	}

	temp[num] = '\0';


	if (i == 1 || i == 2 || i == 3) {

		if (strcmp(temp, "int") == 0 || strcmp(temp, "char") == 0 || strcmp(temp, "float") == 0 || strcmp(temp, "bool") == 0) {

			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "TYPE");
			return 1;   //accept - Type
		}



		else if (strcmp(temp, "if") == 0 || strcmp(temp, "else") == 0 || strcmp(temp, "while") == 0 ||

			strcmp(temp, "for") == 0 || strcmp(temp, "return") == 0) {

			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "KEYWORD");

			return 1;   //accept - Keyword
		}


		else if (strcmp(temp, "false") == 0 || strcmp(temp, "true") == 0) {
			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "BOOL");

			return 1;   //accept - Bool
		}

		else {
			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "ID");
			return 1;
		}
		//accept - Id

	}

	return 0;
}

int isArith() {

	int num = 0;

	if (total[cursor] == '+' || total[cursor] == '-' || total[cursor] == '*' || total[cursor] == '/') {

		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "ARITH");

		return 1;

	}
	return 0;

}







int isBit() {

	int num = 0;
	int first = cursor;

	if (total[cursor] == '<') {

		temp[num++] = total[cursor++];

		if (total[cursor] == '<') {

			temp[num++] = total[cursor++];

			temp[num] = '\0';

			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "BIT");

			return 1;

		}

		num--;
		cursor = first;
	}

	else if (total[cursor] == '>') {

		temp[num++] = total[cursor++];

		if (total[cursor] == '>') {

			temp[num++] = total[cursor++];

			temp[num] = '\0';

			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "BIT");

			return 1;

		}

		num--;
		cursor = first;
	}



	else if (total[cursor] == '&') {
		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "BIT");

		return 1;
	}

	else if (total[cursor] == '|') {
		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "BIT");

		return 1;
	}
	cursor = first;
	return 0;
}







int isAssign() {

	int num = 0;

	if (total[cursor] == '=') {
		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "ASSIGN");

		return 1;

	}

	return 0;

}







int isComp() {
	int num = 0;
	int first = cursor;

	if (total[cursor] == '<') {

		temp[num++] = total[cursor++];

		if (total[cursor] == '=') {

			temp[num++] = total[cursor++];

			temp[num] = '\0';

			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "COMP");

			return 1;

		}

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "COMP");

		return 1;
	}

	else if (total[cursor] == '>') {

		temp[num++] = total[cursor++];

		if (total[cursor] == '=') {

			temp[num++] = total[cursor++];

			temp[num] = '\0';

			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "COMP");

			return 1;

		}

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "COMP");

		return 1;
	}


	else if (total[cursor] == '!') {

		temp[num++] = total[cursor++];

		if (total[cursor] == '=') {
			temp[num++] = total[cursor++];

			temp[num] = '\0';

			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "COMP");

			return 1;
		}
	}


	else if (total[cursor] == '=') {

		temp[num++] = total[cursor++];

		if (total[cursor] == '=') {
			temp[num++] = total[cursor++];

			temp[num] = '\0';

			strcpy(token[token_num].token_value, temp);

			strcpy(token[token_num++].token_name, "COMP");

			return 1;
		}
		cursor--;

	}
	return 0;

}


int isTermi() {

	int num = 0;

	if (total[cursor] == ';') {

		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "TERMINATE");

		return 1;

	}

	return 0;
}

int isLbrac() {

	int num = 0;


	if (total[cursor] == '{') {

		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "LBRACK");

		return 1;

	}

	return 0;

}







int isRbrac() {

	int num = 0;

	if (total[cursor] == '}') {

		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "RBRACK");

		return 1;

	}

	return 0;
}







int isLparen() {

	int num = 0;

	if (total[cursor] == '(') {

		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "LPAREN");

		return 1;

	}

	return 0;
}







int isRparen() {

	int num = 0;

	if (total[cursor] == ')') {

		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "RPAREN");

		return 1;

	}

	return 0;
}







int isComma() {

	int num = 0;

	if (total[cursor] == ',') {

		temp[num++] = total[cursor++];

		temp[num] = '\0';

		strcpy(token[token_num].token_value, temp);

		strcpy(token[token_num++].token_name, "COMMA");

		return 1;

	}

	return 0;
}

void tokenize(int size) {

	if (isNumber()) {}
	else if (isString()) {}
	else if (isID()) {}
	else if (isArith()) {}
	else if (isBit()) {}
	else if (isComma()) {}
	else if (isComp()) {}
	else if (isAssign()) {}
	else if (isLbrac()) {}
	else if (isRbrac()) {}
	else if (isLparen()) {}
	else if (isRparen()) {}
	else if (isTermi()) {}
	else if (isWS(total[cursor])) cursor++;
	else {
		cursor = size + 1;
	}

}
