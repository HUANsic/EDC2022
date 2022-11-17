# EDC2022
Let's do this



------

11.16日志

特别对不起大家，拖到现在才更新上了A*算法，文件位于GeneralCore->Core->Inc/Src中，实现的所有函数如下：

```c
// free the entire A Star queue
void free_queue(A_Star_Node* head);
// append the Node into queue
void queue_append(A_Star_Node* head, A_Star_Node* NewNode);
// append the Node into list
void list_append(A_Star_Node* head, A_Star_Node* tail, A_Star_Node* NewNode);
// search if in queue/list
uint8_t Find_ifin(uint8_t x, uint8_t y, A_Star_Node* head);
// search if crash walls
uint8_t Find_crash(uint8_t x, uint8_t y);
// return the min Node which the Total is the min in the queue
A_Star_Node* TopNode(A_Star_Node* head, A_Star_Node* closehead, A_Star_Node* closetail);
// calculate the H function
uint16_t cal_H(A_Star_Node* current, A_Star_Node* end);
// expand the queue
A_Star_Node* Find_around_node(A_Star_Node* current, int8_t lastdir, A_Star_Node* openhead, A_Star_Node* closehead, A_Star_Node* end, uint8_t step);
// edit the node which is in openlist
void Editcost(A_Star_Node* head, A_Star_Node* p);
// return if turn
int8_t dir(A_Star_Node* from, A_Star_Node* to);
// init Node
A_Star_Node* init_Node(uint8_t x, uint8_t y, A_Star_Node* f, int8_t lastdir, A_Star_Node* end, uint8_t step);
// A Star search
A_Star_Node* A_Star(Coordinate* start, Coordinate* end, uint8_t step);
```

注释也比较言简意赅。emmmm简而言之，A*算法主体都实现了，分别用链表模拟了优先级队列openlist和搜索过的节点闭集closelist。感兴趣的同学可以和我一起改进一下。

TODO1：暂不知道消耗的内存如何，速度在我电脑上模拟没什么问题。

TODO2：如果step选定下，按理说只能搜到以step为分辨率的点，如果非要修改的话，可以修改认为达到终点的条件（目前是按照坐标严格到达写的， 需要调参。
