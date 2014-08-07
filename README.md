LR2csv
=================

### Purpose

lavalse씨가 만든 Lunatic Rave 2는 6년이 지난 지금에서도 상당한 퀄러티의 BMS 구동기이며, 많은 사람들이 애용하고 있고 고퀄러티의 스킨 또한 다수 존재하는 멋진 프로그램입니다. 하지만, 아쉽게도 lavalse씨는 프로그램 개발을 중단한 상태이고, 코드 또한 존재하지 않습니다. 따라서 제가 직접 Lunatic Rave 2의 스킨 에뮬레이팅 프로그램을 제작하게 되었습니다.

### Abilities

* Drawing *.csv file with various commands (#SRC~, #DST~ ...)
* Supports setting storage with *.csv file (#CUSTOMOPTION)
* Supports scene - Select, Play, Decide, Result

### Usage

* It's Alpha version, so many functions are hard-coded. you have to wait if you're searching more better tool.
* in select: up/down, in play: Q(combo animation), W(FC animation), Z(bomb/lane press animation)

### Using library

* DirectX SDK (June 2010) - dx9
* Freetype

### Future works

* Font printing with Freetype
* Sound system with FMOD
* Integrate rendering engine with Irrlicht engine
* Processing BMS file & play
* Result graph
* Optimize DST calculation...?
* 2P / DP
* Editing text
* Storing/Load/configure settings

### Major change log

<pre>
1. FPS 확인 기능 엔진에 추가 ...?
5. 선곡화면 어떻게 그리게 할지 약간 고안..?
  - 텍스트 제외 요소들 그려보기. 자료구조 만들기.

  - 일단 attr 데이터 셋들을 리스트한다.
  - 그 다음 그릴때 attr을 가져오는데, 이때 attr의 index는 dst_center에서 뺀 거 알아서 조율해서 가져오는걸로 함.
  - 그리고 move animation 처리 - BARFLASH는 그대로 두고, 나머지는 offset를 수정해서 쓴다.
  - 어떻게 수정하냐면, 이전값/다음값의 차이를 이용하는 것이다. ON일 경우에는 on 값을 가져와서 처리함.
  - 매개변수는 speed와 val로. 이때 val이 1을 넘어가면 다음것으로 값이 바뀌면서 소리가 나게됨. ..?
  - idle일때만 인풋을 받도록 한다...? 아니면 쓰레드 방식으로 넘기도록 설정? speed를 여러개 돌리도록 해서?

--------------------

5. 선곡화면 - 진행중.

---------------------

5. 선곡화면 끝.
4. 텍스트 그리게 하기 - 진행중.

---------------------

4. 오늘도 반나절을 텍스트 그리기에... FreeFont & Caching 도입은 하였으나 렌더링은 실패.
8. *.*꼴의 파일들 읽어들일 수 있도록 하기 (CSVFile class)
9. 덤으로 CSVSettings 클래스 만듬.

---------------------

10. CSVElement를 CSVRender 클래스랑 분할하기
 - SetdrawFunc 만들어서 간소화, onmouse를 위한 SetCursorPos 메서드 만들기 & onmouse 처리.
11. (10에포함) onmouse처리.
10. 버튼 event
8. 패널

6. 콤보 화면 그리기
7. 노트, 롱노트, 노트 이펙트 화면 그리기 - 관련 메서드 추가.
//

---------------------

140805?
12. 슬라이더 구현.

---------------------

140807
- 옵션을 외부로 빼내어서 컴파일 하지 않고도 사용 가능 (settings.xml)
  * F1: select, F2: decide, F3: play, F4: result screen으로 확인 가능.
- 내부적인 구현방식 다수 변경. DXTexture 별개로 빼냄.
- Memory leak 대다수 해결.

---------------------

TODO
13. JUDGELINE 유효하게 표시 (hdc로 surface에 직접 접근?)
14. 타이머 콜백 함수 만들기 (play에서 ready랑 play의 rhythm timer 시작 부분에 사용할 것)
15. freetype 최적화 및 완성하기.
16. FMOD sound system 구현.
</pre>