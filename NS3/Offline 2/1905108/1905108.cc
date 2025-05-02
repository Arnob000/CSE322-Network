#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/propagation-module.h"
#include "ns3/point-to-point-dumbbell.h"



// ===========================================================================
//           n1                        n2
//           |                         |
//           |                         |
//           r0------------------------r1
//           |10 Mbps - 1 Gbps, 100 ms |
//           |      	               |
//           n3                        n4
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ASGN2ScriptExample");

class TApp : public Application
{
  public:
    TApp();
    ~TApp() override ;

    /**
     * Register this type.
     * \return The TypeId.
     */
    static TypeId GetTypeId();

    /**
     * Setup the socket.
     * \param socket The socket.
     * \param address The destination address.
     * \param packetSize The packet size to transmit.
     * \param nPackets The number of packets to transmit.
     * \param dataRate the data rate to use.
     */
    void Setup(Ptr<Socket> socket,
               Address address,
               uint32_t packetSize,
               uint32_t nPackets,
               DataRate dataRate);

  private:
    void StartApplication() ;
    void StopApplication() ;

    /// Schedule a new transmission.
    void ScheduleTx();
    /// Send a packet.
    void SendPacket();

    Ptr<Socket> m_socket;   //!< The transmission socket.
    Address m_peer;         //!< The destination address.
    uint32_t m_packetSize;  //!< The packet size.
    uint32_t m_nPackets;    //!< The number of packets to send.
    DataRate m_dataRate;    //!< The data rate to use.
    EventId m_sendEvent;    //!< Send event.
    bool m_running;         //!< True if the application is running.
    uint32_t m_packetsSent; //!< The number of packets sent.
};


TApp::TApp()
    : m_socket(nullptr),
      m_peer(),
      m_packetSize(0),
      m_nPackets(0),
      m_dataRate(0),
      m_sendEvent(),
      m_running(false),
      m_packetsSent(0)
{
}

TApp::~TApp()
{
    m_socket = nullptr;
}

/* static */
TypeId
TApp::GetTypeId()
{
    static TypeId tid = TypeId("TApp")
                            .SetParent<Application>()
                            .SetGroupName("Tutorial")
                            .AddConstructor<TApp>();
    return tid;
}

void
TApp::Setup(Ptr<Socket> socket,
                   Address address,
                   uint32_t packetSize,
                   uint32_t nPackets,
                   DataRate dataRate)
{
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
}

void
TApp::StartApplication()
{
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind();
    m_socket->Connect(m_peer);
    SendPacket();
}

void
TApp::StopApplication()
{
    m_running = false;

    if (m_sendEvent.IsRunning())
    {
        Simulator::Cancel(m_sendEvent);
    }

    if (m_socket)
    {
        m_socket->Close();
    }
}

void
TApp::SendPacket()
{
    Ptr<Packet> packet = Create<Packet>(m_packetSize);
    m_socket->Send(packet);

    ScheduleTx();
    
}

void
TApp::ScheduleTx()
{
    if (m_running)
    {
        Time tNext(Seconds(m_packetSize * 8 / static_cast<double>(m_dataRate.GetBitRate())));
        m_sendEvent = Simulator::Schedule(tNext, &TApp::SendPacket, this);
    }
}


Ptr<OutputStreamWrapper> throughPutStream1;
Ptr<OutputStreamWrapper> throughPutStream2;
Ptr<OutputStreamWrapper> confile1;
Ptr<OutputStreamWrapper> confile2;
Ptr<OutputStreamWrapper> jainfair;


/**
 * Congestion window change callback
 *
 * \param oldCwnd Old congestion window.
 * \param newCwnd New congestion window.
 */

static void
CwndChangeTcp(uint32_t oldCwnd, uint32_t newCwnd)
{
    *confile1->GetStream()<<Simulator::Now().GetSeconds()<< "\t" << newCwnd<<std::endl;
      
    // NS_LOG_UNCOND(Simulator::Now().GetSeconds() << "\t" << newCwnd);
}

static void
CwndChangeTcpadp(uint32_t oldCwnd, uint32_t newCwnd)
{
    *confile2->GetStream()<<Simulator::Now().GetSeconds()<< "\t" << newCwnd<<std::endl;
    // NS_LOG_UNCOND(Simulator::Now().GetSeconds() << "\t" << newCwnd);
}

uint32_t rx1=0;
uint32_t rx2=0;

int
main(int argc, char* argv[])
{
    uint32_t nLeftLeaf = 2;
    uint32_t nRightLeaf = 2;
    uint32_t pktsize = 1024;  
    uint32_t bDatarate=50;
    uint32_t bDelay=100;
    double pktLossrate=1/1000000;
    uint32_t pktLossExp=6;
    uint16_t sinkPort = 8080;
    uint32_t rx1=0;
    uint32_t rx2=0;
    uint32_t graphtype=1;
    double avgrx1=0;
    double avgrx2=0;
    double jup=0;
    double jdown=0;
    double j=0;

    AsciiTraceHelper asciiTraceHelper;
    throughPutStream1  = asciiTraceHelper.CreateFileStream("scratch/throughput1.dat", std::ios::out|std::ios::app);
    throughPutStream2  = asciiTraceHelper.CreateFileStream("scratch/throughput2.dat", std::ios::out|std::ios::app);
    confile1  = asciiTraceHelper.CreateFileStream("scratch/confile1.dat", std::ios::out);
    confile2  = asciiTraceHelper.CreateFileStream("scratch/confile2.dat", std::ios::out);
    jainfair  = asciiTraceHelper.CreateFileStream("scratch/jainfair.dat", std::ios::out);
     

    std::string bottleneckDataRate="50Mbps";
    std::string bottleneckDelay="10ms";
    uint32_t simTime=30;
    std::string tcp1 ="ns3::TcpNewReno"; //always 1;
    std::string tcp2 ="ns3::TcpAdaptiveReno";

    // LogComponentEnable("PacketSink", LOG_LEVEL_INFO);

    CommandLine cmd(__FILE__);
    cmd.AddValue("bDatarate", "Bottleneck data rate", bDatarate);
    cmd.AddValue("bDelay", "Bottleneck delay", bDelay);
    cmd.AddValue("pktLossExp", "Packet loss rate exponent", pktLossExp); 
    cmd.AddValue("tcp2", "Packet loss rate exponent", tcp2); 
    cmd.AddValue("graphtype", "Packet loss rate exponent", graphtype); 
    cmd.Parse(argc, argv);

    
     Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(pktsize));

    bottleneckDataRate= std::to_string(bDatarate)+"Mbps";
    bottleneckDelay= std::to_string(bDelay)+"ms";
    pktLossrate=1;
    for(uint32_t i=1; i<= pktLossExp; i++){
        pktLossrate=pktLossrate*10;
    }
    pktLossrate=1.0/pktLossrate;


   Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue("ns3::TcpAdaptiveReno"));
    Config::SetDefault("ns3::TcpSocket::InitialCwnd", UintegerValue(1));
    Config::SetDefault("ns3::TcpL4Protocol::RecoveryType",
                       TypeIdValue(TypeId::LookupByName("ns3::TcpClassicRecovery")));

    NS_LOG_UNCOND("BottleNeck Rate: "<<bottleneckDataRate<<" "<<bDelay<<", Loss Rate: "<<pktLossExp<<" "<<tcp2);

    
    // Create the point-to-point link helpers
    PointToPointHelper pointToPointRouter;
    pointToPointRouter.SetDeviceAttribute("DataRate", StringValue(bottleneckDataRate));
    pointToPointRouter.SetChannelAttribute("Delay", StringValue(bottleneckDelay));
    PointToPointHelper pointToPointLeft;
    pointToPointLeft.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
    pointToPointLeft.SetChannelAttribute("Delay", StringValue("1ms"));
    PointToPointHelper pointToPointRight;
    pointToPointRight.SetDeviceAttribute("DataRate", StringValue("1Gbps"));
    pointToPointRight.SetChannelAttribute("Delay", StringValue("1ms"));
    
    pointToPointRouter.SetQueue ("ns3::DropTailQueue", "MaxSize",
    StringValue (std::to_string (bDatarate*bDelay*1000/pktsize) + "p"));

    PointToPointDumbbellHelper d(nLeftLeaf,
                                 pointToPointLeft,
                                 nRightLeaf,
                                 pointToPointRight,
                                 pointToPointRouter);

    Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(pktLossrate));
    d.m_routerDevices.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));

    Ptr<RateErrorModel> em2 = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(pktLossrate));
    d.m_routerDevices.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(em2));
    // Install Stack
    // TCP new reno
       Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue(tcp1));
    InternetStackHelper stack1, rstack;
    stack1.Install (d.GetLeft (0)); // left leaves
    stack1.Install (d.GetRight (0)); // right leaves
    //bottleneck router
    rstack.Install (d.GetLeft ());
    rstack.Install (d.GetRight ());

     // TCP adaptive reno
    Config::SetDefault("ns3::TcpL4Protocol::SocketType", StringValue(tcp2));
    InternetStackHelper stack2;
    stack2.Install (d.GetLeft (1)); // left leaves
    stack2.Install (d.GetRight (1)); // right leaves

    

    // Assign IP Addresses
     Ipv4AddressHelper l, r, rtr;
    l.SetBase("10.1.1.0", "255.255.255.252");
    r.SetBase("10.1.2.0", "255.255.255.252");
    rtr.SetBase("10.1.3.0", "255.255.255.252");
    
    d.AssignIpv4Addresses(l, r, rtr);

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // d.AssignIpv4Addresses(Ipv4AddressHelper("10.1.1.0", "255.255.255.0"),
    //                       Ipv4AddressHelper("10.2.1.0", "255.255.255.0"),
    //                       Ipv4AddressHelper("10.3.1.0", "255.255.255.0"));

    //receivers
    Address sinkAddress(InetSocketAddress(d.GetRightIpv4Address(0), sinkPort));
    PacketSinkHelper packetSinkHelper("ns3::TcpSocketFactory",
                                      InetSocketAddress(Ipv4Address::GetAny(), sinkPort));
    ApplicationContainer sinkApps = packetSinkHelper.Install(d.GetRight(0));
    sinkApps.Start(Seconds(0.0));
    sinkApps.Stop(Seconds(simTime));

    Address sinkAddress2(InetSocketAddress(d.GetRightIpv4Address(1), sinkPort));
    PacketSinkHelper packetSinkHelper2("ns3::TcpSocketFactory",
                                      InetSocketAddress(Ipv4Address::GetAny(), sinkPort));
    ApplicationContainer sinkApps2 = packetSinkHelper2.Install(d.GetRight(1));
    sinkApps2.Start(Seconds(0.0));
    sinkApps2.Stop(Seconds(simTime));

    //senders
    Ptr<Socket> ns3TcpSocket = Socket::CreateSocket(d.GetLeft(0), TcpSocketFactory::GetTypeId());
    if(graphtype==3)
        ns3TcpSocket->TraceConnectWithoutContext("CongestionWindow", MakeCallback(&CwndChangeTcp));

    Ptr<TApp> app = CreateObject<TApp>();
    app->Setup(ns3TcpSocket, sinkAddress, pktsize, 1000, DataRate("1Gbps"));
    d.GetLeft(0)->AddApplication(app);
    app->SetStartTime(Seconds(1.0));
    app->SetStopTime(Seconds(simTime));

    Ptr<Socket> ns3TcpSocket2 = Socket::CreateSocket(d.GetLeft(1), TcpSocketFactory::GetTypeId());
    if(graphtype==3)
        ns3TcpSocket2->TraceConnectWithoutContext("CongestionWindow", MakeCallback(&CwndChangeTcpadp));

    Ptr<TApp> app2 = CreateObject<TApp>();
    app2->Setup(ns3TcpSocket2, sinkAddress, pktsize, 1000, DataRate("1Gbps"));
    d.GetLeft(1)->AddApplication(app2);
    app2->SetStartTime(Seconds(1.0));
    app2->SetStopTime(Seconds(simTime));

    
    FlowMonitorHelper flowMonitor;
    flowMonitor.SetMonitorAttribute("MaxPerHopDelay", TimeValue(Seconds(3.0)));
    Ptr<FlowMonitor> mflow;
    mflow=flowMonitor.InstallAll();  

    Simulator::Stop(Seconds(simTime));
    Simulator::Run();
    Simulator::Destroy();

    mflow->SerializeToXmlFile("scratch/logFlow.xml",1,1);
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowMonitor.GetClassifier ());
    FlowMonitor::FlowStatsContainer stats = mflow->GetFlowStats ();

     int whichtcp=0;
     
    for (auto it : stats) {
      if(whichtcp%2==0){  
        rx1 += it.second.rxBytes;
        j+=it.second.rxBytes;
      }
      else {
        rx2 += it.second.rxBytes;
        j+=it.second.rxBytes;
      }

        
        jup=jup+j;
        jdown=jdown+(j*j);

    //   NS_LOG_UNCOND(rx1);
    //   NS_LOG_UNCOND(rx2);
      whichtcp++;
    }

    avgrx1=1.0*rx1*8/simTime;
    avgrx2=1.0*rx2*8/simTime;
    jup=1.0*jup*jup;
    jdown=1.0*jup/jdown;
    jdown=jdown/4.0;

      NS_LOG_UNCOND(avgrx1);
      NS_LOG_UNCOND(avgrx2);
      NS_LOG_UNCOND(jdown);

    if(graphtype==1){ //th vs datarate
        *throughPutStream1->GetStream()<<bDatarate<<"\t"<<avgrx1<<"\n";
        *throughPutStream2->GetStream()<<bDatarate<<"\t"<<avgrx2<<"\n"; 
        *jainfair->GetStream()<<bDatarate<<"\t"<<jdown<<"\n"; 
    }
    else if(graphtype==2){ //th vs pdrop
        *throughPutStream1->GetStream()<<pktLossExp<<"\t"<<avgrx1<<"\n";
        *throughPutStream2->GetStream()<<pktLossExp<<"\t"<<avgrx2<<"\n"; 
        *jainfair->GetStream()<<pktLossExp<<"\t"<<jdown<<"\n";
    }


    return 0;
}
