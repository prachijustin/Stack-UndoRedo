
#include<iostream>
#include<stdlib.h>
#include<stdio.h>

using namespace std;

struct LinkedList
{
    int data;
    struct LinkedList *next;
};

struct UndoStack
{
    int data;
    struct UndoStack *next;
};

struct RedoStack
{
    int data;
    struct RedoStack *next;
};

int count_ll=0;           //count of linked list
int count_undo=0;         //count of undo stack
int count_redo=0;         //count of redo stack
int check_insertion=0;    //number of insertions for undo
int check=0;              //number of insertions for redo
int check_deletion=0;     //number of deletions for undo
int check_d=0;            //number of deletions for redo
int check_undo=0;         //count of undo operations

struct LinkedList *head,*current1,*trail1=0,*current2,*trail2=0,*current3,*trail3=0,*current4,*trail4=0,*current5,*trail5=0;
struct LinkedList *current6, *trail6=0, *current7, *trail7=0, *current8, *trail8=0;
struct UndoStack *top_undo;
struct RedoStack *top_redo;

class List_Operations
{
    public:
        int getHead();
        bool find(int v);         //finds "v" in linked list
        void insert(int v);       //insert "v" into linked list
        void deletes(int v);      //delete "v" from linked list
        void undo();              //performs undo operation
        void redo();              //performs redo operation
        void display();           //displays the linked list
}ll;


int List_Operations::getHead()
{
    int h;
    if(head==NULL)
        return 0;
    else
    {
       h=head->data;
       return(h);
    }
}

bool List_Operations::find(int v)
{
    if(head==NULL)
        cout<<endl<<"List is empty";
    else
    {
        //traverse the list
        current1=head;
        while(current1!=0)
       {
           if(current1->data==v)
               return true;
           else
           {
               current1=current1->next;
               if(current1->data==v)
                   return true;
               else
                   return false;
           }
       }
    }
}

void List_Operations::insert(int v)
{
    check_insertion++;
    check++;
    struct LinkedList *ptr= (struct LinkedList*)malloc(sizeof(struct LinkedList));
    struct UndoStack *pt= (struct UndoStack*)malloc(sizeof(struct UndoStack));
    ptr->data=v;
    ptr->next=NULL;
    pt->data=v;
    pt->next=NULL;

    //case 1- empty list
    if((head==NULL)&&(top_undo==NULL))
    {
       head=ptr;
       top_undo=pt;
       count_ll++;
       count_undo++;
    }
    else
    {
        current2=head;
        trail2=0;
        while(current2!=0)
        {
            if(current2->data>=ptr->data)
                break;
            else
            {
                trail2=current2;
                current2=current2->next;
            }
        }

        //case 2- insert at head(not empty)
        if(current2==head)
        {
            ptr->next=head;
            head=ptr;
            count_ll++;
        }

        else
        {
           //case 3- insert after the head(not empty)
            ptr->next=current2;
            trail2->next=ptr;
            count_ll++;
        }

        //inserting into undo stack
        pt->next = top_undo;
        pt->data = v;
        top_undo = pt;
        count_undo++;
    }
}

void List_Operations::deletes(int v)
{
    check_deletion++;
    check_d++;

    //case 1- empty list
    if(head==0)
        cout<<"Node cannot be deleted from an empty list";
    else
    {
        current3=head;
        trail3=0;

        //traverse the list to find the delete node
        while(current3!=0)
        {
            if(current3->data==v)
                break;
            else
            {
                trail3=current3;
                current3=current3->next;
            }
        }

        //inserting that node in undostack
        int t=current3->data;
        struct UndoStack *newNode= (struct UndoStack*)malloc(sizeof(struct UndoStack));
        newNode->data=t;
        newNode->next=top_undo;
        top_undo=newNode;
        count_undo++;

        //case 2- delete node not found
        if(current3==0)
            cout<<endl<<"Node not found";
        else
        {
            //case 3-delete from head node
            if(head==current3)
            {
                head=head->next;
                count_ll--;
            }
            //case 4- delete beyond the head node
            else
            {
                trail3->next=current3->next;
                count_ll--;
            }
            delete current3;
        }
    }
}

void List_Operations::undo()
{
    check_undo++;
    if(check_insertion>=1)
    {
         if(check_deletion>=1)
        {
            int a= top_undo->data;

            //insert a in ll
            struct LinkedList *newNode= (struct LinkedList*)malloc(sizeof(struct LinkedList));
            newNode->data=a;
            newNode->next=NULL;
            //case 1- empty list
            if(head==NULL)
            {
                head=newNode;
                count_ll++;
            }
            else
            {
                current5=head;
                trail5=0;
                //Traverse list to find the insert location
                while(current5!=0)
                {
                    if(current5->data>=newNode->data)
                        break;
                    else
                    {
                        trail5=current5;
                        current5=current5->next;
                    }
                }
                //case 2- insert at head(not empty)
                if(current5==head)
                {
                    newNode->next=head;
                    head=newNode;
                    count_ll++;
                }
                else
                {
                    //case 3- insert after the head(not empty)
                    newNode->next=current5;
                    trail5->next=newNode;
                    count_ll++;
                }
            }

            //delete top i.e., 'a' from undostack
            if(top_undo==NULL)
                cout<<endl<<"Undo operation does not exists to be performed!";
            else
            {
                struct UndoStack *temp1 = top_undo;
                top_undo = temp1->next;
                free(temp1);
                count_undo--;
            }

            //insert 'a' in redostack
            struct RedoStack *newNodes=(struct RedoStack*)malloc(sizeof(struct RedoStack));
            newNodes->data = a;
            if(top_redo == NULL)
            {
                newNodes->next = NULL;
                count_redo++;
            }
            else
            {
            newNodes->next = top_redo;
            count_redo++;
            }
            top_redo = newNodes;

            check_deletion--;
        }
        else
        {
            int x= top_undo->data;

            //insert x in redostack
            struct RedoStack *newNode= (struct RedoStack*)malloc(sizeof(struct RedoStack));
            newNode->data=x;
            if(top_redo==NULL)
            {
                newNode->next=NULL;
                count_redo++;
            }
            else
            {
                newNode->next=top_redo;
                count_redo++;
            }
            top_redo = newNode;

            //delete x from ll
            //case 1- empty list
            if(head==0)
                cout<<"Node cannot be deleted from an empty list";
            else
            {
                current4=head;
                trail4=0;

                //traverse the list to find the delete node
                while(current4!=0)
                {
                    if(current4->data==x)
                        break;
                    else
                    {
                        trail4=current4;
                        current4=current4->next;
                    }
                }
                //case 2- delete node not found
                if(current4==0)
                    cout<<endl<<"Node not found";
                else
                {
                    //case 3-delete from head node
                    if(head==current4)
                    {
                        head=head->next;
                        count_ll--;
                    }
                    //case 4- delete beyond the head node
                    else
                    {
                        trail4->next=current4->next;
                        count_ll--;
                    }
                    delete current4;
                }
            }

            //delete top i.e., x from undostack
            if(top_undo==NULL)
                cout<<endl<<"No undo operation exists to be performed!";
            else
            {
                struct UndoStack *temp = top_undo;
                top_undo = temp->next;
                free(temp);
                count_undo--;
            }
        }

        check_insertion--;
        cout<<endl<<"Undo done!";
    }
    else
    {
        cout<<endl<<"no op left to be undo";
    }
}

void List_Operations::redo()
{
    if(check>=1)
    {
        if(check_undo!=0)
        {
            if(check_d!=0)
            {
                int c= top_redo->data;

                //insert c in undostack
                struct UndoStack *pk=(struct UndoStack*)malloc(sizeof(struct UndoStack));
                pk->data =c;
                if(top_undo== NULL)
                {
                    pk->next = NULL;
                    count_undo++;
                }
                else
                {
                    pk->next = top_undo;
                    count_undo++;
                }
                top_undo = pk;

                //delete c from ll
                //case 1- empty list
                if(head==0)
                    cout<<"Node cannot be deleted from an empty list";
                else
                {
                    current7=head;
                    trail7=0;

                    //traverse the list to find the delete node
                    while(current7!=0)
                    {
                        if(current7->data==c)
                            break;
                        else
                        {
                            trail7=current7;
                            current7=current7->next;
                        }
                    }
                    trail7=head;
                    //case 2- "c" nide not found
                    if(current7==0)
                        cout<<endl<<"Node not found";
                    else
                    {
                        //case 3-delete from head node
                        if(head==current7)
                        {
                            head=head->next;
                            count_ll--;
                        }
                        //case 4- delete beyond the head node
                        else
                        {
                            trail7->next=current7->next;
                            count_ll--;
                        }
                        delete current7;
                    }
                }


            //delete top i.e., c from redostack
            if(top_redo == NULL)
                cout<<endl<<"Redo operations does not exist to be performed!";
            else
            {
                struct RedoStack *po = top_redo;
                top_redo = po->next;
                free(po);
                count_redo--;
            }
            check_d--;
        }
        else
        {
            int b=top_redo->data;

            //insert b in ll
            struct LinkedList *newNoda= (struct LinkedList*)malloc(sizeof(struct LinkedList));
            newNoda->data=b;
            newNoda->next=NULL;
            //case 1- empty list
            if(head==NULL)
            {
                head=newNoda;
                count_ll++;
            }
            else
            {
                current6=head;
                trail6=0;
                //Traverse list to find the insert location
                while(current6!=0)
                {
                    if(current6->data>=newNoda->data)
                        break;
                    else
                    {
                        trail6=current6;
                        current6=current6->next;
                    }
                }
                //case 2- insert at head(not empty)
                if(current6==head)
                {
                    newNoda->next=head;
                    head=newNoda;
                    count_ll++;
                }
                else
                {
                    //case 3- insert after the head(not empty)
                    newNoda->next=current6;
                    trail6->next=newNoda;
                    count_ll++;
                }
            }

            //insert b in undostack
            struct UndoStack *p=(struct UndoStack*)malloc(sizeof(struct UndoStack));
            p->data = b;
            if(top_undo == NULL)
            {
                p->next = NULL;
                count_undo++;
            }
            else
            {
                p->next = top_undo;
                count_undo++;
            }
            top_undo = p;

            //delete top i.e., b from redostack
            if(top_redo == NULL)
                cout<<endl<<"redo stack is empty!";
            else
            {
                struct RedoStack *temph = top_redo;
                top_redo = temph->next;
                free(temph);
                count_redo--;
            }

        }

        check_undo--;


        cout<<endl<<"Redo done!";
        }

        else
        {
            cout<<endl<<"no undo exists nd so no redo can be done";
        }

        check--;

    }

    else
    {
        cout<<endl<<"linked list is empty nd so redo cannot be done";

    }

}

void List_Operations::display()
{
    struct LinkedList *temp=head;
    if(head==NULL)
        cout<<endl<<"Linked list is empty!";
    else
    {
        cout<<endl<<"Linked list is: ";
        while(temp->next!=NULL)
        {
            cout<<"\t"<<temp->data;
            temp=temp->next;
        }
        cout<<"\t"<<temp->data;
    }
}


int main()
{
    int choice, get_head, find_ele, insert_value, delete_value;
    char str;
    do
    {
        cout<<endl<<endl<<"Menu driven program:";
        cout<<endl<<"1. Get head of the list";
        cout<<endl<<"2. Find the value";
        cout<<endl<<"3. Insert a node";
        cout<<endl<<"4. Delete a node";
        cout<<endl<<"5. Undo an operation";
        cout<<endl<<"6. Redo an operation";
        cout<<endl<<"7. Display the list";
        cout<<endl<<"8. Exit"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;
        switch(choice)
        {
        case 1:
            get_head= ll.getHead();
            if(get_head==0)
                cout<<endl<<"List is empty and so head is not pointing to anything.";
            else
                cout<<endl<<"Head of the list: "<<get_head;
            break;
        case 2:
            cout<<endl<<"Enter a value you want to find: ";
            cin>>find_ele;
            ll.find(find_ele);
            if(ll.find(find_ele))
                cout<<endl<<"Value found!";
            else
                cout<<endl<<"Value not found!";
            break;
        case 3:
            cout<<endl<<"Enter a value you want to insert: ";
            cin>>insert_value;
            ll.insert(insert_value);
            cout<<endl<<"Value inserted successfully!!";
            break;
        case 4:
            cout<<endl<<"Enter a value you want to delete: ";
            cin>>delete_value;
            ll.deletes(delete_value);
            cout<<endl<<"Value deleted successfully!!";
            break;
        case 5:
            ll.undo();
            break;
        case 6:
            ll.redo();
            break;
        case 7:
            ll.display();
            break;
        case 8:
            exit(0);
            break;
        default:
            cout<<endl<<"Invalid choice!!";
            break;
        }
        cout<<endl<<"Do you want to continue?";
        fflush(stdin);
        cin>>str;
    }while(str='y'||str=='Y');
    return 0;
}
