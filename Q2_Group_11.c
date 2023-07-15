#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>

int generateRandomNumber(int, int);
void generateRandomArray(int numbers[], int size, int min, int max, int maxDuplicates);
void displayHeader();
void displayFooter();

struct OutputValues
{
    float atat;
    float awt;
    float art;
};
typedef struct OutputValues OutputValues;

OutputValues firstComeFirstServeScheduling(int arrivalTime[], int burstTime[], int priority[], int n);
OutputValues shortestJobFirst(int arrivalTime[], int burstTime[], int priority[], int n);
OutputValues shortestRemainingTimeFirst(int arrivalTime[], int burstTime[], int priority[], int n);
OutputValues roundRobinScheduling(int arrivalTime[], int burstTime[], int priority[], int n);
OutputValues priorityScheduling(int arrivalTime[], int burstTime[], int priority[], int n);
void displaySequence(int seq[100][2], int c);

int generateRandomNumber(int min, int max)
{
    // Seed the random number generator with current time
    srand(time(NULL));
    // Generate a random number within the specified range
    int randomNumber = (rand() % (max - min + 1)) + min;

    return randomNumber;
}

void generateRandomArray(int numbers[], int size, int min, int max, int maxDuplicates)
{
    if (size <= 0 || maxDuplicates <= 0 || maxDuplicates >= size || min > max)
    {
        printf("Invalid input parameters.\n");
        return;
    }

    srand(time(NULL));
    int count = 0;
    int frequency[max - min + 1];

    // Initialize frequency array to 0
    for (int i = 0; i < max - min + 1; i++)
    {
        frequency[i] = 0;
    }

    // Check if min is 0 and generate at least one 0
    if (min == 0)
    {
        numbers[count] = 0;
        frequency[0 - min]++;
        count++;
    }

    while (count < size)
    {
        int number = (rand() % (max - min + 1)) + min; // Generate a random number within the range

        if (number != 0 && frequency[number - min] < maxDuplicates)
        {
            numbers[count] = number;
            frequency[number - min]++;
            count++;
        }
    }
}
/*
generateRandomArray(at, 6, 0, 8, 3);
generateRandomArray(bt, 6, 3, 10, 3);
generateRandomArray(priority, 6, 1, 4, 2);
*/
void generateSpecificArray(int arr[], int burst[], int priority[], int n, int type)
{
    // FCFS -> lots of short processes

    // SJF -> lots of short processes (distribution & arrival times, cannot have a long job then short job)

    // SRTF ->

    // RR -> if time quantum too large, behave like fcfs. if quantum time too small, too much switching. try big quantum time and large processes

    // PS -> priorities need to be well chosen
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

void displaySequence(int seq[100][2], int c)
{
    // top
    printf("╔");
    for (int i = 0; i < c; i++)
    {
        if (i + 1 == c)
        {
            printf("══════╗\n");
        }
        else
        {
            printf("══════╦");
        }
    }

    // middle
    for (int i = 0; i < c; i++)
    {
        printf("║ P %d  ", seq[i][0]);
    }

    // bottom
    printf("║\n╚");
    for (int i = 0; i < c; i++)
    {

        if (i + 1 == c)
        {
            printf("══════╝\n");
        }
        else
        {
            printf("══════╩");
        }
    }

    // time stamp
    printf("%d", 0);
    for (int i = 0; i < c; i++)
    {
        printf("%*d", 7, seq[i][1]);
    }

    printf("\n");
}

// First come first serve
OutputValues firstComeFirstServeScheduling(int at[], int bt[], int priority[], int n)
{
    // Initialize variables
    int i, j, temp = 0, ct[n], tat[n], wt[n];
    // int processes[6] = {1, 2, 3, 4, 5, 6};
    int processes[n];
    float awt = 0, atat = 0;
    int seqCounter = 0, sequence[100][2];
    struct OutputValues result;

    // Sort the processes based on arrival time using bubble sort
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < (n - i - 1); j++)
        {
            // Swap the processes if the arrival time is not in ascending order
            if (at[j] > at[j + 1])
            {
                temp = processes[j + 1];
                processes[j + 1] = processes[j];
                processes[j] = temp;

                // Swap arrival time, burst time, and priority accordingly
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

    // Calculate completion time
    ct[0] = at[0] + bt[0];
    sequence[seqCounter][0] = 0 + 1;
    sequence[seqCounter][1] = ct[0];
    seqCounter++;

    // Calculate completion time for the remaining processes
    for (i = 1; i < n; i++)
    {
        // Reset temp to 0 inside the loop
        temp = 0;
        if (ct[i - 1] < at[i])
        {
            // If the previous process completed before the current process arrives,
            // calculate the waiting time for the current process
            temp = at[i] - ct[i - 1];
        }
        ct[i] = ct[i - 1] + bt[i] + temp;

        sequence[seqCounter][0] = i + 1;
        sequence[seqCounter][1] = ct[i];
        seqCounter++;
    }

    // Calculating turnaround time and waiting time
    for (i = 0; i < n; i++)
    {
        // Turnaround time = Completion time - Arrival time
        tat[i] = ct[i] - at[i];
        // Waiting time = Turnaround time - Burst time
        wt[i] = tat[i] - bt[i];
        // Sum of turnaround times for average calculation
        atat += tat[i];
        // Sum of waiting times for average calculation
        awt += wt[i];
    }

    // Print the Gantt Chart
    printf("\n\nFCFS Gantt Chart:\n");
    displaySequence(sequence, seqCounter);

    // Calculate and print average turnaround time and average waiting time
    atat = atat / n;
    awt = awt / n;
    printf("\nAverage turnaround time: %.3f\n", atat);
    printf("Average waiting time: %.3f\n", awt);
    printf("Average response time: %.3f\n", awt); // non-preemptive response time = waiting time

    result.atat = atat;
    result.awt = awt;
    result.art = awt;

    return result;
}

// Shortest Job First
OutputValues shortestJobFirst(int art[], int bt[], int priority[], int n)
{
    // init variables
    int wt[n], tat[n];
    int total_wt = 0, total_tat = 0;
    int seqCounter = 0, sequence[100][2];
    struct OutputValues result;

    // store burst time in temp array
    int rt[n];
    for (int i = 0; i < n; i++)
    {
        rt[i] = bt[i];
    }

    int complete = 0, t = 0;
    int shortest = -1, finish_time;

    // Process until all processes get completed
    while (complete != n)
    {
        shortest = -1;
        int min_bt = INT_MAX;

        // Find the process with the minimum remaining time
        for (int j = 0; j < n; j++)
        {
            if (art[j] <= t && rt[j] < min_bt && rt[j] > 0)
            {
                if (art[j] == 0)
                {
                    shortest = j;
                    min_bt = rt[j];
                    break;
                }
                if (shortest == -1 || rt[j] < rt[shortest])
                {
                    shortest = j;
                    min_bt = rt[j];
                }
            }
        }

        if (shortest == -1)
        {
            t++;
            continue;
        }

        // Reduce the remaining time by one
        rt[shortest]--;

        // If a process gets completely executed
        if (rt[shortest] == 0)
        {
            complete++;

            // Find the finish time of the current process
            finish_time = t + 1;

            // Calculate the waiting time
            wt[shortest] = finish_time - bt[shortest] - art[shortest];
            if (wt[shortest] < 0)
                wt[shortest] = 0;

            // Sequence tracking
            sequence[seqCounter][0] = shortest + 1;
            sequence[seqCounter][1] = finish_time;
            seqCounter++;
        }

        // Increment time
        t++;
    }

    // Calculate turnaround time
    for (int i = 0; i < n; i++)
    {
        tat[i] = bt[i] + wt[i];

        total_wt += wt[i];
        total_tat += tat[i];
    }

    // Print the Gantt Chart
    printf("\n\nSJF Gantt Chart:\n");
    displaySequence(sequence, seqCounter);

    // Calculate average waiting time and average turnaround time
    float avg_wt = (float)total_wt / (float)n;
    float avg_tat = (float)total_tat / (float)n;
    printf("\nAverage turnaround time = %.3f", avg_tat);
    printf("\nAverage waiting time = %.3f\n", avg_wt);
    printf("Average response time: %.3f\n", avg_wt); // non-preemptive response time = waiting time

    result.atat = avg_tat;
    result.awt = avg_wt;
    result.art = avg_wt;

    return result;
}

OutputValues shortestRemainingTimeFirst(int arrivalTime[], int burstTime[], int priority[], int n)
{
    // init variables
    int arrival_time[n], burst_time[n + 1], temp[n], i, j, p, smallest, count = 0, time = 0;
    float wait_time = 0, turnaround_time = 0, end;
    int seqCounter = 0, sequence[100][2];
    struct OutputValues result;

    // Copy burst time into a temp array
    for (i = 0; i < n; i++)
    {
        arrival_time[i] = arrivalTime[i];
        burst_time[i] = burstTime[i];
        temp[i] = burst_time[i];
    }
    burst_time[n + 1] = 9999;

    // Execute processes until all processes are completed
    for (time = 0; count != n; time++)
    {
        smallest = n + 1;
        for (i = 0; i < n; i++)
        {
            // Find the process with the minimum remaining time
            if (arrival_time[i] <= time && burst_time[i] <= burst_time[smallest] && burst_time[i] > 0)
            {
                if (burst_time[i] < burst_time[smallest])
                {
                    smallest = i;
                }
                else if (arrival_time[i] < arrival_time[smallest])
                {
                    smallest = i;
                }
            }
        }
        burst_time[smallest]--;

        // Sequence tracking
        // check if change in process has occured

        if (seqCounter == 0)
        {
            sequence[seqCounter][0] = smallest + 1;
            sequence[seqCounter][1] = time + 1;
            seqCounter++;
        }
        else
        {
            if (seqCounter > 0 && sequence[seqCounter - 1][0] != smallest + 1)
            {
                sequence[seqCounter][0] = smallest + 1;
                sequence[seqCounter][1] = time + 1;
                seqCounter++;
            }
            else
            {
                sequence[seqCounter - 1][1] = time + 1;
            }
        }

        // When Process is completed, add and calculate the waiting time and turnaround time
        if (burst_time[smallest] == 0)
        {
            count++;
            end = time + 1;
            wait_time = wait_time + end - arrival_time[smallest] - temp[smallest];
            turnaround_time = turnaround_time + end - arrival_time[smallest];
        }
    }

    // Print the Gantt Chart
    printf("\n\nSRTF Gantt Chart:\n");
    displaySequence(sequence, seqCounter);

    // calculating response time
    bool responseTime[n];
    float average_response_time = 0;
    for (i = 0; i < n; i++)
    {
        responseTime[i] = false;
    }
    for (i = 1; i < seqCounter; i++)
    {
        p = sequence[i][0] - 1;
        if (responseTime[p] == false)
        {
            responseTime[p] = true;
            average_response_time += (sequence[i - 1][1] - arrival_time[p]);
        }
    }
    average_response_time /= n;

    // Print average turnaround time and average waiting time
    printf("Average Turnaround Time: %.3f", turnaround_time / n);
    printf("\nAverage Waiting Time: %.3f", wait_time / n);
    printf("\nAverage Response Time: %.3f\n", average_response_time);

    result.atat = turnaround_time / n;
    result.awt = wait_time / n;
    result.art = average_response_time;

    return result;
}

// Round robin
typedef struct rrQ
{
    unsigned int p;
    struct rrQ *next;
} Q;
Q *rrQueue = NULL;
// Function to add process in the Queue
void addToQueue(int i)
{
    Q *n, *n1;
    n = (Q *)malloc(sizeof(Q));
    n->next = NULL;
    n->p = i;
    if (rrQueue == NULL)
    {

        rrQueue = n;
    }
    else
    {
        for (n1 = rrQueue; n1->next != NULL; n1 = n1->next)
            ;
        n1->next = n;
    }
}
// Function returns the next process in the queue
unsigned int getNextProcess()
{
    Q *n;
    int x;
    if (rrQueue == NULL)
    {
        return -1;
    }
    else
    {
        x = rrQueue->p;
        n = rrQueue;
        rrQueue = rrQueue->next;
        free(n);
        return x;
    }
}
// check if have remaining processes
bool remainingProc(int res_burst[])
{
    int i, x = 0, limit = 6;
    for (i = 0; i < limit; i++)
    {
        if (res_burst[i] > 0)
        {
            x = 1;
        }
    }
    return x;
}

OutputValues roundRobinScheduling(int arrivalTime[], int burstTime[], int priority[], int n)
{
    int i, j, p, q = 0, releaseTime, t, limit = n, total = 0, x, time_quantum = 2;
    int arrival_time[n], burst_time[n];
    float average_wait_time = 0, average_turnaround_time = 0;
    x = limit;
    int ProcessScheduler[1000], WaitingTime[100], TurnaroundTime[100];

    int seqCounter = 0, sequence[100][2];
    struct OutputValues result;

    // create copy of burst time and arrival time
    int res_burst[n];
    int res_arrival[n];
    for (i = 0; i < limit; i++)
    {
        // printf("%d %d\n", arrivalTime[i], burstTime[i]);
        burst_time[i] = burstTime[i];
        arrival_time[i] = arrivalTime[i];

        res_burst[i] = burst_time[i];
        res_arrival[i] = arrival_time[i];

        // add process to Queue
        if (arrival_time[i] == total)
        {
            addToQueue(i);
        }
    }

    while (remainingProc(res_burst))
    {
        p = getNextProcess();
        if (p == -1)
        {
            ProcessScheduler[total] = -1;
            total++;
            for (i = 0; i < limit; i++)
            {
                if (res_arrival[i] == total)
                {
                    addToQueue(i);
                }
            }
        }
        else
        {
            q = time_quantum;
            if (res_burst[p] < q)
            {
                q = res_burst[p];
            }
            for (i = q; i > 0; i--)
            {
                ProcessScheduler[total] = p;
                total++;
                res_burst[p]--;
                for (j = 0; j < limit; j++)
                {
                    if (res_arrival[j] == total)
                    {
                        addToQueue(j);
                    }
                }
            }
            if (res_burst[p] > 0)
            {
                addToQueue(p);
            }
        }

        sequence[seqCounter][0] = p + 1;
        sequence[seqCounter][1] = total;
        seqCounter++;
    }

    for (i = 0; i < limit; i++)
    {

        for (t = total - 1; ProcessScheduler[t] != i; t--)
            ;
        releaseTime = t + 1;
        TurnaroundTime[i] = releaseTime - arrival_time[i];
        WaitingTime[i] = releaseTime - arrival_time[i] - burst_time[i];
    }

    // counting Average Waiting Time...
    for (i = 0; i < limit; i++)
    {
        average_turnaround_time = average_turnaround_time + TurnaroundTime[i];
        average_wait_time = average_wait_time + WaitingTime[i];
    }
    average_turnaround_time = average_turnaround_time / limit;
    average_wait_time = average_wait_time / limit;

    // calculating response time
    bool responseTime[n];
    float average_response_time = 0;
    for (i = 0; i < n; i++)
    {
        responseTime[i] = false;
    }
    for (i = 1; i < seqCounter; i++)
    {
        p = sequence[i][0] - 1;
        if (responseTime[p] == false)
        {
            responseTime[p] = true;
            average_response_time += (sequence[i - 1][1] - arrival_time[p]);
            //printf("\n\t%d\t%d\t%.3f", sequence[i-1][1], arrival_time[p], average_response_time);
        }
    }
    average_response_time /= n;

    printf("\n\nRR Gant Chart:\n");
    displaySequence(sequence, seqCounter);

    printf("\nAverage Turnaround Time: %.3f", average_turnaround_time);
    printf("\nAverage Waiting Time: %.3f", average_wait_time);
    printf("\nAverage Response Time: %.3f\n", average_response_time);

    result.atat = average_turnaround_time;
    result.awt = average_wait_time;
    result.art = average_response_time;

    return result;
}

// Priority scheduling (only works for arrival time = 0)
OutputValues priorityScheduling(int at[], int bt[], int priority[], int n)
{
    int i, p;
    int arrivalTime[n];
    int burstTime[n];
    int temp[n];
    int time = 0;
    int count = 0;
    int shortestProcess;
    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;
    float averageWaitingTime, averageTurnaroundTime;
    int seqCounter = 0, sequence[100][2];
    struct OutputValues result;

    for (i = 0; i < n; i++)
    {
        arrivalTime[i] = at[i];
        burstTime[i] = bt[i];

        temp[i] = burstTime[i]; // adding a duplicate of the burst time to a temporary array
    }

    // Scheduling algorithm
    while (count != n)
    {
        shortestProcess = -1;
        for (i = 0; i < n; i++)
        {
            if (burstTime[i] > 0 && arrivalTime[i] <= time)
            {
                if (shortestProcess == -1 || priority[i] < priority[shortestProcess])
                {
                    shortestProcess = i;
                }
                // if same priority value
                else if (priority[i] == priority[shortestProcess] && arrivalTime[i] < arrivalTime[shortestProcess])
                {
                    shortestProcess = i;
                }
            }
        }

        /* NON - PREEMPTIVE */
        int turnaroundTime = time + burstTime[shortestProcess] - arrivalTime[shortestProcess];
        int waitingTime = turnaroundTime - temp[shortestProcess];

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        time += burstTime[shortestProcess];
        burstTime[shortestProcess] = 0;

        count++;

        // sequence track
        sequence[seqCounter][0] = shortestProcess + 1;
        sequence[seqCounter][1] = time;
        seqCounter++;
    }

    averageWaitingTime = totalWaitingTime / n;
    averageTurnaroundTime = totalTurnaroundTime / n;

    // calculating response time
    bool responseTime[n];
    float average_response_time = 0;
    for (i = 0; i < n; i++)
    {
        responseTime[i] = false;
    }
    for (i = 1; i < seqCounter; i++)
    {
        p = sequence[i][0] - 1;
        if (responseTime[p] == false)
        {
            responseTime[p] = true;
            average_response_time += (sequence[i - 1][1] - arrivalTime[p]);
        }
    }
    average_response_time /= n;

    printf("\n\nPS Gant Chart:\n");
    displaySequence(sequence, seqCounter);

    printf("Avg turn around time is %.3f\n", averageTurnaroundTime);
    printf("Avg waiting time is %.3f", averageWaitingTime);
    printf("\nAverage Response Time: %.3f\n", average_response_time);

    result.atat = averageTurnaroundTime;
    result.awt = averageWaitingTime;
    result.art = average_response_time;

    return result;
}

int main()
{
    int n = 6;
    int arrivalTime[n];
    int burstTime[n];
    int priority[n];

    int userInput;
    do
    {
        printf("Enter 1 to continue or 0 to quit: ");
        scanf("%d", &userInput);

        if (userInput == 1)
        {
            printf("Enter a number between 1 and 5: ");
            scanf("%d", &userInput);

            // output to txt
            freopen("Q2_Group_11.txt", "w", stdout);

            // populate array with random data
            generateRandomArray(arrivalTime, n, 0, 8, 3);
            generateRandomArray(burstTime, n, 3, 10, 3);
            generateRandomArray(priority, n, 1, 4, 2);

            // switch case based on choice
            switch (userInput)
            {
            case 1:
                printf("You selected First come first serve scheduling.\n");
                printf("FCFS will not result in the any best outcomes.(SRTF should be best outcome)\n");

                break;
            case 2:
                printf("You selected Shortest job first scheduling.\n");
                printf("SJF should either result in shortest average turnaround & waiting (alongside SRTF)\nor have slightly higher values than SRTF\nSince SJF is a non-preemptive version of SRTF\n");

                break;
            case 3:
                printf("You selected Shortest remaining time first scheduling.\n");
                printf("\nSRTF should result in shortest average turnaround & waiting \nbut not shorterst average response time...\n");

                break;
            case 4:
                printf("You selected Round robin scheduling.\n");
                printf("\nRR should result in shortest average response time...\n");

                break;
            case 5:
                printf("You selected Priority scheduling.\n");
                printf("Priority Scheduling has high chance of resulting in the lowest execution time due to the use of priority values\nThis is best for real time operating systems.\n");

                break;
            default:
                printf("Invalid input. Please enter a number between 1 and 5.\n");
                break;
            }

            displayHeader();
            for (int i = 0; i < n; i++)
            {
                printf("║    P%2d    ║     %2d      ║      %2d       ║       %2d        ║\n",
                       i + 1, arrivalTime[i], burstTime[i], priority[i]);
            }
            displayFooter();

            // Duplicate the arrays for each function call (prevents memory complications)
            int arrivalTimeCopy[n];
            int burstTimeCopy[n];
            int priorityCopy[n];

            struct timeval start, end;
            long seconds, micros[5];

            float turnaround[5];
            float waiting[5];
            float response[5];

            struct OutputValues fcfs, sjf, srtf, rr, ps;

            memcpy(arrivalTimeCopy, arrivalTime, sizeof(arrivalTime));
            memcpy(burstTimeCopy, burstTime, sizeof(burstTime));
            memcpy(priorityCopy, priority, sizeof(priority));
            gettimeofday(&start, NULL);
            fcfs = firstComeFirstServeScheduling(arrivalTimeCopy, burstTimeCopy, priorityCopy, n);
            gettimeofday(&end, NULL);
            seconds = (end.tv_sec - start.tv_sec);
            micros[0] = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            printf("The elapsed time is %d micros\n", micros[0]);

            memcpy(arrivalTimeCopy, arrivalTime, sizeof(arrivalTime));
            memcpy(burstTimeCopy, burstTime, sizeof(burstTime));
            memcpy(priorityCopy, priority, sizeof(priority));
            gettimeofday(&start, NULL);
            sjf = shortestJobFirst(arrivalTimeCopy, burstTimeCopy, priorityCopy, n);
            gettimeofday(&end, NULL);
            seconds = (end.tv_sec - start.tv_sec);
            micros[1] = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            printf("The elapsed time is %d micros\n", micros[1]);

            memcpy(arrivalTimeCopy, arrivalTime, sizeof(arrivalTime));
            memcpy(burstTimeCopy, burstTime, sizeof(burstTime));
            memcpy(priorityCopy, priority, sizeof(priority));
            gettimeofday(&start, NULL);
            srtf = shortestRemainingTimeFirst(arrivalTimeCopy, burstTimeCopy, priorityCopy, n);
            gettimeofday(&end, NULL);
            seconds = (end.tv_sec - start.tv_sec);
            micros[2] = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            printf("The elapsed time is %d micros\n", micros[2]);

            memcpy(arrivalTimeCopy, arrivalTime, sizeof(arrivalTime));
            memcpy(burstTimeCopy, burstTime, sizeof(burstTime));
            memcpy(priorityCopy, priority, sizeof(priority));
            gettimeofday(&start, NULL);
            rr = roundRobinScheduling(arrivalTimeCopy, burstTimeCopy, priorityCopy, n);
            gettimeofday(&end, NULL);
            seconds = (end.tv_sec - start.tv_sec);
            micros[3] = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            printf("The elapsed time is %d micros\n", micros[3]);

            memcpy(arrivalTimeCopy, arrivalTime, sizeof(arrivalTime));
            memcpy(burstTimeCopy, burstTime, sizeof(burstTime));
            memcpy(priorityCopy, priority, sizeof(priority));
            gettimeofday(&start, NULL);
            ps = priorityScheduling(arrivalTimeCopy, burstTimeCopy, priorityCopy, n);
            gettimeofday(&end, NULL);
            seconds = (end.tv_sec - start.tv_sec);
            micros[4] = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
            printf("The elapsed time is %d micros\n", micros[4]);

            turnaround[0] = fcfs.atat;
            turnaround[1] = sjf.atat;
            turnaround[2] = srtf.atat;
            turnaround[3] = rr.atat;
            turnaround[4] = ps.atat;

            waiting[0] = fcfs.awt;
            waiting[1] = sjf.awt;
            waiting[2] = srtf.awt;
            waiting[3] = rr.awt;
            waiting[4] = ps.awt;

            response[0] = fcfs.art;
            response[1] = sjf.art;
            response[2] = srtf.art;
            response[3] = rr.art;
            response[4] = ps.art;


            //compare outcomes
            int smallest_turnaround = 0, smallest_waiting = 0, smallest_response = 0, smallest_exec = 0;
            char algos[5][5] = {"FCFS", "SJF", "SRTF", "RR", "PS"};
            for (int i=1; i<5;i++){
                if (turnaround[i]<turnaround[smallest_turnaround]){
                    smallest_turnaround = i;
                }
                if (waiting[i]<waiting[smallest_waiting]){
                    smallest_waiting = i;
                }
                if (response[i]<response[smallest_response]){
                    smallest_response = i;
                }
                if (micros[i]<micros[smallest_exec]){
                    smallest_exec = i;
                }
            }
            
            switch (userInput)
            {
                case 1:
                    if (turnaround[0]==turnaround[smallest_turnaround]){
                        printf("\nFCFS resulted in best turnaround time of %.3f!\n", turnaround[smallest_turnaround]);
                    }
                    else{
                        printf("\nFCFS did not result in best turnaround time.%s resulted in best turnaround time of %.3f!\n", algos[smallest_turnaround], turnaround[smallest_turnaround]);
                    }
                    if (waiting[0]==waiting[smallest_waiting]){
                        printf("\nFCFS resulted in best waiting time of %.3f!\n", waiting[smallest_waiting]);
                    }
                    else{
                        printf("\n\nFCFS did not result in best waiting time.%s resulted in best waiting time of %.3f!\n", algos[smallest_waiting], waiting[smallest_waiting]);
                    }
                    if (response[0]==response[smallest_response]){
                        printf("\nFCFS resulted in best response time of %.3f!\n", response[smallest_response]);
                    }
                    else{
                        printf("\n\nFCFS did not result in best response time.%s resulted in best resposne time of %.3f!\n", algos[smallest_response], response[smallest_response]);
                    }
                    if (micros[0]==micros[smallest_exec]){
                        printf("\nFCFS resulted in best execution time of %d!\n", micros[smallest_exec]);
                    }
                    else{
                        printf("\n\nFCFS did not result in best execution time.%s resulted in best execution time of %d!\n", algos[smallest_exec], micros[smallest_exec]);
                    }
                    break;
                case 2:
                    if (turnaround[1]==turnaround[smallest_turnaround]){
                        printf("\nSJF resulted in best turnaround time of %.3f!\n", turnaround[smallest_turnaround]);
                    }
                    else{
                        printf("\n\nSJF did not result in best turnaround time.%s resulted in best turnaround time of %.3f!\n", algos[smallest_turnaround], turnaround[smallest_turnaround]);
                    }
                    if (waiting[1]==waiting[smallest_waiting]){
                        printf("\nSJF resulted in best waiting time of %.3f!\n", waiting[smallest_waiting]);
                    }
                    else{
                        printf("\n\nSJF did not result in best waiting time.%s resulted in best waiting time of %.3f!\n", algos[smallest_waiting], waiting[smallest_waiting]);
                    }
                    if (response[1]==response[smallest_response]){
                        printf("\nSJF resulted in best response time of %.3f!\n", response[smallest_response]);
                    }
                    else{
                        printf("\n\nSJF did not result in best response time.%s resulted in best resposne time of %.3f!\n", algos[smallest_response], response[smallest_response]);
                    }
                    if (micros[1]==micros[smallest_exec]){
                        printf("\nSJF resulted in best execution time of %d!\n", micros[smallest_exec]);
                    }
                    else{
                        printf("\n\nSJF did not result in best execution time.%s resulted in best execution time of %d!\n", algos[smallest_exec], micros[smallest_exec]);
                    }
                    break;
                case 3:
                    if (turnaround[2]==turnaround[smallest_turnaround]){
                        printf("\nSRTF resulted in best turnaround time of %.3f!\n", turnaround[smallest_turnaround]);
                    }
                    else{
                        printf("\n\nSRTF did not result in best turnaround time.%s resulted in best turnaround time of %.3f!\n", algos[smallest_turnaround], turnaround[smallest_turnaround]);
                    }
                    if (waiting[2]==waiting[smallest_waiting]){
                        printf("\nSRTF resulted in best waiting time of %.3f!\n", waiting[smallest_waiting]);
                    }
                    else{
                        printf("\n\nSRTF did not result in best waiting time.%s resulted in best waiting time of %.3f!\n", algos[smallest_waiting], waiting[smallest_waiting]);
                    }
                    if (response[2]==response[smallest_response]){
                        printf("\nSRTF resulted in best response time of %.3f!\n", response[smallest_response]);
                    }
                    else{
                        printf("\n\nSRTF did not result in best response time.%s resulted in best resposne time of %.3f!\n", algos[smallest_response], response[smallest_response]);
                    }
                    if (micros[2]==micros[smallest_exec]){
                        printf("\nSRTF resulted in best execution time of %d!\n", micros[smallest_exec]);
                    }
                    else{
                        printf("\n\nSRTF did not result in best execution time.%s resulted in best execution time of %d!\n", algos[smallest_exec], micros[smallest_exec]);
                    }
                    break;
                case 4:
                    if (turnaround[3]==turnaround[smallest_turnaround]){
                        printf("\nRR resulted in best turnaround time of %.3f!\n", turnaround[smallest_turnaround]);
                    }
                    else{
                        printf("\n\nRR did not result in best turnaround time.%s resulted in best turnaround time of %.3f!\n", algos[smallest_turnaround], turnaround[smallest_turnaround]);
                    }
                    if (waiting[3]==waiting[smallest_waiting]){
                        printf("\nRR resulted in best waiting time of %.3f!\n", waiting[smallest_waiting]);
                    }
                    else{
                        printf("\n\nRR did not result in best waiting time.%s resulted in best waiting time of %.3f!\n", algos[smallest_waiting], waiting[smallest_waiting]);
                    }
                    if (response[3]==response[smallest_response]){
                        printf("\nRR resulted in best response time of %.3f!\n", response[smallest_response]);
                    }
                    else{
                        printf("\n\nRR did not result in best response time.%s resulted in best resposne time of %.3f!\n", algos[smallest_response], response[smallest_response]);
                    }
                    if (micros[3]==micros[smallest_exec]){
                        printf("\nRR resulted in best execution time of %d!\n", micros[smallest_exec]);
                    }
                    else{
                        printf("\n\nRR did not result in best execution time.%s resulted in best execution time of %d!\n", algos[smallest_exec], micros[smallest_exec]);
                    }
                    break;
                case 5:
                    if (turnaround[4]==turnaround[smallest_turnaround]){
                        printf("\nPS resulted in best turnaround time of %.3f!\n", turnaround[smallest_turnaround]);
                    }
                    else{
                        printf("\n\nPS did not result in best turnaround time.%s resulted in best turnaround time of %.4f!\n", algos[smallest_turnaround], turnaround[smallest_turnaround]);
                    }
                    if (waiting[4]==waiting[smallest_waiting]){
                        printf("\nPS resulted in best waiting time of %.3f!\n", waiting[smallest_waiting]);
                    }
                    else{
                        printf("\n\nPS did not result in best waiting time.%s resulted in best waiting time of %.4f!\n", algos[smallest_waiting], waiting[smallest_waiting]);
                    }
                    if (response[4]==response[smallest_response]){
                        printf("\nPS resulted in best response time of %.3f!\n", response[smallest_response]);
                    }
                    else{
                        printf("\n\nPS did not result in best response time.%s resulted in best resposne time of %.4f!\n", algos[smallest_response], response[smallest_response]);
                    }
                    if (micros[4]==micros[smallest_exec]){
                        printf("\nPS resulted in best execution time of %d!\n", micros[smallest_exec]);
                    }
                    else{
                        printf("\n\nPS did not result in best execution time.%s resulted in best execution time of %d!\n", algos[smallest_exec], micros[smallest_exec]);
                    }
                    break;
                default:
                    printf("Invalid input. Please enter a number between 1 and 5.\n");
                    break;
            }

            // close stdout
            fclose(stdout);
            // Restore standard output to console
            #ifdef _WIN32
                        freopen("CON", "w", stdout);
            #else
                        freopen("/dev/tty", "w", stdout);
            #endif

            FILE *file = fopen("Q2_Group_11.txt", "r");
            char c;
            while ((c = fgetc(file)) != EOF)
            {
                putchar(c);
            }
            fclose(file);
        }
        else if (userInput == 0)
        {
            printf("Quit the program.\n");
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }

    } while (userInput != 0);
    return 0;
}
