#include "rbtree.h"

#include <stdlib.h>

void exchange_color(node_t *a, node_t *b)
{
  int tmp = a->color;
  a->color = b->color;
  b->color = (tmp == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
}

  // 오른쪽으로 회전하는 함수
  void right_rotate(rbtree *t, node_t *node)
  {
    node_t *parent = node->parent;
    node_t *grand_parent = parent->parent;
    node_t *node_right = node->right;

    // 부모가 루트인 경우: 현재 노드를 루트로 지정 (노드를 삭제한 경우만 해당)
    if (parent == t->root)
      t->root = node;
    else
    { // 1-1) 노드의 부모를 grand_parent로 변경
      if (grand_parent->left == parent)
        grand_parent->left = node;
      else
        grand_parent->right = node;
    }
    node->parent = grand_parent; // 1-2) 노드를 grand_parent의 자식으로 변경 (양방향 연결)
    parent->parent = node;       // 2-1) parent의 부모를 노드로 변경
    node->right = parent;        // 2-2) parent를 노드의 자식으로 변경 (양방향 연결)
    node_right->parent = parent; // 3-1) 노드의 자식의 부모를 parent로 변경
    parent->left = node_right;   // 3-2) 노드의 자식을 부모의 자식으로 변경 (양방향 연결)
  }

  // 왼쪽으로 회전하는 함수
  void left_rotate(rbtree *t, node_t *node)
  {
    node_t *parent = node->parent;
    node_t *grand_parent = parent->parent;
    node_t *node_left = node->left;

    // 부모가 루트인 경우: 현재 노드를 루트로 지정 (노드를 삭제한 경우만 해당)
    if (parent == t->root)
      t->root = node;
    else
    { // 1-1) 노드의 부모를 grand_parent로 변경
      if (grand_parent->left == parent)
        grand_parent->left = node;
      else
        grand_parent->right = node;
    }
    node->parent = grand_parent; // 1-2) 노드를 grand_parent의 자식으로 변경 (양방향 연결)
    parent->parent = node;       // 2-1) parent의 부모를 노드로 변경
    node->left = parent;         // 2-2) parent를 노드의 자식으로 변경 (양방향 연결)
    parent->right = node_left;   // 3-1) 노드의 자식의 부모를 parent로 변경
    node_left->parent = parent;  // 3-2) 노드의 자식을 부모의 자식으로 변경 (양방향 연결)
  }

rbtree *new_rbtree(void) {
  //트리 구조체 메모리 동적할당 초기값 0으로 초기화
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if neede바
  node_t *nil =(node_t*)calloc(1, sizeof(node_t));
  nil ->color = RBTREE_BLACK;
  p -> nil = p ->root = nil;
  return p;
}
// RB트리 구조체가 차지했던 메모리 반환

// 트리를 순회하며 각 노드의 메모리를 반환하는 함수
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *node = t->root;
  if(node != t ->nil)
    traverse_and_delete_node(t, node);

  free(t->nil);
  free(t);
}

// 각 노드와 그 자식 노드들의 메모리를 반환하는 함수
void traverse_and_delete_node(rbtree *t, node_t*node)
{
  if (node -> left != t -> nil)
    traverse_and_delete_node(t, node->left);
  if (node -> right != t -> nil)
    traverse_and_delete_node(t, node->right);

  free(node);
}
//노드를 삽입하고 균형을 복구하는 함수
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement inser균
  node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
  new_node -> key = key;
  new_node -> color = RBTREE_RED; // 삽입노드는 항상 RED
  new_node -> left = new_node -> right = t ->nil; // 추가한 노도의 자식은 nil로 설정

  node_t * current = t -> root;
  while (current != t-> nil)
  {
    if (key < current -> key)
    {
      if(current ->left == t-> nil)
      {
        current -> left = new_node;
        break;
      }
      current = current -> left;
    }
    else
    {
      if (current -> right == t-> nil)
      {
        current -> right = new_node;
        break;
      }
      current = current -> right;
    }
  }

  new_node -> parent = current; // 새노드의 부모 지정

  // root 가 nil 이면 새노드를 트리의 루트로 지정
  if (current == t->nil)
  t -> root = new_node;

  // 불균형 복구
  rbtree_insert_fixup(t,new_node);

  return new_node;
}

void rbtree_insert_fixup(rbtree*t, node_t *node)
{
  node_t * parent = node ->parent;
  node_t *grand_parent = parent -> parent;
  node_t * uncle;
  int is_left = node == parent -> left;
  int is_parent_is_left;

  // 추가된 노드가 root 노드인 경우 : 색만 변경
  if (node == t->root)
  {
    node -> color = RBTREE_BLACK;
    return;
  }

  //부모가 black 인경우 변경사항없음
  if(parent -> color == RBTREE_BLACK)
    return;

    is_parent_is_left = grand_parent -> left == parent;
    uncle = (is_parent_is_left) ? grand_parent -> right : grand_parent ->left;

    //case 1 부모와 부모의 형제가 모두 RED인경우

  if (uncle -> color == RBTREE_RED)
  {
    parent -> color = RBTREE_BLACK;
    uncle -> color = RBTREE_BLACK;
    grand_parent -> color = RBTREE_RED;
    rbtree_insert_fixup(t, grand_parent);
    return;
  }

  if (is_parent_is_left)
  {
    if(is_left)
    // case 2 부모의 형제가 BLACK & 부모가 왼쪽자식 & 현재 노드가 왼쪽자식인 경우
    {
      right_rotate(t,parent);
      exchange_color(parent, parent -> right);
      return;
    }

    // case 3 부모의 형제가 black & 부모가 왼쪽 자식 & 현재노드가 오른쪽자식인 경우
    left_rotate(t, node);
    right_rotate(t, node);
    exchange_color(node, node->right);
    return;
  }

  if(is_left)
  {
    // case 3 부모형제가 black & 부모가 오른쪽 자식 & 현재노드가 왼쪽
    right_rotate(t,node);
    left_rotate(t, node);
    exchange_color(node, node->left);
    return;

  }

  // case 2 부모의 형제가 black & 부모가 오른쪽 자식 & 현재 노드가 오른쪽 자식
  left_rotate(t, parent);
  exchange_color(parent, parent -> left);
  
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *current = t-> root;
  while (current != t-> nil)
  {
    if(key == current -> key)
    return current;
    else 
    current = (key < current -> key ) ? current -> left : current -> right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  
  node_t *current = t -> root;
  while ( current -> left != t-> nil)
  current = current -> left;
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find

  node_t * current = t -> root;
  while(current -> right != t->nil)
  current = current -> right;
  return current;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}



node_t *get_next_node(const rbtree *t, node_t *p)
{
  node_t *current = p->right;
  if (current == t->nil) // 오른쪽 자식이 없으면
  {
    current = p;
    while (1)
    {
      if (current->parent->right == current) // current가 오른쪽 자식인 경우
        current = current->parent;           // 부모 노드로 이동 후 이어서 탐색
      else
        return current->parent; // current가 왼쪽 자식인 경우 부모 리턴
    }
  }
  while (current->left != t->nil) // 왼쪽 자식이 있으면
    current = current->left;      // 왼쪽 끝으로 이동
  return current;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t * current = rbtree_min(t);
  arr[0] = current -> key;
  for(int i = 1; i<n; i++)
  {
    if (current == t -> nil)
    break;
    current = get_next_node(t,current);
    if(current == t-> nil)
    break;
    arr[i] = current -> key;
  }
  return 0;
}
