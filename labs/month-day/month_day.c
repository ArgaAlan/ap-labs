#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday)
{
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    printf("%s %d, %d\n", months[*pmonth], *pday, year);
};

int main(int argc, char **argv)
{

    int day = atoi(argv[2]);
    int year = atoi(argv[1]);
    if (day > 365 || day < 1)
    {
        printf("Incorrect input");
    }
    else
    {
        int daysMonths[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int pmonth = 0;
        int pday = 0;
        for (int i = 0; i < 12; i++)
        {
            if (day <= daysMonths[i])
            {
                pmonth = i;
                pday = day;
                break;
            }
            else
            {
                day -= daysMonths[i];
            }
        }
        month_day(year, day, &pmonth, &pday);
    }

    return 0;
}
