# 그냥 끄적여보면서 만들어보는 CPU 구조
아직 CPU의 구조에 대해 (대학에서) 배우지 않은 상태에서 인터넷 자료와 머릿속 생각으로 16비트 CPU를 만들어보는 프로젝트입니다.<br>
C언어, C++, 베릴로그 순으로 작성하고 있습니다.<br>
C언어로는 전체적으로 이게 어떻게 구동이 되는가를 보여주는 정도로 개발할겁니다.<br>
C++으로는 C언어보다 구체적으로 작성할 예정입니다.<br>
베릴로그는 아예 반도체의 구현을 가정하고 작성할 예정입니다.<br>
<br>
현재 이것을 작성하는 시기는 대학 1학년 2학기를 종강한 방학시즌입니다. 대학 2학년 1,2학기를 통해 디지털 논리회로 과목을 다 떼면 더 깔끔하고 제대로 된 코드를 다시 짜보려 합니다.<br>
~~김도 공부법으로 공부하는 중입니다~~

## 만들어볼 16비트 시퓨 전체적인 구조
16비트 CPU이며, 레지스터, 디코더, 메모리(RAM)로 구성되어 있습니다. 

## 레지스터 구조
먼저 일부 명령어를 제외하고 접근 불가능한 PC, Status, SP, BACK 레지스터가 있습니다<br>
정수 연산용 16개 레지스터가 있으며, 내용은 아래와 같습니다.

|이름|용도|
|---|---|
|zero|항상 0을 가진 레지스터|
|status(csr)|최상위 비트에 음수, 캐리, 제로, 오버(언더)플로우, 부호 여부 상태를 순서대로 저장해 두는 레지스터|
|SP|스택포인터 레지스터, PUSH/POP 명령어 사용시 이 레지스터의 값이 +2/-2가 됨|
|BACK|서브루틴에서 본 루틴으로 돌아갈 주소를 저장하기 위한 레지스터|
|r0|사용자 정의 레지스터|
|r1|사용자 정의 레지스터|
|r2|사용자 정의 레지스터|
|r3|사용자 정의 레지스터|
|r4|사용자 정의 레지스터|
|r5|사용자 정의 레지스터|
|r6|사용자 정의 레지스터|
|r7|사용자 정의 레지스터|
|r8|사용자 정의 레지스터|
|r9|사용자 정의 레지스터|
|r10|사용자 정의 레지스터|
|r11|사용자 정의 레지스터, DIV연산시 나머지값은 여기에 저장됨|

### status 레지스터
|15|14|13|12|11 10 9 8|7 6 5 4|3 2 1 0|
|-|-|-|-|-|-|-|
|S|C|Z|F|하위비트는 이 명령어|체계에서|사용하지 않습니다.|

## 명령어 설명
명령어에서 직접적으로 zero(접근은 가능하나 결론적으로 수정이 안됨), status, PC, SP, BACK은 수정할 수 없습니다.<br><br>
여기 쓰여져 있는 대문자 R은 레지스터 이름에 쓰이는 r과 연관이 없습니다. 같은거 아니니 주의!
|15 14 13 12|11 10 9 8|7 6 5 4|3 2 1 0|
|---|---|---|---|
|R2|R1|Rn|Opcode|

### 명령어 리스트
값을 즉시 집어넣는 행위가 안됩니다.. 모든값은 레지스터나 메모리에사 가져와야 합니다. 였는데.. 현실에 타협하다 보니.. IMM가 생겼습니다..
|Opcode 이진수|이름|용도와 특징|
|----|----|----|
|0000|SYS|시스템콜이나 권한 변경용|
|0001|JUMP|PC레지스터를 바로 변환하기 위해 사용, 사용시 현재 명령어 바로 뒤의 명령어 주소가 BACK레지스터에 저장. Rn은 PC레지스터(if문 쓰듯이 Rn을 마음대로 지정할 수 있도록 할지 고민중), R1에는 주소가 들어있는 레지스터, R2에는 세부 특징이 작성됨(후술함)|
|0010|LOAD|메모리에서 레지스터로 값을 가져옴 (주소: R1)|
|0011|SAVE|레지스터 값을 메모리에 저장 (주소: R1)|
|0100|PUSH|SP레지스터 값을 +2하고 SP레지스터를 주소로 변환하여 R1레지스터의 값을 저장|
|0101|POP|SP레지스터 값을 -2하고 해당 메모리 주소에 있는 값을 Rn의 레지스터에 저장|
|0110|SL|R2만큼 R1의 비트를 왼쪽으로 이동한 것을 Rn에|
|0111|SR|R2만틈 R1의 비트를 오른쪽으로 이동한 것을 Rn에|
|1000|ADD|Rn = R1 + R2|
|1001|SUB|Rn = R1 - R2|
|1010|MUL|Rn = R1 * R2|
|1011|DIV|Rn = R1 / R2|
|1100|AND|Rn = R1 (AND) R2|
|1101|OR|Rn = R1 (OR) R2|
|1110|XOR|Rn = R1 (XOR) R2|
|1111|NOT|Rn = (NOT) R1 / status 레지스터 접근시 R2가 0이면 unsign, 1이면 sign로 지정|

권한은 0(보호모드 X), 1(커널), 2(예약), 3(앱)이며, 
SYS 명령어 세부사항은 아래와 같습니다. (Rn)
|Rn 값|어셈블리 명칭|동작|
|-|-|-|
|0000|SYSCALL|시스템콜용|
|0001|TOAPP|커널에서 앱으로 넘기기|

JUMP 명령어 R2 세부사함은 아래와 같습니다. 
|작성되어야 하는 수|이진수|별칭|의미|
|--|------|---|---|
|0(또는 공백)|0000|NONE|아무 조건도 없음 ~~(경 아무것도안함 축)~~|
|1|0001|SAME|= (status Z=1)|
|2|0010|LSAME|>= (status Z=1)|
|3|0011|L|> (status Z=0 & F=0 | N=0)|
|4|0100|RSAME|<= (status Z=1 | F=1 | N=1)|
|5|0101|R|< (status Z=0 | F=1 | N=1)|
|8|1000|BACK|BACK 레지스터에 저장된 주소로 돌아감|

## C언어로 작성하면서 느끼는 1차후기
16비트로 하는건 좀 너무 제약이 많음.. 32비트 부터 제대로 할수 있을것 같은 느낌이 듬.. 즉시 값을 넣어주는 작업의 구현을 고려하니까 너무 힘듦.. 지금 구현한건 무조건 레지스터에서 가져와야 되니까 데이터 영역에 막 넣고 하는게 너무 불편함

## C++로 작성하면서 느끼는 2차후기


## 베릴로그로 작성하면서 느끼는 3차후기 (이게 2차 후기가 될것 같은데..)


## 2학년을 마치고 디지털 논리회로를 공부한 뒤에 느끼는 4차후기 
