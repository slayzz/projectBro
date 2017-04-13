#include "signal_handlers.hpp"


void hdl (int sig, siginfo_t *siginfo, void *context)
{

}


int signalHandler()
{
  struct sigaction act;

  memset (&act, '\0', sizeof(act));

        /* Use the sa_sigaction field because the handles has two additional parameters */
  act.sa_sigaction = &hdl;

        /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
  act.sa_flags = SA_SIGINFO;

  if (sigaction(SIGINT, &act, NULL) < 0)
    {
      perror ("sigaction");
      return 1;
    }

}
