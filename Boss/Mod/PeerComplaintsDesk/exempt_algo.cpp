#include"Boss/Mod/PeerComplaintsDesk/exempt_algo.hpp"
#include"Boss/Msg/PeerMetrics.hpp"
#include"Ln/Amount.hpp"
#include"Ln/NodeId.hpp"
#include"Util/stringify.hpp"
#include<algorithm>
#include<functional>
#include<math.h>
#include<vector>

namespace {

/* Channels below this age are exempted.
 * The `min_age_description` is a human-readable description of the
 * time frame below.
 */
auto const min_age = double(24 * 60 * 60);
auto const min_age_description = std::string("one day");
/* Top percentile of fee earners.
 * i.e. the top `high_earner_percentile` percent of all peers are
 * exempted from complaints.
 */
auto const high_earner_percentile = double(20);
/* If we own less than this amount in a channel, exempt the
 * channel, "our stake in this channel is inconsequential".
 */
auto const minimum_to_us = Ln::Amount::sat(547);


/* Function to add exemptions to particular node.  */
void add_exempt( std::map<Ln::NodeId, std::string>& exempts
	       , Ln::NodeId const& nid
	       , std::string const& why
	       ) {
	auto it = exempts.find(nid);
	if (it == exempts.end())
		exempts[nid] = why;
	else
		it->second += std::string("; ") + why;
}
/* Function to exempt the top `high_earner_percentile` of nodes
 * according to the given scoring function.
 */
void exempt_top_earner( std::map<Ln::NodeId, std::string>& exempts
		      , std::map<Ln::NodeId, Boss::Msg::PeerMetrics> const&
				metrics
		      , std::string const& what
		      , std::function<double(Ln::NodeId const&)> scorer
		      ) {
	auto full_reason = std::string("node is in top ")
			 + Util::stringify(high_earner_percentile)
			 + "% of highest " + what + " earners"
			 ;
	/* Generate peers.  */
	auto peers = std::vector<Ln::NodeId>();
	for (auto& m : metrics)
		peers.push_back(m.first);
}

}

namespace Boss { namespace Mod { namespace PeerComplaintsDesk {

std::map<Ln::NodeId, std::string>
exempt_algo( std::map<Ln::NodeId, Boss::Msg::PeerMetrics> const& metrics
	   , std::map<Ln::NodeId, Ln::Amount> const& to_us
	   ) {
	auto rv = std::map<Ln::NodeId, std::string>();

	/* Gather data.  */
	auto has_nonzero_in = false;
	auto has_nonzero_out = false;
	for (auto& m : metrics) {
		if (m.second.age < min_age)
			add_exempt( rv
				  , m.first
				  , std::string("channel is ")
				  + "younger than "
				  + min_age_description
				  );
		if (m.second.in_fee_msat_per_day > 0)
			has_nonzero_in = true;
		if (m.second.out_fee_msat_per_day > 0)
			has_nonzero_out = true;
	}
	if (has_nonzero_in)
		exempt_top_earner( rv
				 , metrics
				 , "incoming fee"
				 , [&metrics
				   ](Ln::NodeId const& n) {
			auto it = metrics.find(n);
			auto& inf = it->second;
			return inf.in_fee_msat_per_day;
		});
	if (has_nonzero_out)
		exempt_top_earner( rv
				 , metrics
				 , "outgoing fee"
				 , [&metrics
				   ](Ln::NodeId const& n) {
			auto it = metrics.find(n);
			auto& inf = it->second;
			return inf.in_fee_msat_per_day;
		});

	return rv;
}

}}}
