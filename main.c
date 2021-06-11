#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
typedef struct node
{
	int counter;
	char fileName[100];
	char fileContent[20000];
	struct node *sibling;
	struct node *child;
	struct node *parent;
	int degree;
} node;
node *FIRST = NULL;

node *createNewNode(int counter, char *fileContent, char *fileName)
{
	node *newNode = (node *)malloc(sizeof(node));
	newNode->degree = 0;
	newNode->child = NULL;
	newNode->parent = NULL;
	newNode->sibling = NULL;
	newNode->counter = counter;
	strcpy(newNode->fileContent, fileContent);
	strcpy(newNode->fileName, fileName);
	return newNode;
}

void correctNodes(node *prev, node *current, node *next)
{
	if (current->counter > next->counter)
	{
		current->sibling = next->sibling;
		next->parent = current;
		next->sibling = current->child;
		current->child = next;
		current->degree++;
	}
	else
	{
		if (prev == NULL)
			FIRST = next;
		else
			prev->sibling = next;

		current->parent = next;
		current->sibling = next->child;
		next->child = current;
		next->degree++;

		current = next;
	}
}

void unionHeaps(node *FIRST)
{
	node *prev = NULL;
	node *current = FIRST;
	node *next = FIRST->sibling;
	while (next != NULL)
	{

		if ((current->degree != next->degree))
		{

			prev = current;
			current = next;
		}
		else
		{
			correctNodes(prev, current, next);
		}
		next = current->sibling;
	}
}
void insertNode(node *FIRST, node *newNode)
{
	if (FIRST->sibling == NULL)
	{
		FIRST->sibling = newNode;
	}
	else
	{
		insertNode(FIRST->sibling, newNode);
	}
}

int isItFinished(node *FIRST)
{
	node *current = FIRST;
	node *next = FIRST->sibling;
	while (next != NULL)
	{
		if (current->degree == next->degree)
		{
			return 0;
		}
		else
		{
			current = next;
			next = current->sibling;
		}
	}
	return 1;
}

int checkIfSorted(node *FIRST)
{
	node *current = FIRST;
	node *next = FIRST->sibling;
	while (next != NULL)
	{
		if (current->degree < next->degree)
		{
			return 0;
		}
		else
		{
			current = next;
			next = current->sibling;
		}
	}
	return 1;
}

void sortElements(node *FIRST)
{
	node *prev = NULL;
	node *current = FIRST;
	node *next = FIRST->sibling;
	while (next != NULL)
	{
		if (current->degree < next->degree)
		{
			if (prev == NULL)
			{
				FIRST = next;
			}
			else
			{
				prev->sibling = next;
			}
			current->sibling = next->sibling;
			next->sibling = current;
		}
		prev = current;
		current = next;
		next = current->sibling;
	}
	if (!checkIfSorted(FIRST))
	{
		sortElements(FIRST);
	}
}

node *findMax(node *FIRST)
{
	node *currentNode = FIRST;
	node *maxNode = FIRST;
	while (currentNode->sibling != NULL)
	{
		if (maxNode->counter < currentNode->counter)
		{
			maxNode = currentNode;
		}
		else
		{
			currentNode = currentNode->sibling;
		}
		if (currentNode->sibling == NULL)
		{
			if (maxNode->counter < currentNode->counter)
			{
				maxNode = currentNode;
			}
		}
	}
	/*printf("MAX: %d\n", maxNode->counter);*/
	return maxNode;
}

node *findLastSibling(node *bhNode)
{

	while (1)
	{
		if (bhNode->sibling == NULL)
		{
			return bhNode;
		}
		else
		{
			bhNode = bhNode->sibling;
		}
	}
}

node *popmax(node *FIRST)
{
	static int count = 1;
	if (FIRST->child == NULL)
	{
		printf("%d:\tFile Name: %s | Occurences: %d\n", count++, FIRST->fileName, FIRST->counter);
		return NULL;
	}
	node *maxNode = findMax(FIRST);
	printf("%d:\tFile Name: %s | Occurences: %d\n", count++, maxNode->fileName, maxNode->counter);
	node *prev = NULL;
	node *current = FIRST;
	node *next = FIRST->sibling;
	while (1)
	{
		if (current == maxNode)
		{
			if (prev == NULL)
			{
				FIRST = maxNode->child;
				findLastSibling(maxNode->child)->sibling = maxNode->sibling;
			}
			else
			{
				prev->sibling = maxNode->child;
				if (maxNode->sibling != NULL)
					findLastSibling(maxNode->child)->sibling = maxNode->sibling;
			}
			break;
		}
		else
		{
			prev = current;
			current = next;
			next = current->sibling;
		}
	}
	sortElements(FIRST);
	return FIRST;
}

void toLowerCase(char *p)
{
	char *src = p, *dst = p;

	while (*src)
	{
		if ((ispunct((unsigned char)*src) && *src != '-' && *src != '\''))
		{
			/* Skip this character */
			src++;
		}
		else if (isupper((unsigned char)*src))
		{
			/* Make it lowercase */
			*dst++ = tolower((unsigned char)*src);
			src++;
		}
		else if (src == dst)
		{
			/* Increment both pointers without copying */
			src++;
			dst++;
		}
		else
		{
			/* Copy character */
			*dst++ = *src++;
		}
	}

	*dst = 0;
}

void bh_traverse(node *first)
{

	if (first == NULL)
		return;

	static int count = 1;
	printf("%d: %d - %d \n", count++, first->counter, first->degree);

	bh_traverse(first->sibling);
	/* bh_traverse(first->child); */
}
int main(void)
{
	printf("Please enter a word:\n");
	char word[60];
	scanf("%s", word);

	struct dirent *de; /* Pointer for directory entry */
	DIR *dr = opendir("./files");

	if (dr == NULL) /* opendir returns NULL if couldn't open directory */
	{
		printf("Could not open current directory");
		return 0;
	}

	int fileCounter = 0;
	while ((de = readdir(dr)) != NULL)
	{

		if (!(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")))
		{
			char directory[60] = "./files/";
			strcat(directory, de->d_name);
			FILE *fp = fopen(directory, "r");
			char buff[2];
			buff[1] = '\0';
			int counter = 0;
			fileCounter++;
			char *fileContent = (char *)calloc(20000, sizeof(char));
			while (!feof(fp))
			{
				buff[0] = fgetc(fp);
				buff[0] = tolower(buff[0]);
				strcat(fileContent, buff);
			}
			/*printf("%s -> %s\n",de->d_name, fileContent);*/
			char *strPurge = strtok(fileContent, " *:,.!?\\/\"\n\t()");

			while (strPurge != NULL)
			{

				if (strcmp(strPurge, word) == 0)
				{
					counter++;
				}
				strPurge = strtok(NULL, " *:,.!?\\/\"\n\t()");
			}
			/* creating head. If head is already created then program will create node.*/
			if (FIRST == NULL)
			{
				FIRST = createNewNode(counter, fileContent, de->d_name);
			}
			else
			{
				insertNode(FIRST, createNewNode(counter, fileContent, de->d_name));

				unionHeaps(FIRST);
			}

			fclose(fp);
		}
	}
	int i = 0;
	while (i < fileCounter)
	{

		FIRST = popmax(FIRST);
		if (FIRST != NULL)
			while (!isItFinished(FIRST))
			{
				unionHeaps(FIRST);
			}
		i++;
	}

	/*bh_traverse(FIRST);*/

	closedir(dr);
	return 0;
}
