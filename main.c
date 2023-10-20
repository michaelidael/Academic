
/* COP 3502C Assignment 2 This program is written by: Michael Soto */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"
#define MAX_Z 10
#define MAX_LENGTH 50

typedef struct node
{
	int info;
	struct node *next;
	struct queue *queueInList;
} node;

typedef struct soldier
{
	int data;
	struct soldier *next;
	struct soldier *prev;
} soldier;

typedef struct queue
{
	int zNumber;
	int nodeCount;
	int ogNodeCount;
	int k;
	int th;
	char *name;
	soldier *head;
	soldier *tail;
} queue;

//prototypes
int empty(queue *qPtr);
soldier *createSoldier(int sequence);
void display(queue q);
void printQueue(queue q, FILE *output);
void freeMemory(queue **q);
void beforePhase1(FILE *input, FILE *output, int Z, queue **queuePtr);
void phase1(queue **q, FILE *output);
void rotateQueue(queue *q, int k);
void phase2(queue **q, FILE *output, int Z);
void beforePhase2(queue **q);
void createReverseCircle(queue *q);
void rearrangeCircle(queue *q);
void enqueue(queue *qPtr, int val);
void QueueInitializer(queue *queuePtr);
node *sortInsert(node *head, int numGrounds, char *groundName, int numSoldiers, int k, int th);
void listLiberator(node *head);
void newListSorter(queue *q);
int dequeue(queue *q);

int main()
{
	atexit(report_mem_leak);
	FILE *input;
	FILE *output;
	input = fopen("in.txt", "rt");
	output = fopen("out.txt", "wt");
	int Z;

	if (input == NULL || output== NULL)
	{
		printf("File could not be read\n");
		return 1;
	}

	fscanf(input, "%d", &Z);
	queue **queuePtr = (struct queue **)malloc(MAX_Z * sizeof(queue *));

	for (int i = 0; i < MAX_Z; i++)
	{
		queuePtr[i] = (struct queue *)malloc(sizeof(queue));
		QueueInitializer(queuePtr[i]);
	}

	beforePhase1(input, output, Z, queuePtr);
	phase1(queuePtr, output);
	beforePhase2(queuePtr);
	phase2(queuePtr, output, Z);
	freeMemory(queuePtr);
	fclose(input);
	fclose(output);
	return 0;
}

void QueueInitializer(queue *queuePtr)
{
	queuePtr->head = NULL;
	queuePtr->tail = NULL;
}

node *sortInsert(node *head, int numGrounds, char *groundName, int numSoldiers, int k, int th)
{
	node *temp;
	node *t;
	temp = (node *)malloc(sizeof(node) * MAX_LENGTH);
	temp->info = numGrounds;
	temp->next = NULL;
	temp->queueInList = (queue *)malloc(sizeof(queue) * MAX_LENGTH);
	temp->queueInList->name = (char *)malloc(MAX_LENGTH * sizeof(char));
	temp->queueInList->name = strcpy(temp->queueInList->name, groundName);
	temp->queueInList->nodeCount = numSoldiers;
	temp->queueInList->ogNodeCount = numSoldiers;
	temp->queueInList->k = k;
	temp->queueInList->th = th;
	temp->queueInList->zNumber = temp->info;

	if (head == NULL || head->info >= numGrounds)
	{
		temp->next = head;
		head = temp;
	}
	else
	{
		t = head;
		while (t->next != NULL && t->next->info < numGrounds)
		{
			t = t->next;
		}
		temp->next = t->next;
		t->next = temp;
	}
	return head;
}

soldier *createSoldier(int sequence)
{
	soldier *newSoldier = (soldier *)malloc(sizeof(soldier));

	if (newSoldier != NULL)
	{
		newSoldier->data = sequence;
		newSoldier->next = NULL;
		newSoldier->prev = NULL;
		return newSoldier;
	}
	return 0;
}

void enqueue(queue *qPtr, int val)
{
	struct soldier *temp = createSoldier(val);

	if (!qPtr->head)
	{
		qPtr->head = temp;
		qPtr->head->next = qPtr->head;
		qPtr->head->prev = qPtr->head;
		qPtr->tail = qPtr->head->prev;
		return;
	}

	temp->next = qPtr->head;
	temp->prev = qPtr->head->prev;
	temp->prev->next = temp;
	qPtr->head->prev = temp;
	qPtr->tail = temp;
}

void createReverseCircle(queue *q)
{

	int counter = q->nodeCount;
	for (int j = 0; j < MAX_Z; j++)
	{
		if (counter > 0)
		{
			enqueue(q, counter);
		}
		counter--;
	}
}

void rearrangeCircle(queue *q)
{

	if (q->head == NULL)
	{
		printf("List is empty\n");
		return;
	}

	soldier *current;
	soldier *t;
	current = q->head;
	do
	{
		soldier *temp;
		temp = current->prev;
		current->prev = current->next;
		current->next = temp;
		current = current->next;
	} while (current != q->head);
	t = q->head;
	q->head = q->tail;
	q->tail = t;
}

int empty(queue *qPtr)
{
	return qPtr->nodeCount == 0;
}

void display(queue q)
{
	if (q.head == NULL)
	{
		printf("Queue is empty\n");
		return;
	}
	soldier *t = q.head;
	printf("%d %s ", q.zNumber, q.name);
	printf("%d ", q.head->data);
	q.head = q.head->next;
	while (t != q.head)
	{
		printf("%d ", q.head->data);
		q.head = q.head->next;
	}
	printf("\n");
}

void printQueue(queue q, FILE *output)
{

	if (q.head == NULL)
	{
		fprintf(output, "Queue is empty\n");
		return;
	}
	soldier *t = q.head;
	fprintf(output, "%d %s ", q.zNumber, q.name);
	fprintf(output, "%d ", q.head->data);
	q.head = q.head->next;
	while (t != q.head)
	{
		fprintf(output, "%d ", q.head->data);
		q.head = q.head->next;
	}
	fprintf(output, "\n");
}

void qInfoPopulator(queue *q, node *head)
{

	q->zNumber = head->queueInList->zNumber;
	q->name = head->queueInList->name;
	q->k = head->queueInList->k;
	q->th = head->queueInList->th;
	q->nodeCount = head->queueInList->nodeCount;
	q->ogNodeCount = head->queueInList->ogNodeCount;
}

void listLiberator(node *head)
{
	while (head != NULL)
	{
		free(head->queueInList->name);
		head = head->next;
	}

	free(head);
}

void beforePhase1(FILE *input, FILE *output, int Z, queue **queuePtr)
{
	node *head = NULL;
	int numGrounds, numSoldiers, k, th;
	char groundName[MAX_LENGTH];

	for (int i = 0; i < Z; i++)
	{
		fscanf(input, "%d %s %d %d %d", &numGrounds, groundName, &numSoldiers, &k, &th);
		head = sortInsert(head, numGrounds, groundName, numSoldiers, k, th);
	}

	fprintf(output, "Initial nonempty lists status\n");
	printf("Initial nonempty lists status\n");
	for (int i = 0; i < Z; i++)
	{
		qInfoPopulator(queuePtr[i], head);
		createReverseCircle(queuePtr[i]);
		display(*queuePtr[i]);
		printQueue(*queuePtr[i], output);
		head = head->next;
	}

	fprintf(output, "\nAfter ordering nonempty lists status\n");
	printf("\nAfter ordering nonempty lists status\n");
	for (int i = 0; i < Z; i++)
	{
		rearrangeCircle(queuePtr[i]);
		display(*queuePtr[i]);
		printQueue(*queuePtr[i], output);
	}

	listLiberator(head);
}

int dequeue(queue *q)
{
	soldier *temp = (soldier *)malloc(sizeof(soldier) * MAX_LENGTH);
	temp = NULL;

	int retval = 0;

	if (q->head == NULL)
	{
		return -1;
	}

	retval = q->head->data;
	temp = q->head;
	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	q->head = q->head->next;
	q->tail = q->head->prev;

	if (q->head == NULL)
	{
		q->tail = NULL;
	}

	//free(temp);

	return retval;
}

void rotateQueue(queue *q, int k)
{
	if (k == 0)
	{
		return;
	}

	soldier *current = q->head;
	int count = 0;

	while (count < k - 1)
	{
		current = current->next;
		count++;
	}
	soldier *t = current;

	while (current->next != q->head)
	{
		current = current->next;
	}

	current->next = q->head;
	q->head = t->next;
	t->next = q->head;
}

void phase1(queue **q, FILE *output)
{
	fprintf(output, "\nPhase1 execution\n");
	printf("\nPhase1 execution\n");
	fprintf(output, "\n");
	printf("\n");

	for (int i = 0; i < MAX_Z; i++)
	{
		if (empty(q[i]))
		{
			return;
		}
		int executed;

		rotateQueue(q[i], q[i]->k - 1);
		executed = dequeue(q[i]);
		q[i]->nodeCount--;
		fprintf(output, "Line# %d %s\n", q[i]->zNumber, q[i]->name);
		printf("Line# %d %s\n", q[i]->zNumber, q[i]->name);
		fprintf(output, "Soldier# %d executed\n", executed);
		printf("Soldier# %d executed\n", executed);

		for (int j = q[i]->nodeCount; j > q[i]->th; j--)
		{
			rotateQueue(q[i], q[i]->k - 1);
			q[i]->nodeCount--;
			executed = dequeue(q[i]);
			fprintf(output, "Soldier# %d executed\n", executed);
			printf("Soldier# %d executed\n", executed);
		}
		fprintf(output, "\n");
		printf("\n");
	}
}

void newListSorter(queue *q)
{

	while (q->head->data > q->head->next->data)
	{
		q->head = q->head->next;
		if (q->head->prev->data < q->head->data)
		{
			q->head = q->head->prev;
		}
	}
	q->tail = q->head->prev;
}

void beforePhase2(queue **q)
{
	for (int i = 0; i < MAX_Z; i++)
	{
		if (empty(q[i]))
		{
			return;
		}

		newListSorter(q[i]);
	}
}

void phase2(queue **q, FILE *output, int Z)
{

fprintf(output, "Phase2 execution\n");
printf("Phase2 execution\n");
int count = 0;

    for (int i = 0; i < Z; i++)
    {
        count += q[i]->th;


    }

    for (int i = 0; i < Z; i++)
    {
        if (empty(q[i]))
    {
        break;
    }

int max = 0;

    for (int j = 0; j < Z; j++)
    {
        if (empty(q[j]))
        {
            break;
        }


    if (q[j]->ogNodeCount == q[i]->ogNodeCount && i != j && q[j]->ogNodeCount != 0 &&
    q[i]->ogNodeCount != 0)
    {
        if (q[j]->zNumber < q[i]->zNumber)
        {
            for (int k = 0; k < q[j]->nodeCount; k++)
            {
                if (count == 1)
                {
                    int soldierChosen = dequeue(q[j]);
                    fprintf(output, "\nSoldier %d from line %d will survive", soldierChosen, q[j]->zNumber);
                    printf("\nSoldier %d from line %d will survive", soldierChosen, q[j]->zNumber);
                    break;
                }

                int soldierExecuted = dequeue(q[j]);
                fprintf(output, "Executed Soldier %d from line %d\n", soldierExecuted, q[j]->zNumber);
                printf("Executed Soldier %d from line %d\n", soldierExecuted, q[j]->zNumber);
                count--;
                q[j]->ogNodeCount = 0;
            }
    for (int k = 0; k < q[i]->nodeCount; k++)
    {
        if (count == 1)
        {
            int soldierChosen = dequeue(q[i]);
            fprintf(output, "\nSoldier %d from line %d will survive", soldierChosen, q[i]->zNumber);
            printf("\nSoldier %d from line %d will survive", soldierChosen, q[i]->zNumber);
            break;
        }

        int soldierExecuted = dequeue(q[i]);
        fprintf(output, "Executed Soldier %d from line %d\n", soldierExecuted, q[i]->zNumber);
        printf("Executed Soldier %d from line %d\n", soldierExecuted, q[i]->zNumber);
        count--;
        q[i]->ogNodeCount = 0;
    }
        }

    else
    {
        for (int k = 0; k < q[i]->nodeCount; k++)
        {
            if (count == 1)
            {
                int soldierChosen = dequeue(q[i]);
                fprintf(output, "\nSoldier %d from line %d will survive", soldierChosen, q[i]->zNumber);
                printf("\nSoldier %d from line %d will survive", soldierChosen, q[i]->zNumber);
                break;
            }

            int soldierExecuted = dequeue(q[i]);
            fprintf(output, "Executed Soldier %d from line %d\n", soldierExecuted, q[i]->zNumber);
            printf("Executed Soldier %d from line %d\n", soldierExecuted, q[i]->zNumber);
            count--;
            q[i]->ogNodeCount = 0;
        }

    for (int k = 0; k < q[j]->nodeCount; k++)
    {
        if (count == 1)
        {
            int soldierChosen = dequeue(q[j]);
            fprintf(output, "\nSoldier %d from line %d will survive", soldierChosen, q[j]->zNumber);
            printf("\nSoldier %d from line %d will survive", soldierChosen, q[j]->zNumber);
            break;
        }
        int soldierExecuted = dequeue(q[j]);
        fprintf(output, "Executed Soldier %d from line %d\n", soldierExecuted, q[j]->zNumber);
        printf("Executed Soldier %d from line %d\n", soldierExecuted, q[j]->zNumber);
        count--;
        q[j]->ogNodeCount = 0;
    }
    }
    }


    else if (q[i]->ogNodeCount > q[j]->ogNodeCount)
    {
        if (q[i]->ogNodeCount > q[max]->ogNodeCount)
        {
            max = i;
        }
    }

else if (q[j]->ogNodeCount > q[max]->ogNodeCount)
{
    max = j;
}
}
    for (int k = 0; k < q[max]->nodeCount; k++)
    {
        if (count == 1)
        {
            int soldierChosen = dequeue(q[max]);
            fprintf(output, "\nSoldier %d from line %d will survive", soldierChosen, q[max]->zNumber);
            printf("\nSoldier %d from line %d will survive", soldierChosen, q[max]->zNumber);
            printf("I AM THE CAUSE OF THIS EVIL BUG");
            break;
        }
        int soldierExecuted = dequeue(q[max]);
        fprintf(output, "Executed Soldier %d from line %d\n", soldierExecuted, q[max]->zNumber);
        printf("Executed Soldier %d from line %d\n", soldierExecuted, q[max]->zNumber);
        count--;
        q[max]->ogNodeCount = 0;
}
}
}

void freeMemory(queue **q)
{
	for (int i = 0; i < MAX_Z; i++)
	{
		free(q[i]);
	}
	free(q);
}
