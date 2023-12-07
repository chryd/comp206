#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Program to generate a report on the students having the top CGPAs
***************************************************************
* Author Dept. Date Notes
***************************************************************
* Christine Y.-D. Comp. Science. Apr 02 2021 Error Parsing and File to Array fct.
* Christine Y.-D. Comp. Science. Apr 05 2021 File to Array Function.
* Christine Y.-D. Comp. Science. Apr 07 2021 Final version, debugging and testing
* Christine Y.-D. Comp. Science. Apr 09 2021 Debugging and comments
*/

struct StudentRecord //node
{
    long sid;
    char email[30];
    char lname[20];
    char fname[20];
    float cgpa;
    struct StudentRecord* next;
};

void insertAfter(struct StudentRecord *new, struct StudentRecord *prev){
    //changing the order of the nodes
    struct StudentRecord *temp = prev -> next;
    prev -> next = new;
    new -> next = temp;
}

void freeList(struct StudentRecord *ptr){ //to free the memory of a list
    while (ptr != NULL){ //iterate the lisr
        struct StudentRecord *temp = ptr; //setting a temp, to keep track of next
        ptr = ptr -> next; //iterate to next
        free(temp); //free the node
    }
}

struct StudentRecord* addStudent(struct StudentRecord *head, long s, char e[], char l[], char f[], float c)
{
    //allocating memory space for the student
    struct StudentRecord *temp = (struct StudentRecord*) malloc(sizeof(struct StudentRecord));
    struct StudentRecord *ptr = head; //to prevent changing head

    if (temp == NULL) { //when malloc is not allocated, due to lack of memory space for example
            freeList(head); //free the memory
	    puts("Error! program ran out of memory");
            exit(EXIT_FAILURE); //stop the program
    }

    //setting the data for each attribute
    temp -> sid = s;
    strcpy(temp -> email, e);
    strcpy(temp -> lname, l);
    strcpy(temp -> fname, f);
    temp -> cgpa = c;

    if (head == NULL) {// If student is first node in list
		temp -> next = NULL;
		return temp; //return new student as head
    }
    
    //If the value of the cgpa of the new student is bigger than head
    if (temp -> cgpa > head -> cgpa){ 
        temp -> next = head; // Chain to list (at head of list)
	return temp; // return as the new head of the list
    }

    while (ptr -> next != NULL){ //going through the list
        if (temp -> cgpa >= ptr -> next -> cgpa){ //when the cgpa is bigger than next
            insertAfter(temp, ptr); //insert the student in front of the next
            return head; //return head as head
        }
        ptr = ptr -> next; //to iterate through list
    }
    insertAfter(temp, ptr); //else, if smallest cgpa, insert as last
    return head; //return the head as head
}

struct StudentRecord* lineToStudent(char line[], struct StudentRecord* head){
    char *strsid, *strCGPA, *waste;

    char email[30];
    char lname[20];
    char fname[20];

    strsid = strtok(line, ","); //getting the sid as string
    if (strcmp("sid", strsid) == 0) return NULL; //line is header if first argument is "sid"

    long sid = strtol(strsid, &waste, 10); //get sid as long from string sid
    //allocating the args to attributes of node
    strcpy(email, strtok(NULL, ","));
    strcpy(lname, strtok(NULL, ","));
    strcpy(fname, strtok(NULL, ","));
    strCGPA = strtok(NULL, ","); //get the cgpa as string
    float cgpa = atof(strCGPA); //typecasting string cgpa to float

    //add  a student with the corresponding attributes
    return addStudent(head, sid, email, lname, fname, cgpa); 
}

struct StudentRecord* csvToList(FILE *file_ptr){
    struct StudentRecord* head = NULL; //set head to NULL

    char line[100];
    while (!feof(file_ptr)){
        fgets(line, 100, file_ptr);
	//if the line is not the end of file
    	if (!feof(file_ptr)) head = lineToStudent(line, head); //add student to list 
    }
    return head;
}

int ntop(struct StudentRecord *head){
    struct StudentRecord *temp = head; //to not change head
    int n; //how many top students are there 

    for (n = 1; n < 5; n++) {
            if (temp->next == NULL) return n; //if there is less than 5 students, return the number
            temp = temp -> next; //else, continue to iterate
    }

    while (temp != NULL && (temp -> next) != NULL ){ //iterating through the list
        if (temp -> cgpa == ((temp -> next) -> cgpa)){ //if the cgpas of the current is same as next
            temp = temp -> next;
            n++; //more top students than 5
        } else {
            return n ;
        }
    }
   
    return n;
}


void writeInfo(struct StudentRecord* ptr, FILE *outputf){
    fputs("sid,email,cgpa\n", outputf); //print the header
    int n = ntop(ptr); //get how many top students there are
    for (int i = 0; i < n; i++){ //print as many top students as needed
        fprintf(outputf, "%ld, %s, %.1f\n", ptr -> sid, ptr -> email, ptr -> cgpa); //print the needed info
        ptr = ptr -> next; //iterate the list
    }
}

int main(int argc, char *argv[]){

    if(argc != 3){ //wrong number of arguments
        puts("Usage ./topcgpas <sourcecsv> <outputcsv>");
        return EXIT_FAILURE;
    }
 
    //assigning the argument to variables
    char* input_filename = argv[1];
    char* output_filename = argv[2];

    FILE *inputf = fopen(input_filename, "rt");
    if (inputf == NULL){ //if the file is not open, so pointer points to NULL
        printf("Error! Unable to open the input file %s\n", input_filename);
        return EXIT_FAILURE;
    }

    struct StudentRecord *top = csvToList(inputf); //get the head of list

    if (top == NULL) { //if there are no student in the list
        printf("Error! Input CSV file %s is empty\n", input_filename);
        return EXIT_FAILURE;
    }

    FILE *outputf  = fopen(output_filename, "wt");
    if (outputf == NULL){
    	freeList(top); //free memory for the list
        printf("Error! Unable to open the output file %s\n", output_filename);
        return EXIT_FAILURE;
    }

    writeInfo(top, outputf); //print the top info

    freeList(top); //free the memory of the list
    fclose(inputf); fclose(outputf); //close the files

    return EXIT_SUCCESS;
}
