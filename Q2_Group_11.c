#include <stdio.h>
void FCFS(int[],int,int[]);
void displayTable(int [],int [],int [],int [],int);
void SRTF(int processes[][3], int n);
void FCFS(int processes[], int n, int bt[]) 
{
    int wt[n], tat[n], total_wt = 0, total_tat = 0;

    // Waiting time for the first process is 0
    wt[0] = 0;

    // Calculating waiting time
    for (int i = 1; i < n; i++)
        wt[i] = bt[i - 1] + wt[i - 1];

    // Calculating turnaround time by adding bt[i] + wt[i]
    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];

    displayTable(processes, bt, wt, tat,n);

    int avg_wt = (float)total_wt / (float)n;
    int avg_tat = (float)total_tat / (float)n;
    printf("Average waiting time = %d\n", avg_wt);
    printf("Average turnaround time = %d\n", avg_tat);
}

void displayTable(int processes[], int bt[], int wt[], int tat[], int n) {
    int total_wt = 0, total_tat = 0;

    printf("╔═══════════╦═══════════╦═══════════════╦═════════════════╗\n");
    printf("║ Processes ║ Burst time║ Waiting time  ║ Turnaround time ║\n");
    printf("╠═══════════╬═══════════╬═══════════════╬═════════════════╣\n");

    for (int i = 0; i < n; i++) {
        printf("║     %2d    ║     %2d    ║      %2d       ║       %2d        ║\n",
               processes[i], bt[i], wt[i], tat[i]);
        total_wt += wt[i];
        total_tat += tat[i];
    }

    printf("╚═══════════╩═══════════╩═══════════════╩═════════════════╝\n");
}

void SJF() {
    int A[100][4];
    int i, j, n, total = 0, index, temp;
    float avg_wt, avg_tat;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter Burst Time:\n");
    for (i = 0; i < n; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d", &A[i][1]);
        A[i][0] = i + 1;
    }
    for (i = 0; i < n; i++) {
        index = i;
        for (j = i + 1; j < n; j++)
            if (A[j][1] < A[index][1])
                index = j;
        temp = A[i][1];
        A[i][1] = A[index][1];
        A[index][1] = temp;

        temp = A[i][0];
        A[i][0] = A[index][0];
        A[index][0] = temp;
    }
    A[0][2] = 0;
    for (i = 1; i < n; i++) {
        A[i][2] = 0;
        for (j = 0; j < i; j++)
            A[i][2] += A[j][1];
        total += A[i][2];
    }
    avg_wt = (float)total / n;
    total = 0;
    printf("\n");
    printf("╔═══════════╦═══════════════╦═════════════════╦═════════════════╗\n");
    printf("║ Processes ║ Burst time    ║ Waiting time    ║ Turnaround time ║\n");
    printf("╠═══════════╬═══════════════╬═════════════════╬═════════════════╣\n");
    for (i = 0; i < n; i++) {
        A[i][3] = A[i][1] + A[i][2];
        total += A[i][3];
        printf("║     %2d    ║      %4d     ║       %4d      ║       %5d     ║\n",
               A[i][0], A[i][1], A[i][2], A[i][3]);
    }
    printf("╚═══════════╩═══════════════╩═════════════════╩═════════════════╝\n");
    avg_tat = (float)total / n;
    printf("Average Waiting Time= %f\n", avg_wt);
    printf("Average Turnaround Time= %f\n", avg_tat);
}

void SRTF(int processes[][3], int n) {
    int wt[n], tat[n], rt[n];
    int total_wt = 0, total_tat = 0;

    // Copy the burst time into rt[]
    for (int i = 0; i < n; i++) {
        rt[i] = processes[i][1];
    }

    int complete = 0;
    int t = 0;
    int minm = 999999999;
    int shortest = 0;
    int check = 0;

    // Process until all processes get completed
    while (complete != n) {
        // Find process with minimum remaining time among the processes that arrives till the current time
        for (int j = 0; j < n; j++) {
            if (processes[j][2] <= t && rt[j] < minm && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = 1;
            }
        }

        if (check == 0) {
            t++;
            continue;
        }

        // Reduce remaining time by one
        rt[shortest]--;

        // Update minimum
        minm = rt[shortest];
        if (minm == 0) {
            minm = 999999999;
        }

        // If a process gets completely executed
        if (rt[shortest] == 0) {
            complete++;
            check = 0;

            // Find finish time of the current process
            int fint = t + 1;

            // Calculate waiting time
            wt[shortest] = fint - processes[shortest][1] - processes[shortest][2];

            if (wt[shortest] < 0) {
                wt[shortest] = 0;
            }

            // Calculating turnaround time
            tat[shortest] = processes[shortest][1] + wt[shortest];

            // Update total waiting time and total turn-around time
            total_wt += wt[shortest];
            total_tat += tat[shortest];
        }

        // Increment time
        t++;
    }

    // Display results in table format
    printf("╔═══════════╦═══════════════╦═════════════════╦═════════════════╗\n");
    printf("║ Processes ║ Burst time    ║ Waiting time    ║ Turnaround time ║\n");
    printf("╠═══════════╬═══════════════╬═════════════════╬═════════════════╣\n");
    for (int i = 0; i < n; i++) {
        printf("║     %2d    ║      %4d     ║     %4d        ║       %5d     ║\n",
               processes[i][0], processes[i][1], wt[i], tat[i]);
    }
    printf("╚═══════════╩═══════════════╩═════════════════╩═════════════════╝\n");

    printf("\nAverage waiting time = %.5f\n", (float)total_wt / n);
    printf("Average turnaround time = %.5f\n", (float)total_tat / n);
}

int main() {
    int userInput;

    printf("Enter a number between 1 and 5: ");
    scanf("%d", &userInput);

    switch (userInput) {
        case 1:
            printf("You selected option 1.\n");
            int processes[] = {1, 2, 3};
            int n = sizeof(processes) / sizeof(processes[0]);
            int burst_time[] = {10, 5, 8};
            FCFS(processes, n, burst_time);
            break;
        case 2:
            printf("You selected option 2.\n");
            SJF();
            break;
        case 3:
            printf("You selected option 3.\n");
            // Process id's, burst time, and arrival time
            int proc[][3] = {{1, 6, 1}, {2, 8, 1}, {3, 7, 2}, {4, 3, 3}};
            int m = sizeof(proc) / sizeof(proc[0]);
            SRTF(proc, m);
            break;
        case 4:
            printf("You selected option 4.\n");
            break;
        case 5:
            printf("You selected option 5.\n");
            break;
        default:
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            break;
    }
    
    

    return 0;
}
