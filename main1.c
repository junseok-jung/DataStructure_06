#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include <time.h>

int array_comps_construct = 0;
int bst_comps_construct = 0;
int avl_comps_construct = 0;

typedef struct {
    int data;
    int array_found;   int array_comps;
    int bst_found;     int bst_comps;
    int avl_found;     int avl_comps;
} SearchResult;

int arr[100];
int arr_len = 0;

void insertArray(int data, int* is_dup)
{
    *is_dup = 0;
    for (int i = 0; i < arr_len; i++)
    {
        (array_comps_construct)++;
        if (arr[i] == data)
        {
            *is_dup = 1;
            return;
        }

    }
    arr[arr_len] = data;
    arr_len++;
}

typedef struct BSTNode {
    int data;
    struct BSTNode* left;
    struct BSTNode* right;
}BSTNode;

BSTNode* createBSTNode(int data)
{
    BSTNode* newnode = (BSTNode*)malloc(sizeof(BSTNode));
    newnode->data = data;
    newnode->left = NULL;
    newnode->right = NULL;

    return newnode;
}

BSTNode* insertBST(BSTNode* root, int data, int* is_dup)
{

    if (root == NULL)
        return createBSTNode(data);

    bst_comps_construct++;

    if (data == root->data)
    {
        *is_dup = 1;
        return root;
    }
    else if (data < root->data)
        root->left = insertBST(root->left, data, is_dup);
    else
        root->right = insertBST(root->right, data, is_dup);

    return root;
}

typedef struct AVLNode {
    int data;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
}AVLNode;

int get_height(AVLNode* node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int get_max(int a, int b)
{
    return (a > b) ? a : b;
}

AVLNode* createAVLNode(int data)
{
    AVLNode* newnode = (AVLNode*)malloc(sizeof(AVLNode));
    newnode->data = data;
    newnode->height = 1;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

// rotate
AVLNode* rightRotate(AVLNode* y)
{//x-> right 중복되니까 t2데이터 일시보관
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;

    //높이 다시 계산
    y->height = get_max(get_height(y->left), get_height(y->right)) + 1;
    x->height = get_max(get_height(x->left), get_height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x)
{
    AVLNode* y = x->right;
    AVLNode* T1 = y->left;
    y->left = x;
    x->right = T1;
    //높이 다시 계산
    x->height = get_max(get_height(x->left), get_height(x->right)) + 1;
    y->height = get_max(get_height(y->left), get_height(y->right)) + 1;
    return y;
}

AVLNode* insertAVL(AVLNode* root, int data, int* is_dup)
{
    if (root == NULL)
        return createAVLNode(data);

    avl_comps_construct++;
    if (data == root->data)
    {
        *is_dup = 1;
        return root;
    }
    else if (data < root->data)
        root->left = insertAVL(root->left, data, is_dup);

    else
        root->right = insertAVL(root->right, data, is_dup);

    if (*is_dup == 1)
        return root;

    root->height = get_max(get_height(root->left), get_height(root->right)) + 1;
    int balance = get_height(root->left) - get_height(root->right);

    //LL, RR, LR, RL
    if (balance > 1 && data < root->left->data)
        return rightRotate(root);

    else if (balance < -1 && data > root->right->data)
        return leftRotate(root);

    else if (balance > 1 && data > root->left->data)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    else if (balance < -1 && data < root->right->data) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

int getBSTheight(BSTNode* node)
{
    if (node == NULL)
        return 0;

    int left_h = getBSTheight(node->left);
    int right_h = getBSTheight(node->right);

    return get_max(left_h, right_h) + 1;
}

int searchArray(int target, int* found) {
    int comps = 0;
    *found = 0;
    for (int i = 0; i < arr_len; i++) {
        comps++;
        if (arr[i] == target) {
            *found = 1;
            break;
        }
    }
    return comps;
}

int searchBST(BSTNode* root, int target, int* found) {
    int comps = 0;
    *found = 0;
    BSTNode* current = root;
    while (current != NULL) {
        comps++;
        if (target == current->data) {
            *found = 1;
            break;
        }
        else if (target < current->data) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return comps;
}

int searchAVL(AVLNode* root, int target, int* found) {
    int comps = 0;
    *found = 0;
    AVLNode* current = root;
    while (current != NULL) {
        comps++;
        if (target == current->data) {
            *found = 1;
            break;
        }
        else if (target < current->data) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return comps;
}

int main() {
    srand((unsigned int)time(NULL));

    BSTNode* bstRoot = NULL;
    AVLNode* avlRoot = NULL;

    int duplicated_count = 0;

    
    printf("--- [Generated 100 Integers] ---\n");
    for (int i = 0; i < 100; i++) {
        int r = rand() % 1001;
        printf("%4d ", r);
        if ((i + 1) % 10 == 0) printf("\n");

        int is_dup_arr = 0, is_dup_bst = 0, is_dup_avl = 0;

        insertArray(r, &is_dup_arr);
        bstRoot = insertBST(bstRoot, r, &is_dup_bst);
        avlRoot = insertAVL(avlRoot, r, &is_dup_avl);

        if (is_dup_arr == 1) {
            duplicated_count++;
        }
    }
    printf("\n");

    // =========================================================
    // 저장 결과(생성 비용 및 구조 크기) 출력
   
    printf("Stored values : %d\n", arr_len);
    printf("Duplicated values : %d\n\n", duplicated_count);

    printf("Construction\n");
    printf("Array comparisons : %d\n", array_comps_construct);
    printf("BST comparisons   : %d\n", bst_comps_construct);
    printf("AVL comparisons   : %d\n\n", avl_comps_construct);

    printf("Structure\n");
    printf("Array length : %d\n", arr_len);
    printf("BST height   : %d\n", getBSTheight(bstRoot));
    printf("AVL height   : %d\n\n", get_height(avlRoot));

    // =========================================================
    // 50개의 탐색 대상 생성 및 탐색 진행

    SearchResult results[50];

    int total_arr_comps = 0;
    int total_bst_comps = 0;
    int total_avl_comps = 0;

    for (int i = 0; i < 50; i++) {
        int target = rand() % 1001;
        results[i].data = target;

        results[i].array_comps = searchArray(target, &results[i].array_found);
        results[i].bst_comps = searchBST(bstRoot, target, &results[i].bst_found);
        results[i].avl_comps = searchAVL(avlRoot, target, &results[i].avl_found);

        total_arr_comps += results[i].array_comps;
        total_bst_comps += results[i].bst_comps;
        total_avl_comps += results[i].avl_comps;
    }

    // =========================================================
    //  50회 탐색 결과 상세 출력 및 평균 통계 계산

    printf("Searches : 50\n\n");

    for (int i = 0; i < 50; i++) {
        printf("Search Key : %d\n\n", results[i].data);

        printf("Sequential Search\n");
        printf("Result      : %s\n", results[i].array_found ? "Found" : "Not Found");
        printf("Comparisons : %d\n\n", results[i].array_comps);

        printf("BST Search\n");
        printf("Result      : %s\n", results[i].bst_found ? "Found" : "Not Found");
        printf("Comparisons : %d\n\n", results[i].bst_comps);

        printf("AVL Search\n");
        printf("Result      : %s\n", results[i].avl_found ? "Found" : "Not Found");
        printf("Comparisons : %d\n\n", results[i].avl_comps);

        printf("----------------------------------------\n\n");
    }

    printf("Sequential Search\n");
    printf("Total comparisons   : %d\n", total_arr_comps);
    printf("Average comparisons : %.2f\n\n", (float)total_arr_comps / 50.0);

    printf("BST Search\n");
    printf("Total comparisons   : %d\n", total_bst_comps);
    printf("Average comparisons : %.2f\n\n", (float)total_bst_comps / 50.0);

    printf("AVL Search\n");
    printf("Total comparisons   : %d\n", total_avl_comps);
    printf("Average comparisons : %.2f\n", (float)total_avl_comps / 50.0);

    return 0;
}