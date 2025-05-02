#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"

// Default Network Topology: static
//
//   Wifi2 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n5   n6   n7   n0 -------------- n1   n2   n3   n4
//                   point-to-point  |    |    |    |
//                                   *    *    *    *
//                                     Wifi1 10.1.2.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ThirdScriptExample");

int rxPcktCount=0;
int txPcktCount=0;

void
GetRxCount(std::string context, Ptr< const Packet > packet, const Address &address)
{
    rxPcktCount++;
    //NS_LOG_UNCOND(context <<" count "<<rxPcktCount);
}

void
GetTxCount(std::string context, Ptr< const Packet > packet)
{
    txPcktCount++;
    //NS_LOG_UNCOND(context <<" count "<<txPcktCount);
}


int
main(int argc, char* argv[])
{
    bool verbose = false;
    uint32_t nWifi = 30;
    uint32_t nWifi1 = nWifi/2;
    uint32_t nWifi2 = nWifi/2;
    uint32_t nflow = 60;
    bool tracing = false;
    uint32_t packetSize=1024;
    uint32_t pcktRate=100;
    uint32_t dRate=pcktRate*8; //kbps
    std::string dataRate="10Mbps";
    dataRate=std::to_string(dRate)+"kbps";
    uint32_t pRate=dRate*4/5;
    std::string p2pRate="5Mbps";
    p2pRate=std::to_string(pRate)+"kbps";
    uint32_t TxRange=40;
    uint32_t coverage=1*TxRange;
    uint32_t coverageMultiplier=1;
    double simulationTime=25.0;
    int graphType=1;
  
    Ptr<OutputStreamWrapper> throughPutStream;
    Ptr<OutputStreamWrapper> deliveryStream;
    AsciiTraceHelper asciiTraceHelper;
    throughPutStream  = asciiTraceHelper.CreateFileStream("scratch/1905108/1905108_1throughput.dat", std::ios::out|std::ios::app);
    deliveryStream  = asciiTraceHelper.CreateFileStream("scratch/1905108/1905108_1delivery.dat", std::ios::out|std::ios::app);
      

    CommandLine cmd(__FILE__);
    cmd.AddValue("nWifi", "Number of wifi STA devices", nWifi);
    cmd.AddValue("nFlow", "Number of flows", nflow);
    cmd.AddValue("pcktRate", "Number of Packets per Second", pcktRate);
    cmd.AddValue("coverageMultiplier", "Number of flows", coverageMultiplier);
    cmd.AddValue("graphType", "Type of plotted graph", graphType);   
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);
    cmd.AddValue("tracing", "Enable pcap tracing", tracing);

    cmd.Parse(argc, argv);

    nWifi1=nWifi/2;
    nWifi2=nWifi/2;
    coverage=coverageMultiplier*TxRange;

    std::cout<<nWifi<<" "<<nflow<<" "<<pcktRate<<" "<<coverage<<std::endl;

    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(packetSize));

    if (verbose)
    {
        LogComponentEnable("OnOffApplication", LOG_LEVEL_INFO);
        LogComponentEnable("PacketSink", LOG_LEVEL_INFO);
    }

//wired p2p  n0---n1
    NodeContainer p2pNodes;
    p2pNodes.Create(2);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue(p2pRate));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install(p2pNodes);

    
    WifiHelper wifi;
//wifi1
    NodeContainer wifiStaNodes1;
    wifiStaNodes1.Create(nWifi1);
    NodeContainer wifiApNodeN1 = p2pNodes.Get(1);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    channel.AddPropagationLoss("ns3::RangePropagationLossModel", "MaxRange", DoubleValue(coverage*1.0));
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid1");

    NetDeviceContainer staDevices1;
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    staDevices1 = wifi.Install(phy, mac, wifiStaNodes1);

    NetDeviceContainer apDeviceN1;
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    apDeviceN1 = wifi.Install(phy, mac, wifiApNodeN1);

//wifi2
    NodeContainer wifiStaNodes2;
    wifiStaNodes2.Create(nWifi2);
    NodeContainer wifiApNodeN0 = p2pNodes.Get(0);

    YansWifiChannelHelper channel2 = YansWifiChannelHelper::Default();
    channel2.AddPropagationLoss("ns3::RangePropagationLossModel", "MaxRange", DoubleValue(coverage*1.0));
    YansWifiPhyHelper phy2;
    phy2.SetChannel(channel2.Create());

    WifiMacHelper mac2;
    Ssid ssid2 = Ssid("ns-3-ssid2");

    NetDeviceContainer staDevices2;
    mac2.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid2), "ActiveProbing", BooleanValue(false));
    staDevices2 = wifi.Install(phy2, mac2, wifiStaNodes2);

    NetDeviceContainer apDeviceN0;
    mac2.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid2));
    apDeviceN0 = wifi.Install(phy2, mac2, wifiApNodeN0);


    ///

    MobilityHelper mobility;

    mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX",
                                  DoubleValue(0.0),
                                  "MinY",
                                  DoubleValue(0.0),
                                  "DeltaX",
                                  DoubleValue(0.5),
                                  "DeltaY",
                                  DoubleValue(0.5),
                                  "GridWidth",
                                  UintegerValue(1),
                                  "LayoutType",
                                  StringValue("RowFirst"));

    // mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
    //                           "Bounds",
    //                           RectangleValue(Rectangle(-500, 500, -500, 500)));
    // // mobility.Install(wifiStaNodes);
    // mobility.Install(wifiStaNodes1);
    // mobility.Install(wifiStaNodes2);

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(wifiApNodeN1);
    mobility.Install(wifiApNodeN0);
    mobility.Install(wifiStaNodes1);
    mobility.Install(wifiStaNodes2);

    InternetStackHelper stack;
    stack.Install(p2pNodes);
    stack.Install(wifiStaNodes1);
    stack.Install(wifiStaNodes2);

    Ipv4AddressHelper address;

    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pDevices);

    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer wifiStaInterfaces1;
    wifiStaInterfaces1 = address.Assign(staDevices1);
    Ipv4InterfaceContainer wifiApInterfaces1;
    wifiApInterfaces1 = address.Assign(apDeviceN1);

    address.SetBase("10.1.3.0", "255.255.255.0");
    address.Assign(staDevices2);
    address.Assign(apDeviceN0);

    PacketSinkHelper sinkHelper("ns3::TcpSocketFactory",
                                InetSocketAddress(Ipv4Address::GetAny(), 9));

    ApplicationContainer serverApps = sinkHelper.Install(wifiStaNodes1);
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(simulationTime));

    std::vector<OnOffHelper> onOffHelpers;

    for(uint32_t i=0; i<nWifi1; i++){
        OnOffHelper sender_helper("ns3::TcpSocketFactory", 
                (InetSocketAddress(
                    wifiStaInterfaces1.GetAddress(i), 9)));
        sender_helper.SetAttribute("PacketSize", 
            UintegerValue(packetSize));
        sender_helper.SetAttribute("OnTime", StringValue(
            "ns3::ConstantRandomVariable[Constant=1]"));
        sender_helper.SetAttribute("OffTime", StringValue(
            "ns3::ConstantRandomVariable[Constant=0]"));
        sender_helper.SetAttribute("DataRate", DataRateValue(DataRate(dataRate)));
        onOffHelpers.push_back(sender_helper);
    }

    if(nWifi2>nflow){
        nflow=nWifi2;
    }
    uint32_t ran;
    for(uint32_t i=0; i<nWifi2; i++){
        for(uint32_t f=0; f<(nflow/nWifi2); f++){
                ran=random()%nWifi1;
                ApplicationContainer clientApps = onOffHelpers[ran].Install(wifiStaNodes2.Get(i));
                //clientApps.Add(echoClient.Install(wifiStaNodes2.Get(1)));
                clientApps.Start(Seconds(2.0));
                clientApps.Stop(Seconds(simulationTime));

            }
    }
    

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Simulator::Stop(Seconds(simulationTime+2));

    if (tracing)
    {
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy2.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        pointToPoint.EnablePcapAll("scratch/third");
        phy.EnablePcap("scratch/third", apDeviceN1.Get(0));
        phy2.EnablePcap("scratch/third", apDeviceN0.Get(0));
    }

    
    for(uint32_t i=0; i<nWifi1; i++){
        std::ostringstream oss;
        oss << "/NodeList/" << wifiStaNodes1.Get(i)->GetId()
        << "/ApplicationList/*/$ns3::PacketSink/Rx" ;
        Config::Connect(oss.str(), MakeCallback(&GetRxCount));      
    }
   

    for(uint32_t i=0; i<nWifi2; i++){    
        std::ostringstream oss2;
        oss2 << "/NodeList/" << wifiStaNodes2.Get(i)->GetId()
        << "/ApplicationList/*/$ns3::OnOffApplication/Tx" ;
        Config::Connect(oss2.str(), MakeCallback(&GetTxCount));      
    }


    Simulator::Run();
    Simulator::Destroy();

    double throughput=1.0*rxPcktCount*8/simulationTime;
    double deliveryRatio=1.0*rxPcktCount*100/txPcktCount;
    std::cout<<"Rx "<<rxPcktCount<<" Tx "<<txPcktCount<<std::endl;
    std::cout<<dataRate<<std::endl;
    std::cout<<"ThroughPut "<<throughput<<" kbps\n";
    std::cout<<"Pckt Delivery ratio "<<deliveryRatio<<"%\n";
    
    //graph
    if(graphType==1){ //nodes vs th,delv
        *throughPutStream->GetStream()<<nWifi<<"\t"<<throughput<<"\n";
        *deliveryStream->GetStream()<<nWifi<<"\t"<<deliveryRatio<<"\n"; 
    }
    else if(graphType==2){ //flows vs th,delv
        *throughPutStream->GetStream()<<nflow<<"\t"<<throughput<<"\n";
        *deliveryStream->GetStream()<<nflow<<"\t"<<deliveryRatio<<"\n";        
    }
    else if(graphType==3){//pcktRate vs th,delv
        *throughPutStream->GetStream()<<pcktRate<<"\t"<<throughput<<"\n";
        *deliveryStream->GetStream()<<pcktRate<<"\t"<<deliveryRatio<<"\n";       
    }
    else if(graphType==4){//coverage vs th,delv
        *throughPutStream->GetStream()<<coverage<<"\t"<<throughput<<"\n";
        *deliveryStream->GetStream()<<coverage<<"\t"<<deliveryRatio<<"\n";      
    }

    return 0;
}
