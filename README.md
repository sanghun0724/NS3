# NS3 

NS3를 활용하여 가상 클라이언트들과 서버들을 만들고 TCP소켓을 통해 데이터를 날려보는 시뮬레이션을 해보았다.
클라이언트 노드는 총 4개이며 사이에는 두개의 라우터를 두고 4개의 서버 노드를 두었다.클라이언트 측은 BulkSendApplication 객체를 사용했으며 서버측은 PacketSink 객체를 사용하였다. 로그를 받아오는 형식은 직접 

<img src="https://user-images.githubusercontent.com/66512239/142597232-d7393400-966b-49b0-ac45-dc4de4f5fcd5.jpg"  width="700" height="370">
<img src="https://user-images.githubusercontent.com/66512239/142596851-55097e35-e73a-4f3e-a2b2-b5f5fcac93ff.png"  width="700" height="700">



