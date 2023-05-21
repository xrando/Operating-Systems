#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateRandomNumber(int, int);
void displayHeader();
void displayFooter();
//void displayTable(int [],int [],int [],int [],int);
void firstComeFirstServeScheduling(int[],int,int[]);
void shortestJobFirst();
void shortestRemainingTimeFirst(int processes[][3], int);
void roundRobinScheduling(int processes[], int, int bt[], int);
void priorityScheduling();


int generateRandomNumber(int min, int max) 
{
    // Seed the random number generator with current time
    srand(time(NULL));  
    // Generate a random number within the specified range
    int randomNumber = (rand() % (max - min + 1)) + min;  
    
    return randomNumber;
}

void displayHeader()
{
    printf("╔═══════════╦═══════════╦═══════════════╦═════════════════╗\n");
    printf("║ Processes ║ Burst time║ Waiting time  ║ Turnaround time ║\n");
    printf("╠═══════════╬═══════════╬═══════════════╬═════════════════╣\n");
}
void displayFooter()
{
    printf("╚═══════════╩═══════════╩═══════════════╩═════════════════╝\n");
}
/* void displayTable(int processes[], int bt[], int wt[], int tat[], int n) {
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
} */

//First come first serve
void firstComeFirstServeScheduling(int processes[], int n, int bt[]) 
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

    displayHeader();
    for (int i = 0; i < n; i++) {
        printf("║    P%2d    ║     %2d    ║      %2d       ║       %2d        ║\n",
               processes[i], bt[i], wt[i], tat[i]);
        total_wt += wt[i];
        total_tat += tat[i];
    }
    displayFooter();
    int avg_wt = (float)total_wt / (float)n;
    int avg_tat = (float)total_tat / (float)n;
    printf("Average waiting time = %d\n", avg_wt);
    printf("Average turnaround time = %d\n", avg_tat);
}

//Shortest job first
void shortestJobFirst() 
{
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
    displayHeader();
    for (i = 0; i < n; i++) 
    {
        A[i][3] = A[i][1] + A[i][2];
        total += A[i][3];
        printf("║    P%2d    ║     %2d    ║      %2d       ║       %2d        ║\n",
               A[i][0], A[i][1], A[i][2], A[i][3]);
    }
    displayFooter();
    avg_tat = (float)total / n;
    printf("Average Waiting Time= %f\n", avg_wt);
    printf("Average Turnaround Time= %f\n", avg_tat);
}

//Shortest remaining time first
void shortestRemainingTimeFirst(int processes[][3], int n) {
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
    displayHeader();
    for (int i = 0; i < n; i++) {
        printf("║    P%2d    ║     %2d    ║      %2d       ║       %2d        ║\n",
               processes[i][0], processes[i][1], wt[i], tat[i]);
    }
    displayFooter();

    printf("\nAverage waiting time = %.5f\n", (float)total_wt / n);
    printf("Average turnaround time = %.5f\n", (float)total_tat / n);
}

//Round robin scheduling
void roundRobinScheduling(int processes[], int n, int bt[], int quantum) 
{
	int wt[n], tat[n];
	int rem_bt[n], t = 0, done = 0;

	// Copy the burst time into rem_bt[]
	for (int i = 0; i < n; i++) {
		rem_bt[i] = bt[i];
	}

	// Keep traversing processes in round-robin manner until all of them are not done.
	while (1) {
		done = 1;
		// Traverse all processes one by one repeatedly
		for (int i = 0; i < n; i++) {
			// If burst time of a process is greater than 0, process further
			if (rem_bt[i] > 0) {
				done = 0; // There is a pending process

				if (rem_bt[i] > quantum) {
					t += quantum; // Increase the value of t i.e. shows how much time a process has been processed
					rem_bt[i] -= quantum; // Decrease the burst_time of the current process by quantum
				}
				else {
					t += rem_bt[i]; // Increase the value of t i.e. shows how much time a process has been processed
					wt[i] = t - bt[i]; // Waiting time is current time minus time used by this process
					rem_bt[i] = 0; // As the process gets fully executed, make its remaining burst time = 0
				}
			}
		}

		// If all processes are done, break the loop
		if (done == 1) {
			break;
		}
	}

	// Calculating turnaround time
	for (int i = 0; i < n; i++) {
		tat[i] = bt[i] + wt[i];
	}

	// Display processes along with all details
	double total_wt = 0, total_tat = 0;
    displayHeader();
	for (int i = 0; i < n; i++) {
		total_wt += wt[i];
		total_tat += tat[i];
		printf("║    P%2d    ║     %2d    ║      %2d       ║       %2d        ║\n",
         processes[i], bt[i], wt[i], tat[i]);
	}
    displayFooter();
	printf("\nAverage waiting time = %.5f\n", total_wt / n);
	printf("Average turn around time = %.5f\n", total_tat / n);
}

//Priority scheduling
void priorityScheduling()
{
    int bt[20], p[20], wt[20], tat[20], pr[20], i, j, n, total = 0, pos, temp, avg_wt, avg_tat;
    
    printf("Enter Total Number of Processes:");
    scanf("%d", &n);
    
    printf("\nEnter Burst Time and Priority\n");
    for (i = 0; i < n; i++)
    {
        printf("\nP[%d]\n", i + 1);
        printf("Burst Time:");
        scanf("%d", &bt[i]);
        printf("Priority:");
        scanf("%d", &pr[i]);
        p[i] = i + 1; // contains process number
    }
    
    // Sorting burst time, priority, and process number in ascending order using selection sort
    for (i = 0; i < n; i++)
    {
        pos = i;
        for (j = i + 1; j < n; j++)
        {
            if (pr[j] < pr[pos])
                pos = j;
        }
        
        temp = pr[i];
        pr[i] = pr[pos];
        pr[pos] = temp;
        
        temp = bt[i];
        bt[i] = bt[pos];
        bt[pos] = temp;
        
        temp = p[i];
        p[i] = p[pos];
        p[pos] = temp;
    }
    
    wt[0] = 0; // Waiting time for the first process is zero
    
    // Calculate waiting time
    for (i = 1; i < n; i++)
    {
        wt[i] = 0;
        for (j = 0; j < i; j++)
            wt[i] += bt[j];
        total += wt[i];
    }
    
    avg_wt = total / n; // Average waiting time
    total = 0;
    
    displayHeader();
    
    for (i = 0; i < n; i++)
    {
        tat[i] = bt[i] + wt[i]; // Calculate turnaround time
        total += tat[i];
        printf("║    P%2d    ║     %2d    ║      %2d       ║       %2d        ║\n",
         p[i], bt[i], wt[i], tat[i]);
    }
    displayFooter();
    avg_tat = total / n; // Average turnaround time
    printf("\n\nAverage Waiting Time=%d", avg_wt);
    printf("\nAverage Turnaround Time=%d\n", avg_tat);
}



int main() {
    int userInput;

    do {
        printf("Enter 1 to continue or 2 to quit: ");
        scanf("%d", &userInput);

        if (userInput == 1) 
        {
            printf("Enter a number between 1 and 5: ");
            scanf("%d", &userInput);

            switch (userInput) 
            {
                case 1:
                    printf("You selected First come first serve scheduling.\n");
                    int processes[] = {1, 2, 3};
                    int n = sizeof(processes) / sizeof(processes[0]);
                    int burst_time[] = {10, 5, 8};
                    firstComeFirstServeScheduling(processes, n, burst_time);
                    break;
                case 2:
                    printf("You selected Shortest job first scheduling.\n");
                    shortestJobFirst();
                    break;
                case 3:
                    printf("You selected Shortest remaining time first scheduling.\n");
                    // Process id's, burst time, and arrival time
                    int proc[][3] = {{1, 6, 1}, {2, 8, 1}, {3, 7, 2}, {4, 3, 3}};
                    int m = sizeof(proc) / sizeof(proc[0]);
                    shortestRemainingTimeFirst(proc, m);
                    break;
                case 4:
                    printf("You selected Round robin scheduling.\n");
                    // Process ids
	                int process[] = { 1, 2, 3 };
	                int l = sizeof(proc) / sizeof(proc[0]);
	                // Burst time of all processes
	                int burstTime[] = { 10, 5, 8 };
	                // Time quantum
	                int quantum = 2;
	                roundRobinScheduling(process, l, burstTime, quantum);
                    break;
                case 5:
                    printf("You selected Priority scheduling.\n");
                    priorityScheduling();
                    break;
                default:
                    printf("Invalid input. Please enter a number between 1 and 5.\n");
                    break;
            }
        } 
        else if (userInput == 2) 
        {
            printf("Quit the program.\n");
        } 
        else 
        {
            printf("Invalid choice. Please try again.\n");
        }
    } 
    while (userInput != 2);



    
    

    return 0;
}
