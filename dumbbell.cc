#include <iostream>
#include <stdio.h>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Dumbbell");

void handler(ApplicationContainer* app) {
   Ptr<PacketSink> sink1 = DynamicCast<PacketSink> (app->Get (0));
  std::cout << "Total Bytes : " << sink1->GetTotalRx () << std::endl;
  }


//static void CwndTracer (uint32_t oldval, uint32_t newval)   {
//      NS_LOG_INFO ("Moving cwnd from " << oldval << " to " << newval);
//  } 

int main (int argc, char *argv[]){
 // bool tracing = false;
  uint32_t maxBytes = 0;

  Time::SetResolution (Time::NS);
  //LogComponentEnable ("TcpL4Protocol", LOG_LEVEL_ALL);
  //LogComponentEnable("BulkSendApplication",LOG_LEVEL_ALL);
   LogComponentEnable("PacketSink",LOG_LEVEL_INFO); 
  	
  // Creating Network Nodes
  NodeContainer clients;
  NodeContainer routers;
  NodeContainer servers;

  
  clients.Create(4);
  routers.Create(2);
  servers.Create(4);

  // Install Internet Stack on each node
  InternetStackHelper stack;
  stack.Install(clients);
  stack.Install(routers);
  stack.Install(servers);

  // Create Point-To-Point helper
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue("100Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue("2ms"));

  // Create an address helper
  Ipv4AddressHelper address;

  // subet1
  // Configuring subnets to allocate Ip address
  NodeContainer subnet1;
  subnet1.Add(clients.Get(0));
  subnet1.Add(routers.Get(0));
  
  //Create a device container 
  NetDeviceContainer subnet1Devices = p2p.Install(subnet1);

  //Configure the subnet address and mask 
  address.SetBase("10.1.1.0","255.255.255.0");
  Ipv4InterfaceContainer subnet1Interfaces = address.Assign (subnet1Devices);
  
  //subnet2
  // Configuring subnets to allocate Ip address
  NodeContainer subnet2;
  subnet2.Add(clients.Get(1));
  subnet2.Add(routers.Get(0));

  //Create a device container
  NetDeviceContainer subnet2Devices = p2p.Install(subnet2);

  //Configure the subnet address and mask
  address.SetBase("10.1.2.0","255.255.255.0");
  Ipv4InterfaceContainer subnet2Interfaces = address.Assign (subnet2Devices);
  
  //subnet3
  // Configuring subnets to allocate Ip address
  NodeContainer subnet3;
  subnet3.Add(clients.Get(2));
  subnet3.Add(routers.Get(0));

  //Create a device container
  NetDeviceContainer subnet3Devices = p2p.Install(subnet3);

  //Configure the subnet address and mask
  address.SetBase("10.1.3.0","255.255.255.0");
  Ipv4InterfaceContainer subnet3Interfaces = address.Assign (subnet3Devices);
  
  //subnet4
  // Configuring subnets to allocate Ip address
  NodeContainer subnet4;
  subnet4.Add(clients.Get(3));
  subnet4.Add(routers.Get(0));

  //Create a device container
  NetDeviceContainer subnet4Devices = p2p.Install(subnet4);

  //Configure the subnet address and mask
  address.SetBase("10.1.4.0","255.255.255.0");
  Ipv4InterfaceContainer subnet4Interfaces = address.Assign (subnet4Devices);
  	
  //subnet5 (routers)
  // Configuring subnets to allocate Ip address
  NodeContainer subnet5;
  subnet5.Add(routers.Get(0));
  subnet5.Add(routers.Get(1));

  //Create a device container
  NetDeviceContainer subnet5Devices = p2p.Install(subnet5);

  //Configure the subnet address and mask
  address.SetBase("10.1.5.0","255.255.255.0");
  Ipv4InterfaceContainer subnet5Interfaces = address.Assign (subnet5Devices);
  
  //  subnet6
  // Configuring subnets to allocate Ip address
  NodeContainer subnet6;
  subnet6.Add(routers.Get(1));
  subnet6.Add(servers.Get(0));

  //Create a device container
  NetDeviceContainer subnet6Devices = p2p.Install(subnet6);

  //Configure the subnet address and mask
  address.SetBase("10.1.6.0","255.255.255.0");
  Ipv4InterfaceContainer subnet6Interfaces = address.Assign (subnet6Devices);

  // subnet7
  // Configuring subnets to allocate Ip address
  NodeContainer subnet7;
  subnet7.Add(routers.Get(1));
  subnet7.Add(servers.Get(1));

  //Create a device container
  NetDeviceContainer subnet7Devices = p2p.Install(subnet7);

  //Configure the subnet address and mask
  address.SetBase("10.1.7.0","255.255.255.0");  
  Ipv4InterfaceContainer subnet7Interfaces = address.Assign (subnet7Devices);

  // subnet8
  // Configuring subnets to allocate Ip address
  NodeContainer subnet8;
  subnet8.Add(routers.Get(1));
  subnet8.Add(servers.Get(2));

  //Create a device container
  NetDeviceContainer subnet8Devices = p2p.Install(subnet8);

  //Configure the subnet address and mask
  address.SetBase("10.1.8.0","255.255.255.0");  
  Ipv4InterfaceContainer subnet8Interfaces = address.Assign (subnet8Devices);
  
  // subnet9
  // Configuring subnets to allocate Ip address
  NodeContainer subnet9;
  subnet9.Add(routers.Get(1));
  subnet9.Add(servers.Get(3));

  //Create a device container
  NetDeviceContainer subnet9Devices = p2p.Install(subnet9);

  //Configure the subnet address and mask
  address.SetBase("10.1.9.0","255.255.255.0");  
  Ipv4InterfaceContainer subnet9Interfaces = address.Assign (subnet9Devices);

  

  // install Application

  // client with BulkSendHelper
  uint16_t port = 10100;
  BulkSendHelper bulkClient0 ("ns3::TcpSocketFactory",
                        InetSocketAddress (subnet6Interfaces.GetAddress (1), port));

  bulkClient0.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  //bulkClient0.SetAttribute ("SendSize", UintegerValue (1024));

  ApplicationContainer client0Apps = bulkClient0.Install (clients.Get (0));

  client0Apps.Start (Seconds (0.0));
  client0Apps.Stop (Seconds (20.0));
  
 
  // server with PacketSinkHelper
  PacketSinkHelper sinkServer0 ("ns3::TcpSocketFactory",
                        InetSocketAddress (Ipv4Address::GetAny () , port));
  ApplicationContainer server0Apps = sinkServer0.Install (servers.Get (0));
  server0Apps.Start (Seconds (0.0));
  server0Apps.Stop (Seconds (20.0));
  
  //client1
  BulkSendHelper bulkClient1 ("ns3::TcpSocketFactory",
                        InetSocketAddress (subnet7Interfaces.GetAddress (1), port)); 
  
  bulkClient1.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  //bulkClient1.SetAttribute ("SendSize", UintegerValue (1024));
  ApplicationContainer client1Apps = bulkClient1.Install (clients.Get (1));

  client1Apps.Start (Seconds (0.0));
  client1Apps.Stop (Seconds (20.0));
  
    PacketSinkHelper sinkServer1 ("ns3::TcpSocketFactory",
                        InetSocketAddress (Ipv4Address::GetAny () , port));
  ApplicationContainer server1Apps = sinkServer1.Install (servers.Get (1));
  server1Apps.Start (Seconds (0.0));
  server1Apps.Stop (Seconds (20.0));
  
  //client2
  BulkSendHelper bulkClient2 ("ns3::TcpSocketFactory",
                        InetSocketAddress (subnet8Interfaces.GetAddress (1), port));
  bulkClient2.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  //bulkClient2.SetAttribute ("SendSize", UintegerValue (1024));
  ApplicationContainer client2Apps = bulkClient2.Install (clients.Get (2));

  client2Apps.Start (Seconds (0.0));
  client2Apps.Stop (Seconds (20.0));
  
    PacketSinkHelper sinkServer2 ("ns3::TcpSocketFactory",
                        InetSocketAddress (Ipv4Address::GetAny () , port));
  ApplicationContainer server2Apps = sinkServer2.Install (servers.Get (2));
  server2Apps.Start (Seconds (0.0));
  server2Apps.Stop (Seconds (20.0));
  
  //client3 (last)
  BulkSendHelper bulkClient3 ("ns3::TcpSocketFactory",
                        InetSocketAddress (subnet9Interfaces.GetAddress (1), port));
  bulkClient3.SetAttribute ("MaxBytes", UintegerValue (maxBytes));
  //bulkClient3.SetAttribute ("SendSize", UintegerValue (1024));
  ApplicationContainer client3Apps = bulkClient3.Install (clients.Get (3));

  client3Apps.Start (Seconds (0.0));
  client3Apps.Stop (Seconds (20.0));
  
    PacketSinkHelper sinkServer3 ("ns3::TcpSocketFactory",
                        InetSocketAddress (Ipv4Address::GetAny () , port));
  ApplicationContainer server3Apps = sinkServer3.Install (servers.Get (3));
  server3Apps.Start (Seconds (0.0));
  server3Apps.Stop (Seconds (20.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

//   if (tracing) 
//   { 
//   AsciiTraceHelper ascii;
//   p2p.EnableAsciiAll (ascii.CreateFileStream ("dumbbell-trasfer.tr"));
//   p2p.EnablePcapAll ("dumbbell-transfer",false); 
//   }
  

  Simulator::Schedule(Seconds(1.0),&handler,&server0Apps);   

  Simulator::Run ();
  Simulator::Destroy ();


  

  return 0;

}
