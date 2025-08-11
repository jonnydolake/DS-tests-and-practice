#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    unsigned int index;
    int drawing_test;
} Frame;

typedef struct {
    unsigned int arr_size;
    Frame *element;
} Timeline;



Timeline *grow_timeline(Timeline *time_l, int append);

int init_size = 24;

int main(void)
{
    Timeline *test1;
    test1 = malloc(sizeof *test1);
    if (test1 == NULL) return 1;

    test1->element = malloc((sizeof *test1->element) * init_size);
    if (test1->element == NULL) return 2;
    test1->arr_size = init_size;

    for (int i = 0; i < init_size; i++)
    {
        test1->element[i].index = i;
        test1->element[i].drawing_test = 69;
    }

    // testing
    printf("%d\n", test1->element[5].index);
    printf("%d\n", test1->element[9].index);
    printf("%d\n", test1->element[20].index);
    printf("%d\n", test1->element[20].drawing_test);

    test1 = grow_timeline(test1, 10); //need to realloc only for the frame array

    printf("%d\n", test1->element[32].index);
    printf("%d\n", test1->arr_size);

    printf("%zu\n", sizeof(*test1->element));


    printf("C Version: %ld\n", __STDC_VERSION__); // print the C version

    free(test1->element);
    free(test1);

    return 0;
}

// TODO: change the function to take only the frame array! (not the timeline struct)
Timeline *grow_timeline(Timeline *time_l, int append)
{
    Timeline *temp; 
    temp = malloc (sizeof *temp);
    if (temp == NULL) {
        printf("Append failled! Not enough memory!\n");
        return time_l; // return the original pointer to the timeline 
        // so we dont lose it in failure case, not really sure if this is how it work tho...
    }
    int new_size = time_l->arr_size + append;

    temp->element = realloc(time_l->element, (sizeof *temp->element) * new_size);
    if (temp->element == NULL) {
        free(temp);
        printf("Append failled! Not enough memory!\n");
        return time_l;
        // return the original pointer to the timeline so we dont lose it in failure case 
        // not really sure if this is how it works tho...
    }

    temp->arr_size = new_size;
    for (int i = time_l->arr_size; i < new_size; i++) {
        temp->element[i].index = i;
        temp->element[i].drawing_test = 69;
    }

    printf("Append succesful!\n");

    return temp;
}
