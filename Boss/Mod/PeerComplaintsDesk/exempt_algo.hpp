#ifndef BOSS_MOD_PEERCOMPLAINTSDESK_EXEMPT_ALGO_HPP
#define BOSS_MOD_PEERCOMPLAINTSDESK_EXEMPT_ALGO_HPP

#include<map>
#include<string>

namespace Boss { namespace Msg { struct PeerMetrics; }}
namespace Ln { class Amount; }
namespace Ln { class NodeId; }

namespace Boss { namespace Mod { namespace PeerComplaintsDesk {

/** Boss::Mod::PeerComplaintsDesk::exempt_algo
 *
 * @brief the actual function which proposes that certain peers
 * be exempted (i.e. have complaints against them ignored),
 * also provides human-understandable reasons why they should
 * be exempted.
 */
std::map<Ln::NodeId, std::string>
exempt_algo( std::map<Ln::NodeId, Boss::Msg::PeerMetrics> const& metrics
	   , std::map<Ln::NodeId, Ln::Amount> const& to_us
	   );

}}}

#endif /* !defined(BOSS_MOD_PEERCOMPLAINTSDESK_EXEMPT_ALGO_HPP) */
