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

#ifndef _MISC_TRAFFIC_H_
#define _MISC_TRAFFIC_H_

#include "ntop_includes.h"

class MiscTraffic : public HostCheck {
private:
  u_int64_t misc_bytes_threshold;  
  u_int16_t proto;

  HostAlert *allocAlert(HostCheck *c, Host *f, risk_percentage cli_pctg, u_int64_t _misc_bytes, u_int64_t _misc_bytes_threshold, u_int64_t _proto) {
    return new MiscTrafficAlert(c, f, cli_pctg, _misc_bytes, _misc_bytes_threshold, _proto);
  };
  
public:
  MiscTraffic();
  ~MiscTraffic() {};
  
  void periodicUpdate(Host *h, HostAlert *engaged_alert);

  bool loadConfiguration(json_object *config);  

  HostCheckID getID() const { return host_check_misc_traffic; }
  std::string getName()  const { return(std::string("misc")); }
};

#endif
