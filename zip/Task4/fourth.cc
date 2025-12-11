#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

int main(int argc, char *argv[])
{
    // Enable logging (optional for debugging)
    // LogComponentEnable("OnOffApplication", LOG_LEVEL_INFO);
    // LogComponentEnable("PacketSink", LOG_LEVEL_INFO);

    // Create 4 nodes: A, B, C, D
    NodeContainer nodes;
    nodes.Create(4);
    Ptr<Node> A = nodes.Get(0);
    Ptr<Node> B = nodes.Get(1);
    Ptr<Node> C = nodes.Get(2);
    Ptr<Node> D = nodes.Get(3);

    // Install Internet stack (TCP/IP + routing) on all nodes
    InternetStackHelper internet;
    internet.Install(nodes);

    // Configure point-to-point channel attributes (35 Mbps, 27 ms delay)
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("35Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("27ms"));

    // Create point-to-point links for A-B, A-C, C-D, B-D
    NodeContainer p2pAB(A, B);
    NodeContainer p2pAC(A, C);
    NodeContainer p2pCD(C, D);
    NodeContainer p2pBD(B, D);

    NetDeviceContainer devAB = p2p.Install(p2pAB);
    NetDeviceContainer devAC = p2p.Install(p2pAC);
    NetDeviceContainer devCD = p2p.Install(p2pCD);
    NetDeviceContainer devBD = p2p.Install(p2pBD);

    // Assign IPv4 addresses to each link (networks as specified)
    Ipv4AddressHelper address;
    address.SetBase("88.77.66.0", "255.255.255.0");
    Ipv4InterfaceContainer ipAB = address.Assign(devAB);  // A-B link
    address.SetBase("88.77.77.0", "255.255.255.0");
    Ipv4InterfaceContainer ipAC = address.Assign(devAC);  // A-C link
    address.SetBase("88.77.88.0", "255.255.255.0");
    Ipv4InterfaceContainer ipCD = address.Assign(devCD);  // C-D link
    address.SetBase("88.77.99.0", "255.255.255.0");
    Ipv4InterfaceContainer ipBD = address.Assign(devBD);  // B-D link

    // Populate routing tables (enables A to reach D via B or C)
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // **Application Configuration**:
    // UDP OnOff flow 1: Node A -> Node B
    uint16_t port1 = 8001;
    Ipv4Address destB = ipAB.GetAddress(1);  // B's IP on A-B link (dest for flow1)
    OnOffHelper onoff1("ns3::UdpSocketFactory", InetSocketAddress(destB, port1));
    // Configure continuous traffic: constant On time, no Off time
    onoff1.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1000]"));
    onoff1.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    onoff1.SetConstantRate(DataRate("700kbps"), 1500);  // data rate 700 kb/s, packet size 1500 bytes
    // Install OnOff app on A (client), running from 2.0s to 8.0s
    ApplicationContainer clientApp1 = onoff1.Install(A);
    clientApp1.Start(Seconds(2.0));
    clientApp1.Stop(Seconds(8.0));
    // Corresponding PacketSink on B (server) to receive flow1
    PacketSinkHelper sink1("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port1));
    ApplicationContainer sinkApp1 = sink1.Install(B);
    sinkApp1.Start(Seconds(2.0));
    sinkApp1.Stop(Seconds(10.0));

    // UDP OnOff flow 2: Node A -> Node D (indirect via B–D path)
    uint16_t port2 = 8002;
    Ipv4Address destD_viaB = ipBD.GetAddress(1);  // D's IP on B-D link (dest for flow2)
    OnOffHelper onoff2("ns3::UdpSocketFactory", InetSocketAddress(destD_viaB, port2));
    onoff2.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1000]"));
    onoff2.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    onoff2.SetConstantRate(DataRate("600kbps"), 15500); // 600 kb/s, packet size 15500 bytes
    ApplicationContainer clientApp2 = onoff2.Install(A);
    clientApp2.Start(Seconds(4.0));
    clientApp2.Stop(Seconds(9.0));
    // PacketSink on D to receive flow2
    PacketSinkHelper sink2("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port2));
    ApplicationContainer sinkApp2 = sink2.Install(D);
    sinkApp2.Start(Seconds(4.0));
    sinkApp2.Stop(Seconds(10.0));

    // **Tracing Configuration**:
    // ASCII trace file
    AsciiTraceHelper ascii;
    p2p.EnableAsciiAll(ascii.CreateFileStream("prasadma_4.tr"));
    // pcap trace files for all point-to-point devices
    p2p.EnablePcapAll("prasadma_4");
    // NetAnim XML trace
    AnimationInterface anim("prasadma_4.xml");

    // Assign descriptive node names and constant positions for NetAnim
    anim.UpdateNodeDescription(A, "NodeA");
    anim.UpdateNodeDescription(B, "NodeB");
    anim.UpdateNodeDescription(C, "NodeC");
    anim.UpdateNodeDescription(D, "NodeD");
    anim.SetConstantPosition(A,  0.0, 50.0);
    anim.SetConstantPosition(B, -50.0, 0.0);
    anim.SetConstantPosition(C,  50.0, 0.0);
    anim.SetConstantPosition(D,  0.0, -50.0);

    // **Flow Monitor Configuration**:
    Ptr<FlowMonitor> flowMon;
    FlowMonitorHelper flowMonHelper;
    // (Optional) set flow monitor attributes for time resolution
    flowMonHelper.SetMonitorAttribute("StartTime", TimeValue(Seconds(0.0)));
    flowMonHelper.SetMonitorAttribute("DelayBinWidth", DoubleValue(0.1));  // 0.1s bin (for delay stats)
    flowMonHelper.SetMonitorAttribute("JitterBinWidth", DoubleValue(0.1));
    flowMonHelper.SetMonitorAttribute("PacketSizeBinWidth", DoubleValue(20.0));
    flowMon = flowMonHelper.InstallAll();

    // Run simulation
    Simulator::Stop(Seconds(10.0));
    Simulator::Run();

    // Serialize FlowMonitor stats to XML for post-processing
    flowMon->CheckForLostPackets();
    flowMon->SerializeToXmlFile("prasadma_4_flow.xml", true, true);

    Simulator::Destroy();
    return 0;
}
