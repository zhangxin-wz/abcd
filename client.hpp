#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "event.hpp"
#include "connection.hpp"
#include "graph.hpp"
#include "module.hpp"
#include "stats.hpp"

#include <boost/random.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

#include <utility>

namespace ba = boost::accumulators;

class client: public module
{
  // The graph the client operates on.
  graph &g;

  // The client ID.
  int id;

  // The client random number generator.
  boost::mt19937 &rng;

  // The mean holding time.
  double h_time;
  // The holding time distribution.
  boost::exponential_distribution<> htd;
  // The holding time generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > cdg;

  // The mean DC-change time.
  double mdct;
  // The DC-change time distribution.
  boost::exponential_distribution<> dctd;
  // The DC-change time generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > dctg;

  // The mean BS-change time.
  double mbst;
  // The BS-change time distribution.
  boost::exponential_distribution<> bstd;
  // The BS-change time generator.
  boost::variate_generator<boost::mt19937 &,
                           boost::exponential_distribution<> > bstg;

  // The distribution for the number of subcarriers.
  double mnsc;
  boost::poisson_distribution<> nscd;
  boost::variate_generator<boost::mt19937 &,
                           boost::poisson_distribution<> > nscdg;

  // The connection.
  connection conn;

  // The statistics object.
  stats *st;

public:
  /**
   * q is the DES queue, id is the id of the client, r is the random
   * number generator, l_sleep is the lambda of the exponential
   * distribution with mean (1/lambda), mnc is the mean number (for
   * the Poisson distribution) of cell changes that a client makes,
   * l_change is the lambda of the exponential distribution of time
   * between changes.
   */
  client(graph &g, pqueue &q, int id, boost::mt19937 &r,
         double l_sleep, double mnc, double l_change, double mnsc);
  ~client();

  // Schedule the new event based on the current state of the client.
  // This function doesn't change the state of the client.
  void schedule(double t);

  // Processes the event and changes the state of the client.
  void operator()(double t);

  // Tell whether the client is idle.
  bool is_idle();

private:
  std::pair<bool, int>
  set_up();

  std::pair<bool, int>
  reconfigure();

  void
  tear_down();
};

#endif /* CLIENT_HPP */
