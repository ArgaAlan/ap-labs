#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void merge(void *lineptr[], int l, int m, int r, int (*comp)(void *, void *))
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    char *L[n1], *R[n2];

    for (i = 0; i < n1; i++)
    {
        L[i] = lineptr[l + i];
    }

    for (j = 0; j < n2; j++)
    {
        R[j] = lineptr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2)
    {
        if ((*comp)(L[i], R[j]) < 0)
        {
            lineptr[k] = L[i];
            i++;
        }
        else
        {
            lineptr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        lineptr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        lineptr[k] = R[j];
        j++;
        k++;
    }
}

int numcmp(const char *s1, const char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);

    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else
        return 0;
}

void sort(void *lineptr[], int l, int r, int (*comp)(void *, void *))
{
    if (l < r)
    {
        int m = l + (r - l) / 2;

        sort(lineptr, l, m, (*comp));
        sort(lineptr, m + 1, r, (*comp));

        merge(lineptr, l, m, r, (*comp));
    }
}

int main(int argc, char *argv[])
{
    //printf("%d\n", argc);
    int numeric = 0; //1 if numeric sort
    if (argc > 1 && isdigit(argv[1][0]))
    {
        numeric = 1;
    }

    if (argc > 0)
    {

        char *lineptr[argc];

        for (int i = 0; i < argc - 1; i++)
        {
            lineptr[i] = argv[i + 1];
        }
        sort((void **)lineptr, 0, argc - 1, (int (*)(void *, void *))(numeric ? numcmp : strcmp));
        if (numeric)
        {
            for (int i = 1; i < argc; i++)
            {
                printf("%s ", lineptr[i]);
            }
            printf("\n");
        }
        else
        {
            for (int i = 1; i < argc; i++)
            {
                printf("%s ", lineptr[i]);
            }
            printf("\n");
        }
        /*
        for (int i = 1; i < argc; i++)
        {
            printf("%s ,", lineptr[i]);
        }
        */
    }

    return 0;
}

