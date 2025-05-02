#include "1905108_tcp-adaptive-reno.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "rtt-estimator.h"
#include "tcp-socket-base.h"

NS_LOG_COMPONENT_DEFINE ("TcpAdaptiveReno");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (TcpAdaptiveReno);

TypeId
TcpAdaptiveReno::GetTypeId (void)
{
  static TypeId tid = TypeId("ns3::TcpAdaptiveReno")
    .SetParent<TcpNewReno>()
    .SetGroupName ("Internet")
    .AddConstructor<TcpAdaptiveReno>()
    .AddAttribute("FilterType", "Use this to choose no filter or Tustin's approximation filter",
                  EnumValue(TcpAdaptiveReno::TUSTIN), MakeEnumAccessor(&TcpAdaptiveReno::m_fType),
                  MakeEnumChecker(TcpAdaptiveReno::NONE, "None", TcpAdaptiveReno::TUSTIN, "Tustin"))
    .AddTraceSource("EstimatedBW", "The estimated bandwidth",
                    MakeTraceSourceAccessor(&TcpAdaptiveReno::m_currentBW),
                    "ns3::TracedValueCallback::Double")
  ;
  return tid;
}

TcpAdaptiveReno::TcpAdaptiveReno (void) :
  TcpWestwoodPlus(),
  minimumRtt(Time (0)),
  currentRtt (Time (0)),
  prevcongestionOfRttj (Time(0)),
  congestionOfRttj (Time (0)),
  jPacketLRtt (Time (0)),
  baseW (0),
  incW(0),
  probeW(0)
{
  //NS_LOG_FUNCTION (this);
}

TcpAdaptiveReno::TcpAdaptiveReno (const TcpAdaptiveReno& sock) :
  TcpWestwoodPlus (sock),
  minimumRtt(Time (0)),
  currentRtt (Time (0)),
  prevcongestionOfRttj (Time(0)),
  congestionOfRttj (Time (0)),
  jPacketLRtt (Time (0)),
  baseW (0),
  incW(0),
  probeW(0)
{
  // NS_LOG_FUNCTION (this);
}

TcpAdaptiveReno::~TcpAdaptiveReno (void)
{
}

void
TcpAdaptiveReno::PktsAcked (Ptr<TcpSocketState> tcb, uint32_t packetsAcked,
                        const Time& rtt)
{
  // NS_LOG_FUNCTION (this << tcb << packetsAcked << rtt);

  if (rtt.IsZero ())
  {
      NS_LOG_WARN ("RTT measured is zero!");
      return;
  }

  m_ackedSegments += packetsAcked;

  currentRtt = rtt;

  //minimum rtt init
  if(minimumRtt.IsZero()) { 
    minimumRtt = currentRtt; 
  }
  
  if(currentRtt < minimumRtt) {
     minimumRtt = currentRtt; 
  }
   
  TcpWestwoodPlus::EstimateBW (currentRtt, tcb);
}


double
TcpAdaptiveReno::EstimateCongestionLevel()
{
  float a = 0.85; // exponential smoothing factor
  double jthRtt=0;
  double cl=0, cal=0;

  if(prevcongestionOfRttj < minimumRtt) 
      a = 0; // for initial value take jth value

  jthRtt = a*prevcongestionOfRttj.GetSeconds();
  jthRtt = jthRtt + (1-a)*jPacketLRtt.GetSeconds(); 
  congestionOfRttj = Seconds(jthRtt); 

  cal=1.0*(currentRtt.GetSeconds() - minimumRtt.GetSeconds()) ;
  cal=1.0*cal / (jthRtt - minimumRtt.GetSeconds());
  
  if(cal<1)
    cl=1.0;
  else
    cl=cal;
  return cl;
}


void 
TcpAdaptiveReno::EstimateIncWnd(Ptr<TcpSocketState> tcb)
{  
  double alpha = 10, beta=0, gamma=0; 
  double maxIncW=0;
  double B = m_currentBW.Get().GetBitRate();
  const int M = 1000; 
  
  double MSS = tcb->m_segmentSize * tcb->m_segmentSize;
  double congestion = EstimateCongestionLevel(); 
  
  maxIncW = static_cast < double > (B / M * static_cast<double> (MSS)) ; 
  beta = 2 * maxIncW;
  beta = beta * ((1/alpha) - ((1/alpha + 1)/(std::exp(alpha))));
  gamma =  (2 * maxIncW * ((1/alpha) - ((1/alpha + 0.5)/(std::exp(alpha)))));
  gamma = 1- gamma;
  incW = (maxIncW / std::exp(alpha * congestion));
  incW = (int)(incW + (beta * congestion) + gamma);
}


void
TcpAdaptiveReno::CongestionAvoidance (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{

  if (segmentsAcked > 0)
    {
      EstimateIncWnd(tcb);
      // base_window = USE NEW RENO IMPLEMENTATION
      double adder = static_cast<double> (tcb->m_segmentSize * tcb->m_segmentSize) / tcb->m_cWnd.Get ();
      if(adder < 1.0){
        adder = 1.0;
      }
      baseW = baseW + static_cast<uint32_t> (adder);
      
      // change probe window
      probeW= 1.0 * (probeW+incW/ (int)tcb->m_cWnd.Get());
      if(probeW<0.0)
        probeW=0.0;
      
      tcb->m_cWnd = baseW + probeW;
    }

}

uint32_t
TcpAdaptiveReno::GetSsThresh (Ptr<const TcpSocketState> tcb,
                          uint32_t bytesInFlight)
{
  prevcongestionOfRttj = congestionOfRttj; 
  jPacketLRtt = currentRtt; 
  
  double congestion = EstimateCongestionLevel();

  uint32_t ssthresh = tcb->m_cWnd / (1.0+congestion);

  // reset calculations
  baseW = ssthresh;
  if(baseW<(2*tcb->m_segmentSize))
    baseW=2*tcb->m_segmentSize;
  probeW= 0;
  
  return ssthresh;
}

Ptr<TcpCongestionOps>
TcpAdaptiveReno::Fork ()
{
  return CreateObject<TcpAdaptiveReno> (*this);
}

} // namespace ns3
