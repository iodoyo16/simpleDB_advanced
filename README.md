# simpleDB_advanced
간단한 명령을 수행하는 simpleDB를 red-black tree를 이용해서 구현하고, 반복적인 메모리 할당, 해제에 의한 overhead를 memory_pool을 이용해서 optimize했다.

1.프로젝트 설명
---
insert, delete, search 총 3가지 간단한 명령을 입력파일을 통해 입력받고, 
작업을 수행하는 simpleDB를 자료구조를 이용해 구현해 봅니다.<br><br>

(1) insert
해당 명령어는 정수 하나를 DB에 삽입합니다. 만약 명령어로 “i 3”이 들어오면 프로
그램은 DB에 3을 삽입합니다. DB에는 중복되는 값이 들어갈 수 있습니다.<br><br>
(2) delete
해당 명령어는 DB에 저장된 정수를 삭제합니다. 만약 명령어로 “d 3”이 들어오면 
해당 DB에서 3을 삭제합니다. 만약 DB에 해당 숫자가 없다면 메세지 “ignore”을 출
력파일에 출력합니다. 중복되는 값의 경우엔 랜덤으로 선택되어 삭제된다.<br><br>
(3) search
해당 명령어는 DB에 저장된 정수를 찾습니다. 만약 명령어로 “s 3”이 들어오면 DB
에서 3을 찾아 출력파일에 존재하다면 “true”를 존재하지 않는다면 “false”를 출력합
니다. 중복되는 값의 경우엔 랜덤으로 search되어 true를 출력한다<br><br>

2.Red-Black tree
---
일반적인 binary search tree의 경우 삽입,삭제,탐색의 시간복잡도가 O(h)이다. 그런데 만약 data의 입력이 오름차순 또는 내림차순으로 들어온다면
tree depth h가 데이터의 개수 n이 되어 linear 한 시간복잡도를 가지게 된다. 따라서 balanced tree로 DB를 구현하기위해 AVL tree또는 Red-Black tree를 
사용할 수 있는데, AVL tree의 경우 balance factor 를 위한 공간이 노드마다 필요하고, stricted하게 balance를 맞추기 때문에 rotate가 잦아
Red-Black tree로 구현했다.
<br><br>
Red-Black tree 에는 4가지의 규칙이 있는데, 그 규칙은 아래와 같다.

    1. Root Property : 루트노드의 색깔은 검정(Black)이다.

    2. External Property : 모든 external node들은 검정(Black)이다.

    3. Internal Property : 빨강(Red)노드의 자식은 검정(Black)이다. 

    == No Double Red(빨간색 노드가 연속으로 나올 수 없다.) 

    4. Depth Property : 모든 리프노드에서 Black Depth는 같다. 

    == 리프노드에서 루트노드까지 가는 경로에서 만나는 블랙노드의 개수는 같다. 

이러한 규칙을 만족할때, RB tree 에서 black depth가 같으면서 가장 짧은경우는 black 노드만 있는경우이고 가장 긴 경우는
black-red-black-...-black 이 반복되는 경우 이므로 가장 긴 leaf와 짧은 leaf의 높이의 차이가 2배 이내로 balancing 한다.
