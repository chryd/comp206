#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "zoomrecs.h"

void insertAfter(struct ZoomRecord *new, struct ZoomRecord *prev){
	//changing the order of the nodes
	struct ZoomRecord *temp = prev -> next;
	prev -> next = new;
	new -> next = temp;
}

void mergeInfo(struct ZoomRecord *new, struct ZoomRecord *ptr){
	//merge the durations
	for (int i = 0; i < 9; i++){
		ptr -> durations[i] += (new -> durations[i]);
	}
}

void updateAttendance(struct ZoomRecord *ptr){
	//compute the attendance for each student
	while(ptr != NULL){
		float attended = 0;
		for (int i = 0; i < 9; i++) {
			if(ptr->durations[i] >= 45) attended++;
		}
		ptr -> attendance = attended/9.00 * 100.00;
		ptr = ptr -> next;
	}
}

int isBefore(char s1[], char s2[]){
	int i = 0;

	while (s1[i] != '\0' && s2[i] != '\0'){
		if (s1[i] < s2[i]) return 1; //for true
		if (s1[i] > s2[i]) return 0;
        i++;
	}
	return 0; //for false
}

struct ZoomRecord* addZoomRecord(struct ZoomRecord *head, char e[], char n[], int d[]) {
	//allocating memory space for the student
	struct ZoomRecord *temp = (struct ZoomRecord*) malloc(sizeof(struct ZoomRecord));
	struct ZoomRecord *ptr = head; //to prevent changing head

	//setting the data for each attribute
	strcpy(temp -> email, e);
	strcpy(temp -> name, n);
	memcpy(temp -> durations, d, sizeof(temp -> durations));

	if (head == NULL) {    // If student is first node in list
		temp -> next = NULL;
		return temp; //return new student as head
	}

	if (isBefore(temp->email, head->email)){ //strictly before
		temp -> next = head;
		return temp;
	}

	while (ptr != NULL) {    //going through the list
		if (strcmp(temp->email,ptr->email) == 0){ //when the student is already in the list
			mergeInfo(temp, ptr); //merge the info
			return head;
		}

		if (ptr->next != NULL) {
			if (isBefore(temp->email, ptr->next->email)){
				//if the next email has is alphabetically after the new record
				insertAfter(temp, ptr); //insert abefore the next, after the current point of iteration
				return head;
			}
		} else {
			insertAfter(temp, ptr);
			return head;
		}
		ptr = ptr -> next;
	}
}

struct ZoomRecord* lineToRecording(char line[], struct ZoomRecord* head){
    char *strdur;
    char email[60], name[60], lab[2];
    int attendance[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    strcpy(email, strtok(line, ","));
    if (strcmp("User Email", email)==0) return NULL; //line is header if first argument is "User Email"

    //allocating the args to attributes of node
    strcpy(name, strtok(NULL, ","));
    strcpy(lab, strtok(NULL, ","));
    strdur = strtok(NULL, ","); //get the duration as string
    int dur = atof(strdur); //typecasting string duration to int
    attendance[(lab[0] - 65)] = dur;

    //add a recording with the corresponding attributes
    return addZoomRecord(head, email, name, attendance);
}

struct ZoomRecord* csvToList(FILE *file_ptr){
    struct ZoomRecord* head = NULL; //set head to NULL

    char line[200];
    while (!feof(file_ptr)){
        fgets(line, 200, file_ptr);
	//if the line is not the end of file
        if (!feof(file_ptr)) head = lineToRecording(line, head); //add recording
    }
    updateAttendance(head);
    return head;
}

void generateAttendance(struct ZoomRecord *ptr, FILE *outputf){
	//print the header
	fputs("User Email,Name (Original Name),A,B,C,D,E,F,G,H,I,Attendance (Percentage)\n", outputf);
	while (ptr != NULL){
		//print the needed info
		fprintf(outputf, "%s,%s,",ptr->email,ptr->name);
		for (int i = 0; i < 9; i++) fprintf(outputf,"%d,",ptr->durations[i]);
		fprintf(outputf,"%.2f\n",ptr->attendance);

		ptr = ptr -> next; //iterate the list
	}
}

void freeList(struct ZoomRecord *ptr){
	while (ptr != NULL){
		struct ZoomRecord *temp = ptr;
        	ptr = ptr -> next;
        	free(temp);
	}
}
