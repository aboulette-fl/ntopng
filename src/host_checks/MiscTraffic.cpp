/*
 *
 * (C) 2013-22 - ntop.org
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include "ntop_includes.h"
#include "host_checks_includes.h"

/* ***************************************************** */

MiscTraffic::MiscTraffic() : HostCheck(ntopng_edition_community, false /* All interfaces */, false /* Don't exclude for nEdge */, false /* NOT only for nEdge */) {
  misc_bytes_threshold = (u_int64_t)-1;
};

/* ***************************************************** */

void MiscTraffic::periodicUpdate(Host *h, HostAlert *engaged_alert) {
  HostAlert *alert = engaged_alert;
  u_int64_t delta;

  if((delta = h->cb_status_delta_misc_bytes(h->get_ndpi_stats()->getProtoBytes(proto))) > misc_bytes_threshold) {
    if (!alert) alert = allocAlert(this, h, CLIENT_FULL_RISK_PERCENTAGE, delta, misc_bytes_threshold, proto);
    if (alert) h->triggerAlert(alert);
  }
}

/* ***************************************************** */

bool MiscTraffic::loadConfiguration(json_object *config) {
  json_object *json_threshold;
  json_object *items;

  HostCheck::loadConfiguration(config); /* Parse parameters in common */

  if(json_object_object_get_ex(config, "threshold", &json_threshold))
    misc_bytes_threshold = json_object_get_int64(json_threshold);

  if(json_object_object_get_ex(config, "items", &items)) {
    /* GT */
    json_object *item = json_object_array_get_idx(items, 0);
    misc_bytes_threshold = json_object_get_int(item);

    /* LT */
    item = json_object_array_get_idx(items, 1);
    /* misc_bytes_threshold = json_object_get_int(item); */

    /* Protocol */
    item = json_object_array_get_idx(items, 2);
    proto = json_object_get_int(item);
  }

  // ntop->getTrace()->traceEvent(TRACE_NORMAL, "%s %u", json_object_to_json_string(config), misc_bytes_threshold);

  return(true);
}

/* ***************************************************** */

