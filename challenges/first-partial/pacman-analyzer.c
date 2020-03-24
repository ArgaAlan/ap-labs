#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define REPORT_FILE "packages_report.txt"

struct Program
{
    char *name;
    char *install;
    char *update;
    int nUpdates;
    char *removal;
};

struct Program programs[1000];
int totalPrograms, numPrograms, upPrograms, remPrograms;

char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 2);
    strcpy(result, s1);
    strcat(result, " ");
    strcat(result, s2);
    return result;
}

char* readLine(int rd){
    int size = 50;
    char *line;
    line = calloc(size, sizeof(char));
    int acum = 0;
    int n;
    while((n = read(rd, line+acum, size)) > 0){
        for(int i=acum;i<acum+n;i++){
            if(line[i] == '\n'){
                lseek(rd, i-(acum+n)+1, SEEK_CUR);
                line[i] = '\0';
                return line;
            }
        }
        acum+=n;
        line = realloc(line, acum+size);
    }
    if(acum>0){
        line[acum] = '\0';
        return line;
    }
    return NULL;
}

char **linetoSentence(char *string) //La linea que lei la paso a un array de palabras
{
    char **words = calloc(20, sizeof(char *)); //Array de strings de tamaño 20 y 30 letras por palabra
    for (int i = 0; i < 20; i++)
    {
        words[i] = calloc(100, sizeof(char));
    }
    int j = 0;
    int word = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == ' ')
        {
            words[word][j] = '\0';
            word++;
            j = 0;
        }
        else if (string[i] == '\0')
        {
            return words;
        }
        else
        {
            if (string[i] != '[' && string[i] != ']')
            { //Elimino los corchetes para poder manejar mejor la fecha
                words[word][j] = string[i];
                j++;
            }
        }
    }
    printf("\n");
    return words;
}

void createReport(int idW)
{
    char number[10];
    int x;
    write(idW, "Pacman Packages Report\n", 23);
    write(idW, "----------------------\n", 23);
    write(idW, "- Installed packages : ", 23);
    x = sprintf(number, "%d\n", totalPrograms);
    write(idW, number, x);
    write(idW, "- Removed packages   : ", 23);
    x = sprintf(number, "%d\n", remPrograms);
    write(idW, number, x);
    write(idW, "- Upgraded packages  : ", 23);
    x = sprintf(number, "%d\n", upPrograms);
    write(idW, number, x);
    write(idW, "- Current installed  : ", 23);
    x = sprintf(number, "%d\n", numPrograms);
    write(idW, number, x);

    write(idW, "\nList of packages\n----------------\n", 35);
    for (int i = 0; i < totalPrograms; i++)
    {
        write(idW, "- Package Name        : ", 24);
        write(idW, programs[i].name, strlen(programs[i].name));
        write(idW, "\n  - Install date      : ", 25);
        write(idW, programs[i].install, strlen(programs[i].install));
        write(idW, "\n  - Last update date  : ", 25);
        if (programs[i].update != NULL)
        {
            write(idW, programs[i].update, strlen(programs[i].update));
        }
        else
        {
            write(idW, "-", 1);
        }
        write(idW, "\n  - How many updates  : ", 25);
        x = sprintf(number, "%d\n", programs[i].nUpdates);
        write(idW, number, x);
        write(idW, "  - Removal date      : ", 24);
        if (programs[i].removal != NULL)
        {
            write(idW, programs[i].removal, strlen(programs[i].removal));
        }
        else
        {
            write(idW, "-", 1);
        }
        write(idW, "\n", 1);
    }
    close(idW);
}

void analizeLog(char *logFile, char *report)
{
    printf("Generating Report from: [%s] log file\n", logFile);
    int idR = open(logFile, O_RDONLY);
    if (idR < 0)
    {
        printf("Input file not found.\n");
        return;
    }
    char *line;
    char **words;
    while ((line = readLine(idR)) != NULL)
    {
        words = linetoSentence(line);
        if (strcmp("installed", words[3]) == 0)
        {
            programs[totalPrograms].name = words[4];
            programs[totalPrograms].install = concat(words[0], words[1]);
            totalPrograms++;
            numPrograms++;
        }
        else if (strcmp("upgraded", words[3]) == 0)
        {
            char *prog = words[4];
            int notComplete = 1;
            for (int i = 0; i < totalPrograms; i++)
            {
                if (strcmp(prog, programs[i].name) == 0)
                {
                    programs[i].update = concat(words[0], words[1]);
                    programs[i].nUpdates++;
                    upPrograms++;
                    notComplete = 1;
                }
            }
            if (notComplete)
            {
                printf("Program not found, can't be upgraded.\n");
            }
	    free(prog);
        }
        else if (strcmp("removed", words[3]) == 0)
        {
            char *prog = words[4];
            int notComplete = 1;
            for (int i = 0; i < totalPrograms; i++)
            {
                if (strcmp(prog, programs[i].name) == 0)
                {
                    programs[i].removal = concat(words[0], words[1]);
                    numPrograms--;
                    remPrograms++;
                    notComplete = 1;
                }
            }
            if (notComplete)
            {
                printf("Program not found, can't be removed.\n");
            }
            free(prog);
        }
        free(line);
        free(words);
    }

    close(idR);
    //Comienza la escritura del reporte
    int idW = open(report, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (idW < 0)
    {
        printf("Output file not found.\n");
        return;
    }

    createReport(idW);
    close(idW);
    printf("Report is generated at: [%s]\n", report);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage:./pacman-analizer.o pacman.log\n");
        return 1;
    }
    analizeLog(argv[1], REPORT_FILE);
    return 0;
}

