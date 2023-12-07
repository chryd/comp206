#include <stdio.h>
struct ZoomRecord
{
        char email[60]; // email of the student
        char name[60]; // name of the student
        int durations[9]; // duration for each lab.
	float attendance;
        struct ZoomRecord *next;
};

extern struct ZoomRecord *head;
struct ZoomRecord* csvToList(FILE *file_ptr);
void generateAttendance(struct ZoomRecord *head, FILE *output);
void freeList(struct ZoomRecord *ptr);
