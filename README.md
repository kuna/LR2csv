LR2csv
=================

### Purpose

lavalse씨가 만든 Lunatic Rave 2는 6년이 지난 지금에서도 상당한 퀄러티의 BMS 구동기이며, 많은 사람들이 애용하고 있고 고퀄러티의 스킨 또한 다수 존재하는 멋진 프로그램입니다. 하지만, 아쉽게도 lavalse씨는 프로그램 개발을 중단한 상태이고, 코드 또한 존재하지 않습니다. 따라서 제가 직접 Lunatic Rave 2의 스킨 에뮬레이팅 프로그램을 제작하게 되었습니다.

이 라이브러리는 이후 [Rhythmus](https://github.com/kuna/Rhythmus)의 skin engine으로 사용될 예정입니다.

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
* FMOD Library
* BMSParser / md5 (included)
* sqlite3 (included, not related with CSV module)
* CMarkup XML parser (included)
* AutoEncoder(iconv, uchardet) (included)
* boost 1.53

### Future works

* Integrate rendering engine with Irrlicht engine
* Processing BMS file & play
* Result graph
* Optimize DST calculation...?
* 2P / DP
* Editing text
* Storing/Load/configure settings

### Sample
[![preview](http://img.youtube.com/vi/kpOnwJO2H_g/0.jpg)](http://www.youtube.com/watch?v=kpOnwJO2H_g)

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
16. FMOD sound system 구현.

---------------------

140808
- Directshow movie playback support!
- finally freetype font implemented...

---------------------

140809
- list module updated - now sample works with folder in/out

---------------------

140810
- module refactored & added Stdafx.h for precompile header
- colorkey support
- BMS parser added (alpha ver.)
- callback function added for CSVTimer

---------------------

140811
- CSVFile filtering method now working well
- CSVSetting save & load method implemented (not tested)
- Automatic encode module implemented

---------------------

140812
- fadein/out implemented
- rendertarget changing works - implementation of skin setting is available.
- different key input & environment system with each scene

---------------------

140813
- now BMS playable (just sample...)
- fade in/out implemented for every scene.
- different key event for each scene implemented.
- add CSVEventHandler module (setscenestart, setinputstart, setfadeinstart, ~end, setfadeoutstart, setfadeoutend, setsceneend ...)

---------------------

140814
- sample movie uploaded.
- more detail result/playing screen & dst/timing events.
- negative number rendering bug fixed

---------------------

140818
- ONMOUSE rendering process fixed.
- 

---------------------

~ 140917
35. panel works (+ related button...)
36. rendering thread 별개로 돌리기. (<- 취소됨.)
28. csv parser 만들기
  - 자동으로 encode detect.
  - method: ReadCSVFile(wstring &path), ResetPos(), next(), ReadLineRaw(wstring &), ReadLine(CSVLine *csvLine), SetTriggerName(wstring &), SetTrigger, ResetTrigger(), dispose()
  - csvLine method: int GetElement(int num) / wstring& GetElement(int num)
23. directinput
29. csv parser로 button script 등 여러 parser에 적용.
  - CSVReader engine 수정.
    * if 구문 살려두어다가 item query시 확인.
    * item 기준으로 CSVItem이 리턴됨.
    * a
    * b
    * c
  - music csv reader을 CSV Parser로 대체.
37. CSVUI 만들기
  - CSV에서 여러 동작과 관련된 switch 변경 등의 action을 수행해주는 method들을 제공해준다
  - (1p/2p 콤보 표시, 패널 표시, IR랭크 표시, select bar 이펙트 표시 등)
38. CSVObject 객체 도입.
  - CSV preload가 가능하도록 구조를 고안해 보자...
39. music play 함수를 CSVMusic 클래스 같은 곳에 넘겨주도록 한다. (알아서 CSV가 음악 재생 함수 호출할 수 있도록 ...)

---------------------

TODO
렌더링 엔진 원리 분석? - 노트는 어느 시점에 그리도록 하는가. 데이터 수정 후 테스트 도입.
- grouping 시점은 일정하지 않다, 임의로 만들도록 처리하기.
- text랑 bg는 별개로 그려진다 ... parsing 방식을 바꿀까?
- [index가 다를 때에 관한 문제] - index가 다른 경우도 묶어서 item으로 줄까? 아니면 item 파싱시 다르게 줘야 하나? 다르게 준다면 그걸 어떻게 처리하나, 매번 파싱할수도 없고. -> 따라서 묶어서 주는 것으로... 근데 묶어서 준다면 전체 scan도 아니고 순차적으로 가져오는건데 그게 가능하나. -> grouping 시점이 일정하지 않으므로 시작할 때 grouping 하는 걸로 선정, 즉 시작할 때 모든 element search 해서 가져오는 것으로 한다...? 

33. 노트 제대로 그리기
34. DP, 2P 모드 지원...

13. JUDGELINE 유효하게 표시 (hdc로 surface에 직접 접근?)
18. irrlicht 엔진과 통합
19. change to rhw coordinate
22. skinselect implementation
24. press effect when longnote

25. option 창 기능 모두 구현하기.
  - 첫 로드시 text 갱신시도.
  - 옵션 변경시 text 갱신시도.
  - 옵션변경? -> button class에서 처리.
26. 키음 로드 및 재생.
27. 버튼 커맨드 스크립트 처리.
// button script
// #ONCLICK,(id), #ONLEFT,(id) - , #ONRIGHT,(id) - ,
// #OPTION,(num),(val),, - 
// #TIMER,(num),(offset),, - offset은 비워놓을 수 있다.
// #OPTION,
  - 버튼 외 스크립트 처리 가능한 게 있나? * elements: image, button, bar_~~, 
  - TIMER및 OPTION 수정하는대부분의 모드에 대한 SCRIPT 작성이 가능함. (하지만 SELECT 부분은 하드코딩하는걸로...)
27-1. 버튼에 이벤트 핸들러 추가/삭제 기능 넣기.
27-2. 옵션도 바깥으로 빼놓기 ...?
  - 일단 inner로 만들어서 컴파일 한 후에.

30. float speed 적용하기. (hide & show 타이머와 option 만들기.)
31. V-REFX 바꿀때 애니메이션
 - 게임에서는 가운데 기준으로 alpha값이 적용됨. (밖->안 효과도 적용됨.)
 - 현재 LR2에서는 밖으로 나갔다가 다시 돌아오는 효과가 나옴.
32. OPTiON window
  - 게임에서는 위쪽에 키모습, 아래에 버튼들 상태 나옴.
  - 배경은 약간 darker 해짐.
  - 왼쪽에 target 나오는데 이건 어떻게 처리해야 하나 ...

</pre>