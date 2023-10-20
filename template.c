#include<stdio.h>
#include<stdlib.h>
 typedef struct node{
        int data;
        struct node *next;
}node;


//this function takes an item and insert it in the linked list pointed by root.
node*  insert_front(node *root, int item)
{

node *temp =(node *)malloc(sizeof(node));
temp->data = item;
temp->next = NULL;

    if(root == NULL)
    {
    root = temp;
    }
    else
    {
        temp->next = root;
        root = temp;
    }

    return root;


}


void display(node* t)
{
  printf("\nPrinting your linked list.......");


        while(t!=NULL)
        {
                printf("%d ",t->data);
                t=t->next;
        }


}
node* reverse(node* head)
{

    //if we only have none or one node
    if(head == NULL || head->next == NULL)
    {
        return head;
    }

//if we have more than one node
//main list is initialized to the 2nd value in linked list since we already have the first being passed
//as a parameter
node *main_list = head->next;
//in the reverse list we need to start from back to front
node *reversed_list = head;

reversed_list->next = NULL; //next after head should be null since it is placed into the end of the linked list.

while(main_list != NULL)
{
    node *temp = main_list;
    main_list = main_list->next; //looping through elements

    temp->next = reversed_list; //adding second to last element
    reversed_list = temp;
}

    return reversed_list;

}







void insertToPlace(node* list, int val, int place)
{

//if there is a node
if(list == NULL || place<= 1)
{
    printf("\nList is empty or place is not valid.");
    return;
}

node * newnode = (node*)malloc(sizeof(node)); //make a new node
newnode->data = val;
int counter = 1; //define a counter

//while loop to find the right place for previous node of new node
while(list -> next != NULL && counter < place-1 )
{
list = list->next;
counter++;
}

//patch in the note by updating the pointer(s)

newnode->next = list->next;
list->next = newnode;

}






int main()
{
        node *root=NULL; //very important line. Otherwise all function will fail
        node *t;
        int ch,ele,v, del;
        while(1)
        {
                printf("\nMenu: 1. insert at front, 2. reverse list 3. Insert to place 0. exit: ");
            scanf("%d",&ch);
                if(ch==0)
                {
                        printf("\nGOOD BYE>>>>\n");
                        break;
                }
                if(ch==1)
                {
                        printf("\nEnter data (an integer): ");
                        scanf("%d",&ele);
                        root = insert_front(root, ele);


            display(root);


                }
                if(ch==2)
                {
                        root = reverse(root);
                        printf("List reversed.\n");
                        display(root);


                }
                if(ch==3)
                {
                    int place;
                        printf("\nEnter data (an integer) and place (>1) separated by space: ");
                        scanf("%d %d",&ele, &place);
                        insertToPlace(root, ele, place);


            display(root);


                }


        }
  return 0;
}
