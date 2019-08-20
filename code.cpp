#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <windows.h>
using namespace std;

void ClearConsoleToColors(int ForgC, int BackC);


 int pathtracerforcost[100][100];
 int pathtracerfordistance[100][100];
 int distforallsourcecost[100][100];
 int distforallsourcedist[100][100];
 int matforcost[100][100];
 int matfordist[100][100];
struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
};

struct AdjList
{
    struct AdjListNode *head;
};

struct Graph
{
    int V;
    struct AdjList* array;
};

struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    for(int i=0;i<V;++i)
    {
        graph->array[i].head=NULL;
    }

    return graph;
}

void addEdge(struct Graph* graph,int src,int dest,int weight)
{

    struct AdjListNode* newNode=newAdjListNode(dest,weight);
    newNode->next=graph->array[src].head;
    graph->array[src].head=newNode;

    newNode=newAdjListNode(src,weight);
    newNode->next=graph->array[dest].head;
    graph->array[dest].head=newNode;
}

struct MinHeapNode
{
    int  v;
    int dist;
};

struct MinHeap
{
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};


struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode =
           (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v=v;
    minHeapNode->dist=dist;
    return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
         (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void Pathtracingforcost(int source,int destination,string data[])
{
    int a=destination;
    if(distforallsourcecost[source][destination]==INT_MAX)
    {
        cout<<"SORRY this journey is not possible\n";
    }
    else
        {
        stack <int> s;
        while(pathtracerforcost[source][destination]!=source && pathtracerforcost[source][destination]!=NULL)
        {
            s.push(pathtracerforcost[source][destination]);
            destination=pathtracerforcost[source][destination];
        }
        s.push(source);
        while(!s.empty())
        {
            cout<<data[s.top()]<<"-> ";
            s.pop();
        }
        cout<<data[a]<<"\n";
        }
}

void Pathtracingfordist(int source,int destination,string data[])
{
    int a=destination;
    if(distforallsourcedist[source][destination]==INT_MAX)
    {
        cout<<"\n\nSORRY this journey is not possible\n";
    }
    else
        {
        stack <int> s;
        while(pathtracerfordistance[source][destination]!=source && pathtracerfordistance[source][destination]!=NULL)
        {
            s.push(pathtracerfordistance[source][destination]);
            destination=pathtracerfordistance[source][destination];
        }
        s.push(source);
        while(!s.empty())
        {
            cout<<data[s.top()]<<"-> ";
            s.pop();
        }
        cout<<data[a]<<"\n";
    }
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}


void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist<minHeap->array[smallest]->dist )
        {
      smallest=left;
        }

    if (right<minHeap->size &&
        minHeap->array[right]->dist<minHeap->array[smallest]->dist )
      {

      smallest=right;
      }

    if (smallest != idx)
    {

        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}


int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
    {
        return NULL;
    }

    struct MinHeapNode* root = minHeap->array[0];

    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    int i = minHeap->pos[v];

    minHeap->array[i]->dist = dist;

    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, int v)
{
   if (minHeap->pos[v] < minHeap->size)
   {

     return true;
   }
   return false;
}

void printArr(int dist[], int n)
{
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
    {
        printf("%d \t\t %d\n", i, dist[i]);
    }
}

void pathtracingforcost(int source,int destination,string data[])
{
    int a=destination;
    if(distforallsourcecost[source][destination]==INT_MAX)
    {
        cout<<"SORRY this journey is not possible\n";
    }
    else
        {
        stack <int> s;
        while(pathtracerforcost[source][destination]!=source && pathtracerforcost[source][destination]!=NULL)
        {
            s.push(pathtracerforcost[source][destination]);
            destination=pathtracerforcost[source][destination];
        }
        s.push(source);
        while(!s.empty())
        {
            cout<<data[s.top()]<<"-> ";
            s.pop();
        }
        cout<<data[a]<<"\n";
        }
}
void pathtracingfordist(int source,int destination,string data[])
{
    int a=destination;
    if(distforallsourcedist[source][destination]==INT_MAX)
    {
        cout<<"\n\nSORRY this journey is not possible\n";
    }
    else
        {
        stack <int> s;
        while(pathtracerfordistance[source][destination]!=source && pathtracerfordistance[source][destination]!=NULL)
        {
            s.push(pathtracerfordistance[source][destination]);
            destination=pathtracerfordistance[source][destination];
        }
        s.push(source);
        while(!s.empty())
        {
            cout<<data[s.top()]<<"-> ";
            s.pop();
        }
        cout<<data[a]<<"\n";
    }
}
void dijkstraforcost(struct Graph* graph2, int src)
{
    int V = graph2->V;

    struct MinHeap* minHeap = createMinHeap(V);

    for (int v=0;v<V;++v)
    {
        distforallsourcecost[src][v]=INT_MAX;
        pathtracerforcost[src][v]=NULL;
        minHeap->array[v]=newMinHeapNode(v,distforallsourcecost[src][v]);
        minHeap->pos[v]=v;
    }

    minHeap->array[src]=newMinHeapNode(src,distforallsourcecost[src][src]);
    minHeap->pos[src]=src;
    distforallsourcecost[src][src]=0;
    decreaseKey(minHeap,src,distforallsourcecost[src][src]);

    minHeap->size=V;

    while (!isEmpty(minHeap))
    {
        struct MinHeapNode* minHeapNode=extractMin(minHeap);
        int u=minHeapNode->v;

        struct AdjListNode* pCrawl=graph2->array[u].head;
        while (pCrawl!=NULL)
        {
            int v=pCrawl->dest;

            if (isInMinHeap(minHeap,v) && distforallsourcecost[src][u]!=INT_MAX &&
                                          pCrawl->weight+distforallsourcecost[src][u]<distforallsourcecost[src][v])
            {
                distforallsourcecost[src][v]=distforallsourcecost[src][u]+pCrawl->weight;

                pathtracerforcost[src][v]=u;
                decreaseKey(minHeap,v,distforallsourcecost[src][v]);
            }
            pCrawl=pCrawl->next;
        }
    }
}
void dijkstrafordistance(struct Graph* graph2, int src)
{
    int V = graph2->V;

    struct MinHeap* minHeap = createMinHeap(V);

    for(int v=0;v<V;++v)
    {
        distforallsourcedist[src][v]=INT_MAX;
        pathtracerfordistance[src][v]=NULL;
        minHeap->array[v]=newMinHeapNode(v,distforallsourcedist[src][v]);
        minHeap->pos[v]=v;
    }

    minHeap->array[src]=newMinHeapNode(src,distforallsourcedist[src][src]);
    minHeap->pos[src]=src;
    distforallsourcedist[src][src]=0;
    decreaseKey(minHeap,src,distforallsourcedist[src][src]);

    minHeap->size=V;

    while (!isEmpty(minHeap))
    {
        struct MinHeapNode* minHeapNode=extractMin(minHeap);
        int u=minHeapNode->v;

        struct AdjListNode* pCrawl=graph2->array[u].head;
        while (pCrawl!=NULL)
        {
            int v=pCrawl->dest;

            if (isInMinHeap(minHeap,v) && distforallsourcedist[src][u]!=INT_MAX &&
                                          pCrawl->weight+distforallsourcedist[src][u]<distforallsourcedist[src][v])
            {
                distforallsourcedist[src][v]=distforallsourcedist[src][u]+pCrawl->weight;

                pathtracerfordistance[src][v]=u;
                decreaseKey(minHeap,v,distforallsourcedist[src][v]);
            }
            pCrawl=pCrawl->next;
        }
    }
}

int minKey(int key[], bool mstSet[])
{
   int min = INT_MAX, min_index;

   for (int i = 0; i <100 ; i++)
   {
     if (mstSet[i] == false && key[i] < min)
     {
         min = key[i];
        min_index = i;
     }
   }

   return min_index;
}

void primMST(int grap[100][100],int source,string data[])
{
     int parent[100];
     int key[100];
     bool mstSet[100];

     for(int i=0; i<100; i++)
     {
        key[i]=INT_MAX;
        mstSet[i]=false;
     }

     key[source]=0;
     parent[source]=-1;

     for (int count=0; count<100-1;count++)
     {
        int u=minKey(key,mstSet);

        mstSet[u]=true;

        for (int v=0;v<100; v++)
         {
          if(grap[u][v]&&mstSet[v]==false&&grap[u][v]<key[v])
          {
             parent[v]  = u;
            key[v] = grap[u][v];
          }
         }
     }
     int result=0;
     //cout<<"cities covered are:-\n";
      for(int i=0;i<100;i++)
      {
          if(key[i]!=INT_MAX)
          {
              //cout<<data[i]<<endl;
              result=result+key[i];
          }
      }
      cout<<2*result<<endl;
     //printMST(parent, V, grap);

}

void ClearConsoleToColors(int ForgC, int BackC)
 {
 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
               //Get the handle to the current output buffer...
 HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
                     //This is used to reset the carat/cursor to the top left.
 COORD coord = {0, 0};
                  //A return value... indicating how many chars were written
                    //   not used but we need to capture this since it will be
                      //   written anyway (passing NULL causes an access violation).
  DWORD count;

                               //This is a structure containing all of the console info
                      // it is used here to find the size of the console.
 CONSOLE_SCREEN_BUFFER_INFO csbi;
                 //Here we will set the current color
 SetConsoleTextAttribute(hStdOut, wColor);
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
                          //This fills the buffer with a given character (in this case 32=space).
      FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

      FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
                          //This will set our cursor position for the next print statement.
      SetConsoleCursorPosition(hStdOut, coord);
 }
 return;
}


int main()
{
    ClearConsoleToColors(0,15);
    int u,v,weight;
    int V=100;
    struct Graph* graph1=createGraph(V);
    struct Graph* graph2=createGraph(V);
     srand(time(NULL));
    for(int i=0;i<300;i++)
    {

    u= rand()%(V-1);
    v= rand() % (V-1);
    weight=rand() % (250);
    addEdge(graph1,u,v,weight);
    weight=rand() % (250);
    addEdge(graph2,u,v,weight);
    }
    /*
    cout<<"GRAPH 1 IS SHOWN BELOW\n";
    for(u=0;u<100;u++)
    {
    struct AdjListNode* pCrawl=graph1->array[u].head;
        while (pCrawl!=NULL)
        {
            cout<<u<<"  "<<pCrawl->dest<<"  "<<pCrawl->weight<<endl;
            pCrawl=pCrawl->next;
        }
    }

    cout<<"GRAPH 2 IS SHOWN BELOW\n";
    for(u=0;u<100;u++)
    {
    struct AdjListNode* pCrawl=graph2->array[u].head;
        while (pCrawl!=NULL)
        {
            cout<<u<<"  "<<pCrawl->dest<<"  "<<pCrawl->weight<<endl;
            pCrawl=pCrawl->next;
        }
    }
*/
    for(int i=0;i<100;i++)
    {
    dijkstrafordistance(graph1,i);
    }

    for(int i=0;i<100;i++)
    {
    dijkstraforcost(graph2,i);
    }
   /* for(int i=0;i<100;i++)
    {
        for(int j=0;j<100;j++)
        {
            cout<<"distforallsourcecost[i][j] "<<distforallsourcecost[i][j]<<"  pathtracerforcost[i][j] "<<pathtracerforcost[i][j]<<endl;
        }
    }*/

    for(int i=0;i<100;i++)
    {
        for(int j=0;j<100;j++)
        {

            matforcost[i][j]=0;
            matfordist[i][j]=0;
        }
    }
     for(u=0;u<100;u++)
    {
    struct AdjListNode* pCrawl=graph2->array[u].head;
        while (pCrawl!=NULL)
        {
            matfordist[u][pCrawl->dest]=pCrawl->weight;
            pCrawl=pCrawl->next;
        }
    }
    for(u=0;u<100;u++)
    {
    struct AdjListNode* pCrawl=graph1->array[u].head;
        while (pCrawl!=NULL)
        {
            matforcost[u][pCrawl->dest]=pCrawl->weight;
            pCrawl=pCrawl->next;
        }
    }

    	cout<<"\t \t \t \tTRAVELLING CITIES  -  U.S. WANDERLUST" ;
     string data[] ={"New York","Los Angeles","Chicago","Brooklyn","Queens"," Houston"," Manhattan",
	"Philadelphia","Phoenix","San Antonio","Bronx","San Diego"," Dallas","San Jose",
	" East San Gabriel Valley",	"Austin"," Jacksonville","San Francisco","Indianapolis","Columbus","Fort Worth",
	"Charlotte","Detroit","El Paso","Seattle","Denver","Washington","Memphis",
	" Boston","Nashville-Davidson","Baltimore","Metropolitan Government","Oklahoma City","Portland","Las Vegas",
	"Milwaukee","Albuquerque","Tucson","Fresno","East Seattle","Central Contra Costa","Sacramento",
	"Staten Island","Long Beach","Northeast Tarrant","Kansas City","Mesa"," Northwest Harris","Atlanta",
	"Northeast Jefferson"," Virginia Beach","Omaha","Colorado Springs","Raleigh","Miami","Oakland",
	"Minneapolis","Tulsa",	"Cleveland","Wichita"," New Orleans"," Arlington"," North Coast",
	"Honolulu"," Bakersfield","Tampa","Aurora","Urban Honolulu","Anaheim","Kendale Lakes-Lindgren Acres",
	" Santa Ana","South Aurora"," Upper San Gabriel Valley","Southwest San Gabriel Valley","Corpus Christi","Riverside","St. Louis",
	" Lexington-Fayette","West Adams","Pittsburgh","Stockton","Anchorage","South Coast"," Cincinnati",
	"St. Paul","Southeast Montgomery","Fayette","Greensboro","Toledo","Newark","Ewa",
	"Plano","Henderson","Lincoln","Louisville","Orlando","Jersey City","Chula Vista",
	"Buffalo","Fort Wayne"};
	int i=0;


	char ch='Y';
       int source,dest,imp;
         while(ch=='Y'||ch=='y')
		 {
		 cout<<"\n\n\n\n Select any One option :-\n\n\n\n1. To go from your hometown to any other place \n2. To visit all the connected cities in minimum time and cost\n3. Want to reach a destination by showing love to your favourite city \n";

	   cin>> i;
	   if (i==1)
	   {i=0;
	   cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t\t LIST OF CITIES\n\n\n\n";
	    while(i<100)
	{ cout<<i<<".\t"<<data[i]<<"\n";


		i++;
	}
	    cout<<"\n\nEnter Source:-\n";
	   	cin>>source;
	   	 cout<<"\n\nEnter Destination:-\n";
	   	cin>>dest;
	   	//system("cls");
	   	cout<<"Minimum Cost in travelling from "<<data[source]<<" to "<<data[dest]<<" is "<<distforallsourcecost[source][dest]<<endl;
	   	 cout<<"MINIMUM COST PATH IS:-\n";
	   	pathtracingforcost(source,dest,data);
	   	cout<<"Minimum Distance to be covered in travelling from "<<data[source]<<" to "<<data[dest]<<" is "<<distforallsourcedist[source][dest]<<endl;
	   	 cout<<"MINIMUM COST PATH IS:-\n";
        pathtracingfordist(source,dest,data);
	   }
	   else if(i==2)
	   {
	   	i=0;
	   cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t\t LIST OF CITIES\n\n\n\n";
	    while(i<100)
	{ cout<<i<<".\t"<<data[i]<<"\n";

		i++;
	}
	    cout<<"\n\nEnter Source:-\n";
	   	cin>>source;
	   	cout<<"Minimum cost required to travel all the connected cities from "<<data[source]<<endl;
        primMST(matforcost,source,data);
        cout<<"Minimum distance to be covered to visit all the connected cities from "<<data[source]<<endl;
        primMST(matfordist,source,data);
	   }
	   else if(i==3)
       {

           cout<<"\n\n\n\n\n\n\n\n\n\n\t\t\t\t LIST OF CITIES\n\n\n\n";
	    while(i<100)
	{ cout<<i<<".\t"<<data[i]<<"\n";

		i++;
	}
        cout<<"\n\nEnter Source:-\n";
	   	cin>>source;

	   	//transform(source.begin(), source.end(), source.begin(), ::tolower);
	   	 cout<<"\n\nEnter Destination:-\n";
	   	cin>>dest;

	   	cout<<"\nEnter city you want to travel certainly\n";
	   	cin>>imp;

        cout<<"\n\n\nMinimum Cost in travelling from "<<data[source]<<" to "<<data[dest]<<" via "<<data[imp]<<" is "<<(distforallsourcecost[source][imp]+distforallsourcecost[imp][dest])<<endl;
	     cout<<"\nMINIMUM cost PATH IS:-\n";
	   	pathtracingforcost(source,imp,data);
	   	pathtracingforcost(imp,dest,data);
	   	cout<<"\nMinimum Distance to be covered in travelling from "<<data[source]<<" to "<<data[dest]<<" via "<<data[imp]<<" is "<<distforallsourcedist[source][imp]+distforallsourcedist[imp][dest]<<endl;
         cout<<"\nMINIMUM distance PATH IS:-\n";
        pathtracingfordist(source,imp,data);
        pathtracingfordist(imp,dest,data);
       }
       else if(i==4)
       {

        cout<<"\n\nEnter Source:-\n";
	   	cin>>source;

	   	 cout<<"\n\nEnter Destination:-\n";
	   	cin>>dest;

	   	cout<<"\nEnter city you want to  certainly avoid\n";
	   	cin>>imp;
        cout<<"Minimum Cost in travelling from "<<data[source]<<" to "<<data[dest]<<" via "<<data[imp]<<" is "<<(distforallsourcecost[source][imp]+distforallsourcecost[imp][dest])<<endl;
	     cout<<"MINIMUM cost PATH IS:-\n";
	   	pathtracingforcost(source,imp,data);
	   	pathtracingforcost(imp,dest,data);
	   	cout<<"Minimum Distance to be covered in travelling from "<<data[source]<<" to "<<data[dest]<<" via "<<data[imp]<<" is "<<distforallsourcedist[source][imp]+distforallsourcedist[imp][dest]<<endl;
         cout<<"MINIMUM distance PATH IS:-\n";
        pathtracingfordist(source,imp,data);
        pathtracingfordist(imp,dest,data);
       }
        cout<<"\n\n\n\n\n More queries??\n";
        cin>>ch;
        system("cls");
          }

    return 0;
}


