#include <iostream>
#include <stdio.h>

 #include "ns3/config.h"
 #include "ns3/string.h"
 #include "ns3/core-module.h"
 #include "ns3/point-to-point-module.h"
 #include "ns3/network-module.h"
 #include "ns3/applications-module.h"
 #include "ns3/mobility-module.h"
 #include "ns3/csma-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/yans-wifi-helper.h"
 #include "ns3/ssid.h"

using namespace ns3; 

// void CalculateThroughput (ApplicationContainer udpServer,int num,uint64_t lastTotalRx)
// {

 //  Time now = Simulator::Now ();
 //  uint64_t total = DynamicCast<UdpServer> (udpServer.Get (1))->GetReceived ();
  // sink = DynamicCast<PacketSink> (packetSink.Get (0));
   //double cur = (total - lastTotalRx);
//   std::cout << "flow" << num << "  " << now.GetSeconds () << "s: \t" << total << " Mbit/s" << std::endl;
   //lastTotalRx = total;
 //  Simulator::Schedule ((MilliSeconds (100)), &CalculateThroughput,udpServer,num,lastTotalRx);
// }
 
  
 NS_LOG_COMPONENT_DEFINE ("wifiCustom");
  
 int 
 main (int argc, char *argv[])
 {
   bool verbose = true;
   uint32_t nCsma = 9;
   uint32_t nWifi = 9;
   bool tracing = false;
  
   CommandLine cmd (__FILE__);
   cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
   cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
   cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
   cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
  
   cmd.Parse (argc,argv);
  
   // The underlying restriction of 18 is due to the grid position
   // allocator's configuration; the grid layout will exceed the
   // bounding box if more than 18 nodes are provided.
   if (nWifi > 18)
     {
       std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;
       return 1;
     }
  
   if (verbose)
     {
       LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
       LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
     }
  
   NodeContainer p2pNodes;
   p2pNodes.Create (2);
  
   PointToPointHelper pointToPoint;
   pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("50Mbps"));
   pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  
   NetDeviceContainer p2pDevices;
   p2pDevices = pointToPoint.Install (p2pNodes);
  
   NodeContainer csmaNodes;
   csmaNodes.Add (p2pNodes.Get (1));
   csmaNodes.Create (nCsma);
  
   CsmaHelper csma;
   csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
   csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
  
   NetDeviceContainer csmaDevices;  
   csmaDevices = csma.Install (csmaNodes);
  
   NodeContainer wifiStaNodes;
   wifiStaNodes.Create (nWifi);
   NodeContainer wifiApNode = p2pNodes.Get (0);
  

   YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  // YansWifiChannelHelper channel;
  //  channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  // channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel");
  // if(rayleigh)  wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel" , "m0", DoubleValue(1.0), "m1", DoubleValue(1.0), "m2", DoubleValue(1.0)); 
   YansWifiPhyHelper phy;
   phy.SetChannel (channel.Create ());
  
   WifiHelper wifi;
   wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
   wifi.SetStandard(WIFI_STANDARD_80211g);

   WifiMacHelper mac;
   Ssid ssid = Ssid ("ns-3-ssid");
   mac.SetType ("ns3::StaWifiMac",
                "Ssid", SsidValue (ssid),
                "ActiveProbing", BooleanValue (false));
  
   NetDeviceContainer staDevices;
   staDevices = wifi.Install (phy, mac, wifiStaNodes);
  
   mac.SetType ("ns3::ApWifiMac",
                "Ssid", SsidValue (ssid));
  
   NetDeviceContainer apDevices;
   apDevices = wifi.Install (phy, mac, wifiApNode);
  
   MobilityHelper mobility;
  
   mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue (0.0),
                                  "MinY", DoubleValue (0.0),
                                  "DeltaX", DoubleValue (5.0),
                                  "DeltaY", DoubleValue (10.0),
                                  "GridWidth", UintegerValue (3),
                                  "LayoutType", StringValue ("RowFirst"));
  
   mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
   mobility.Install (wifiStaNodes);
  
   mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   mobility.Install (wifiApNode);
  
   InternetStackHelper stack;
   stack.Install (csmaNodes);
   stack.Install (wifiApNode);
   stack.Install (wifiStaNodes);
  
   Ipv4AddressHelper address;
  
   address.SetBase ("10.1.1.0", "255.255.255.0");
   Ipv4InterfaceContainer p2pInterfaces;
   p2pInterfaces = address.Assign (p2pDevices);
  
   address.SetBase ("10.1.2.0", "255.255.255.0");
   Ipv4InterfaceContainer csmaInterfaces;
   csmaInterfaces = address.Assign (csmaDevices);
  
   address.SetBase ("10.1.3.0", "255.255.255.0");
   address.Assign (staDevices);
   address.Assign (apDevices);
  
  // 1
   UdpEchoServerHelper echoServer (9);
  
   ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
   serverApps.Start (Seconds (0.0));
   serverApps.Stop (Seconds (20.0));
  
   UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
   echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  
   ApplicationContainer clientApps = 
     echoClient.Install (wifiStaNodes.Get (nWifi - 1));
   clientApps.Start (Seconds (1.0));
   clientApps.Stop (Seconds (20.0));

  //2 
   UdpEchoServerHelper echoServer1 (8);
 
   ApplicationContainer serverApps1 = echoServer1.Install (csmaNodes.Get (nCsma-1));
   serverApps1.Start (Seconds (0.0));
   serverApps1.Stop (Seconds (20.0));
 
   UdpEchoClientHelper echoClient1 (csmaInterfaces.GetAddress (nCsma-1), 8);
   echoClient1.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));
 
   ApplicationContainer clientApps1 =
   echoClient1.Install (wifiStaNodes.Get (nWifi - 2));
   clientApps1.Start (Seconds (1.0));
   clientApps1.Stop (Seconds (20.0));
  
  //3
   UdpEchoServerHelper echoServer2 (7);

   ApplicationContainer serverApps2 = echoServer2.Install (csmaNodes.Get (nCsma-2));
   serverApps2.Start (Seconds (0.0));
   serverApps2.Stop (Seconds (20.0));

   UdpEchoClientHelper echoClient2 (csmaInterfaces.GetAddress (nCsma-2), 7);
   echoClient2.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

   ApplicationContainer clientApps2 =
   echoClient2.Install (wifiStaNodes.Get (nWifi - 3));
   clientApps2.Start (Seconds (1.0));
   clientApps2.Stop (Seconds (20.0));

  //4
   UdpEchoServerHelper echoServer3 (6);

   ApplicationContainer serverApps3 = echoServer3.Install (csmaNodes.Get (nCsma-3));
   serverApps3.Start (Seconds (0.0));
   serverApps3.Stop (Seconds (20.0));

   UdpEchoClientHelper echoClient3 (csmaInterfaces.GetAddress (nCsma-3), 6);
   echoClient3.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient3.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient3.SetAttribute ("PacketSize", UintegerValue (1024));

   ApplicationContainer clientApps3 =
   echoClient3.Install (wifiStaNodes.Get (nWifi - 4));
   clientApps3.Start (Seconds (1.0));
   clientApps3.Stop (Seconds (20.0));
   
  //5 
   UdpEchoServerHelper echoServer4 (5);

   ApplicationContainer serverApps4 = echoServer4.Install (csmaNodes.Get (nCsma-4));
   serverApps4.Start (Seconds (0.0));
   serverApps4.Stop (Seconds (20.0));

   UdpEchoClientHelper echoClient4 (csmaInterfaces.GetAddress (nCsma-4), 5);
   echoClient4.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient4.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient4.SetAttribute ("PacketSize", UintegerValue (1024));

   ApplicationContainer clientApps4 =
   echoClient4.Install (wifiStaNodes.Get (nWifi - 5));
   clientApps4.Start (Seconds (1.0));
   clientApps4.Stop (Seconds (20.0));

  //6
   UdpEchoServerHelper echoServer5 (4);

   ApplicationContainer serverApps5 = echoServer5.Install (csmaNodes.Get (nCsma-5));
   serverApps5.Start (Seconds (0.0));
   serverApps5.Stop (Seconds (20.0));

   UdpEchoClientHelper echoClient5 (csmaInterfaces.GetAddress (nCsma-5), 4);
   echoClient5.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient5.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient5.SetAttribute ("PacketSize", UintegerValue (1024));

   ApplicationContainer clientApps5 =
   echoClient5.Install (wifiStaNodes.Get (nWifi - 6));
   clientApps5.Start (Seconds (1.0));
   clientApps5.Stop (Seconds (20.0));

  //7
   UdpEchoServerHelper echoServer6 (3);

   ApplicationContainer serverApps6 = echoServer6.Install (csmaNodes.Get (nCsma-6));
   serverApps6.Start (Seconds (0.0));
   serverApps6.Stop (Seconds (20.0));

   UdpEchoClientHelper echoClient6 (csmaInterfaces.GetAddress (nCsma-6), 3);
   echoClient6.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient6.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient6.SetAttribute ("PacketSize", UintegerValue (1024));

   ApplicationContainer clientApps6 =
   echoClient6.Install (wifiStaNodes.Get (nWifi - 7));
   clientApps6.Start (Seconds (1.0));
   clientApps6.Stop (Seconds (20.0));
  
  //8
   UdpEchoServerHelper echoServer7 (2);

   ApplicationContainer serverApps7 = echoServer7.Install (csmaNodes.Get (nCsma-7));
   serverApps7.Start (Seconds (0.0));
   serverApps7.Stop (Seconds (20.0));

   UdpEchoClientHelper echoClient7 (csmaInterfaces.GetAddress (nCsma-7), 2);
   echoClient7.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient7.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient7.SetAttribute ("PacketSize", UintegerValue (1024));

   ApplicationContainer clientApps7 =
   echoClient7.Install (wifiStaNodes.Get (nWifi - 8));
   clientApps7.Start (Seconds (1.0));
   clientApps7.Stop (Seconds (20.0));

  //9
   UdpEchoServerHelper echoServer8 (1);

   ApplicationContainer serverApps8 = echoServer8.Install (csmaNodes.Get (nCsma-8));
   serverApps8.Start (Seconds (0.0));
   serverApps8.Stop (Seconds (20.0));

   UdpEchoClientHelper echoClient8 (csmaInterfaces.GetAddress (nCsma-8), 1);
   echoClient8.SetAttribute ("MaxPackets", UintegerValue (1));
   echoClient8.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
   echoClient8.SetAttribute ("PacketSize", UintegerValue (1024));

   ApplicationContainer clientApps8 =
   echoClient8.Install (wifiStaNodes.Get (nWifi - 9));
   clientApps8.Start (Seconds (1.0));
   clientApps8.Stop (Seconds (20.0));

   Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  
   //Simulator::Stop (Seconds (20.0));
  
   if (tracing)
     {
       phy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
       pointToPoint.EnablePcapAll ("third");
       phy.EnablePcap ("third", apDevices.Get (0));
       csma.EnablePcap ("third", csmaDevices.Get (0), true);
     }
  // Simulator::Schedule (Seconds (1.1), &CalculateThroughput,serverApps,0,0);    
   
   //uint64_t total1 = DynamicCast<UdpServer> (serverApps1.Get (1))->GetReceived ();
   //std::cout << "flow" << " " << "  " << " " << "s: \t" << total1 << " Mbit/s" << std::endl;
   Simulator::Stop (Seconds (20.0)); 
   Simulator::Run ();
   Simulator::Destroy ();
   return 0;
 }
