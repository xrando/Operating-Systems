#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int generateRandomNumber(int, int);
void generateRandomArray(int numbers[], int size, int min, int max, int maxDuplicates);
void displayHeader();
void displayFooter();
//void displayTable(int [],int [],int [],int [],int);
void firstComeFirstServeScheduling();
void shortestJobFirst();
void shortestRemainingTimeFirst();
void roundRobinScheduling();
void priorityScheduling();


int generateRandomNumber(int min, int max) 
{
    // Seed the random number generator with current time
    srand(time(NULL));  
    // Generate a random number within the specified range
    int randomNumber = (rand() % (max - min + 1)) + min;  
    
    return randomNumber;
}


/* void generateRandomArray(int numbers[], int size, int min, int max, int maxDuplicates) {
    if (size <= 0 || maxDuplicates <= 0 || maxDuplicates >= size || min > max) {
        printf("Invalid input parameters.\n");
        return;
    }

    srand(time(NULL));
    int count = 0;
    int frequency[max - min + 1];

    // Initialize frequency array to 0
    for (int i = 0; i < max - min + 1; i++) {
        frequency[i] = 0;
    }
    while (count < size) {
        int number = (rand() % (max - min)) + min; // Generate a random number within the range

        if (frequency[number - min] < maxDuplicates) {
            numbers[count] = number;
            frequency[number - min]++;
            count++;
        }
    }
} */


void generateRandomArray(int numbers[], int size, int min, int max, int maxDuplicates) {
    if (size <= 0 || maxDuplicates <= 0 || maxDuplicates >= size || min > max) {
        printf("Invalid input parameters.\n");
        return;
    }

    srand(time(NULL));
    int count = 0;
    int frequency[max - min + 1];

    // Initialize frequency array to 0
    for (int i = 0; i < max - min + 1; i++) {
        frequency[i] = 0;
    }

    // Check if min is 0 and generate at least one 0
    if (min == 0) {
        numbers[count] = 0;
        frequency[0 - min]++;
        count++;
    }

    while (count < size) {
        int number = (rand() % (max - min + 1)) + min; // Generate a random number within the range

        if (number != 0 && frequency[number - min] < maxDuplicates) {
            numbers[count] = number;
            frequency[number - min]++;
            count++;
        }
    }
}


void displayHeader()
{
    printf("╔═══════════╦═════════════╦═══════════════╦═════════════════╗\n");
    printf("║ Processes ║ Arrival time║  Burst time   ║    Priority     ║\n");
    printf("╠═══════════╬═════════════╬═══════════════╬═════════════════╣\n");
}
void displayFooter()
{
    printf("╚═══════════╩═════════════╩═══════════════╩═════════════════╝\n");
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
void firstComeFirstServeScheduling() {
    int i, j, temp = 0, ct[10], tat[10], wt[10], at[10], bt[10],priority[10], n=6;
    int processes[6] = {1,2,3,4,5,6};
    float awt = 0, atat = 0;

    generateRandomArray(at, 6, 0, 8, 3);
    generateRandomArray(bt, 6, 3, 10, 3);
    generateRandomArray(priority, 6, 1, 4, 2);

    // Sorting the processes based on arrival time
    for (i = 0; i < n; i++) {
        for (j = 0; j < (n - i - 1); j++) {
            if (at[j] > at[j + 1]) {
                temp = processes[j + 1];
                processes[j + 1] = processes[j];
                processes[j] = temp;

                temp = at[j + 1];
                at[j + 1] = at[j];
                at[j] = temp;

                temp = bt[j + 1];
                bt[j + 1] = bt[j];
                bt[j] = temp;

                temp = priority[j + 1];
                priority[j + 1] = priority[j];
                priority[j] = temp;
            }
        }
    }

    // Calculating completion time
    ct[0] = at[0] + bt[0];
    for (i = 1; i < n; i++) {
        temp = 0; // Reset temp to 0 inside the loop
        if (ct[i - 1] < at[i]) {
            temp = at[i] - ct[i - 1];
        }
        ct[i] = ct[i - 1] + bt[i] + temp;
    }

    // Calculating turnaround time and waiting time
    for (i = 0; i < n; i++) {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
        atat += tat[i];
        awt += wt[i];
    }

    displayHeader();
    for (i = 0; i < n; i++) {
        printf("║    P%2d    ║     %2d      ║      %2d       ║       %2d        ║\n",
               processes[i], at[i], bt[i], priority[i]);
    }
    displayFooter();

    atat = atat / n;
    awt = awt / n;
    printf("\nAverage turnaround time: %.3f\n", atat);
    printf("Average waiting time: %.3f\n", awt);
}
//Shortest Job First
void shortestJobFirst()
{
    int n = 6;
    int bt[n], art[n], wt[n], tat[n],priority[n];
    int total_wt = 0, total_tat = 0;

    //populate array with random data
    generateRandomArray(art, 6, 0, 8, 3);
    generateRandomArray(bt, n, 3, 10, 3);
    generateRandomArray(priority, n, 1, 4, 2);
    // Calculate waiting time
    int rt[n];
    for (int i = 0; i < n; i++)
        rt[i] = bt[i];

    int complete = 0, t = 0;
    int shortest = -1, finish_time;

    // Process until all processes get completed
    while (complete != n) {
        shortest = -1;
        int min_bt = INT_MAX;

        // Find the process with the minimum remaining time
        for (int j = 0; j < n; j++) {
            if (art[j] <= t && rt[j] < min_bt && rt[j] > 0) {
                if (art[j] == 0) {
                    shortest = j;
                    min_bt = rt[j];
                    break;
                }
                if (shortest == -1 || rt[j] < rt[shortest]) {
                    shortest = j;
                    min_bt = rt[j];
                }
            }
        }

        if (shortest == -1) {
            t++;
            continue;
        }

        // Reduce the remaining time by one
        rt[shortest]--;

        // If a process gets completely executed
        if (rt[shortest] == 0) {
            complete++;

            // Find the finish time of the current process
            finish_time = t + 1;

            // Calculate the waiting time
            wt[shortest] = finish_time - bt[shortest] - art[shortest];
            if (wt[shortest] < 0)
                wt[shortest] = 0;
        }

        // Increment time
        t++;
    }

    // Calculate turnaround time
    for (int i = 0; i < n; i++)
        tat[i] = bt[i] + wt[i];

    // Display processes along with all details
    displayHeader();
    for (int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
        printf("║    P%2d    ║     %2d      ║      %2d       ║       %2d        ║\n",
               i+1, art[i], bt[i], priority[i]);
    }
    displayFooter();
    // Calculate average waiting time and average turnaround time
    float avg_wt = (float)total_wt / (float)n;
    float avg_tat = (float)total_tat / (float)n;
    printf("\nAverage turnaround time = %.3f", avg_tat);
    printf("\nAverage waiting time = %.3f\n", avg_wt);
}
//Shortest remaining time first
void shortestRemainingTimeFirst()
{
    int arrival_time[10], burst_time[10], temp[10], priority[10];
    int i, smallest, count = 0, time, limit=6;
    double wait_time = 0, turnaround_time = 0, end;
    float average_waiting_time, average_turnaround_time;
    //populate arrays with random data
    generateRandomArray(arrival_time, 6, 0, 8, 3);
    generateRandomArray(burst_time, 6, 3, 10, 3);
    generateRandomArray(priority, 6, 1, 4, 2);
    for(i=0;i<limit;i++)
    {
        temp[i] = burst_time[i];
    }

    burst_time[9] = 9999;

    for (time = 0; count != limit; time++)
    {
        smallest = 9;
        for (i = 0; i < limit; i++)
        {
            if (arrival_time[i] <= time && burst_time[i] < burst_time[smallest] && burst_time[i] > 0)
            {
                smallest = i;
            }
        }
        burst_time[smallest]--;
        if (burst_time[smallest] == 0)
        {
            count++;
            end = time + 1;
            wait_time = wait_time + end - arrival_time[smallest] - temp[smallest];
            turnaround_time = turnaround_time + end - arrival_time[smallest];
        }
    }

    average_waiting_time = wait_time / limit;
    average_turnaround_time = turnaround_time / limit;
    displayHeader();
    for (i = 0; i < limit; i++) {
        printf("║    P%2d    ║     %2d      ║      %2d       ║       %2d        ║\n",
               i+1, arrival_time[i], temp[i], priority[i]);
    }
    displayFooter();
    printf("Average Turnaround Time: %.3f", average_turnaround_time);
    printf("\nAverage Waiting Time: %.3f\n", average_waiting_time);

}

//Round robin (only works for arrival time = 0)
void roundRobinScheduling() 
{
    // Input no of processed
    int n=6;
    int wait_time = 0, ta_time = 0, arr_time[n], burst_time[n], temp_burst_time[n], time_slot=2, priority[n];
    int x = n;

    //populate arrays with random data
    generateRandomArray(arr_time, 6, 0, 8, 3);
    generateRandomArray(burst_time, 6, 3, 10, 3);
    generateRandomArray(priority, 6, 1, 4, 2);

    // Input details of processes
    for(int i = 0; i < n; i++)
    {
        //arr_time[i]=0;
        temp_burst_time[i] = burst_time[i];
    }
    // Total indicates total time
    // Counter indicates which process is executed
    int total = 0, counter = 0, i;
    displayHeader();
    for(total = 0, i = 0; x != 0; )
    {
        // Define the conditions
        if(temp_burst_time[i] <= time_slot && temp_burst_time[i] > 0)
        {
            total = total + temp_burst_time[i];
            temp_burst_time[i] = 0;
            counter = 1;
        }
        else if(temp_burst_time[i] > 0)
        {
            temp_burst_time[i] = temp_burst_time[i] - time_slot;
            total += time_slot;
        }
        if(temp_burst_time[i] == 0 && counter == 1)
        {
            x--; // Decrement the process no.
            printf("║    P%2d    ║     %2d      ║      %2d       ║       %2d        ║\n",
               i+1, arr_time[i], burst_time[i], priority[i]);
            wait_time = wait_time + total - arr_time[i] - burst_time[i];
            ta_time += total - arr_time[i];
            counter = 0;
        }
        if(i == n - 1)
        {
            i = 0;
        }
        else if(arr_time[i + 1] <= total)
        {
            i++;
        }
        else
        {
            i = 0;
        }
    }
    displayFooter();
    float average_wait_time = wait_time * 1.0 / n;
    float average_turnaround_time = ta_time * 1.0 / n;
    printf("\nAvg Turnaround Time:%.3f", average_turnaround_time);
    printf("\nAverage Waiting Time:%.3f\n", average_wait_time);
}


//test data
//arrival time {0 1 3 2 4}
//burst time {3 6 1 2 4}
//priorities {3 4 9 7 8}
void priorityScheduling()
{
    int proc[6][4];
    int arrivaltime[6]/*  = {1, 2, 3, 4, 5} */;
    int bursttime[6]/*  = {3, 5, 1, 7, 4} */;
    int priority[6]/*  = {3, 4, 1, 7, 8} */;
    int totalprocess=6;

    //populate arrays with random data
    generateRandomArray(arrivaltime, 6, 0, 8, 3);
    generateRandomArray(bursttime, 6, 3, 10, 3);
    generateRandomArray(priority, 6, 1, 4, 2);

    for (int i = 0; i < totalprocess; i++) {
        proc[i][0] = arrivaltime[i];
        proc[i][1] = bursttime[i];
        proc[i][2] = priority[i];
        proc[i][3] = i + 1;
    }

    // Using FCFS Algorithm to find Waiting time
    int* wt = (int*)malloc(totalprocess * sizeof(int));
    int* tat = (int*)malloc(totalprocess * sizeof(int));
    int* service = (int*)malloc(totalprocess * sizeof(int));
    int wavg = 0;
    int tavg = 0;

    service[0] = 0;
    wt[0] = 0;

    for (int i = 1; i < totalprocess; i++) {
        service[i] = proc[i - 1][1] + service[i - 1];
        wt[i] = service[i] - proc[i][0] + 1;

        if (wt[i] < 0) {
            wt[i] = 0;
        }
    }

    // Filling turnaround time array
    for (int i = 0; i < totalprocess; i++) {
        tat[i] = proc[i][1] + wt[i];
    }

    int* stime = (int*)malloc(totalprocess * sizeof(int));
    int* ctime = (int*)malloc(totalprocess * sizeof(int));
    stime[0] = 1;
    ctime[0] = stime[0] + tat[0];

    // calculating starting and ending time
    for (int i = 1; i < totalprocess; i++) {
        stime[i] = ctime[i - 1];
        ctime[i] = stime[i] + tat[i] - wt[i];
    }

    printf("Process_no\tStart_time\tComplete_time\tTurn_Around_Time\tWaiting_Time\n");

    // Display the process details
    for (int i = 0; i < totalprocess; i++) {
        wavg += wt[i];
        tavg += tat[i];

        printf("%d\t\t%d\t\t", proc[i][3], stime[i]);
        printf("%d\t\t%d\t\t\t%d\n", ctime[i], tat[i], wt[i]);
    }

    // Display the average waiting time and average turn around time
    printf("Average waiting time is: %f\n", (float)wavg / totalprocess);
    printf("Average turnaround time: %f\n", (float)tavg / totalprocess);

    // Free dynamically allocated memory
    free(wt);
    free(tat);
    free(service);
    free(stime);
    free(ctime);
}

//Priority scheduling (only works for arrival time = 0)
/* void priorityScheduling() {
    int n=6, i;
    int arrivalTime[10];
    int burstTime[10];
    int priority[10];
    int temp[10];
    int time = 0;
    int count = 0;
    int shortestProcess;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;
    float averageWaitingTime, averageTurnaroundTime;
    //populate array with random data
    //generateRandomArray(arrivalTime, 6, 0, 8, 3);
    generateRandomArray(burstTime, n, 3, 10, 3);
    generateRandomArray(priority, n, 1, 4, 2);
    for (i = 0; i < n; i++)
    {
        arrivalTime[i] = 0;
        temp[i] = burstTime[i]; // adding a duplicate of the burst time to a temporary array
    }

    // Scheduling algorithm
    while (count != n)
    {
        shortestProcess = -1;
        for (i = 0; i < n; i++)
        {
            if (burstTime[i] > 0 && arrivalTime[i] <= time && (shortestProcess == -1 || priority[i] < priority[shortestProcess]))
            {
                shortestProcess = i;
            }
        }

        burstTime[shortestProcess]--;

        if (burstTime[shortestProcess] == 0)
        {
            count++;

            int waitingTime = time + 1 - arrivalTime[shortestProcess] - temp[shortestProcess];
            int turnaroundTime = time + 1 - arrivalTime[shortestProcess];

            totalWaitingTime += waitingTime;
            totalTurnaroundTime += turnaroundTime;
        }

        time++;
    }

    averageWaitingTime = totalWaitingTime / n;
    averageTurnaroundTime = totalTurnaroundTime / n;
    displayHeader();
    for (i = 0; i < n; i++) {
        printf("║    P%2d    ║     %2d      ║      %2d       ║       %2d        ║\n",
               i+1, arrivalTime[i], temp[i], priority[i]);
    }
    displayFooter();

    printf("Avg turn around time is %.3f\n", averageTurnaroundTime);
    printf("Avg waiting time is %.3f\n", averageWaitingTime);
} */

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
                    firstComeFirstServeScheduling();
                    break;
                case 2:
                    printf("You selected Shortest job first scheduling.\n");
                    shortestJobFirst();
                    break;
                case 3:
                    printf("You selected Shortest remaining time first scheduling.\n");
                    shortestRemainingTimeFirst();
                    break;
                case 4:
                    printf("You selected Round robin scheduling.\n");
                    roundRobinScheduling();
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
