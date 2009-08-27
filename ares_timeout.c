/* Copyright 1998 by the Massachusetts Institute of Technology.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */

static const char rcsid[] = "$Id";

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include "ares.h"
#include "ares_private.h"

struct timeval *ares_timeout(ares_channel channel, struct timeval *tv)
{
  struct query *query;
  time_t now;
  int offset, min_offset;

  /* No queries, no timeout (and no fetch of the current time). */
  if (!channel->queries)
    return NULL;

  /* Find the minimum timeout for the current set of queries. */
  time(&now);
  min_offset = -1;
  for (query = channel->queries; query; query = query->next)
    {
      if (query->timeout == 0)
	continue;
      offset = query->timeout - now;
      if (offset < 0)
	offset = 0;
      if (min_offset == -1 || offset < min_offset)
	min_offset = offset;
    }

  /* Return the minimum timeout found, or NULL if there are no timeouts. */
  if (min_offset != -1)
    {
      tv->tv_sec = min_offset;
      tv->tv_usec = 0;
      return tv;
    }
  else
    return NULL;
}
