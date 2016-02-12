#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int id_count = 1;

// Point structure
// Contains:
//  id (spot in list)
//  temp (temperature to heat oven)
//  t (amount of time to heat)
//  next (pointer to next point)
struct Point
{
  // Unique identifier for the point
  int id;
  
  // Relevent point info
  int temp;
  int t;
  
  // Pointer to next point in list
  struct Point *next;
};

// List structure
// Contains:
//  head (pointer to top of list)
//  current (pointer to current point on list)
//  previous (pointer to previous point on list)
struct List
{
  // First point in the list. A value equal to NULL indicates that the
  // list is empty.
  struct Point *head;
  
  // Current point in the list. A value equal to NULL indicates a
  // past-the-end position.
  struct Point *current;
  
  // Pointer to the element appearing before 'current'. It can be NULL if
  // 'current' is NULL, or if 'current' is the first element in the list.
  struct Point *previous;
};

// Give an initial value to all the fields in the list.
void ListInitialize(struct List *list)
{
  list->head = NULL;
  list->current = NULL;
  list->previous = NULL;
}
 
// Move the current position in the list one element forward. If last element
// is exceeded, the current position is set to a special past-the-end value.
void ListNext(struct List *list)
{
  if (list->current)
  {
    list->previous = list->current;
    list->current = list->current->next;
  }
}

// Move the current position to the first element in the list.
void ListHead(struct List *list)
{
  list->previous = NULL;
  list->current = list->head;
}

// Get the element at the current position, or NULL if the current position is
// past-the-end.
struct Point *ListGet(struct List *list)
{
  return list->current;
}

// Set the current position to the point with the given id. If no point
// exists with that id, the current position is set to past-the-end.
void ListFind(struct List *list, int id)
{
  ListHead(list);
  
  while (list->current && list->current->id != id)
    ListNext(list);
}

// Insert a point before the element at the current position in the list. If
// the current position is past-the-end, the point is inserted at the end of
// the list. The new point is made the new current element in the list.
void ListInsert(struct List *list, struct Point *point)
{
  // Set 'next' pointer of current element
  point->next = list->current;
  
  // Set 'next' pointer of previous element. Treat the special case where
  // the current element was the head of the list.
  if (list->current == list->head)
    list->head = point;
  else
    list->previous->next = point;
    
  // Set the current element to the new point
  list->current = point;
}
 
// Remove the current element in the list. The new current element will be the
// element that appeared right after the removed element.
void ListRemove(struct List *list)
{
  // Ignore if current element is past-the-end
  if (!list->current)
    return;
    
  // Remove element. Consider special case where the current element is
  // in the head of the list.
  if (list->current == list->head)
    list->head = list->current->next;
  else
    list->previous->next = list->current->next;
    
  // Free element, but save pointer to next element first.
  struct Point *next = list->current->next;
  free(list->current);
  
  // Set new current element
  list->current = next;
}

// Displays point information (number, heat, time) on LCD
// **Output method will need to be changed for LCD
void PrintPoint(struct Point *point)
{
  printf("\nPoint %d:\n", point->id);
  printf("\tHeat: %d \tTime: %d", point->temp, point->t);
}

// Add a point to the list
// Will mostly likely be added at the end of the list
// but could be used to add points wherever needed
void AddPoint(struct List *list)
{
  struct Point *point;
  point = malloc(sizeof(struct Point));

  printf("Enter temp: ");
  fgets(point->temp, 20, stdin);
  printf("Enter t: ");
  scanf("%d", &(point->t));
  
  point->id = id_count;
  id_count++;

  ListInsert(list, point);
}

// Find a point in the list
// May or may not be useful for reflow oven
void FindPoint(struct List *list)
{
  int find_id;
  printf("Enter point number: ");
  scanf("%d", &find_id);

  ListFind(list, find_id);
  
  struct Point *point = ListGet(list);
  
  if (point)
    PrintPoint(point);
  else
    printf("Point with ID %d not found\n", find_id);
}

// Removes an individual point
// Could be implemented usefully if a list already exists
// in order to change list slightly
void RemovePoint(struct List *list)
{
  int find_id;
  printf("Enter ID: ");
  scanf("%d", &find_id);

  ListFind(list, find_id);

  ListRemove(list);
}
