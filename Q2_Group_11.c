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
    /* // Input no of processed
    int n=6;
    int wait_time = 0, ta_time = 0, arr_time[n], burst_time[n], temp_burst_time[n], time_slot=2, priority[n];
    int x = n;

    //populate arrays with random data
    //generateRandomArray(arr_time, 6, 0, 8, 3);
    generateRandomArray(burst_time, 6, 3, 10, 3);
    generateRandomArray(priority, 6, 1, 4, 2);

    // Input details of processes
    for(int i = 0; i < n; i++)
    {
        arr_time[i]=0;
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
    printf("\nAverage Waiting Time:%.3f\n", average_wait_time); */


    int i, limit=6, total = 0, x, counter = 0, time_quantum=2;
    int wait_time = 0, turnaround_time = 0, arrival_time[10], burst_time[10], temp[10], priority[10];
    float average_wait_time, average_turnaround_time;

    /* printf("Enter Total Number of Processes: ");
    scanf("%d", &limit); */
    x = limit;

    //populate arrays with random data
    //generateRandomArray(arr_time, 6, 0, 8, 3);
    generateRandomArray(burst_time, 6, 3, 10, 3);
    generateRandomArray(priority, 6, 1, 4, 2);

    for(i = 0; i < limit; i++) 
    {
        /* printf("\nEnter Details of Process[%d]\n", i + 1);

        printf("Arrival Time: ");
        scanf("%d", &arrival_time[i]);

        printf("Burst Time: ");
        scanf("%d", &burst_time[i]); */
        arrival_time[i]=0;
        temp[i] = burst_time[i];
    }

    /* printf("\nEnter Time Quantum: ");
    scanf("%d", &time_quantum); */

    /* printf("\nProcess ID\tBurst Time\tTurnaround Time\tWaiting Time\n"); */
    displayHeader();
    for(total = 0, i = 0; x != 0;) {
        if(temp[i] <= time_quantum && temp[i] > 0) {
            total = total + temp[i];
            temp[i] = 0;
            counter = 1;
        }
        else if(temp[i] > 0) {
            temp[i] = temp[i] - time_quantum;
            total = total + time_quantum;
        }
        
        if(temp[i] == 0 && counter == 1) {
            x--;
            //printf("Process[%d]\t%d\t\t%d\t\t%d\n", i + 1, burst_time[i], total - arrival_time[i], total - arrival_time[i] - burst_time[i]);
            printf("║    P%2d    ║     %2d      ║      %2d       ║       %2d        ║\n",
               i+1, arrival_time[i], burst_time[i], priority[i]);
            wait_time = wait_time + total - arrival_time[i] - burst_time[i];
            turnaround_time = turnaround_time + total - arrival_time[i];
            counter = 0;
        }
        if(i == limit - 1) {
            i = 0;
        }
        else if(arrival_time[i + 1] <= total) {
            i++;
        }
        else {
            i = 0;
        }
    }
    displayFooter();
    average_wait_time = wait_time * 1.0 / limit;
    average_turnaround_time = turnaround_time * 1.0 / limit;
    printf("\nAverage Turnaround Time: %.3f\n", average_turnaround_time);
    printf("\nAverage Waiting Time: %.3f", average_wait_time);
}

//sorting issue, process with lower priority num upon 
//previous process completion time cant be sorted properly
/* void priorityScheduling()
{
    int bt[20], at[10], n=6, i, j, temp, p[10], st[10], ft[10], wt[10], ta[10];
    int totwt = 0, totta = 0;
    float awt, ata;
    char pn[10][10]={
        "1",
        "2",
        "3",
        "4",
        "5",
        "6"
    }, t[10];

    printf("Enter the number of processes:");
    scanf("%d", &n);

    for(i = 0; i < n; i++)
    {
        printf("Enter process name, arrival time, execution time, and priority:");
        scanf("%s%d%d%d", pn[i], &at[i], &et[i], &p[i]);
    }

    //populate arrays with random data
    generateRandomArray(at, 6, 0, 8, 3);
    generateRandomArray(bt, 6, 3, 10, 3);
    generateRandomArray(p, 6, 1, 4, 2);

    // Sort the processes based on arrival time and priority
    for(i = 0; i < n; i++)
    {
        for(j = i + 1; j < n; j++)
        {
            if(at[i] > at[j] || (at[i] == at[j] && p[i] > p[j]))
            {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;

                temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                strcpy(t, pn[i]);
                strcpy(pn[i], pn[j]);
                strcpy(pn[j], t);
            }
        }
    }

    for(i = 0; i < n; i++)
    {
        if(i == 0)
        {
            st[i] = at[i];
            wt[i] = st[i] - at[i];
            ft[i] = st[i] + bt[i];
            ta[i] = ft[i] - at[i];
        }
        else
        {
            st[i] = (ft[i - 1] > at[i]) ? ft[i - 1] : at[i];
            wt[i] = st[i] - at[i];
            ft[i] = st[i] + bt[i];
            ta[i] = ft[i] - at[i];
        }

        totwt += wt[i];
        totta += ta[i];
    }

    awt = (float)totwt / n;
    ata = (float)totta / n;

    printf("\nPname\tarrivaltime\texecutiontime\tpriority\twaitingtime\ttatime");
    for(i = 0; i < n; i++)
    {
        printf("\n%s\t%5d\t\t%5d\t\t%5d\t\t%5d\t\t%5d", pn[i], at[i], bt[i], p[i], wt[i], ta[i]);
    }
    printf("\nAverage waiting time is: %f", awt);
    printf("\nAverage turnaround time is: %f\n", ata);
} */

//Priority scheduling (only works for arrival time = 0)
void priorityScheduling() {
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
}

int main() {
    int userInput;
    do {
        printf("Enter 1 to continue or 0 to quit: ");
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
        else if (userInput == 0) 
        {
            printf("Quit the program.\n");
        } 
        else 
        {
            printf("Invalid choice. Please try again.\n");
        }
    } 
    while (userInput != 0);
    return 0;
}
