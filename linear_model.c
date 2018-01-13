#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 1000
#define DELIMITER ","
#define NUM_ITERATIONS 100000

FILE *read(FILE *file_p, int argc, char *argv[])
{
	if (argc != 2) {
		printf("No filename given\n");
		printf("usage: %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	file_p = fopen(argv[1],"r");
	if (file_p == NULL) {
		printf("File \"%s\" could not be opened\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	return file_p;
}

char *str_tok(char *s, const char *delim)
{
    static char *lasts;
    register int ch;

    if (s == NULL) {
    	s = lasts;
    }
    ch = *s;
    if (ch == '\0')
        return 0;
    lasts = s + strcspn(s, delim); 
    if (*lasts != 0) 
    *lasts++ = 0;

    return s;
}
 
char **split(char *string, int *num_entries, char *sep) 
{
    char *tok;
    char **split_string = 0;
    int i = 0;
    tok = str_tok(string, sep);

    while (tok != 0) {
        split_string = realloc(split_string,(i + 1)*sizeof(char *));
        split_string[i] = malloc(strlen(tok) + 1);
        strcpy(split_string[i], tok);
        ++i;
        tok = str_tok(NULL, sep);
    }
	*num_entries = i;

    return split_string;
}

double square(double x)
{
	return (double)(x * x);
}

double tol_err(double *beta_0,double *beta_1,double *x,double *y)
{
	double total_error = 0;
	int count = 0;
	while (*x && *y) {
		total_error += square(*y - (*beta_0 + (*beta_1) * (*x)));
		x++;
		y++;
		count++;
	}
	return total_error/((double) count);
}

void gradient_decent(double *beta_0, double *beta_1, double *x, double *y, int
num_entries,double learning_rate)
{
	double beta_0_gradient = 0, beta_1_gradient = 0;

	while (*x && *y) {
		beta_0_gradient += -(2/num_entries) * (*y - ((*beta_0 + (*beta_1) *
		(*x))));
		beta_1_gradient += -(2/num_entries) * (*x) * (*y -((*beta_0 + (*beta_1) *
		(*x))));
		x++;
		y++;
	}
	*beta_0 = *beta_0 - (learning_rate * (beta_0_gradient));
	*beta_1 = *beta_1 - (learning_rate * (beta_1_gradient));
}

int main(int argc, char *argv[])
{
	FILE *file_p;
	char s[MAXCHAR], **entries, *delimiter;
	double beta_0, beta_1, *x, *y;
	int i = 0, num_entries = 0, iter = 0;
	
	x = (double *)malloc(10 * sizeof(double));
	y = (double *)malloc(10 * sizeof(double));
	
	file_p = read(file_p, argc, argv);
	while ((fgets(s,MAXCHAR,file_p)) != NULL) {
		entries = split(s, &num_entries, DELIMITER);
		if (i > 9) {
			x = (double *)realloc(x, (i + 1)*sizeof(double));
			y = (double *)realloc(y, (i + 1)*sizeof(double));
		}
		*(x + i) = atof(entries[0]);
		*(y + i) = atof(entries[1]);
		i++;
	}
	
	for (iter = 0; iter < NUM_ITERATIONS; iter++) {
		if (iter % 10 == 0) {
			printf("loss: %f\n",tol_err(&beta_0,&beta_1,x,y));
		}
		gradient_decent(&beta_0,&beta_1,x,y,num_entries,0.00001);
	}
	printf("total loss: %f\n",tol_err(&beta_0,&beta_1,x,y));
	printf("intercept: %f gradient:  %f\n", beta_0, beta_1);
	fclose(file_p);

	return EXIT_SUCCESS;
}
