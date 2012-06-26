/*
    Daylight savings time in/not in effect notification
    Copyright (C) 2012 Savvas Radevic <vicedar@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 Compile command: gcc isdstd.c -Wall -o isdstd
 Before running: touch isdst
 Run: ./test
*/

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>

//Timestamp file -- .isdst
#define TSFILE ".isdst"

// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void terminate(int signum)
{
    FILE *fp;
    time_t ternow;
    
    printf("Caught signal %d - Saving timestamp to file\n", signum);
    ternow = time(NULL);
    fp = fopen(TSFILE, "w");
    fprintf(fp, "%ld", ternow);
    fflush(fp);
    fclose(fp);
    exit(signum);
}

int compare_fromfile(time_t now) {
    FILE *fp;
    long lastchk;
    struct tm *tlast;
    int isdst;
    int wasdst;
    struct tm *tm_now;
    
    tm_now = localtime(&now);
    isdst = tm_now->tm_isdst;

    //Create the file
    FILE *fh = fopen(TSFILE, "w");
    fclose(fh);

    //w+ doesn't work with fscanf
    fp = fopen(TSFILE, "r+");
    fscanf(fp, "%ld", &lastchk);
    rewind(fp);
    tlast = localtime(&lastchk);
    wasdst = tlast->tm_isdst;

    printf("Retrieved last check from file: %ld DST: %d\n", lastchk, wasdst);
    if (isdst != wasdst)
    {
        printf("DST changed!\n");
    }

    printf("New value written to file: %ld\n", now);    
    fprintf(fp, "%ld", now);
    fflush(fp);
    fclose(fp);

    // if there was a change in DST (isdst != wasdst), return 1
    if (isdst == wasdst) { return 0; }
    else { return 1; }
}

int compare_live(time_t now, time_t now2) {
    struct tm *tnow2;
    int isdst2;
    int isdst;
    struct tm *tm_now;
    
    tm_now = localtime(&now);
    isdst = tm_now->tm_isdst;
    
    tnow2 = localtime(&now2);
    isdst2 = tnow2->tm_isdst;
    
    // if there was a change in DST (isdst2 != isdst), return 1
    if (isdst2 == isdst) { return 0; }
    else { return 1; }
}

int main(void)
{
    // Register signal and signal handler
    signal(SIGINT, terminate);
    signal(SIGTERM, terminate);
    
    time_t now;
    time_t now2;
    
    int cmpfile;
    int cmplive;

    now = time(NULL);
    // Compare from file (last check)
    cmpfile = compare_fromfile(now);
    if (cmpfile == 1) { printf("ALARM! DST changed since last check!\n"); }
    
    while (1)
    {
        now2 = time(NULL);
        cmplive = compare_live(now, now2);
        if (cmplive == 1)
        {
            printf("LIVE ALARM! DST changed since last live check!\n");
        }
        else
        {
            printf("No changes detected in DST. Sleeping...\n");
        }
        // Prepare for next cycle
        now = now2;
        sleep(2);
    }
    return 0;
}
