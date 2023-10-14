#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Global Definition
#define MAXLEN 30
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
FILE* outFile;


//Structure definition(s)
typedef struct itemNode {
char name[MAXLEN];
int count;
struct itemNode *left, *right;
} itemNode;

typedef struct treeNameNode {
char treeName[MAXLEN];
struct treeNameNode *left, *right;
itemNode *theTree;
} treeNameNode;

//function prototypes
treeNameNode* createTreeNameNode(char* name);
itemNode* createItemNode(char* name, int count);
treeNameNode* insertNameNode(treeNameNode* root, treeNameNode* newNode);
itemNode* insertItemNode(itemNode* treeRoot, itemNode* newNode);
treeNameNode* buildNameTree(FILE* inFile, int N);
treeNameNode* searchNameNode(treeNameNode* root, char treeName[50]);
void delete_name(char *tree);
void Delete(char *tree, char *item);
void Count(char *tree);
void itemSumCount(itemNode *a);
void height_balance(char *tree);
int findDepth(itemNode *a);
void item_before (char *tree, char *item);
int ValToPos(itemNode *a, char *item);
void itemSize(itemNode *a);
void search(char *tree, char *item);
void searchSubItem(itemNode *a, char *str);
void searchSubTree(treeNameNode *a, char *str);

treeNameNode *searchRes;
treeNameNode *subTreeHead;
treeNameNode *searchResItem;
int size = 0;
int sumCount = 0;



void searchSubTree(treeNameNode *a, char *str){

  if (a == NULL)
    {
      searchRes = NULL;
      return;
    }

    if(strcmp(a->treeName,str) == 0)
    {
      searchRes = a;
    }
      else
      {
        if(strcmp(str, a->treeName) < 0)
        {
          searchSubTree(a->left,str);
        }
          else if (strcmp(str, a->treeName) > 0)
          {
            searchSubTree(a->right, str);
          }

      }
return;
}

void searchSubItem(itemNode *a, char *str){
  itemNode *searchResItem;
if (a == NULL)
{
  searchResItem = NULL;
  return;
}

if(strcmp(a->name, str) == 0)
{
  searchResItem = a;
}

else{
  if (strcmp(str, a->name) < 0)
    {
       searchSubItem(a->left, str);
    }
    else if (strcmp(str, a->name) > 0)
      {
        searchSubItem(a->right, str);
      }
    }
    return;
}

void search(char *tree, char *item){
  itemNode *searchResItem;
searchSubTree(subTreeHead, tree);

  if(searchRes == NULL)
  {
    printf("%s does not exist\n",tree);
    return;
  }

  itemNode *subItem = searchRes->theTree;
  searchSubItem(subItem, item);
    if( searchResItem == NULL)
    {
      printf("%s not found in %s\n",item, tree);
      return;
    }
  itemNode *ans = searchResItem;
      if( ans->count == 0)
      {
        printf("%s not found in %s\n",item, tree);
      }
      else
      {
        printf("%d %s found in %s\n", ans->count, item, tree);
        return;
      }
}

void itemSize(itemNode *a)
{
    if (a == NULL)
        return;
    itemSize(a->left);
    size++;
    itemSize(a->right);
}

int ValToPos(itemNode *a, char *item)
{
    if (a == NULL)
    {
        return 0;
    }
    if (strcmp(item, a->name) == 0)
    {
        size = 0;
        itemSize(a->left);
        return size;
    }
    else if (strcmp(item, a->name) > 0)
    {
        size = 0;
        itemSize(a->left);
        int now = size;
        return (ValToPos(a->right, item) + now + 1);
    }
    else if (strcmp(item, a->name) < 0)
        return ValToPos(a->left, item);

}

void item_before(char *tree, char *item){
int ans = 0;
searchSubTree(subTreeHead, tree);
if (searchRes == NULL)
    {
        printf("%s does not exist\n", tree);
        return;
    }
    itemNode *subItem = searchRes->theTree;
    printf("item before %s : %d\n", item, ValtoPos(subItem,item));
}

int findDepth(itemNode *a)
{
    if (a == NULL)
        return -1;
    else
        return MAX(findDepth(a->left), findDepth(a->right)) + 1;
}

void height_balance(char *tree)
{
    searchSubTree(subTreeHead, tree);
    if (searchRes == NULL)
    {
        printf("%s does not exist\n", tree);
        return;
    }
    itemNode *subItem = searchRes->theTree;
    int l, r;
    l = findDepth(subItem->left);
    r = findDepth(subItem->right);
    printf("tree: left height %d right height %d difference %d\n",l, r, abs(l-r));

    if (abs(l - r) > 1)
    {
        printf("not balanced\n");
    }
    else
    {
      printf("balanced\n");
    }
}

void itemSumCount(itemNode *a)
{
    if (a == NULL)
        return;
    itemSumCount(a->left);
    sumCount += a->count;
    itemSumCount(a->right);
}

void Count(char *tree)
{
    searchSubTree(subTreeHead, tree);
    if (searchRes == NULL)
    {
        printf("%s does not exist",tree);
        return;
    }

    itemNode *subItem = searchRes->theTree;
    sumCount = 0;
    itemSumCount(subItem);
    printf("%s count %d", tree, sumCount);
}

void Delete(char *tree, char *item)
{
  itemNode *searchResItem;
    searchSubTree(subTreeHead, tree);
    if (searchRes == NULL)
    {
        printf("%s does not exist",tree);
        return;
    }
    itemNode *subItem = searchRes->theTree;
    searchSubItem(subItem, item);
    if (searchResItem == NULL)
    {
        printf("%s not found in %s", item, tree);
        return;
    }
    itemNode *subItemI = searchResItem;
    subItemI->count = 0;
    printf("%s deleted from %s",item, tree);
    return;
}

//deletes tree of a given name
void delete_name(char *tree)
{
    searchSubTree(subTreeHead, tree);
    if (searchRes == NULL)
    {
        printf("%s does not exist", tree);
        return;
    }
    itemNode *subItem = searchRes->theTree;
    free(searchRes->theTree);
    searchRes->theTree = NULL;
    printf("%s deleted", tree);
    return;
}


//function that will create a node for the tree name
treeNameNode* createTreeNameNode(char* name) {
treeNameNode* nameNode = (treeNameNode*)malloc(sizeof(treeNameNode));
strcpy(nameNode->treeName, name);
nameNode->left = NULL;
nameNode->right = NULL;
nameNode->theTree = NULL;

return nameNode;
}

//function that will create a node for an item.
itemNode* createItemNode(char* name, int count) {

itemNode* node = (itemNode*)malloc(sizeof(itemNode));
strcpy(node->name, name);
node->count = count;
node->left = NULL;
node->right = NULL;

return node;
}

//function that inserts the name node into a destination.
treeNameNode* insertNameNode(treeNameNode* root, treeNameNode* newNode){

if (root == NULL) {
return newNode;
}
else {
// root name > newNode name
if (strcmp(root->treeName, newNode->treeName) > 0) {
// if an element exists to the left of 'root' then call again
if (root->left != NULL) {
root->left = insertNameNode(root->left, newNode);
}
else {
root->left = newNode;
}
}
// root name < newNode name OR they are equal strings
else {
// if an element exists to the right of 'root' then call again
if (root->right != NULL) {
root->right = insertNameNode(root->right, newNode);
}
else {
root->right = newNode;
}
}
}
return root;
}


//inserts an item node
itemNode* insertItemNode(itemNode* treeRoot, itemNode* newNode) {

if (treeRoot == NULL) {
return newNode;
}
else {
// treeRoot name > newNode name
if (strcmp(treeRoot->name, newNode->name) > 0) {
// if an element exists to the left of it's 'root' then call again
if (treeRoot->left != NULL) {
treeRoot->left = insertItemNode(treeRoot->left, newNode);
}
else {
treeRoot->left = newNode;
}
}
// treeRoot name < newNode name OR they are equal strings
else {
// if an element exists to the right of it's 'root' then call again
if (treeRoot->right != NULL) {
treeRoot->right = insertItemNode(treeRoot->right, newNode);
}
else {
treeRoot->right = newNode;
}
}
}
return treeRoot;
}

//creates a tree of a certain name and returns the root of the tree.
treeNameNode* buildNameTree(FILE* inFile, int N) {

if (inFile == NULL) {
return NULL;
}
treeNameNode* nameRoot = NULL;
char name[MAXLEN];
for (int x = 0; x < N; x++) {
fscanf(inFile, "%s", name);
treeNameNode* newNameNode = createTreeNameNode(name);
nameRoot = insertNameNode(nameRoot, newNameNode);
}
return nameRoot;
}

//this is the search function to find a node with matching name.
treeNameNode* searchNameNode(treeNameNode* root, char treeName[50]) {

if (root == NULL) {
return NULL;
}

treeNameNode* nameNode;
if (strcmp(root->treeName, treeName) == 0) {
return root;
}
else if (strcmp(root->treeName, treeName) > 0) {
nameNode = searchNameNode(root->left, treeName);
}
else {
nameNode = searchNameNode(root->right, treeName);
}
return nameNode;
}


//populates the trees with information
void populateTrees(FILE* inFile, treeNameNode* nameRoot, int I) {

if (inFile == NULL) {
return;
}

else {
int count;
char treeName[MAXLEN];
char itemName[MAXLEN];
for (int x = 0; x < I; x++) {
fscanf(inFile, "%s %s %d", treeName, itemName, &count);
itemNode* newItemNode = createItemNode(itemName, count);
treeNameNode* treeNode = searchNameNode(nameRoot, treeName);
treeNode->theTree = insertItemNode(treeNode->theTree, newItemNode);
}
}
}


//displays the tree of a certain name in order
void displayInOrderNameTree(treeNameNode* root) {

if(root != NULL) {
displayInOrderNameTree(root->left);
printf("%s ", root->treeName);
displayInOrderNameTree(root->right);
}
}

//to traverse the tree
void traverseSubTree(itemNode* root){

if(root != NULL) {
traverseSubTree(root->left);
printf("%s ", root->name);
traverseSubTree(root->right);
}
}


//to traverse a tree and print the items in the tree.
void traverse_in_traverse(treeNameNode* root) {

if(root != NULL) {
traverse_in_traverse(root->left);
printf("\n===%s===\n", root->treeName);
traverseSubTree(root->theTree);
traverse_in_traverse(root->right);
}
}

//free fucntion to free the nodes inside of a tree
void freeTree(itemNode* root) {

if(root != NULL) {
freeTree(root->left);
freeTree(root->right);
free(root);
}
}


//frees the entire node of a tree.
void freeAll(treeNameNode* root) {

if(root != NULL) {
freeAll(root->left);
freeAll(root->right);
freeTree(root->theTree);
free(root);
}
}



//heart of the code.
int main(void) {

int N, I, Q;
FILE* inFile = fopen("in.txt", "r");
outFile = fopen("out.txt", "w");
fscanf(inFile, "%d %d %d", &N, &I, &Q);


treeNameNode* nameRoot = buildNameTree(inFile, N);
populateTrees(inFile, nameRoot, I);
displayInOrderNameTree(nameRoot);
traverse_in_traverse(nameRoot);


freeAll(nameRoot);

fclose(inFile);
fclose(outFile);

}
