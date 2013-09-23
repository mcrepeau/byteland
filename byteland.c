/*
*
* @file Byteland.c
* @brief Solving a graph theory problem using C
* @author Matthieu Crepeau
* @version 1.01
* @date 09/16/2013
*
* @todo
*
*
*/

#include <stdio.h>
#include <stdlib.h>

// Used to define the use of assertion and debugging messages depending on the compilation options
 #ifdef DEBUG
  #define ASSERT_PERROR(assertion) if (!(assertion)) \
    printf("Error in file %s, line %d : " #assertion "@n", __FILE__, __LINE__), exit(10)
  #define DEBUG_PRINTF \
    printf
  #define DEBUG_PRINTGRAPH \
	printGraph
 #else
  #define ASSERT_PERROR(assertion)
  #define DEBUG_PRINTF(format, args...) ((void)0)
  #define DEBUG_PRINTGRAPH(graph)
 #endif

// Structure to represent an adjacency list node
struct AdjListNode
{
    int dest;
    struct AdjListNode* next;
};

// Structure to represent an adjacency list
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
    int Ci; // cost of destroying the city
    char army; // army protecting the city (either A or B)
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be N (number of cities, or vertices, in graph)
struct Graph
{
    int N;
    struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int N)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->N = N;

    // Create an array of adjacency lists.  Size of array will be N
    graph->array = (struct AdjList*) malloc(N * sizeof(struct AdjList));

     // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < N; ++i)
        graph->array[i].head = NULL;
	graph->array[i].Ci = 0;
	graph->array[i].army = ' ';

    return graph;
}

void removeElement(struct Graph* graph, int n, int i)
{

	struct AdjListNode *temp, *prev;
	struct AdjListNode *head = graph->array[i].head;
	temp=head;

	while(temp!=NULL)
	{
		if(temp->dest==n)
		{
			if(temp==head) //If node is first node
			{
				if(head->next == NULL)
				{
					graph->array[i].head = NULL;
				}
				else
				{
					head->dest = temp->next->dest; //Copy the data of the next node to head
					head->next = temp->next->next; //Remove the link of the next node
				}
				return;
			}
			else
			{
				prev->next=temp->next; // Remove node from Linked List
				return;
			}
		}
		else
		{
			prev = temp;
			temp = temp->next;
		}
	}
	return;

}

void deleteNode(struct Graph* graph, int n)
{
	int i;
	
	if (graph->array[n].head == NULL && graph->array[n].army == ' ' && graph->array[n].Ci == 0){
		DEBUG_PRINTF("Error : node %d has already been deleted !\n\n", n);
		return;
	}
	else
	{
		graph->array[n].head = NULL;
		graph->array[n].army = ' ';
		graph->array[n].Ci = 0;
	}
	
	// Deletes element in the linked lists
	for (i=0;i<graph->N;++i)
    {
		removeElement(graph, n, i);
	}
}

void setNodeProperty(struct Graph* graph, int n, int Ci, char army){

	graph->array[n].Ci = Ci;
	graph->array[n].army = army;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// A utility function to print the adjacency list representation of graph
void printGraph(struct Graph* graph)
{
    int n;

    for (n = 0; n < graph->N; ++n)
    {
        struct AdjListNode* pCrawl = graph->array[n].head;
        if (graph->array[n].army != ' ')
        {
        	printf("\n Adjacency list of city %d, protected by army %c of cost %d ->", n, graph->array[n].army, graph->array[n].Ci);
        	while (pCrawl)
        	{
        		printf(" %d", pCrawl->dest);
        		pCrawl = pCrawl->next;
        	}
        }
        printf("\n");

    }
}

void executeTestCase(struct Graph* graph)
{
	int i, n;
	int	nbs_cities_protected[graph->N], nb_cities_protected, total_nb_cities_protected;
	int costs_cities_protected[graph->N];
	float cost_ratios[graph->N], cost_ratio;
	int city, cost = 0;

	DEBUG_PRINTF("\n Cities linked to other cities protected by a different army :\n\n");

	//Run the city selection algorithm
	do
	{
		total_nb_cities_protected = 0; // Reinitializes the overall number of cities linked && protected by two different armies
		
		// Scans the array looking for links to cities protected by an opposite army
		for (n = 0; n < graph->N; ++n)
		{
			struct AdjListNode* pCrawl = graph->array[n].head;
			nbs_cities_protected[n] = 0; // Reinitializes the number of cities linked && protected by two different armies for this node
			costs_cities_protected[n] = 0;
			if (graph->array[n].army != ' ')
			{
				DEBUG_PRINTF("  %d |", n);
				while (pCrawl)
				{
					if (graph->array[n].army != graph->array[pCrawl->dest].army)
					{
						DEBUG_PRINTF(" %d ", pCrawl->dest);
						nbs_cities_protected[n]++; // Increments the number of cities linked && protected by two different armies for this node
						costs_cities_protected[n] += graph->array[pCrawl->dest].Ci;
					}
					pCrawl = pCrawl->next;
				}

				if (nbs_cities_protected[n] > 0)	cost_ratios[n]= (float) (graph->array[n].Ci)/(nbs_cities_protected[n]);
				total_nb_cities_protected+=nbs_cities_protected[n];
			}
			DEBUG_PRINTF("\n");
		}
		
		for(i=0;i<n;++i) // We scan the array of nbs_cities_protected
		{			
			if (nbs_cities_protected[i] > 0 && graph->array[i].Ci <= costs_cities_protected[i]) // When we find the first value that's above 0
			{
				cost_ratio = cost_ratios[i]; // We store it in another variable
				nb_cities_protected = nbs_cities_protected[i];
				city = i;
				for(i++;i<n;++i) // We scan the rest of the array, starting where we left off
				{
					if (nbs_cities_protected[i] > 0 && graph->array[i].Ci <= costs_cities_protected[i])
					{
						if (nbs_cities_protected[i] == nb_cities_protected) // The other variable is used to compare the rest of the array
						{	
							if (cost_ratios[i] < cost_ratio)
							{
								cost_ratio = cost_ratios[i];
								nb_cities_protected = nbs_cities_protected[i];
								city = i;
							}
						}
						if (nbs_cities_protected[i] > nb_cities_protected) // The other variable is used to compare the rest of the array
						{
							cost_ratio = cost_ratios[i];
							nb_cities_protected = nbs_cities_protected[i];
							city = i;
						}		
					}
				}
				cost += graph->array[city].Ci; //Add the cost
				//Delete the city
				DEBUG_PRINTF("\n  ->Deletion of node %d<- \n\n", city);
				deleteNode(graph, city);
			}
		}
		
	}while(total_nb_cities_protected != 0);

	DEBUG_PRINTF("\nTotal cost : ");
	printf("%d\n", cost);
}

// Driver program
int main(void)
{

    char *line = NULL;
    size_t size;
    int i, j;
    int nb_test_cases, N, M, Ci;
    char army[2];
    int city1, city2;

    struct Graph* graph;

    if (getline(&line, &size, stdin) == -1) DEBUG_PRINTF("No line\n");
    else {
            sscanf(line,"%d", &nb_test_cases);
            DEBUG_PRINTF("There are %d test cases\n", nb_test_cases);
    }

    for(i=0;i<nb_test_cases;i++){

            if (getline(&line, &size, stdin) == -1) DEBUG_PRINTF("No line\n");
            else {
                    sscanf(line,"%d %d", &N, &M);
            }

            graph = createGraph(N);

            DEBUG_PRINTF("\nTest case %d : Byteland has %d cities and %d roads\n", i+1, N, M);

            for(j=0;j<N;j++){
                    if (getline(&line, &size, stdin) == -1) DEBUG_PRINTF("No line\n");
                    else {
                            sscanf(line,"%c %d", army, &Ci);
                            //DEBUG_PRINTF("%s %d\n", army, Ci);
                            setNodeProperty(graph, j, Ci, army[0]);
                    }
            }

            for(j=0;j<M;j++){
                    if (getline(&line, &size, stdin) == -1) printf("No line\n");
                    else {
                            sscanf(line,"%d %d", &city1, &city2);
                            //DEBUG_PRINTF("%d %d\n", city1, city2);
                            addEdge(graph, city1-1, city2-1);
                    }
            }

            DEBUG_PRINTGRAPH(graph);
            executeTestCase(graph);

    }

    return 0;
}

