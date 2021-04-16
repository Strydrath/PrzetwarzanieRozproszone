#include  <string.h>
#include  <stdio.h>
#include  <stdlib.h>
/***
 * Definicja kolejki priorytetowej
 */

struct queue {
	struct element* head=NULL;
}Q;
struct element {
	int val;
	struct element* next;
	int priority;
};

typedef struct queue queue;
typedef struct element element;

/***
 * Alokacja pamięci na nowy element
 */
element* newElement(int val, int priority) {
	struct element* e = (struct element*)malloc(sizeof(struct element));
	e->val = val;
	e->priority = priority;
	e->next = NULL;
	return e;
}

/*
 * 5 pkt - push(queue* a), pop(queue* a), print(queue*)
 */

 /***
  * Dodanie elementu na koniec listy
  */
void push(struct queue* a, int val) {

	struct element* n = newElement(val, 0);
	struct element* now = a->head;
	if (a->head == NULL) {
		a->head = n;
	}
	else {
		while (now->next != NULL) {
			now = now->next;
		} 
		now->next = n;
	}
}
/***
 * zwrócenie wartości elementu o najwyższym priorytecie i usunięcie go z kolejki
 */
int pop(queue* a)
{
	element* temp = a->head;
	int val = temp->val;
	a->head = a->head->next;
	free(temp);
	return val;
}
/***
*Drukowanie wszysytkich elementów kolejki
*/
void print(struct queue* a) {
	struct element* n = a->head;
	if (n != NULL) {
		do {
			printf(" %d ,", n->val);
			n = n->next;
		} while (n != NULL);
	}	printf("\n");
}

/*
 * 8 pkt - insert(queue* a, element* c, int priority), delete(queue* a, priority)
 */
void insert(queue* a, int val, int priority) {
	element* el = newElement(val, priority);
	element* now = a->head;
	element* temp;
	if (now == NULL) {
		a->head = el;
		return;
	}
	if (priority > a->head->priority) {
		el->next = a->head;
		a->head = el;
	}
	else {
		if (now != NULL) {
			while ( now->next != NULL) {
				if (now->next->priority < priority)
					break;
				now = now->next;
			}
		}
		temp = now->next;
		now->next = el;
		el->next = temp;
	}
}

void qDelete(queue* a, int priority) {
	element* now = a->head;
	element* temp;
		
	if(now!=NULL) {
		if (now->priority == priority) {
			while (now->priority == priority) {
				temp = now;
				a->head = temp->next;
				now = now->next;
				free(temp);
			}
		}
		else {
			while (now->next != NULL) {
				if (now->next->priority == priority) {
					temp = now->next;
					now->next = temp->next;
					free(temp);
				}
				else {
					now = now->next;
				}
				if (now->priority < priority)
					break;
			}
		}
	}
}
 /*
  *
  * 10 pkt -  merge_two(queue* a, queue* b)        ; wynikiem jest nowa kolejka
  */


queue* merge_two(queue* a, queue* b) {
	queue* wynik = (queue*)malloc(sizeof(queue));
	wynik->head = NULL;
	element* now = a->head;
	do {
		insert(wynik, now->val, now->priority);
		now = now->next;
	} while (now!= NULL);
	now = b->head;
	do {
		insert(wynik, now->val, now->priority);
		now = now->next;
	} while (now!= NULL);
	return wynik;
}

int main() {
	queue q1,q2;
	push(&q1, 1);
	push(&q1, 2);
	push(&q1, 3);
	insert(&q1, 4, 2);
	insert(&q1, 5, 4);
	insert(&q1, 6, 2);
	insert(&q1, 7, 3);
	print(&q1);
	pop(&q1);
	print(&q1);
	qDelete(&q1, 2);
	print(&q1);
	push(&q2, 8);
	push(&q2, 9);
	push(&q2, 10);
	insert(&q2, 11, 2);
	insert(&q2, 12, 4);
	insert(&q2, 13, 2);
	insert(&q2, 14, 3);
	print(&q2);
	queue *q3 = merge_two(&q1, &q2);
	print(q3);
	return 0;
}
