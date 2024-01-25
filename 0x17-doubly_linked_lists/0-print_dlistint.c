#include "lists.h"
/**
 * node previous and next
 * dlistint_t list
 *
 * Return: the number of nodes
 */
size_t print_dlistint(const dlistint_t *p)
{
	int count;
	count = 0;
	if (p == NULL)
		return (count);
	while (p->prev != NULL)
		p = p->prev;
	while (p != NULL)
	{
		printf("%d\n", p->n);
		count++;
		p = p->next;
	}

	return (count);
}
