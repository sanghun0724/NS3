# NS3 

NS3를 활용하여 가상 클라이언트들과 서버들을 만들고 TCP소켓을 통해 데이터를 날려보는 시뮬레이션을 해보았다.

클라이언트 노드는 총 4개이며 사이에는 두개의 라우터를 두고 4개의 서버 노드를 두었다.클라이언트 측은 BulkSendApplication 객체를 사용했으며 서버측은 PacketSink 객체를 사용하였다. 로그를 받아오는 형식은 직접 Simulator:Schedule을 이용해 recursive한 함수를 만들어 출력로그가 계속 이벤트큐에 쌓이게 하고 시간이 되면 주기적으로 이벤트를 발생시켜 로그를 받았다. cwnd 같은 경우는 TCPProtocol INFO를 모두 받아오는 로그 함수를 사용 하였다. 

그래프를 그리는데에는 파이썬과 파이썬에 있는 matplotlib.pyplot를 사용하였다.

## 전체 노드들
<img src="https://user-images.githubusercontent.com/66512239/142597232-d7393400-966b-49b0-ac45-dc4de4f5fcd5.jpg"  width="700" height="370">

## flows pacekt size graph
<img src="https://user-images.githubusercontent.com/66512239/142596851-55097e35-e73a-4f3e-a2b2-b5f5fcac93ff.png"  width="700" height="400">
 패킷들이 겹치는 부분이 많은 것처럼 보인다. 플로우 4개가 비슷한 조건이라 그런 것 같다.  
 
 

## CWND graph
<img src="https://user-images.githubusercontent.com/66512239/142605314-2a8c1134-ed3a-43ae-839e-f9aa63a336ca.png"  width="700" height="400">

CWND가 정확히 뭔지 몰라서 이번 기회로 알아가려고 한다. 
CWND란 기본적으로 Congestion Window 혹은 flight size 라고 한다. Congestion Window란 ack을 확인하지 않고도 보낼 수 있는 데이터의 양 을 말한다. 예를 들어, 전송자에게 Congestion Window가 2로 세팅된다면, 전송자는 2 segments를 보낸 후 수신자로부터 ack이 올 때까지 기다려야 하는 것이다.
당연히 Congestion Window의 크기가 크면 한 번에 보낼 수 있는 데이터의 양이 많으니 데이터를 완전하게 보내는 데에 걸리는 시간이 줄어들 것이다. 그러나, 그 크기를 키우는 것은 Congestion을 발생시킬 수 있으니, 이 적당한 사이즈를 찾는 알고리즘이 필요하다. 
 
내 그래프를 보면 window 값이 65535가 나타나는 것을 볼수 있다.  보기에 굉장히 큰값같은데 아마 maxByte값을 무제한으로 줘서 그런것 같기도 하다. 이렇게 크기가 커지면 위의 말대로 congestion이 발생활 확률이 굉장히 커지게 된다. 
